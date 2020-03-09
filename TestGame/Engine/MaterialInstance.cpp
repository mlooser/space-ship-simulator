#include "MaterialInstance.h"

#include "MaterialDefinition.h"

using namespace MustacheEngine;

MaterialInstance::MaterialInstance(const std::shared_ptr<MaterialDefinition>& matDef, const TextureID& textureId, const TextureID& textureSamplerId)
	: m_MaterialDefinition(matDef),
	m_TextureId(textureId),
	m_TextureSamplerId(textureSamplerId)
{

}

ShaderProgramID MaterialInstance::GetProgramID() const
{
	return m_MaterialDefinition->GetShaderProgramID();
}

LocationID MaterialInstance::GetMVPMatrixID() const { return m_MaterialDefinition->GetMVPMatrixID(); }
LocationID MaterialInstance::GetViewMatrixID() const { return m_MaterialDefinition->GetViewMatrixID(); }
LocationID MaterialInstance::GetModelMatrixID() const { return m_MaterialDefinition->GetModelMatrixID(); }
LocationID MaterialInstance::GetLightID() const { return m_MaterialDefinition->GetLightID(); }

MaterialInstance::~MaterialInstance() = default;