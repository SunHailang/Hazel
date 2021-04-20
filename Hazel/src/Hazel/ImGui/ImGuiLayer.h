#pragma once

#include "Hazel/Core/Layer.h"

namespace Hazel
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}
