#include "hzpch.h"
#include "Scene.h"

#include "Hazel/Renderer/Renderer2D.h"
#include "Entity.h"

//#include <glm/glm.hpp>

namespace Hazel
{

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity(const std::string& name)
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{

	}

}