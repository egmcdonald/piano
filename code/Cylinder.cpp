/*
	Cylinder.cpp

	Child of class Shape.
	Cylinder object with various settable properties.

	Written by: Emily McDonald October 2014
*/

#include "wrapper_glfw.h" //glfw wrapper header
#include <vector>
#include <glm/glm.hpp> //glm core
#include "Shape.h"
#include "Cylinder.h"

Cylinder::Cylinder() { }

Cylinder::Cylinder(GLfloat diameter, GLfloat height, glm::vec4 color)
{
	this->width = diameter;
	this->height = height;
	this->depth = diameter;
	this->diameter = diameter;

	this->vertices = generateVertices();
	this->normals = generateNormals();
	this->color = generateColor(color);
}

/*
	Function to generate vertices for a Cylinder object.
*/
std::vector<glm::vec3> Cylinder::generateVertices()
{
	GLfloat edges[EDGE_POINTS][2]; //declare 2d array to hold the x and z edge points of the outer circumference

	float increment = 360 / EDGE_POINTS; //how far apart the points on the edge should be spaced
	float radius = diameter / 2; //define radius of cylinder
	float pi = 3.141592f; //declare pi for calculations

	//iterate through the 2d array for edge points
	for (int i = 0; i < EDGE_POINTS; i++)
	{
		edges[i][0] = (radius * cos((increment * i) * (pi / 180))); //x edge value
		edges[i][1] = (radius * sin((increment * i) * (pi / 180))); //z edge value
	}

	std::vector<glm::vec3> positions(0); //declare vector to hold the vertex positions for the cylinder
	
	//cylinder lid
	positions.push_back(glm::vec3(0.0, height / 2, 0.0)); //centre of lid
	for (int j = 0; j < EDGE_POINTS; j++)
	{
		positions.push_back(glm::vec3(edges[j][0], height / 2, edges[j][1]));
	}
	positions.push_back(positions.at(1)); //close lid

	//cylinder base
	positions.push_back(glm::vec3(0.0, -height / 2, 0.0)); //centre of base
	for (int k = 0; k < EDGE_POINTS; k++)
	{
		positions.push_back(glm::vec3(edges[k][0], -height / 2, edges[k][1]));
	}
	positions.push_back(positions.at(EDGE_POINTS + 3)); //close base

	//cylinder tube
	for (int l = 0; l < EDGE_POINTS; l++)
	{
		positions.push_back(glm::vec3(edges[l][0], height / 2, edges[l][1]));
		positions.push_back(glm::vec3(edges[l][0], -height / 2, edges[l][1]));
	}
	positions.push_back(glm::vec3(edges[0][0], height / 2, edges[0][1])); //close tube
	positions.push_back(glm::vec3(edges[0][0], -height / 2, edges[0][1]));

	return positions;
}

std::vector<glm::vec3> Cylinder::generateNormals()
{
	std::vector<glm::vec3> normals(0);

	//generate normals for cylinder lid
	for (int i = 0; i < EDGE_POINTS + 2; i++)
	{
		normals.push_back(glm::vec3(0.0, 1.0f, 0.0)); //all normals for the lid point directly upwards
	}

	//generate normals for cylinder base
	for (int j = 0; j < EDGE_POINTS + 2; j++)
	{
		normals.push_back(glm::vec3(0.0, -1.0f, 0.0)); //all points for the base point directly downwards
	}

	//generate normals for cylinder tube
	for (int k = 0; k < EDGE_POINTS + 1; k++)
	{
		//normal generated by subtracting the centre vertex position from the edge vertex position
		normals.push_back(vertices.at(k + 1) - vertices.at(0)); 
		normals.push_back(vertices.at(k + 2) - vertices.at(EDGE_POINTS));
	}

	return normals;
}