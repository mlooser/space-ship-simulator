#pragma once

#include <memory>

#include "EngineTypes.h"

namespace MustacheEngine
{
	class MaterialDefinition;

	class MaterialInstance
	{
	private:
		std::shared_ptr<MaterialDefinition> m_MaterialDefinition;
		TextureID m_TextureId;
		TextureID m_TextureSamplerId;

	public:
		MaterialInstance(const std::shared_ptr<MaterialDefinition>& matDef, const TextureID& textureId, const TextureID& textureSamplerId);
		~MaterialInstance();

		ShaderProgramID GetProgramID() const;

		TextureID GetTextureId()const { return m_TextureId; }
		TextureID GetTextureSamplerId()const { return m_TextureSamplerId; }

		LocationID GetMVPMatrixID() const;
		LocationID GetViewMatrixID() const;
		LocationID GetModelMatrixID() const;
		LocationID GetLightID() const;
	};
}