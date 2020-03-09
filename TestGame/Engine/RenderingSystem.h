#pragma once

#include <vector>

#include "EngineTypes.h"
#include "BaseSystem.h"

namespace MustacheEngine
{
	class CameraComponent;
	class LightSourceComponent;
	class MeshRendererComponent;

	class RenderingSystem : public BaseSystem
	{
	private:
		GLFWwindow* m_Window;

		CameraComponent* m_ActiveCamera;
		LightSourceComponent* m_ActiveLightSource;
		std::vector<MeshRendererComponent*> m_MeshRenderers;

		GLuint VertexArrayID;
		GLuint Texture;
		GLuint TextureID;

	public:
		virtual void Initialize(BaseGame* owner) override;
		virtual void Update() override;
		virtual void Shutdown() override;

		void SetActiveCamera(CameraComponent*);

		void SetActiveLightSource(LightSourceComponent*);

		void AddMeshRenderer(MeshRendererComponent*);
		void RemoveMeshRenderer(MeshRendererComponent*);
		GLFWwindow* GetWindow() const { return m_Window; }
	};
}