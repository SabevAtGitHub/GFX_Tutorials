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

	void drawDebug(ge::DebugRenderer debugRenderer);
	
private:
	glm::vec4 getBodyDestRect() const;

private:
	b2Body * m_body = nullptr;
	b2Fixture* m_fixtures[3];
	glm::vec2 m_dims;
};
