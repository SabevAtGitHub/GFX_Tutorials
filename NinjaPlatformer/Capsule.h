#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>

class Capsule
{
public:
	Capsule();
	~Capsule();

	void init(b2World* world, glm::vec2 pos, glm::vec2 dims, bool fixedRotation = false);
private:
	b2Body * m_body = nullptr;
	b2Fixture* m_fixtures[3];
	glm::vec2 m_dims;
};
