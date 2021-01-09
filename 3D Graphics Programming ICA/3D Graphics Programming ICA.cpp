#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

//Talk about sending sizeof pointers and errors
void update_input(GLFWwindow* window)
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

void update_input(GLFWwindow* window, Mesh& mesh)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mesh.SetPosition({ mesh.GetPosition().x, mesh.GetPosition().y, mesh.GetPosition().z - 0.01f });
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mesh.SetPosition({ mesh.GetPosition().x, mesh.GetPosition().y, mesh.GetPosition().z + 0.01f });
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mesh.SetPosition({ mesh.GetPosition().x - 0.01f, mesh.GetPosition().y, mesh.GetPosition().z});
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mesh.SetPosition({ mesh.GetPosition().x + 0.01f, mesh.GetPosition().y, mesh.GetPosition().z });
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mesh.SetRotation({ mesh.GetRotation().x, mesh.GetRotation().y - 1.f, mesh.GetRotation().z });
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		mesh.SetRotation({ mesh.GetRotation().x, mesh.GetRotation().y + 1.f, mesh.GetRotation().z });
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		mesh.SetScale({ mesh.GetScale().x + 0.1f, mesh.GetScale().y + 0.1f, mesh.GetScale().z + 0.1f });
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		mesh.SetScale({ mesh.GetScale().x - 0.1f, mesh.GetScale().y + 0.1f, mesh.GetScale().z + 0.1f });
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
	                                      "3D Graphics Programming ICA SCOTT Thomas W9036922", nullptr, nullptr);

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
	
	//TEXTURE INIT

	//TEXTURE 0
	Texture texture0("Data/alien.png", GL_TEXTURE_2D, 0);

	//TEXTURE 1
	Texture texture1("Data/box.png", GL_TEXTURE_2D, 1);

	//Material 0
	Material material0(
		glm::vec3(1.f),
		glm::vec3(0.f),
		glm::vec3(1.f),
		texture0.GetTextureUnit(),
		texture1.GetTextureUnit()
	);
	
	//INIT MESH
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);
	Quad quad;
	
	Mesh test(position, 
		rotation,
		scale, 
		quad
	);


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

	// coreProgram.SetMat4Fv(modelMatrix, "model_matrix");
	coreProgram.SetMat4Fv(viewMatrix, "view_matrix");
	coreProgram.SetMat4Fv(projectionMatrix, "projection_matrix");

	coreProgram.SetVec3F(lightPosition, "light_position");
	coreProgram.SetVec3F(camPosition, "camera_position");

	//MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//UPDATE INPUT ---
		glfwPollEvents();
		update_input(window, test);

		//UPDATE --- 
		update_input(window);

		//DRAW ---
		//Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Update uniforms
		coreProgram.Set1I(texture0.GetTextureUnit(), "texture0");
		coreProgram.Set1I(texture1.GetTextureUnit(), "texture1");

		material0.SendToShader(coreProgram);

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

		test.Render(coreProgram);
		
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
