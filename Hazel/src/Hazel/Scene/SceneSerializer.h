#pragma once

#include "Hazel/Core/Core.h"
#include "Scene.h"

namespace Hazel
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serializer(const std::string& filePath);
		void SerializerRuntime(const std::string& filePath);

		bool Deserializer(const std::string& filePath);
		bool DeserializerRuntime(const std::string& filePath);

	private:
		Ref<Scene> m_Scene;
	};
}
