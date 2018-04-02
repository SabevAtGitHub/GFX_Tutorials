#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <GameEngineOpenGL\DebugRenderer.h>

class Capsule
{
public:
	Capsule();
	~Capsule();

	void init(b2World* world, glm::vec2 pos, glm::vec2 dims,
		float density, float friction, bool fixedRotation = false);


	void drawDebug(ge::DebugRenderer& debugRenderer);

	inline b2Body* getBody() const { return m_body; }
	inline b2Fixture* getFixtures(int idx) const { return m_fixtures[idx]; }
	inline const glm::vec2& getDimentions() const { return m_dims; }

	inline glm::vec4 getBodyDestRect() const;
private:

private:
	b2Body * m_body = nullptr;
	b2Fixture* m_fixtures[3];
	glm::vec2 m_dims;
};
