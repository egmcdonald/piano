// Minimal vertex shader

#version 400

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;

void main()
{
	vec3 light_direction = normalize(vec3(0.0, 0.0, 5.0));

	mat4 mod_view = view * model;
	mat3 mod_view_norm = mat3(mod_view); 	
	vec3 transformed = normalize(mod_view_norm * normal);

	float diffuse_component = max(dot(transformed, light_direction), 0.0);

	vec4 diffuse_colour;
	vec4 ambient_colour;
	vec4 diffuse_lighting;
	vec4 position_h = vec4(position, 1.0);
	
	diffuse_colour = colour;

	ambient_colour = diffuse_colour * 0.2;

	diffuse_lighting = diffuse_component * diffuse_colour;

	// Define the vertex colour
	fcolour = ambient_colour + diffuse_lighting;

	// Define the vertex position
	gl_Position = (projection * view * model) * position_h;
}