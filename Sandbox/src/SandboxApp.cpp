#include <Hazel.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		:Hazel::Layer("Example"),
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f),
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
		m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

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

		std::string vertexSrcSquare = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;
	
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec2 v_TexCoord;				

			void main()
			{
				v_Position = a_Position;
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrcSquare = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec2 v_TexCoord;	

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(v_TexCoord, 0.0, 1.0);
			}
		)";
		m_ShaderSquare.reset(Hazel::Shader::Create(vertexSrcSquare, fragmentSrcSquare));
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Hazel::RendererCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		Hazel::RendererCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

		

		/*Hazel::MaterialRef material = new Metrial(m_FlatColorShader);
		Hazel::MetrialInstanceRef material = new MetrialInstance(material);

		material->SetValue("u_Color", readColor);
		material->SetTexture("u_AlbedoMap", texture);
		squareMesh->SetMaterial(material);*/
		glm::vec4 blueColor(0.2f, 0.2f, 0.8f, 1.0f);
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_ShaderSquare)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_ShaderSquare)->UploadUniformFloat3("u_Color", blueColor);
		Hazel::Renderer::Submit(m_ShaderSquare, m_SquareVA);

		glm::vec4 readColor(0.8f, 0.2f, 0.3f, 1.0f);
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_SquareColor);
		Hazel::Renderer::Submit(m_Shader, m_VertexArraty, transform * scale);



		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override
	{

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
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArraty;

	Hazel::Ref<Hazel::Shader> m_ShaderSquare;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.5f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 10.0f;

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