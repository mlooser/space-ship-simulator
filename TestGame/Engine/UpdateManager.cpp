#include "UpdateManager.h"

using namespace MustacheEngine;

void UpdateManager::Initialize(BaseGame* owner)
{
	BaseSystem::Initialize(owner);
	m_PrevTime = m_Timer.now();
}

void UpdateManager::Update()
{
	BaseSystem::Update();

	auto now = m_Timer.now();
	float deltaTime = std::chrono::duration_cast<MSDuration>(now - m_PrevTime).count()/1000;
	m_PrevTime = now;
	OnUpdate->Invoke(deltaTime);
}