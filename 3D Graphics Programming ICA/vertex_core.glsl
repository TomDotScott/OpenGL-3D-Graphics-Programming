#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_colour;
layout (location = 2) in vec2 vertex_texcoord;

out vec3 varying_position;
out vec3 varying_colour;
out vec2 varying_texcoord;

void main()
{
	varying_position = vertex_position;
	varying_colour = vertex_colour;
	varying_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1); // textures are flipped by default. Multiply the y by -1 to fix 

	gl_Position = vec4(vertex_position, 1.f);
}