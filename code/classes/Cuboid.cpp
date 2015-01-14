/*
	Cuboid.cpp

	Child of class Shape.
	Cuboid object with various settable properties.

	Written by: Emily McDonald October 2014
*/

#include "wrapper_glfw.h" //glfw wrapper header
#include <vector>
#include <glm/glm.hpp> //glm core
#include "Shape.h"
#include "Cuboid.h"

Cuboid::Cuboid() { }

Cuboid::Cuboid(GLfloat width, GLfloat height, GLfloat depth, glm::vec4 color)
{ 	
	this->width = width;
	this->height = height;
	this->depth = depth;

	this->vertices = generateVertices();
	this->normals = generateNormals();
	this->color = generateColor(color);
}

/*
	Function to generate the vertices for a Cuboid object.
*/
std::vector<glm::vec3> Cuboid::generateVertices()
{
	GLfloat x = width / 2;
	GLfloat y = height / 2;
	GLfloat z = depth / 2;

	glm::vec3 A = { -x, y, -z },
		B = { -x, -y, -z },
		C = { x, -y, -z },
		D = { x, y, -z },
		E = { x, -y, z },
		F = { x, y, z },
		G = { -x, -y, z },
		H = { -x, y, z };

	return std::vector<glm::vec3>
	{
		A, B, C,
		C, D, A,
		C, E, D,
		E, F, D,
		E, G, F,
		G, H, F,
		G, B, H,
		B, A, H,
		G, E, C,
		C, B, G,
		A, D, F,
		F, H, A
	};
}