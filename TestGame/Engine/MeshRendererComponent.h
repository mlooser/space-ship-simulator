#pragma once

#include <memory>

#include "Component.h"
#include "EngineTypes.h"

namespace MustacheEngine
{
	class Mesh;
	class MaterialInstance;
	class RenderingSystem;
	class TransformComponent;

	class MeshRendererComponent : public Component
	{
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<MaterialInstance> m_MaterialInstance;

		TransformComponent* m_Transform;

		ShaderProgramID m_ShaderProgramId;
		TextureID m_TextureId;
		TextureID m_TextureSamplerId;
	
		BufferID m_VertexBufferId;
		BufferID m_UVBufferId;
		BufferID m_NormalBufferId;

	public:
		MeshRendererComponent(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<MaterialInstance>& materialInstance);
		~MeshRendererComponent();

		virtual void OnAttached(GameObject* owner) override;
		void Render(RenderingSystem* renderingSystem, const Matrix4& vp, const Matrix4& v, const Vector3& lightPosition);

	protected:
		virtual void Activate() override;
		virtual void Deactivate() override;
	};
}