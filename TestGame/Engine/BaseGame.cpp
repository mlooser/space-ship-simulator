#include <iostream>
#include <algorithm>

#include "BaseGame.h"
#include "BaseSystem.h"
#include "RenderingSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "UpdateManager.h"
#include "Logger.h"
#include "ConfigReader.h"

#include "MeshFactory.h"
#include "MaterialInstanceFactory.h"


using namespace MustacheEngine;

BaseGame* BaseGame::s_Instance;

BaseGame::BaseGame() = default;
BaseGame::~BaseGame() = default;

void BaseGame::Run()
{
	ReadConfig();

	s_Instance = this;

	InitializeFactories();
	InitializeSystems();
	StartGame();

	while (!m_ShouldQuit)
	{
		Update();
	}
	
	ShutDown();
	
	s_Instance = nullptr;
}

void BaseGame::InitializeSystems()
{
 	m_Systems.push_back(std::make_unique<InputSystem>());
 	m_Systems.push_back(std::make_unique<UpdateManager>());
 	m_Systems.push_back(std::make_unique<PhysicsSystem>());
	m_Systems.push_back(std::make_unique<RenderingSystem>());

	std::for_each(m_Systems.begin(), m_Systems.end(), [this](auto& s) {s->Initialize(this); });
	std::for_each(m_Systems.begin(), m_Systems.end(), [](auto& s) {s->PostInitialize(); });
}

void BaseGame::ReadConfig()
{
	ConfigReader cfg;
	cfg.ReadConfig("Resources/BaseConfig.txt");

	m_Config.RunInFullScreen = cfg.GetBool("Base.RunInFullScreen");
	m_Config.HideCursor = cfg.GetBool("Base.HideCursor");
}

void BaseGame::Update()
{
	std::for_each(m_Systems.begin(), m_Systems.end(), [](auto& s) {s->Update(); });
}

void BaseGame::InitializeFactories()
{
	m_MeshFactory = std::make_unique<MeshFactory>();
	m_MaterialInstanceFactory = std::unique_ptr<MaterialInstanceFactory>(new MaterialInstanceFactory());
}