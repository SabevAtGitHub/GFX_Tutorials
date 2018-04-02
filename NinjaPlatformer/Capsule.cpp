#include "Capsule.h"


Capsule::Capsule() { /* empty */ }

Capsule::~Capsule() { /* empty */ }

void Capsule::init(b2World * world, glm::vec2 pos, glm::vec2 dims, bool fixedRotation)
{
	m_dims = dims;

#pragma region Setting up m_fixtures[0] (the rectangle shape)
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = fixedRotation;

	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(m_dims.x / 2.f, m_dims.y / 2.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;
	m_fixtures[0] = m_body->CreateFixture(&fixtureDef);

#pragma endregion // Setting up m_fixtures[0] (the rectangle shape)


}
