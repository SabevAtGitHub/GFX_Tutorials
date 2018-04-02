#include "Capsule.h"


Capsule::Capsule() { /* empty */ }

Capsule::~Capsule() { /* empty */ }

void Capsule::init(b2World * world, glm::vec2 pos, glm::vec2 dims, bool fixedRotation)
{
	m_dims = dims;

#pragma region Creating the Rectangle shape
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

#pragma endregion Creating the Rectangle shape

#pragma region Creating the Circle shapes



#pragma endregion Creating the Circle shapess

}

void Capsule::drawDebug(ge::DebugRenderer& debugRenderer)
{
	debugRenderer.drawBox(getBodyDestRect(), 
		ge::ColorRGBA8(255, 255, 255, 255), m_body->GetAngle());
}

glm::vec4 Capsule::getBodyDestRect() const
{
	return glm::vec4(
		m_body->GetPosition().x - m_dims.x / 2.f,
		m_body->GetPosition().y - m_dims.y / 2.f,
		m_dims.x,
		m_dims.y);
}