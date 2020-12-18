#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Constants.h"
#include <fstream>
#include <string>

void input(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
	if(!inFile.is_open())
	{
		std::cout << "Failed to open the vertex shader" << std::endl;
		return false;
	}else
	{
		while(std::getline(inFile, temp))
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
	if(!success)
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
	const GLchar * fragmentShaderSource = source.c_str();

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
	
	if(!success)
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
	if(glewInit() != GLEW_OK)
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
	
	if(!load_shaders(coreProgram))
	{
		glfwTerminate();
	}

	// The program loop
	while(!glfwWindowShouldClose(window))
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
		
		// AFTER DRAWING....
		// swapping buffers allows the images to be shown to the user
		glfwSwapBuffers(window); 
		glFlush();
	}

	// At the end of the program
	glfwDestroyWindow(window);
	glfwTerminate();

	// Delete the openGL program
	glDeleteProgram(coreProgram);

	// Delete the VAOs and Buffers

	return 0;	
}