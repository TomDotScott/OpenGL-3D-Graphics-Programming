#version 440

in vec3 varying_position;
in vec3 varying_colour;
in vec2 varying_texcoord;

out vec4 fragment_colour;

// Samples one pixel at a time from the image data
uniform sampler2D texture0;

void main()
{
	//fragment_colour = vec4(varying_colour, 1.f);
	fragment_colour = texture(texture0, varying_texcoord);
}