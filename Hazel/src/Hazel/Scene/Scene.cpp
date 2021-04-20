#include "hzpch.h"
#include "Scene.h"

#include "Hazel/Renderer/Renderer2D.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace Hazel
{
	static void DoMaths(const glm::mat4& transform)
	{

	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{

	}

}