#include "Sandbox2D.h"


#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	:Hazel::Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_texture = Hazel::Texture2D::Create("assets/textures/wall.jpg");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Renderer
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RendererCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		Hazel::RendererCommand::Clear();
	}
	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Hazel::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

		//Hazel::Renderer2D::DrawRotatedQuad({ 0.2f,0.5f,-0.1f }, { 10.0f, 10.0f }, glm::radians(45.0f), m_texture, 10.0f, {0.0f, 0.9f, 0.9f, 1.0f});

		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Setting");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (auto& result : m_ProfileResults)
	{
		//char label[50];
		//strcpy(label, result.Name);
		//strcat(label, " %.3fms");
		//ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}
