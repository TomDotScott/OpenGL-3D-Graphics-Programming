#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>


#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"

enum class eShaders { CORE_PROGRAM = 0 };
enum class eTextures { ALIEN, ALIEN_SPECULAR, BOX, BOX_SPECULAR };
enum class eMaterials { ALIEN_MATERIAL = 0 };
enum class eMeshes { ALIEN = 0 };
enum class eLights { MAIN_LIGHT = 0 };

class Game
{
public:
	Game(
		const std::string& title,
		int width, 
		int height,
		int glVersionMajor, 
		int glVersionMinor,
		bool resizable
	);

	~Game();

	void Update();
	void Render();
	
	int GetWindowShouldClose() const;
	void SetWindowShouldClose() const;

	static void FrameBufferResizeCallback(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight);
private:
	GLFWwindow* m_window;
	const int m_windowWidth;
	const int m_windowHeight;
	int m_frameBufferWidth;
	int m_frameBufferHeight;

	const int m_glVersionMajor;
	const int m_glVersionMinor;

	float m_deltaTime;
	float m_currentFrameTime;
	float m_prevFrameTime;

	double m_prevMouseX;
	double m_prevMouseY;
	double m_currentMouseX;
	double m_currentMouseY;
	double m_mouseOffsetX;
	double m_mouseOffsetY;
	bool m_firstMouse;
	
	Camera m_camera;
	eDirection m_cameraDirection;

	glm::mat4 m_projectionMatrix;
	float m_fov;
	float m_nearPlane;
	float m_farPlane;

	std::vector<Shader*> m_shaders;
	std::vector<Texture*> m_textures;
	std::vector<Material*> m_materials;
	std::vector<Mesh*> m_meshes;
	std::vector<glm::vec3*> m_lights;

	void InitGLFW();
	void InitWindow(const std::string& title, bool resizable);
	void InitGLEW();
	void InitOpenGlOptions();
	void InitMatrices();
	void InitShaders();
	void InitTextures();
	void InitMaterials();
	void InitMeshes();
	void InitLights();
	void InitUniforms();

	void UpdateDeltaTime();
	void UpdateUniforms();
	void UpdateInput();
	void KeyBoardInput();
	void MouseInput();
};