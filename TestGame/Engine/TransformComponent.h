#pragma once

#include "Component.h"
#include "EngineTypes.h"

namespace MustacheEngine
{
	class TransformComponent : public Component
	{
	private:
		Vector3 m_Position;
		Vector3 m_Scale;
		Quaternion m_Rotation;

	public:
		TransformComponent()
			: TransformComponent({ 0,0,0 }, { 0,0,0 }, { 1,1,1 })
		{
		}

		TransformComponent(const Vector3& position)
			: TransformComponent(position, { 0,0,0 }, { 1,1,1 })
		{
		}

		TransformComponent(const Vector3& position, const Vector3& rotation)
			: TransformComponent(position, rotation, { 1,1,1 })
		{
		}

		TransformComponent(const Vector3& position, const Vector3& rotation, const Vector3& scale)
			: m_Position(position)
			, m_Rotation(rotation)
			, m_Scale(scale)

		{
		}

		Matrix4 CalculateModelMatrix() const;

		const Vector3& GetPosition() const { return m_Position; }
		void SetPosition(const Vector3& position) { m_Position = position; }

		void SetRotation(const Vector3& eulerAngles) { m_Rotation = Quaternion(eulerAngles); }
		void SetRotation(const Quaternion& rotation) { m_Rotation = rotation; }
		const Quaternion& GetRotation() const { return m_Rotation; }
		Vector3 GetRotationEulerAngles() const { return glm::eulerAngles(m_Rotation); }

		void SetScale(const Vector3& scale) { m_Scale = scale; }

		void Move(const Vector3& moveBy)
		{
			m_Position += moveBy;
		}

		void Rotate(const Quaternion& rotation)
		{
			m_Rotation = rotation * m_Rotation;
		}
	};
}