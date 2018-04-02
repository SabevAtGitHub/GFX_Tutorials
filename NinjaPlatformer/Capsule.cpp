#include "Capsule.h"


Capsule::Capsule() { /* empty */ }

Capsule::~Capsule() { /* empty */ }

void Capsule::init(b2World * world, glm::vec2 pos, glm::vec2 dims, bool fixedRotation)
{
	m_dims = dims;
	auto halfDims = glm::vec2(m_dims.x / 2.f, m_dims.y / 2.f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = fixedRotation;
	m_body = world->CreateBody(&bodyDef);

#pragma region Creating the Rectangle shape

	b2PolygonShape boxShape;
	boxShape.SetAsBox(halfDims.x, halfDims.y - halfDims.x);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;
	m_fixtures[0] = m_body->CreateFixture(&fixtureDef);

#pragma endregion Creating the Rectangle shape

#pragma region Creating the Circle shapes

	b2CircleShape circleShape;
	circleShape.m_radius = halfDims.x;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = 1.f;
	circleDef.friction = 0.3f;

	// Bottom Circle
	circleShape.m_p.Set(0.f, -halfDims.y + halfDims.x);
	m_fixtures[1] = m_body->CreateFixture(&circleDef);

	// Top Circle
	circleShape.m_p.Set(0.f, halfDims.y - halfDims.x);
	m_fixtures[2] = m_body->CreateFixture(&circleDef);

#pragma endregion Creating the Circle shapess

}

void Capsule::drawDebug(ge::DebugRenderer& debugRenderer)
{
	auto color = ge::ColorRGBA8(255, 255, 255, 255);

	// drawing the box
	auto destRect = getBodyDestRect();
	destRect.y += m_dims.x / 2.f;
	destRect.w -= m_dims.x;

	debugRenderer.drawBox(destRect, color, m_body->GetAngle());

	// drawing the circles
	glm::vec2 pos = glm::vec2(destRect.x + m_dims.x / 2.f , destRect.y);
	glm::vec2 pos2 = glm::vec2(destRect.x + m_dims.x / 2.f, destRect.y + destRect.w);

	debugRenderer.drawCircle(pos, color, m_dims.x / 2.f);
	debugRenderer.drawCircle(pos2, color, m_dims.x / 2.f);
}

glm::vec4 Capsule::getBodyDestRect() const
{
	return glm::vec4(
		m_body->GetPosition().x - m_dims.x / 2.f,
		m_body->GetPosition().y - m_dims.y / 2.f,
		m_dims.x,
		m_dims.y);
	//m_body->GetPosition().y - (m_dims.y - m_dims.x) / 2.f,
}