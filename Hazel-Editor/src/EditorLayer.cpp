#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuizmo.h"

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
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Hazel::Framebuffer::Create(fbSpec);

		//m_CameraController.SetZoomLevel(5.0f);

		m_ActiveScene = Hazel::CreateRef<Hazel::Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
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
#endif
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

		m_EditorCamera.SetViewportSize(m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Height);

		// Update
		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);

			m_EditorCamera.OnUpdate(ts);
		}

		// Renderer
		Hazel::Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		Hazel::RendererCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		Hazel::RendererCommand::Clear();

		//Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 2.0f, 2.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
		//Hazel::Renderer2D::EndScene();

		// Update Scene
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			m_Framebuffer->ReadPixel(1, mouseX, mouseY);
		}

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
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}
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
				auto viewportOffset = ImGui::GetCursorPos(); // Include tab bar

				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
				{
					m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
					m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

					m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
				}
				uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
				ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

				auto windowSize = ImGui::GetWindowSize();
				auto minBound = ImGui::GetWindowPos();
				minBound.x += viewportOffset.x;
				minBound.y += viewportOffset.y;

				ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
				m_ViewportBounds[0] = { minBound.x, minBound.y };
				m_ViewportBounds[1] = { maxBound.x, maxBound.y };

				// ImGuizmo
				Entity selectEntity = m_SceneHierarchyPanel.GetSelectEntity();
				if (selectEntity && m_GizmoType >= 0)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();
					float windowWidth = (float)ImGui::GetWindowWidth();
					float windowHeight = (float)ImGui::GetWindowHeight();
					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

					// Camera
					// Runtime camera from entity
					//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
					//const auto& camera = cameraEntity.GetComponent<CameraComponent>();
					//const glm::mat4& cameraProjection = camera.Camera.GetProjection();
					//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

					// Editor camera
					const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
					glm::mat4 cameraView = m_EditorCamera.GetViewMartix();


					// Entity transform
					auto& tc = selectEntity.GetComponent<TransformComponent>();
					glm::mat4 transform = tc.GetTransform();

					// Snapping
					bool snap = Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL);
					float snapValue = 0.5f; // Snap to 0.5m for translation/scale
					// Snap to 45 degrees for rotation
					if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
						snapValue = 45.0f;

					float snapValues[3] = { snapValue, snapValue, snapValue };

					ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
						(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
						nullptr, snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing())
					{
						glm::vec3 translation, rotation, scale;
						Math::DecomposeTransform(transform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - tc.Rotation;
						tc.Translation = translation;
						tc.Rotation += deltaRotation;
						tc.Scale = scale;
					}
				}


				ImGui::End();
				ImGui::PopStyleVar();
			}
		}

		ImGui::End();
	}

	void EditorLayer::OnEvent(Hazel::Event& event)
	{
		m_CameraController.OnEvent(event);
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		// Shortcuts
		if (event.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL) || Input::IsKeyPressed(HZ_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT) || Input::IsKeyPressed(HZ_KEY_RIGHT_SHIFT);
		switch (event.GetKeyCode())
		{
		case HZ_KEY_N:
		{
			if (control) NewScene();
		}
		break;
		case HZ_KEY_O:
		{
			if (control) OpenScene();
		}
		break;
		case HZ_KEY_S:
		{
			if (control && shift) SaveSceneAs();
		}
		break;

		// Gizmos
		case HZ_KEY_Q:
		{
			m_GizmoType = ImGuizmo::OPERATION::BOUNDS;
		}
		break;
		case HZ_KEY_W:
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		break;
		case HZ_KEY_E:
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		break;
		case HZ_KEY_R:
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
		break;
		}
		return true;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == HZ_MOUSE_BUTTON_1)
		{
			//if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
			//	m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filePath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserializer(filePath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filePath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filePath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serializer(filePath);
		}
	}

}
