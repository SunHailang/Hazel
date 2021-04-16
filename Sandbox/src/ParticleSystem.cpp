#include "ParticleSystem.h"

#include "Random.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx//compatibility.hpp>

ParticleSystem::ParticleSystem()
{
	m_PariclePool.resize(1000);
}

void ParticleSystem::OnUpdate(Hazel::Timestep ts)
{
	for (auto& particle : m_PariclePool)
	{
		if (!particle.Active) continue;

		if (particle.LiftRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LiftRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.1f * ts;
	}
}

void ParticleSystem::OnRender(Hazel::OrthographicCamera& camera)
{
	Hazel::Renderer2D::BeginScene(camera);
	for (auto& particle : m_PariclePool)
	{
		if (!particle.Active) continue;

		// Fade away particles
		float life = particle.LiftRemaining / particle.LiftTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		// Render
		Hazel::Renderer2D::DrawRotatedQuad(particle.Position, { size, size }, particle.Rotation, color);
	}
	Hazel::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Paricle& paricle = m_PariclePool[m_PoolIndex];
	paricle.Active = true;
	paricle.Position = particleProps.Position;
	paricle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	paricle.Velocity = particleProps.Velocity;
	paricle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	paricle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	paricle.ColorBegin = particleProps.ColorBegin;
	paricle.ColorEnd = particleProps.ColorEnd;

	// Size
	paricle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	paricle.SizeEnd = particleProps.SizeEnd;

	// Lift
	paricle.LiftTime = particleProps.LiftTime;
	paricle.LiftRemaining = particleProps.LiftTime;

	m_PoolIndex = --m_PoolIndex % m_PariclePool.size();
}


