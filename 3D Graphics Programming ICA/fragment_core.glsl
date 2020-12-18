#version 440

in vec3 varying_position;
in vec3 varying_colour;
in vec2 varying_texcoord;

out vec4 fragment_colour;

void main()
{
	fragment_colour = vec4(varying_colour, 1.f);
}