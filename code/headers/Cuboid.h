#pragma once

#include "Shape.h"

class Cuboid : public Shape
{
	private:
		std::vector<glm::vec3> generateVertices();

	public:
		Cuboid();
		Cuboid(GLfloat, GLfloat, GLfloat, glm::vec4);
};