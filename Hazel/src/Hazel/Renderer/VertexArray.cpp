#include "hzpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel
{

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RndererAPI::None:		HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RndererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

}