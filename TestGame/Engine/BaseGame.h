#pragma once

#include <memory>
#include <vector>
#include <string>

#include "EngineMacros.h"

namespace MustacheEngine
{
	class BaseSystem;
	class MeshFactory;
	class MaterialInstanceFactory;

	struct BaseConfig
	{
		bool RunInFullScreen = true;
		bool HideCursor = true;
	};

	class BaseGame
	{
	private:
		static BaseGame* s_Instance;

		BaseConfig m_Config;
		std::string m_GameTitle;
		std::vector<std::unique_ptr<BaseSystem>> m_Systems;
		bool m_ShouldQuit = false;

		std::unique_ptr<MeshFactory> m_MeshFactory;
		std::unique_ptr<MaterialInstanceFactory> m_MaterialInstanceFactory;

	public:
		BaseGame();
		virtual ~BaseGame();

		PREVENT_COPY_AND_MOVE(BaseGame);

		void Run();
		void QuitGame() { m_ShouldQuit = true; }

		static BaseGame* GetInstance()
		{
			return s_Instance;
		}

		template<class TSystem>
		TSystem* GetSystem()
		{
			for (auto& it = m_Systems.begin(); it != m_Systems.end(); ++it)
			{
				if (TSystem* sys = dynamic_cast<TSystem*>(it->get()))
				{
					return sys;
				}
			}
			return nullptr;
		}

		MeshFactory* GetMeshFactory() { return m_MeshFactory.get(); }
		MaterialInstanceFactory* GetMaterialInstanceFactory() { return m_MaterialInstanceFactory.get(); }

		void SetTitle(const std::string& title) { m_GameTitle = title; }
		const std::string& GetTitle() const { return m_GameTitle; }

		const BaseConfig& GetConfig() const { return m_Config; }

	private:
		void InitializeFactories();
		void ReadConfig();

	protected:
		virtual void InitializeSystems();
		virtual void StartGame() {}
		virtual void Update();
		virtual void ShutDown() {}
	};
}
