#include "RenderingSystem.h"

#include <stdexcept>
#include <algorithm>

#include "MeshRendererComponent.h"
#include "BaseGame.h"
#include "CameraComponent.h"
#include "LightSourceComponent.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace MustacheEngine;

void RenderingSystem::Initialize(BaseGame* owner)
{
	BaseSystem::Initialize(owner);

	if (!glfwInit())
	{
		throw std::runtime_error("Can't initialize GLFW.");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (m_Owner->GetConfig().RunInFullScreen)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		m_Window = glfwCreateWindow(mode->width, mode->height, m_Owner->GetTitle().c_str(), monitor, NULL);
	}
	else
	{
		m_Window = glfwCreateWindow(1024, 768, m_Owner->GetTitle().c_str(), NULL, NULL);
	}

	if (m_Window == nullptr)
	{
		throw std::runtime_error("Failed to open GLFW window.");
	}

	glfwMakeContextCurrent(m_Window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Failed to initialize GLEW\n");
	}

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void RenderingSystem::Update()
{
	BaseSystem::Update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewMatrix = m_ActiveCamera->CalculateView();
	glm::mat4 projectionMatrix = m_ActiveCamera->CalculateProjection();
	glm::mat4 vp = projectionMatrix * viewMatrix;

	Vector3 lightPosition{};
	if (m_ActiveLightSource != nullptr)
	{
		lightPosition = m_ActiveLightSource->GetLightPosition();
	}

	for (MeshRendererComponent* meshRenderer : m_MeshRenderers)
	{
		meshRenderer->Render(this, vp, viewMatrix, lightPosition);
	}

	glfwSwapBuffers(m_Window);

	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(m_Window) != 0)
	{
		m_Owner->QuitGame();
	}
}

void RenderingSystem::Shutdown()
{
	BaseSystem::Shutdown();

	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();
}

void RenderingSystem::SetActiveLightSource(LightSourceComponent* lightSource)
{
	if (m_ActiveLightSource != nullptr && lightSource != nullptr)
	{
		throw std::logic_error("Active light source already set");//no support for multiple lights
	}
	m_ActiveLightSource = lightSource;
}

void RenderingSystem::SetActiveCamera(CameraComponent* cam)
{
	if (m_ActiveCamera != nullptr && cam != nullptr)
	{
		throw std::logic_error("Active camera already set");//no support for camera stack
	}
	m_ActiveCamera = cam;
}

void RenderingSystem::AddMeshRenderer(MeshRendererComponent* meshRenderer)
{
	m_MeshRenderers.push_back(meshRenderer);
}

void RenderingSystem::RemoveMeshRenderer(MeshRendererComponent* meshRenderer)
{
	m_MeshRenderers.erase(
		std::find(m_MeshRenderers.begin(), m_MeshRenderers.end(), meshRenderer)//TODO change to remove?
		);
}