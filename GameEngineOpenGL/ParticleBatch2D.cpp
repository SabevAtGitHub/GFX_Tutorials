#include "ParticleBatch2D.h"


namespace ge {

	ParticleBatch2D::ParticleBatch2D()
	{ /*empty*/ }


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] particles_;
	}

	void ParticleBatch2D::init(
		int maxParticles, 
		float decayRate,
		GLTexture texture, 
		std::function<void(Particle2D&, float)> updateFunc/* = defaultParticleupdate */)
	{
		maxParticles_ = maxParticles;
		decayRate_ = decayRate;
		texture_ = texture;
		updateFunc_ = updateFunc;

		particles_ = new Particle2D[maxParticles_];
	}

	void ParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i < maxParticles_; i++) {
			
			// if a particle is active, update it
			if (particles_[i].life > 0.f) {
				updateFunc_(particles_[i], deltaTime);
				particles_[i].life -= decayRate_ * deltaTime;
			}
		}

	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.f, 0.f, 1.f, 1.f);

		for (int i = 0; i < maxParticles_; i++) {
			auto& p = particles_[i];

			// if a particle is active, draw it
			if (p.life > 0.f) {
				glm::vec4 destRect(p.pos.x, p.pos.y, p.width, p.width);

				spriteBatch->draw(destRect, uvRect, texture_.id, 0.f, p.color);
			}
		}
	}
	void ParticleBatch2D::addParticle(const glm::vec2 & pos,
		const glm::vec2 & velocity,
		const ColorRGBA8 & color, float size)
	{
		int particleIdx = findFreeParticle();

		auto&p = particles_[particleIdx];
		p.life = 1.0f;
		p.pos = pos;
		p.velocity = velocity;
		p.color = color;
		p.width = size;
	}
	int ParticleBatch2D::findFreeParticle()
	{
		// this loop should find a free particle
		for (int i = freeParticleIdx_; i < maxParticles_; i++) {
			if (particles_[i].life <= 0.f) {
				freeParticleIdx_ = i;
				return i;
			}
		}

		// this loop is going to check the rest of the array for a free particle
		for (int i = 0; i < freeParticleIdx_; i++) {
			if (particles_[i].life <= 0.f) {
				freeParticleIdx_ = i;
				return i;
			}
		}

		// and finaly, if no particle is free,
		// we need to overwrite the first particle
		return 0;
	}
}