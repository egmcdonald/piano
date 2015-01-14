/*
	Piano.cpp

	Class to generate a piano model with key, lever, pivot, hammer/arm, damper/arm and wire objects.
	"wire" so named due to inability to call it "string"

	Written by: Emily McDonald October 2014
*/

#include "wrapper_glfw.h" //glfw wrapper header
#include <vector>
#include <glm/glm.hpp> //glm core
#include "Shape.h"
#include "Cuboid.h"
#include "Tetrahedron.h"
#include "Cylinder.h"
#include "Piano.h"

Piano::Piano() { }

Piano::Piano(std::string type)
{
	glm::vec4 color;

	if (type == "natural") color = { 1.0f, 1.0f, 1.0f, 1.0f }; //white key
	if (type == "sharp") color = { 0.1f, 0.1f, 0.1f, 1.0f }; //black key
	key = Cuboid(1.2f, 0.2f, 0.2f, color);

	color = { 0.8, 0.56f, 0.35f, 1.0f };
	lever = Cuboid(3.8f, 0.05f, 0.05f, color);

	color = { 0.8f, 0.35f, 0.36f, 1.0f };
	pivot = Tetrahedron(0.15f, 0.65f, 0.15f, color);

	color = { 0.59f, 0.8f, 0.35f, 1.0f };
	hammerarm = Cuboid(0.08f, 1.6f, 0.03f, color);
	hammer = Cuboid(0.6f, 0.2f, 0.15f, color);

	color = { 0.8f, 0.35f, 0.59f, 1.0f };
	damperarm = Cuboid(0.08f, 2.1f, 0.03f, color);
	damper = Cuboid(0.6f, 0.2f, 0.15f, color);

	color = { 0.8f, 0.78f, 0.35f, 1.0f };
	wire = Cylinder(0.05f, 5.0f, color);
}

/*
	Function to return the object shape, given the index within the model.
*/
Shape Piano::getObjectByIndex(int index)
{
	switch (index)
	{
		case 0: return key; break;
		case 1: return lever; break;
		case 2: return pivot; break;
		case 3: return hammerarm; break;
		case 4: return hammer; break;
		case 5: return damperarm; break;
		case 6: return damper; break;
		case 7: return wire; break;
	}

	return Shape();
}

/*
	Function to return the object index, given the shape type.
*/
int Piano::getIndexByObject(std::string object)
{
	if (object == "key") return 0;
	else if (object == "lever") return 1;
	else if(object == "pivot") return 2;
	else if(object == "hammerarm") return 3;
	else if(object == "hammer") return 4;
	else if(object == "damperarm") return 5;
	else if(object == "damper") return 6;
	else if(object == "wire") return 7;

	return NULL;
}