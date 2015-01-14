#pragma once

class Shape
{
	protected:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec4> color;

		std::vector<glm::vec3> generateNormals();
		std::vector<glm::vec4> generateColor(glm::vec4);

	public:
		GLfloat width;
		GLfloat height;
		GLfloat depth;

		Shape();
		Shape(GLfloat, GLfloat, glm::vec4);
		Shape(GLfloat, GLfloat, GLfloat, glm::vec4);
		std::vector<glm::vec3> getVertices();
		std::vector<glm::vec3> getNormals();
		std::vector<glm::vec4> getColor();
};