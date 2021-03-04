#include "hzpch.h"

#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"



namespace Hazel
{

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::API::None is currently not support!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		}
		HZ_CORE_ASSERT(false, "Unknow RendererAPI::API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::API::None is currently not support!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		HZ_CORE_ASSERT(false, "Unknow RendererAPI::API!");
		return nullptr;
	}

	// Shader Library -------------------------------------------------
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		HZ_CORE_ASSERT(!Exists(name), "Shader already exist!");
		m_Shader[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		HZ_CORE_ASSERT(!Exists(name), "Shader already exist!");
		m_Shader[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		HZ_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shader[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shader.find(name) != m_Shader.end();
	}

}
