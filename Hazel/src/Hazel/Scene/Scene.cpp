#include "hzpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "Hazel/Renderer/Renderer2D.h"

//#include <glm/glm.hpp>

namespace Hazel
{

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = "Entity";
		if (!name.empty()) tag.Tag = name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto grop = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : grop)
		{
			auto& [transform, sprite] = grop.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform.Transform, sprite.Color);
		}
	}

}