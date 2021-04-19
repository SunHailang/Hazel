#pragma once

#include "Hazel/Core/Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Timestep.h"

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel 
{
	class Application
	{
	public:
		Application(const std::string& name = "Hazel App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastTime = 0.0f;

	private:
		static Application* s_Instance;

	};

	Application* CreateApplication();
}

