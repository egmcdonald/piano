/*
	main.cpp

	Main class to handle drawing and movement of program objects.
	Runs a collection of piano key objects and allows them to move independently.
	Other key functions include rotating the view and object, zoom, altering the hammer speed and vibration of piano wire.

	Based on lab by: Iain Martin October 2014
	Edited by: Emily McDonald October 2014
*/

#include "wrapper_glfw.h" //glfw wrapper header
#include <vector>
#include <iostream> //input/output
#include <glm/glm.hpp> //glm core
#include "glm/gtc/matrix_transform.hpp" //matrix extension
#include <glm/gtc/type_ptr.hpp>
#include "Shape.h"
#include "Cuboid.h"
#include "Tetrahedron.h"
#include "Cylinder.h"
#include "Piano.h"
#include "main.h"

using namespace std;
using namespace glm;

/* 
	This function is called before entering the main rendering loop.
	Initialises variables and uniforms.
*/
void initialise(GLWrapper *glw)
{
	#pragma region View Variables

	x = y = z = 0; //view position

	angle_x = angle_y = angle_z = 0; //object position
	angle_inc_x = angle_inc_y = angle_inc_z = 0;

	zoom = 8; //initial camera placement

	aspect_ratio = 1.3333f; //for window resizing

	#pragma endregion

	glGenVertexArrays(1, &vao); //generate index (name) for one vertex array object	
	glBindVertexArray(vao); //create the vertex array object and make it current

	//initialise the piano key models
	for (int i = 0; i < MODELS; i++)
	{
		MOVING_UP[i] = MOVING_DOWN[i] = false; //key is not currently moving
		INCREMENT[i] = 0; //to control angle of lever, currently at resting zero

		if (i % 2 == 0) //if the key is at position 0, 2, 4
			piano[i] = Piano("natural"); //make it a white, natural key
		else //if the key is at position 1, 3
			piano[i] = Piano("sharp"); //make it a black, sharp key
	
		createModel(i); //create the piano key model
	}

	//try load the vertex and fragment shaders, catch if file load is invalid
	try
	{
		program = glw->LoadShader("diffuse.vert", "main.frag"); //load and build the vertex and fragment shaders
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	//declare and initialise the uniforms
	modelID = glGetUniformLocation(program, "model");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	normalmatrixID = glGetUniformLocation(program, "normalmatrix");
}

/*
	Function to create the specified model within the specified model.
	Binds the position, color and normal buffers.
	Data fetch from shape object, previously generated in createModel.
*/
void createObject(int model, Shape object, int index)
{
	//positions
	vector<vec3> positions = object.getVertices();
	glGenBuffers(1, &positionBuffer[model][index]);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[model][index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//colors
	vector<vec4> color = object.getColor();
	glGenBuffers(1, &colorBuffer[model][index]);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer[model][index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * color.size(), color.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//normals
	vector<vec3> normals = object.getNormals();
	glGenBuffers(1, &normalsBuffer[model][index]);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer[model][index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
	Function to create the main outer model, consisting of multiple inner piano key models.
	Generate as many objects as specified in OBJECTS constant.
	Fill the objects with data within the Shape class.
*/
void createModel(int model)
{
	//run through the model objects
	for (int i = 0; i < OBJECTS; i++)
	{
		createObject(model, piano[model].getObjectByIndex(i), i); //create each individual object within the key model
	}
}

/*
	Function to bind the object shapes.
	Binds vertices, colors and normals to the specified model/shape buffer.
*/
void bindObject(int model, int shape)
{
	//bind object vertices, attribute index 0
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer[model][shape]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//bind object colours, attribute index 1
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer[model][shape]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//bind object normals, attribute index 2
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer[model][shape]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

/*
	Function to draw the specified shape within the specified model.
	Handles each object type individually, including:
	Rotates the key and lever upon key press,
	Rotates the pivot tetrahedron to position it side on,
	Translates the hammer/arm and damper/arm upon key press,
	Vibrates the wire upon hammer contact
*/
void drawShape(int modelnumber, mat4 View, vec3 translatevec, int shape)
{
	bindObject(modelnumber, shape); //bind the specified shape within the specified model

	mat4 model = mat4(1.0f); //create the model variable for the shape
	model = rotate(model, -angle_x, vec3(1, 0, 0)); //rotating object around x-axis
	model = rotate(model, -angle_y, vec3(0, 1, 0)); //rotating object around y-axis
	model = rotate(model, -angle_z, vec3(0, 0, 1)); //rotating object around z-axis

	if (shape < 2) //key or lever
	{
		//translate back to the origin, rotate around the origin the height required, translate back to original position
		model = translate(model, pivotpoint);
		model = rotate(model, INCREMENT[modelnumber] * (18.0f) / LIMIT, vec3(0, 0, 1));
		model = translate(model, -pivotpoint);
		model = translate(model, translatevec);
	}
	else if (shape == 2) //pivot
	{
		//rotate the pivot shape to be side on
		model = translate(model, pivotpoint);
		model = rotate(model, 65.0f, vec3(0, 1, 0));
		model = translate(model, -pivotpoint);
		model = translate(model, translatevec);
	}
	else if (shape >= 3 && shape <= 6) //hammerarm, hammer, damperarm or damper
	{
		vec3 difference(0.0); //vector to hold difference between the hammer/damper and the wire

		if (MOVING_UP[modelnumber] ^ MOVING_DOWN[modelnumber]) //if the model is moving upwards xor model is moving downwards
		{
			difference = vec3(0.0, INCREMENT[modelnumber] * (wirecentre - piano[modelnumber].damperarm.height / 2) / LIMIT, 0.0); ///calculate the difference for the hammer/arm

			if (shape > 4) difference += vec3(0.0, INCREMENT[modelnumber] * (0.31f) / LIMIT, 0.0); //calculate the difference for the damper/arm			
		}

		model = translate(model, translatevec + difference); //translate the hammer/damper objects to required height
	}
	else if (shape == 7) //wire
	{
		//rotate the cylinder so it's lying horizontally, parallel to the x-axis
		model = translate(model, translatevec);
		model = rotate(model, 90.0f, vec3(0, 0, 1));
		model = translate(model, vec3(-piano[modelnumber].damper.height / 2 - piano[modelnumber].wire.width / 2, 1.5f, 0.05f));
		if (MOVING_DOWN[modelnumber]) //if the wire has just been hit with hammer
		{
			//vibrate the wire
			float offset = sin(3.5f * 3.141592f * INCREMENT[modelnumber] * 100);
			model = translate(model, vec3(offset + 0.05f, offset, offset));
		}
	}
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	mat3 normalmatrix = transpose(inverse(mat3(View * model))); //define normal matrix
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
}

/* 
	Called to update the display. 
	This function is called in the event loop in the wrapper class.
*/
void display()
{
	//run through the array of models
	for (int i = 0; i < MODELS; i++)
	{
		if (INCREMENT[i] == LIMIT) //if the key has reached maximum height
		{
			MOVING_UP[i] = false; //key is no longer moving up
			MOVING_DOWN[i] = true; //key is now moving down
		}
		else if (MOVING_DOWN[i] && INCREMENT[i] == 0) //if the key is moving down and the minimum height is reached
		{
			MOVING_DOWN[i] = false; //object is no longer moving down
			OBJECTS_MOVING--; //remove the objects from the count of objects currently moving
		}
	}

	glClearColor(0.19f, 0.05f, 0.12f, 1.0f); //background color of dark purple
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color and frame buffers
	glEnable(GL_DEPTH_TEST); //enable depth test
	glUseProgram(program); //make the compiled shader program current

	//projection matrix
	mat4 Projection = perspective(45.0f, aspect_ratio, 0.1f, 100.0f); //45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]); 

	//camera matrix
	mat4 View = lookAt
	(
		vec3(0, 0, zoom), //camera is at in world space, accounts for zoom variable
		vec3(0, 0, 0), //looks at the origin
		vec3(0, 1, 0)  //head is up
	);
	View = rotate(View, x, vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	View = rotate(View, y, vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	View = rotate(View, z, vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);

	#pragma region Draw Objects

	float zpos = -2.0f; //where to place the first model upon the z-axis
	
	//iterate through the models array
	for (int model = 0; model < MODELS; model++)
	{
		vec3 initial = vec3(0 - 2.0f, 0 - 1.0f, zpos / 3.6); //set the initial position of the model
		wirecentre = -1.0f + piano[model].key.height + piano[model].damperarm.height; //hold the wire centre y-position for use later on, needed to detect hammer contact with wire

		//position and draw key
		vec3 translatevec = initial;
		drawShape(model, View, translatevec, piano[model].getIndexByObject("key"));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//position and draw lever
		translatevec += vec3(piano[model].key.width / 2 + piano[model].lever.width / 2, piano[model].key.height / 2 - piano[model].lever.height / 2, 0);
		drawShape(model, View, translatevec, piano[model].getIndexByObject("lever"));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//position and draw pivot
		translatevec += vec3(-0.8f, -piano[model].pivot.height / 2 - piano[model].lever.height / 2, 0.0);
		pivotpoint = translatevec + vec3(0.0, piano[model].pivot.height / 2 + piano[model].lever.height / 2, 0.0);
		drawShape(model, View, translatevec, piano[model].getIndexByObject("pivot"));
		glDrawArrays(GL_TRIANGLES, 0, 12);

		//position and draw hammerarm
		translatevec += vec3(0.8f, piano[model].pivot.height / 2 + piano[model].lever.height + piano[model].hammerarm.height / 2, 0.0);
		drawShape(model, View, translatevec, piano[model].getIndexByObject("hammerarm"));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//position and draw hammer
		translatevec += vec3(0.0, piano[model].hammerarm.height / 2 + piano[model].hammer.height / 2, 0.0);
		drawShape(model, View, translatevec, piano[model].getIndexByObject("hammer"));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//position and draw damperarm
		translatevec += vec3(1.0f, -piano[model].hammer.height / 2 - piano[model].hammerarm.height + piano[model].damperarm.height / 2, 0.0);
		drawShape(model, View, translatevec, piano[model].getIndexByObject("damperarm"));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//position and draw damper
		translatevec += vec3(0.0, piano[model].damperarm.height / 2 + piano[model].damper.height / 2, 0.0);
		drawShape(model, View, translatevec, piano[model].getIndexByObject("damper"));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//position and draw wire
		drawShape(model, View, translatevec, piano[model].getIndexByObject("wire"));
		glDrawArrays(GL_TRIANGLE_FAN, 0, piano[model].wire.EDGE_POINTS + 2);
		glDrawArrays(GL_TRIANGLE_FAN, piano[model].wire.EDGE_POINTS + 2, piano[model].wire.EDGE_POINTS + 2);
		glDrawArrays(GL_TRIANGLE_STRIP, 2 * piano[model].wire.EDGE_POINTS + 4, 2 * piano[model].wire.EDGE_POINTS + 2);

		zpos += 1.0f; //increment the z-position of the model to place it further up the z-axis
	}

	#pragma	endregion

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	angle_x += angle_inc_x; //increment the object position on x-axis
	angle_y += angle_inc_y; //increment the object position on y-axis
	angle_z += angle_inc_z; //increment teh object position on z-axis

	//iterate through the model collection
	for (int j = 0; j < MODELS; j++)
	{
		if (MOVING_UP[j]) INCREMENT[j]++; //if the model is currently moving upwards, increase the position of the key arm
		if (MOVING_DOWN[j]) INCREMENT[j]--; //if the model is currently moving downwards, decrease the position of the key arm
	}
}

/* 
	Function called whenever the window is resized. 
	The new window size is given, in pixels. 
*/
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
}

/*
	Function to move the hammer of the specified model.
*/
void moveHammer(int model)
{
	if (!MOVING_UP[model] && !MOVING_DOWN[model]) //if the key is not currently moving
	{
		MOVING_UP[model] = true; //trigger the model to begin moving
		OBJECTS_MOVING++; //increment the number of currently moving objects by 1
	}
}

/*
	This function decreases the speed of the hammer.
	This is called whenever the user presses the assigned decrease speed key.
*/
void decSpeed()
{
	if (OBJECTS_MOVING == 0) //if there are currently no moving objects
	{
		if (LIMIT < 500) //if the hammer speed lower limit is not reached
		{
			LIMIT += 50; //slow the speed of the hammer
			cout << "Speed of hammer: " << LIMIT / 2 << "ms" << endl; //inform the user of the new hammer speed
		}
		else //if the lowest hammer speed has been reached
		{
			cout << "Error: slowest speed reached" << endl; //inform the user that the hammer speed cannot be decreased any further
		}
	}
	else cout << "Error: cannot change speed whilst key is moving" << endl; //if objects are currently moving (hammer speed cannot be altered when objects are moving)
}

/*
	This function increases the speed of the hammer.
	This is called whenever the user presses the assigned increase speed key.
*/
void incSpeed()
{
	if (OBJECTS_MOVING == 0) //if there are currently no moving objects
	{
		if (LIMIT > 50) //if the hammer speed upper limit is not reached
		{
			LIMIT -= 50; //hurry the speed of the hammer
			cout << "Speed of hammer: " << LIMIT / 2 << "ms" << endl; //inform the user of the new hammer speed
		}
		else //if the highest speed has been reached
		{
			cout << "Error: fastest speed reached" << endl; //inform the user that the hammer speed cannot be increased any further
		}
	}
	else cout << "Error: cannot change speed whilst key is moving" << endl; //if objects are currently moving (hammer speed cannot be altered when objects are moving)
}

/* 
	Function to handle key presses from the user.
	Handles: anti/clockwise rotation on object/view, individual model movement, zooming in/out, in/decrease speed of hammer.
	Program can also be exited through ESC key press.
*/
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	//you can press and hold these keys to alter view
	if (key == 'Z') zoom += 0.2f; //zoom out
	if (key == 'X') zoom -= 0.2f; //zoom in

	if (key == '1') x += 0.5f; //rotate view anti-clockwise on x-axis
	if (key == '2') x -= 0.5f; //rotate view clockwise on x-axis
	if (key == '3') y += 0.5f; //rotate view anti-clockwise on y-axis
	if (key == '4') y -= 0.5f; //rotate view clockwise on y-axis
	if (key == '5') z += 0.5f; //rotate view anti-clockwise on z-axis
	if (key == '6') z -= 0.5f; //rotate view clockwise on z-axis

	if (action != GLFW_PRESS) return; //disable key responses to a held down key

	//you can only increment these keys, no press and hold
	if (key == 'Q') angle_inc_x -= 0.05f; //rotate object anti-clockwise on x-axis
	if (key == 'W') angle_inc_x += 0.05f; //rotate object clockwise on x-axis
	if (key == 'E') angle_inc_y -= 0.05f; //rotate object anti-clockwise on y-axis
	if (key == 'R') angle_inc_y += 0.05f; //rotate object clockwise on y-axis
	if (key == 'T') angle_inc_z -= 0.05f; //rotate object anti-clockwise on z-axis
	if (key == 'Y') angle_inc_z += 0.05f; //rotate object clockwise on z-axis

	if (key == 'A') moveHammer(0); //move model 0
	if (key == 'S') moveHammer(1); //move model 1
	if (key == 'D') moveHammer(2); //move model 2
	if (key == 'F') moveHammer(3); //move model 3
	if (key == 'G') moveHammer(4); //move model 4

	if (key == 'C') decSpeed(); //decrease the speed of the hammer
	if (key == 'V') incSpeed(); //increase the speed of the hammer

	if (key == GLFW_KEY_ESCAPE)	glfwSetWindowShouldClose(window, GL_TRUE); //quit the program 
}

/*
	Function to print the program controls to the console.
	Builds the statement and prints.
*/
void printControls()
{
	//build the controls statement
	const char* controls = "Welcome to Piano Hammer\n"
		".:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.\n"
		"':':':':':':':':':':':':':':':':':'\n"
		"Rotate\n"
		"Q: object, 1: view; anti-clockwise on x-axis\n"
		"W: object, 2: view; clockwise on x-axis\n"
		"E: object, 3: view; anti-clockwise on y-axis\n"
		"R: object, 4: view; clockwise on y-axis\n"
		"T: object, 5: view; anti-clockwise on z-axis\n"
		"Y: object, 6: view; clockwise on z-axis\n\n"
		"A: play C key\n"
		"S: play C# key\n"
		"D: play D key\n"
		"F: play D# key\n"
		"G: play E key\n\n"
		"Z: zoom out\n"
		"X: zoom in\n\n"
		"C: decrease speed of hammer\n"
		"V: increase speed of hammer\n\n"
		"ESC: quit program\n"
		".:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.\n"
		"':':':':':':':':':':':':':':':':':'";

	cout << controls << endl; //print the controls to the console
}

/* 
	Function is the entry point of the program.
	Initialises window and binds methods to event loop.
*/
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Piano Hammer"); //specify dimensions of window plus title

	printControls(); //print the program controls in the console

	if (!ogl_LoadFunctions()) //if load fails
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n"); //inform user of error
		return 0;
	}

	glw->setRenderer(display); //bind display within event loop
	glw->setKeyCallback(keyCallback); //bind display within event loop
	glw->setReshapeCallback(reshape); //bind reshape within event loop

	initialise(glw); //initialise the window

	glw->eventLoop(); //bind the event loop

	delete(glw);
	return 0;
}