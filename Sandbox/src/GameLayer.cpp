#include "GameLayer.h"

#include "Random.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GameLayer::GameLayer()
	:Hazel::Layer("GameLayer")
{
	auto& window = Hazel::Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());

	Random::Init();
}

void GameLayer::OnAttach()
{
	m_Level.Init();

	ImGuiIO io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 30.f);
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(Hazel::Timestep ts)
{
	m_Time += ts;
	if ((int)(m_Time*10.0f) % 8 > 4)
		m_Blink = !m_Blink;

	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;

	const auto& playerPos = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ playerPos.x, playerPos.y,0.0f });

	switch (m_State)
	{
	case GameState::Play:
		m_Level.OnUpdate(ts);
		break;
	}

	//Render
	Hazel::RendererCommand::SetClearColor({ 0.0f,0.0f,0.0f,1.0f });
	Hazel::RendererCommand::Clear();

	Hazel::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	Hazel::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	switch (m_State)
	{
	case GameLayer::GameState::Play:
	{
		uint32_t playerScore = m_Level.GetPlayer().GetScore();
		std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
		break;
	}
	case GameLayer::GameState::MainMenu:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;
		if (m_Blink) ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
		break;
	}
	case GameLayer::GameState::GameOver:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;
		if (m_Blink) ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

		pos.x += 200.0f;
		pos.y += 150.0f;
		uint32_t playerScore = m_Level.GetPlayer().GetScore();
		std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
		break;
	}
	}
}

void GameLayer::OnEvent(Hazel::Event& event)
{
	Hazel::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<Hazel::MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event)
{
	if (m_State == GameState::GameOver)
		m_Level.Reset();

	m_State = GameState::Play;
	return false;
}

bool GameLayer::OnWindowResize(Hazel::WindowResizeEvent& event)
{
	CreateCamera(event.GetWidth(), event.GetHeight());
	return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / (float)height;

	float camWidth = 8.0f;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom*aspectRatio;
	float right = top*aspectRatio;
	m_Camera = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
}

