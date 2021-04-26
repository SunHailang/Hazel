#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace Hazel
{
	EditorLayer::EditorLayer()
		:Hazel::Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
	{

	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();


		Hazel::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Hazel::Framebuffer::Create(fbSpec);

		//m_CameraController.SetZoomLevel(5.0f);

		m_ActiveScene = Hazel::CreateRef<Hazel::Scene>();

		auto square = m_ActiveScene->CreateEntity("Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.2f, 0.0f, 1.0f });

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCameraEntity = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCameraEntity.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{

			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;
				if (Input::IsKeyPressed(HZ_KEY_A))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(HZ_KEY_D))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(HZ_KEY_W))
					translation.y -= speed * ts;
				if (Input::IsKeyPressed(HZ_KEY_S))
					translation.y += speed * ts;

			}
		};
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		// Panels
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Hazel::Timestep ts)
	{
		HZ_PROFILE_FUNCTION();


		// Resize
		m_ActiveScene->OnViewportResize(m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Height);

		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Renderer
		Hazel::Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		Hazel::RendererCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		Hazel::RendererCommand::Clear();

		//Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 2.0f, 2.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
		//Hazel::Renderer2D::EndScene();

		// Update Scene
		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 180.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) HZ_INFO("Menu File Item New");
				ImGui::Separator();
				if (ImGui::MenuItem("Open")) HZ_INFO("Menu File Item Open");
				ImGui::Separator();
				if (ImGui::MenuItem("Save")) HZ_INFO("Menu File Item Save");
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) Hazel::Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Options")) HZ_INFO("Menu View Item Options");
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		{
			m_SceneHierarchyPanel.OnImGuiRender();

			{
				ImGui::Begin("Stats");
				auto stats = Hazel::Renderer2D::GetStats();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertics: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
				ImGui::End();
			}

			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
				ImGui::Begin("Viewport");

				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
				{
					m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
					m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

					m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
				}
				uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
				ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::End();
				ImGui::PopStyleVar();
			}
		};

		ImGui::End();
	}

	void EditorLayer::OnEvent(Hazel::Event& event)
	{
		m_CameraController.OnEvent(event);
	}
}