#pragma once

#include "Hazel.h"

namespace Hazel
{
	class SceneHierarchyPanel
	{
	public: 
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectEntity() const { return m_SelectionContext; }

	private: 
		void DrawEntityNode(Entity& entity);
		void DrawComponent(Entity& entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};
}
