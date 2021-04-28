#pragma once

#include "Hazel.h"

#include "Panels/SceneHierarchyPanel.h"

namespace Hazel
{

	class EditorLayer : public Hazel::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Hazel::Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Hazel::Event& event) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		Hazel::OrthographicCameraController m_CameraController;

		Hazel::Ref<Scene> m_ActiveScene;
		Hazel::Entity m_CameraEntity;
		Hazel::Entity m_SecondCameraEntity;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;
		// Temp
		Hazel::Ref<Hazel::VertexArray> m_SquareVA;
		Hazel::Ref<Hazel::Shader> m_FlatColorShader;

		Hazel::Ref<Hazel::Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}