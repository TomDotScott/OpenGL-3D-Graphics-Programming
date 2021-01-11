#include "Game.h"

Game::Game(const std::string& title, const int width, const int height, const int glVersionMajor, const int glVersionMinor, const bool resizable) :
	m_window(nullptr),
	m_windowWidth(width),
	m_windowHeight(height),
	m_frameBufferWidth(m_windowWidth),
	m_frameBufferHeight(m_windowHeight),
	m_glVersionMajor(glVersionMajor),
	m_glVersionMinor(glVersionMinor),
	m_deltaTime(0.f),
	m_currentFrameTime(0.f),
	m_prevFrameTime(0.f),
	m_prevMouseX(0.0),
	m_prevMouseY(0.0),
	m_currentMouseX(0.0),
	m_currentMouseY(0.0),
	m_mouseOffsetX(0.0),
	m_mouseOffsetY(0.0),
	m_firstMouse(true),
	m_camera(
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f, 1.f, 0.f)
	),
	m_cameraDirection(eDirection::e_None),
	m_projectionMatrix(1.f),
	m_fov(90.f),
	m_nearPlane(0.1f),
	m_farPlane(1000.f)
{
	InitGLFW();
	InitWindow(title, resizable);
	InitGLEW();
	InitOpenGlOptions();

	InitMatrices();
	InitShaders();
	InitTextures();
	InitMaterials();
	InitMeshes();
	InitLights();
	InitUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();

	for (auto* shader : m_shaders)
	{
		delete shader;
	}

	for (auto* texture : m_textures)
	{
		delete texture;
	}

	for (auto* material : m_materials)
	{
		delete material;
	}

	for (auto* mesh : m_meshes)
	{
		delete mesh;
	}

	for (auto* light : m_lights)
	{
		delete light;
	}
}

//Accessor
int Game::GetWindowShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

//Modifier
void Game::SetWindowShouldClose() const
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

//Functions
void Game::Update()
{
	UpdateDeltaTime();
	UpdateInput();
	//Game::updateInput(window, *meshes[MESH_QUAD]);
}

void Game::Render()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	UpdateUniforms();

	m_materials[static_cast<int>(eMaterials::ALIEN_MATERIAL)]->SendToShader(*m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]);

	m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]->Use();

	m_textures[static_cast<int>(eTextures::BOX)]->Bind(0);
	m_textures[static_cast<int>(eTextures::BOX_SPECULAR)]->Bind(1);

	m_meshes[static_cast<int>(eMeshes::ALIEN)]->Render(*m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]);

	glfwSwapBuffers(m_window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::InitGLFW()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::InitWindow(const std::string& title, const bool resizable)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_glVersionMinor);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, title.c_str(), nullptr, nullptr);

	if (!m_window)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);
	glfwSetFramebufferSizeCallback(m_window, Game::FrameBufferResizeCallback);

	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::InitGLEW()
{
	glewExperimental = GL_TRUE;

	//Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::InitOpenGlOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::InitMatrices()
{
	m_projectionMatrix = glm::mat4(1.f);
	m_projectionMatrix = glm::perspective(
		glm::radians(m_fov),
		static_cast<float>(m_frameBufferWidth) / static_cast<float>(m_frameBufferHeight),
		m_nearPlane,
		m_farPlane
	);
}

void Game::InitShaders()
{
	m_shaders.push_back(new Shader(m_glVersionMajor, m_glVersionMinor, "vertex_core.glsl", "fragment_core.glsl"));
}

void Game::InitTextures()
{
	m_textures.push_back(new Texture("Data/alien.png", GL_TEXTURE_2D));
	m_textures.push_back(new Texture("Data/alien_specular.png", GL_TEXTURE_2D));
	m_textures.push_back(new Texture("Data/box.png", GL_TEXTURE_2D));
	m_textures.push_back(new Texture("Data/box_specular.png", GL_TEXTURE_2D));
}

void Game::InitMaterials()
{
	m_materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 1));
}

void Game::InitMeshes()
{
	m_meshes.push_back(
		new Mesh(
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f),
			ePrimitiveType::e_Cube
		)
	);

	m_meshes.push_back(
		new Mesh(
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f),
			ePrimitiveType::e_Cube
		)
	);
}

void Game::InitLights()
{
	m_lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::InitUniforms()
{
	m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]->SetMat4Fv(m_camera.GetViewMatrix(), "view_matrix");
	m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]->SetMat4Fv(m_projectionMatrix, "projection_matrix");

	m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]->SetVec3F(*m_lights[static_cast<int>(eLights::MAIN_LIGHT)], "light_position");
}

void Game::UpdateDeltaTime()
{
	m_currentFrameTime = static_cast<float>(glfwGetTime());
	m_deltaTime = m_currentFrameTime - m_prevFrameTime;
	m_prevFrameTime = m_currentFrameTime;
}

void Game::UpdateUniforms()
{
	// Update the view matrix 
	const glm::mat4 viewMatrix = m_camera.GetViewMatrix();
	m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]->SetVec3F(m_camera.GetPosition(), "camera_position");

	m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]->SetMat4Fv(viewMatrix, "view_matrix");

	glfwGetFramebufferSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);

	m_projectionMatrix = glm::perspective(
		glm::radians(m_fov),
		static_cast<float>(m_frameBufferWidth) / static_cast<float>(m_frameBufferHeight),
		m_nearPlane,
		m_farPlane
	);

	m_shaders[static_cast<int>(eShaders::CORE_PROGRAM)]->SetMat4Fv(m_projectionMatrix, "projection_matrix");
}

void Game::FrameBufferResizeCallback(GLFWwindow* window, const int frameBufferWidth, const int frameBufferHeight)
{
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
};

void Game::UpdateInput()
{
	glfwPollEvents();
	KeyBoardInput();
	MouseInput();

	m_camera.Update(static_cast<float>(m_mouseOffsetX), static_cast<float>(m_mouseOffsetY), true);
}

void Game::KeyBoardInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		SetWindowShouldClose();
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_camera.Move(m_deltaTime, eDirection::e_Forward);
	}
	
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_camera.Move(m_deltaTime, eDirection::e_Backward);
	}
	
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_camera.Move(m_deltaTime, eDirection::e_Left);
	}
	
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_camera.Move(m_deltaTime, eDirection::e_Right);
	}
}

void Game::MouseInput()
{
	glfwGetCursorPos(m_window, &m_currentMouseX, &m_currentMouseY);

	if (m_firstMouse)
	{
		m_prevMouseX = m_currentMouseX;
		m_prevMouseY = m_currentMouseY;
		m_firstMouse = false;
	}

	m_mouseOffsetX = m_currentMouseX - m_prevMouseX;
	m_mouseOffsetY = m_currentMouseY - m_prevMouseY;

	m_prevMouseX = m_currentMouseX;
	m_prevMouseY = m_currentMouseY;
}
