#include "Box.h"



Box::Box()
{
}


Box::~Box()
{
}

void Box::init(b2World * world, glm::vec2 pos, glm::vec2 dims, ge::ColorRGBA8 color)
{
	m_dims = dims;
	m_color = color;
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);

	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(m_dims.x / 2.f, m_dims.y / 2.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;

	m_fixture = m_body->CreateFixture(&fixtureDef);

}
