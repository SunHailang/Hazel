#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"

#include <GLFW/glfw3.h>

namespace Hazel
{

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return{ (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		//auto [x, y] = GetMousePositionImpl();
		auto pos = GetMousePositionImpl();
		return std::get<0>(pos);
	}

	float WindowsInput::GetMouseYImpl()
	{
		//auto[x, y] = GetMousePositionImpl();
		auto pos = GetMousePositionImpl();
		return std::get<1>(pos);
	}

}
