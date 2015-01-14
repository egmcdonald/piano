// Minimal fragment shader

#version 400

in vec4 fcolour;
out vec4 outputColor;
void main()
{
	outputColor = fcolour;
}