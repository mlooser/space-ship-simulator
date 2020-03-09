#include "MeshRendererComponent.h"

#include "BaseGame.h"
#include "RenderingSystem.h"
#include "MaterialInstance.h"
#include "Mesh.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace MustacheEngine;

MeshRendererComponent::MeshRendererComponent(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<MaterialInstance>& materialInstance)
	: m_Mesh(mesh)
	, m_MaterialInstance(materialInstance)
	, m_ShaderProgramId(materialInstance->GetProgramID())
{
}

void MeshRendererComponent::OnAttached(GameObject* owner)
{
	Component::OnAttached(owner);

	m_Transform = owner->GetComponent<TransformComponent>();	

	glGenBuffers(1, &m_VertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_Mesh->Vertices.size() * sizeof(glm::vec3), &m_Mesh->Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_UVBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_UVBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_Mesh->UVs.size() * sizeof(glm::vec2), &m_Mesh->UVs[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &m_NormalBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_Mesh->Normals.size() * sizeof(glm::vec3), &m_Mesh->Normals[0], GL_STATIC_DRAW);	

	m_TextureId = m_MaterialInstance->GetTextureId();
	m_TextureSamplerId = m_MaterialInstance->GetTextureSamplerId();
}

void MeshRendererComponent::Activate()
{
	BaseGame::GetInstance()->GetSystem<RenderingSystem>()->AddMeshRenderer(this);
}

void MeshRendererComponent::Deactivate()
{
	BaseGame::GetInstance()->GetSystem<RenderingSystem>()->RemoveMeshRenderer(this);
}

MeshRendererComponent::~MeshRendererComponent()
{
	glDeleteBuffers(1, &m_VertexBufferId);
	glDeleteBuffers(1, &m_UVBufferId);
	glDeleteBuffers(1, &m_NormalBufferId);

	BaseGame::GetInstance()->GetSystem<RenderingSystem>()->RemoveMeshRenderer(this);
}

void MeshRendererComponent::Render(RenderingSystem* renderingSystem, const Matrix4& vp, const Matrix4& viewMatrix, const Vector3& lightPosition)
{
	glUseProgram(m_ShaderProgramId);

	Matrix4 modelMatrix = m_Transform->CalculateModelMatrix();

	Matrix4 mvp = vp *modelMatrix;		

	glUniformMatrix4fv(m_MaterialInstance->GetMVPMatrixID(), 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(m_MaterialInstance->GetModelMatrixID(), 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(m_MaterialInstance->GetViewMatrixID(), 1, GL_FALSE, &viewMatrix[0][0]);
	
	glUniform3f(m_MaterialInstance->GetLightID(), lightPosition.x, lightPosition.y, lightPosition.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);	
	glUniform1i(m_TextureSamplerId, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_UVBufferId);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 3nd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBufferId);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, m_Mesh->Vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
