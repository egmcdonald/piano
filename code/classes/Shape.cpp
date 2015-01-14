/*
	Shape.cpp

	Base class for shapes to inherit from as many shapes share the same properties.

	Written by: Emily McDonald October 2014
*/

#include "wrapper_glfw.h" //glfw wrapper header
#include <vector>
#include <glm/glm.hpp> //glm core
#include "Shape.h"

Shape::Shape() { }

/*
	Generate the normals, given a triangle array of vertices. 
	Standard normals calculation, using cross product.
*/
std::vector<glm::vec3> Shape::generateNormals()
{
	int NUMBER_OF_VERTICES = vertices.size(); //numebr of vertices within the shape

	std::vector<glm::vec3> normals(NUMBER_OF_VERTICES);
	for (int i = 0; i < NUMBER_OF_VERTICES; i += 3)
	{
		//generate cross product of the vertex, using the opposing two vertices in the traingle
		glm::vec3 normal = glm::cross
		(
			vertices.at(i + 1) - vertices.at(i),
			vertices.at(i + 2) - vertices.at(i)
		);
		normals.at(i) = normals.at(i + 1) = normals.at(i + 2) = -normal;
	}
	return normals;
}

std::vector<glm::vec4> Shape::generateColor(glm::vec4 color)
{
	std::vector<glm::vec4> colors(0);
	for (glm::vec3 vertex : vertices)
	{
		colors.push_back(color);
	}
	return colors;
}

std::vector<glm::vec3> Shape::getVertices()
{
	return vertices;
}

std::vector<glm::vec3> Shape::getNormals()
{
	return normals;
}

std::vector<glm::vec4> Shape::getColor()
{
	return color;
}