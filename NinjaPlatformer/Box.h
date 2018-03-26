#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, glm::vec2 pos, glm::vec2 dims);

	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }
	glm::vec2& getDimentions() const { return m_dims; }

private:
	b2Body * m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dims;

};

