#include "hzpch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel
{

	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();

}