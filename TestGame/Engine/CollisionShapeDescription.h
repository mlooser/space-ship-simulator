#pragma once

#include <string>
#include <sstream>

#include "EngineTypes.h"

namespace MustacheEngine
{
	struct CollisionShapeDescription
	{
		enum class EShapeType
		{
			_Undefined,
			Box,
			Cylinder
		};

		EShapeType Type = EShapeType::_Undefined;
		Vector3 Size;

		bool IsValid()
		{
			return Type != EShapeType::_Undefined;
		}

		std::string GetDescriptionAsString() const
		{
			std::stringstream stream{};
			stream
				<< "t:"
				<< (int)Type
				<< ";"
				<< Size.x
				<< ";"
				<< Size.y
				<< ";"
				<< Size.z;


			return stream.str();
		}

	};
}