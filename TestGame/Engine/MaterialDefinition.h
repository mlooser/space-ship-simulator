#pragma once

#include <string>

#include "EngineTypes.h"


namespace MustacheEngine
{
	class MaterialDefinition
	{
	private:
		ShaderProgramID m_ShaderProgramId;
		//maybe compiled data may be here later
		LocationID m_MVPMatrixID;
		LocationID m_ViewMatrixID;
		LocationID m_ModelMatrixID;
		LocationID m_LightID;

	public:
		MaterialDefinition(
			const ShaderProgramID& shaderProgramId,
			const LocationID& mvpMatrixID,
			const LocationID& viewMatrixID,
			const LocationID& modelMatrixID,
			const LocationID& lightID
		)
			: m_ShaderProgramId(shaderProgramId)
			, m_MVPMatrixID(mvpMatrixID)
			, m_ViewMatrixID(viewMatrixID)
			, m_ModelMatrixID(modelMatrixID)
			, m_LightID(lightID)
		{}

		ShaderProgramID GetShaderProgramID() const { return m_ShaderProgramId; }

		LocationID GetMVPMatrixID() { return m_MVPMatrixID; }
		LocationID GetViewMatrixID() { return m_ViewMatrixID; }
		LocationID GetModelMatrixID() { return m_ModelMatrixID; }
		LocationID GetLightID() { return m_LightID; }
	};
}