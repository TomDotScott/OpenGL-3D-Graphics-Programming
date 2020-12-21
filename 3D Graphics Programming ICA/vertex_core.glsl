#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_colour;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

out vec3 varying_position;
out vec3 varying_colour;
out vec2 varying_texcoord;
out vec3 varying_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	varying_position = vec4(model_matrix * vec4(vertex_position, 1.f)).xyz;
	varying_colour = vertex_colour;
	varying_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1); // textures are flipped by default. Multiply the y by -1 to fix 
	varying_normal = mat3(model_matrix) * vertex_normal; // Normals come from world space - we don't want them to be affected by the perspective

	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.f);
}