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
	bool fixedRotation/* = false*/, 
	glm::vec4 uvRect/* = glm::vec4(0.f, 0.f, 1.f, 1.f) */)
{
	m_dims = dims;
	m_color = color;
	m_texture2D = texture;
	m_uvRect = uvRect;

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

	m_fixture = m_body->CreateFixture(&fixtureDef);

}

void Box::draw(ge::SpriteBatch& spriteBatch)
{
	spriteBatch.draw(this->getDestRect(), m_uvRect,
		m_texture2D.id, 0.f, m_color, m_body->GetAngle());
}

glm::vec4 Box::getDestRect() const
{
	return glm::vec4(
		m_body->GetPosition().x - m_dims.x / 2.f, 
		m_body->GetPosition().y - m_dims.y / 2.f, 
		m_dims.x, 
		m_dims.y);
}
