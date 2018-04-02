#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <GameEngineOpenGL\DebugRenderer.h>

class Capsule
{
public:
	Capsule();
	~Capsule();

	void init(b2World* world, glm::vec2 pos, glm::vec2 dims, bool fixedRotation = false);

	void drawDebug(ge::DebugRenderer& debugRenderer);

	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixtures(int idx) const { return m_fixtures[idx]; }
	const glm::vec2& getDimentions() const { return m_dims; }
	
	glm::vec4 getBodyDestRect() const;
private:

private:
	b2Body * m_body = nullptr;
	b2Fixture* m_fixtures[3];
	glm::vec2 m_dims;
};
