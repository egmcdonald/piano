/*
	Tetrahedron.cpp

	Child of class Shape.
	Tetrahedron object with various settable properties.

	Written by: Emily McDonald October 2014
*/

#include "wrapper_glfw.h" //glfw wrapper header
#include <vector> //vector 
#include <glm/glm.hpp> //glm core
#include "Shape.h"
#include "Tetrahedron.h"

Tetrahedron::Tetrahedron() { }

Tetrahedron::Tetrahedron(GLfloat width, GLfloat height, GLfloat depth, glm::vec4 color)
{
	this->width = width;
	this->height = height;
	this->depth = depth;

	this->vertices = generateVertices();
	this->normals = generateNormals();
	this->color = generateColor(color);
}

/*
	Function to generate the vertices of a tetrahedron.
*/
std::vector<glm::vec3> Tetrahedron::generateVertices()
{
	//four vertices within a tetrahedron
	glm::vec3 A = { 0, height / 2, 0 },
		B = { 0, 0, -depth / 2 },
		C = { -width / 2, 0, depth / 2 },
		D = { width / 2, 0, depth / 2 };

	//return the vector of vertices
	return std::vector<glm::vec3>
	{
		A, D, C,
		A, B, D,
		D, B, C,
		A, C, B
	};
}