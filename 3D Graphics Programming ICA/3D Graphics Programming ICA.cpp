#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Constants.h"

void on_frame_buffer_resize_callback(GLFWwindow* window, const int frameBufferWidth, const int frameBufferHeight)
{
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
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

	// The program loop
	while(!glfwWindowShouldClose(window))
	{
		// HANDLE INPUT

		// PollEvents() allows for the cursor to interact with the
		// newly created window, as well as handling other events
		// in the event queue
		glfwPollEvents();
		
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


	glfwTerminate();
	
}