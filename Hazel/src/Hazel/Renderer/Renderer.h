#pragma once

namespace Hazel
{
	enum class RndererAPI
	{
		None = 0,
		OpenGL = 1
	};
	class Renderer
	{
	public:
		inline static RndererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RndererAPI s_RendererAPI;
	};
	
}
