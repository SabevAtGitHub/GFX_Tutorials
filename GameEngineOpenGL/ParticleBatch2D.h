#pragma once

#include <functional>
#include <glm\glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace ge {

	class Particle2D {
	public:
		Particle2D() {};
		~Particle2D() {};

		glm::vec2 pos = glm::vec2(0.f);
		glm::vec2 velocity = glm::vec2(0.f);
		ColorRGBA8 color;
		float life = 0.f;
		float width = 0.f;
	};

	// if a particle is active, update it
	inline void defaultParticleupdate(Particle2D& p, float deltaTime) {
		p.pos += p.velocity * deltaTime;
	}

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles, 
				  float decayRate,
				  GLTexture texture,
				  std::function<void(Particle2D&, float)> updateFunc = defaultParticleupdate);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

		void addParticle(const glm::vec2& pos, const glm::vec2& velocity, 
			const ColorRGBA8& color, float size);

	private:
		int findFreeParticle();

		std::function<void(Particle2D&, float)> updateFunc_;

		float decayRate_ = 0.1f;
		Particle2D* particles_ = nullptr;
		int maxParticles_ = 0;
		int freeParticleIdx_ = 0;
		GLTexture texture_;
	};
}

