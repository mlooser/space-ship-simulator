#pragma once

#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace MustacheEngine
{
	using Vector3 = glm::vec3;
	using Vector2 = glm::vec2;
	using Quaternion = glm::quat;

	using Matrix4 = glm::mat4;

	using ShaderProgramID = GLuint;
	using TextureID = GLuint;
	using BufferID = GLuint;
	using LocationID = GLuint;


	using TimePoint = std::chrono::high_resolution_clock::time_point;
}