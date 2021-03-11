#include <Hazel.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		:Hazel::Layer("Example"),
		m_CameraController(1280.0f / 720.0f, true),
		m_SquareColor(0.0f)
	{
		m_VertexArraty.reset(Hazel::VertexArray::Create());

		float vertics[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertics, sizeof(vertics)));

		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArraty->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArraty->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
	
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
		m_Shader = Hazel::Shader::Create("VertexColorTriangle", vertexSrc, fragmentSrc);

		// ----------------------------------------

		m_SquareVA.reset(Hazel::VertexArray::Create());

		float verticsSquare[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> vertexBufferSquare;
		vertexBufferSquare.reset(Hazel::VertexBuffer::Create(verticsSquare, sizeof(verticsSquare)));

		Hazel::BufferLayout layoutSquare = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		};
		vertexBufferSquare->SetLayout(layoutSquare);
		m_SquareVA->AddVertexBuffer(vertexBufferSquare);

		uint32_t indicesSquare[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> indexBufferSquare;
		indexBufferSquare.reset(Hazel::IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(indexBufferSquare);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");



		m_TextureWall = Hazel::Texture2D::Create("assets/textures/wall.jpg");
		m_TextureBox = Hazel::Texture2D::Create("assets/textures/box.png");

	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		Hazel::RendererCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		Hazel::RendererCommand::Clear();


		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

		/*Hazel::MaterialRef material = new Metrial(m_FlatColorShader);
		Hazel::MetrialInstanceRef material = new MetrialInstance(material);

		material->SetValue("u_Color", readColor);
		material->SetTexture("u_AlbedoMap", texture);
		squareMesh->SetMaterial(material);*/

		m_TextureBox->Bind();
		auto textureShader = m_ShaderLibrary.Get("Texture");
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
		Hazel::Renderer::Submit(textureShader, m_SquareVA,
			glm::translate(glm::mat4(1.0), glm::vec3(0.25f, -0.25f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_TextureWall->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
		Hazel::Renderer::Submit(textureShader, m_SquareVA);




		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_SquareColor);
		Hazel::Renderer::Submit(m_Shader, m_VertexArraty, transform * scale);




		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
	{
		return false;
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArraty;

	//Hazel::Ref<Hazel::Shader> m_ShaderSquare;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_TextureWall;
	Hazel::Ref<Hazel::Texture2D> m_TextureBox;

	Hazel::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor;

};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}

private:
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}