#pragma once

static const int MODELS = 5; //number of piano key models
static const int OBJECTS = 8; //number of objects in the piano key model

Piano piano[MODELS]; //piano models

GLuint positionBuffer[MODELS][OBJECTS], colorBuffer[MODELS][OBJECTS], normalsBuffer[MODELS][OBJECTS]; //buffer objects

GLuint program; //identifier for the shader program
GLuint vao;	//vertex array (container) object, index of the VAO that is container for buffer objects

GLfloat x, y, z; //rotate around point on each axis
GLfloat angle_x, angle_y, angle_z; //speed of rotate angles
GLfloat angle_inc_x, angle_inc_y, angle_inc_z; //increment speed of the rotate angles
GLfloat zoom; //zoom in and out on the object

bool MOVING_UP[MODELS], MOVING_DOWN[MODELS]; //is the lever currently moving up or down?
int INCREMENT[MODELS]; //position state of hammer
int LIMIT = 150; //determines speed of hammer
int OBJECTS_MOVING = 0; //number of hammers currently moving

glm::vec3 pivotpoint; //point of pivot
GLfloat wirecentre; //y-coordinate of the wire's centre

GLfloat aspect_ratio; //deals with resizing of window

GLuint modelID, viewID, projectionID, normalmatrixID; //uniforms

void createModel(int); //declared to allow calling within initialise