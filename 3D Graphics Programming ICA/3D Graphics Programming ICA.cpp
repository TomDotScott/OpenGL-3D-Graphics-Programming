#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "Shader.h"
#include "Texture.h"

struct Vertex
{
	Vertex(const glm::vec3& pos, const glm::vec3& col, const glm::vec2& uvs, const glm::vec3& normals) :
		m_position(pos),
		m_colour(col),
		m_texcoord(uvs),
		m_normal(normals) { }

	glm::vec3 m_position;
	glm::vec3 m_colour;
	glm::vec2 m_texcoord;
	glm::vec3 m_normal;
};

//Talk about sending sizeof pointers and errors
void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.z -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.z += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.y -= 1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y += 1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		scale += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		scale -= 0.1f;
	}
}

int main()
{
	//INIT GLFW
	glfwInit();

	//CREATE WINDOW

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(constants::k_screenWidth, constants::k_screenHeight,
	                                      "3D Graphics Programming ICA 2", nullptr, nullptr);

	int frameBufferWidth;
	int frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	//Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Shader coreProgram("vertex_core.glsl", "fragment_core.glsl");

	//MODEL
	Vertex vertices[] =
	{
		//Position								//Color							//Texcoords					//Normals
		{glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, -1.f)},
		{glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, -1.f)},
		{glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, -1.f)},
		{glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, -1.f)}
	};

	unsigned numOfVertices = sizeof(vertices) / sizeof(Vertex);

	GLuint indices[] =
	{
		0, 1, 2, //Triangle 1
		0, 2, 3  //Triangle 2
	};
	unsigned numOfIndices = sizeof(indices) / sizeof(GLuint);

	
	//VAO, VBO, EBO
	//GEN VAO AND BIND
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//GEN VBO AND BIND AND SEND DATA
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//GEN EBO AND BIND AND SEND DATA
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<GLvoid*>(offsetof(Vertex, m_position)));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<GLvoid*>(offsetof(Vertex, m_colour)));
	glEnableVertexAttribArray(1);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<GLvoid*>(offsetof(Vertex, m_texcoord)));
	glEnableVertexAttribArray(2);
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<GLvoid*>(offsetof(Vertex, m_normal)));
	glEnableVertexAttribArray(3);

	//BIND VAO 0
	glBindVertexArray(0);

	//TEXTURE INIT

	//TEXTURE 0
	Texture texture0("Data/alien.png", GL_TEXTURE_2D, 0);

	//TEXTURE 1
	Texture texture1("Data/box.png", GL_TEXTURE_2D, 1);

	//INIT MATRICES
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 modelMatrix(1.f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix = rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	modelMatrix = glm::scale(modelMatrix, scale);

	glm::vec3 camPosition(0.f, 0.f, 1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);

	glm::mat4 viewMatrix(1.f);
	viewMatrix = lookAt(camPosition, camPosition + camFront, worldUp);

	float     fov       = 90.f;
	float     nearPlane = 0.1f;
	float     farPlane  = 1000.f;
	glm::mat4 projectionMatrix(1.f);

	projectionMatrix = glm::perspective(
	                                    glm::radians(fov),
	                                    static_cast<float>(frameBufferWidth) / static_cast<float>(frameBufferHeight),
	                                    nearPlane,
	                                    farPlane
	                                   );

	//LIGHTS
	glm::vec3 lightPosition(0.f, 0.f, 1.f);

	//INIT UNIFORMS

	coreProgram.SetMat4Fv(modelMatrix, "model_matrix");
	coreProgram.SetMat4Fv(viewMatrix, "view_matrix");
	coreProgram.SetMat4Fv(projectionMatrix, "projection_matrix");

	coreProgram.SetVec3F(lightPosition, "light_position");
	coreProgram.SetVec3F(camPosition, "camera_position");

	//MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//UPDATE INPUT ---
		glfwPollEvents();
		updateInput(window, position, rotation, scale);

		//UPDATE --- 
		updateInput(window);

		//DRAW ---
		//Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Update uniforms
		coreProgram.Set1I(texture0.GetTextureUnit(), "texture0");
		coreProgram.Set1I(texture1.GetTextureUnit(), "texture1");

		//Move, rotate and scale
		modelMatrix = glm::mat4(1.f);
		modelMatrix = translate(modelMatrix, position);
		modelMatrix = rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		modelMatrix = rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		modelMatrix = rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		modelMatrix = glm::scale(modelMatrix, scale);

		coreProgram.SetMat4Fv(modelMatrix, "model_matrix");

		glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

		projectionMatrix = glm::perspective(
		                                    glm::radians(fov),
		                                    static_cast<float>(frameBufferWidth) / static_cast<float>(frameBufferHeight
		                                    ),
		                                    nearPlane,
		                                    farPlane
		                                   );

		coreProgram.SetMat4Fv(projectionMatrix, "projection_matrix");

		//Use a program
		coreProgram.Use();

		//Activate texture
		texture0.Bind();
		texture1.Bind();

		//Bind vertex array object
		glBindVertexArray(VAO);

		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, nullptr);

		//End Draw
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//END OF PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();

	//Delete VAO and Buffers

	return 0;
}
