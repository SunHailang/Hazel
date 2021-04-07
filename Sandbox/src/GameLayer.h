#pragma once

#include <Hazel.h>

#include "Level.h"
#include <imgui/imgui.h>

class GameLayer : public Hazel::Layer
{
public:
	GameLayer();
	~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& event) override;

	bool OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event);
	bool OnWindowResize(Hazel::WindowResizeEvent& event);

private:
	void CreateCamera(uint32_t width, uint32_t height);

private:
	Hazel::Scope<Hazel::OrthographicCamera> m_Camera;
	Level m_Level;
	ImFont* m_Font;
	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play = 0, MainMenu = 1, GameOver = 2
	};

	GameState m_State = GameState::MainMenu;
};
