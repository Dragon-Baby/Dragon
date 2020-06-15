#pragma once

#include "Dragon/Layer.h"

namespace Dragon
{
	class DRAGON_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		float m_Time;
	};
}
