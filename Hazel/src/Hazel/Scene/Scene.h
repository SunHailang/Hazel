#pragma once

#include "Hazel/Core/Timestep.h"

#include "entt.hpp"

namespace Hazel
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);


		void OnUpdate(Timestep ts);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}
