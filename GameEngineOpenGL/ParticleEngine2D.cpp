#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"


namespace ge {

	ParticleEngine2D::ParticleEngine2D()
	{ /*empty*/ }

	ParticleEngine2D::~ParticleEngine2D()
	{
		for (auto& b : m_batches) {
			delete b;
		}
	}

	/// <summary>
	/// after adding a particle batch, the ParticleEngine2D
	/// becomes responsible for deallocation.
	/// </summary>
	/// <param name="particleBatch">Do not delete the batch, after passing it to this function</param>
	void ParticleEngine2D::addParticleBatch(ParticleBatch2D * particleBatch)
	{
		m_batches.push_back(particleBatch);
	}

	void ParticleEngine2D::update(float deltaTime)
	{
		for (auto& b : m_batches) {
			b->update(deltaTime);
		}
	}

	void ParticleEngine2D::draw(SpriteBatch * spriteBatch)
	{
		for (auto& b  : m_batches) {
			spriteBatch->begin();
			b->draw(spriteBatch);
			spriteBatch->end();
			spriteBatch->renderBatch();
		}


	}

}