#pragma once

#include <chrono>

#include "BaseSystem.h"
#include "Event.h"
#include "EngineTypes.h"

namespace MustacheEngine
{
	class UpdateManager : public BaseSystem
	{
	public:
		using UpdateBinding = EventBinding<float>;
		using UpdateCallback = std::function<float>;

	private:
		using MSDuration = std::chrono::duration<float, std::milli>;

		std::chrono::high_resolution_clock m_Timer;
		TimePoint m_PrevTime;

	public:
		EventPtr<float> OnUpdate;

		UpdateManager()
			:OnUpdate(std::make_shared<Event<float>>())
		{

		}

		virtual void Initialize(BaseGame* owner) override;
		virtual void Update() override;
		TimePoint Now() const { return m_Timer.now(); }
	};
}
