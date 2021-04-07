#pragma once

#include <Hazel.h>

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LiftTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void Emit(const ParticleProps& particleProps);

	void OnUpdate(Hazel::Timestep ts);
	void OnRender();

private:
	struct Paricle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LiftTime = 1.0f;
		float LiftRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Paricle> m_PariclePool;
	uint32_t m_PoolIndex = 999;
};
