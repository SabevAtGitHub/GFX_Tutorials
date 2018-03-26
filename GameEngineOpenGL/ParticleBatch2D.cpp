#include "ParticleBatch2D.h"


namespace ge {

	ParticleBatch2D::ParticleBatch2D()
	{ /*empty*/ }


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_particles;
	}

	void ParticleBatch2D::init(
		int maxParticles, 
		float decayRate,
		GLTexture m_texture, 
		std::function<void(Particle2D&, float)> updateFunc/* = defaultParticleupdate */)
	{
		m_maxParticles = maxParticles;
		m_decayRate = decayRate;
		m_texture = m_texture;
		m_updateFunc = updateFunc;

		m_particles = new Particle2D[m_maxParticles];
	}

	void ParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i < m_maxParticles; i++) {
			
			// if a particle is active, update it
			if (m_particles[i].life > 0.f) {
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].life -= m_decayRate * deltaTime;
			}
		}

	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.f, 0.f, 1.f, 1.f);

		for (int i = 0; i < m_maxParticles; i++) {
			auto& p = m_particles[i];

			// if a particle is active, draw it
			if (p.life > 0.f) {
				glm::vec4 destRect(p.pos.x, p.pos.y, p.width, p.width);

				spriteBatch->draw(destRect, uvRect, m_texture.id, 0.f, p.color);
			}
		}
	}
	void ParticleBatch2D::addParticle(const glm::vec2 & pos,
		const glm::vec2 & velocity,
		const ColorRGBA8 & color, float size)
	{
		int particleIdx = findFreeParticle();

		auto&p = m_particles[particleIdx];
		p.life = 1.0f;
		p.pos = pos;
		p.velocity = velocity;
		p.color = color;
		p.width = size;
	}
	int ParticleBatch2D::findFreeParticle()
	{
		// this loop should find a free particle
		for (int i = m_freeParticleIdx; i < m_maxParticles; i++) {
			if (m_particles[i].life <= 0.f) {
				m_freeParticleIdx = i;
				return i;
			}
		}

		// this loop is going to check the rest of the array for a free particle
		for (int i = 0; i < m_freeParticleIdx; i++) {
			if (m_particles[i].life <= 0.f) {
				m_freeParticleIdx = i;
				return i;
			}
		}

		// and finaly, if no particle is free,
		// we need to overwrite the first particle
		return 0;
	}
}