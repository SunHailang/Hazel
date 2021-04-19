#include <Hazel.h>

#include <Hazel/Core/EntryPoint.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

class Hazelnut : public Hazel::Application
{
public:
	Hazelnut()
		:Application("Hazelnut")
	{
		PushLayer(new Hazel::EditorLayer());
	}
	~Hazelnut()
	{

	}

private:
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Hazelnut();
}