#include "Box.h"



Box::Box()
{
}


Box::~Box()
{
}

void Box::init(
	b2World* world,
	glm::vec2 pos,
	glm::vec2 dims,
	ge::GLTexture texture,
	ge::ColorRGBA8 color,
	bool fixedRotation,
	bool isDynamic,
	float angle = 0.0f,
	glm::vec4 uvRect/* = glm::vec4(0.f, 0.f, 1.f, 1.f) */)
{
	m_dims = dims;
	m_color = color;
	m_texture2D = texture;
	m_uvRect = uvRect;
	m_fixedRotation = fixedRotation;
	m_isDynamic = isDynamic;

	b2BodyDef bodyDef;
	if (m_isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}else{
		bodyDef.type = b2_staticBody;
	}
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = m_fixedRotation;
	bodyDef.angle = angle;
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(m_dims.x / 2.0f, m_dims.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Box::destroy(b2World * world)
{
	world->DestroyBody(m_body);
}

void Box::draw(ge::SpriteBatch& spriteBatch)
{
	spriteBatch.draw(this->getDestRect(), m_uvRect,
		m_texture2D.id, 0.0f, m_color, m_body->GetAngle());
}

glm::vec4 Box::getDestRect() const
{
	return glm::vec4(
		m_body->GetPosition().x - m_dims.x / 2.0f,
		m_body->GetPosition().y - m_dims.y / 2.0f,
		m_dims.x,
		m_dims.y);
}
