#pragma once

#include "Shape.h"

class Tetrahedron : public Shape
{
	private:
		std::vector<glm::vec3> generateVertices();

	public:
		Tetrahedron();
		Tetrahedron(GLfloat, GLfloat, GLfloat, glm::vec4);
};