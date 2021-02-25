#include <Hazel.h>


class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new Hazel::ImGuiLayer());		
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