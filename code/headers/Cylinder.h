#pragma once

#include "Shape.h"

class Cylinder : public Shape
{
	private:
		GLfloat diameter;

		std::vector<glm::vec3> generateVertices();
		std::vector<glm::vec3> generateNormals();

	public:
		const static int EDGE_POINTS = 20;

		Cylinder();
		Cylinder(GLfloat, GLfloat, glm::vec4);
};