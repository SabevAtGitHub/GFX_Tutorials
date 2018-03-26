#include "Box.h"



Box::Box()
{
}


Box::~Box()
{
}

void Box::init(b2World * world, const glm::vec2 & pos, const glm::vec2 dims)
{
	// creating a body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	m_body = world->CreateBody(&bodyDef);

	// creating body fixture
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dims.x / 2.f, dims.y / 2.f);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);

}
