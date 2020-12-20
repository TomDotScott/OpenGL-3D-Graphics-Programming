#include <fstream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "Constants.h"
#include <SOIL2/SOIL2.h>

void input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void on_frame_buffer_resize_callback(GLFWwindow* window, const int frameBufferWidth, const int frameBufferHeight)
{
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}

bool load_shaders(GLuint& program)
{
	char infoLog[512];

	std::string temp;
	std::string source;

	std::ifstream inFile;

	// Open the vertex shader
	inFile.open("vertex_core.glsl");

	// Check for errors
	if (!inFile.is_open())
	{
		std::cout << "Failed to open the vertex shader" << std::endl;
		return false;
	} else
	{
		while (std::getline(inFile, temp))
		{
			source += temp + "\n";
		}
	}

	inFile.close();

	// Set up and compile the shader from the 
	// vertex shader source file
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSource = source.c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Check for errors during compilation of the shader
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::cout << "Failed to compile the vertex shader" << std::endl;
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

		std::cout << infoLog << std::endl;

		return false;
	}

	// Open the fragment shader
	temp = "";
	source = "";

	// Open the vertex shader
	inFile.open("fragment_core.glsl");

	// Check for errors
	if (!inFile.is_open())
	{
		std::cout << "Failed to open the fragment shader" << std::endl;
		return false;
	} else
	{
		while (std::getline(inFile, temp))
		{
			source += temp + "\n";
		}
	}

	inFile.close();

	// Set up and compile the shader from the 
	// fragment shader source file
	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource = source.c_str();

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Check for errors during compilation of the shader
	success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::cout << "Failed to compile the fragment shader" << std::endl;
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

		std::cout << infoLog << std::endl;

		return false;
	}

	// Set up the program
	program = glCreateProgram();

	// Attach and link the shaders to the program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	// Check for any errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		std::cout << "Failed to link the shaders to the program" << std::endl;
		glGetProgramInfoLog(vertexShader, 512, nullptr, infoLog);

		std::cout << infoLog << std::endl;

		return false;
	}


	// Clean up 
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return true;
}

struct Vertex
{
	Vertex(const glm::vec3 pos, const glm::vec3 col, const glm::vec2 uvs) : m_position(pos), m_colour(col), m_texcoord(uvs) {}
	glm::vec3 m_position;
	glm::vec3 m_colour;
	glm::vec2 m_texcoord;
};

int main()
{
	// Initialise GLFW
	glfwInit();

	// CREATE THE WINDOW
	int frameBufferWidth{ 0 };
	int frameBufferHeight{ 0 };

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(constants::k_screenWidth, constants::k_screenHeight, "3D Graphics Programming ICA 2", nullptr, nullptr);

	// Make sure the viewport changes to fit the newly resized window
	glfwSetFramebufferSizeCallback(window, on_frame_buffer_resize_callback);

	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	// the viewport is the area of the window that can be rendered to
	// it's the "canvas size" of the render
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);

	// Sets the window up for being rendered to
	glfwMakeContextCurrent(window);

	// Initialise GLEW with the window and OpenGL context
	glewExperimental = GL_FALSE;

	// check for errors
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialisation failed" << std::endl;
		glfwTerminate();
	}

	// The OpenGL Options

	// Allows us to use the Z Axis
	glEnable(GL_DEPTH_TEST);

	// Only draws faces that can be seen
	glEnable(GL_CULL_FACE);

	// GL_BACK means the back of the triangles
	glCullFace(GL_BACK);

	// The front faces are always read Counter-Clockwise
	// if vertices are Clockwise, OpenGL knows not to draw them
	glFrontFace(GL_CCW);

	// Allows for the blending of colours
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Initialise the shader program
	GLuint coreProgram;

	if (!load_shaders(coreProgram))
	{
		glfwTerminate();
	}


	// Basic triangle
	Vertex vertices[]
	{
		{ glm::vec3(-0.5f, 0.5f, 0.f),    glm::vec3(1.f, 0.f, 0.f),     glm::vec2(0.f, 1.f)},
		{ glm::vec3(-0.5f, -0.5f, 0.f),   glm::vec3(0.f, 1.f, 0.f),     glm::vec2(0.f, 0.f)},
		{ glm::vec3(0.5f, -0.5f, 0.f),    glm::vec3(0.f, 0.f, 1.f),     glm::vec2(1.f, 0.f)},
		{ glm::vec3(0.5f, 0.5f, 0.f),     glm::vec3(1.f, 1.f, 0.f),     glm::vec2(1.f, 1.f)}
	};

	unsigned numOfVertices = sizeof(vertices) / sizeof(Vertex);

	// Set up the indices to draw anticlockwise
	// The numbers refer to the indexes of the
	// vertices array
	GLuint indices[]
	{
		0, 1, 2,
		0, 2, 3
	};

	unsigned numOfIndices = sizeof(indices) / sizeof(GLuint);

	// Pass the triangle information into the shaders
	// We want to generate the VAO, VBO and EBO before the main program
	// since sending information from the CPU to the GPU is intensive
	// and very slow
	//
	//
	// Set up VAO, VBO and EBO
	// Generate and bind the VAO 
	GLuint vertexArrayObject;
	glCreateVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Generate and bind the VBO
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	// send the vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Generate and bind the EBO
	GLuint elementBufferObject;
	glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	// Send the indices data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set up and enable the vertex attribute pointers (input assembly)
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, m_position)));
	glEnableVertexAttribArray(0);

	// Colour
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, m_colour)));
	glEnableVertexAttribArray(1);

	// UVs (texcoords)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, m_texcoord)));
	glEnableVertexAttribArray(2);

	// Bind VAO 0
	glBindVertexArray(0);

	// Initialise texture
	// Create texture id
	GLuint texture0;

	// IMAGE ZERO
	int textureWidth = 0;
	int textureHeight = 0;
	unsigned char* image = SOIL_load_image("Data/alien.png", &textureWidth, &textureHeight, nullptr, SOIL_LOAD_RGBA);
	
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	
	// Configure options for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeats the image across the x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repeats the image across the y axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // as the camera gets closer
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // as the camera gets further away
	
	// Check if the image loaded correctly
	if(!image)
	{
		std::cout << "Error loading image" << std::endl;
	}else
	{
		// Create an OpenGL image from the pixel data loaded through SOIL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	// IMAGE ONE
	GLuint texture1;
	textureWidth = 0;
	textureHeight = 0;
	image = SOIL_load_image("Data/box.png", &textureWidth, &textureHeight, nullptr, SOIL_LOAD_RGBA);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Check if the image loaded correctly
	if (!image)
	{
		std::cout << "Error loading image" << std::endl;
	} else
	{
		// Create an OpenGL image from the pixel data loaded through SOIL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	glm::mat4 modelMatrix(1.f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f));

	// Camera vectors
	const glm::vec3 worldUpVector = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camPosition(0.f, 0.f, 2.f);
	glm::vec3 camFrontVector = glm::vec3(0.f, 0.f, -1.f);
	
	glm::mat4 viewMatrix(1.f);

	viewMatrix = glm::lookAt(camPosition, camPosition + camFrontVector, worldUpVector);

	const float fieldOfView = 90.f;
	const float nearPlane = 0.1f;
	const float farPlane = 5000.f;
	
	glm::mat4 projectionMatrix(1.f);

	projectionMatrix = glm::perspective(glm::radians(fieldOfView), static_cast<float>(frameBufferWidth) / static_cast<float>(frameBufferHeight), nearPlane, farPlane);
	
	// Send the matrices to the shaders
	glUseProgram(coreProgram);
	glUniformMatrix4fv(glGetUniformLocation(coreProgram, "model_matrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(coreProgram, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(coreProgram, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUseProgram(0);
	
	// The program loop
	while (!glfwWindowShouldClose(window))
	{
		// HANDLE INPUT

		// PollEvents() allows for the cursor to interact with the
		// newly created window, as well as handling other events
		// in the event queue
		glfwPollEvents();

		input(window);

		// HANDLE UPDATING THE SCENE

		// CLEAR THE SCREEN TO COLOUR

		// REMEMBER : OpenGL colours are in the range 0 - 1
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// HANDLE DRAWING TO THE SCREEN
		// Use a program
		glUseProgram(coreProgram);

		// Update the uniforms
		glUniform1i(glGetUniformLocation(coreProgram, "texture0"), 0); // setting to 0 to bind the current texture to texture location 0
		glUniform1i(glGetUniformLocation(coreProgram, "texture1"), 1); // setting to 1 to bind the current texture to texture location 1

		// Move, rotate and scale the model matrix
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(tan(translation)));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(1.f), glm::vec3(1.f, 1.f, 1.f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1));
		
		glUniformMatrix4fv(glGetUniformLocation(coreProgram, "model_matrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

		// Reset the projection matrix
		projectionMatrix = glm::mat4(1.f);
		// Calculate the new projection matrix
		projectionMatrix = glm::perspective(glm::radians(fieldOfView), static_cast<float>(frameBufferWidth) / static_cast<float>(frameBufferHeight), nearPlane, farPlane);
		glUniformMatrix4fv(glGetUniformLocation(coreProgram, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		
		glUniformMatrix4fv(glGetUniformLocation(coreProgram, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

		
		// Activate the textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		
		// Bind the vertex array objects
		glBindVertexArray(vertexArrayObject);

		// Draw to the screen
		//glDrawArrays(GL_TRIANGLES, 0, numOfIndices); // Draw arrays draws every point from our vertex array
		glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0); // Draw elements draws every point from our elements array

		
		// AFTER DRAWING....
		// swapping buffers allows the images to be shown to the user
		glfwSwapBuffers(window);
		glFlush();

		// Reset the bindings
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// At the end of the program
	glfwDestroyWindow(window);
	glfwTerminate();

	// Delete the openGL program
	glDeleteProgram(coreProgram);

	// Delete the VAOs and Buffers

	return 0;
}