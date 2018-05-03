#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <GameEngineOpenGL\Vertex.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\GLTexture.h>

class Box
{
public:
	Box();
	~Box();

	void init(
		b2World* world, 
		glm::vec2 pos, 
		glm::vec2 dims, 
		ge::GLTexture texture,
		ge::ColorRGBA8 color, 
		bool fixedRotation = false, 
		glm::vec4 uvRect = glm::vec4(0.f, 0.f, 1.f, 1.f));
	void destroy(b2World* world);

	void draw(ge::SpriteBatch& spriteBatch);

	bool isDynamic() const { return m_body->GetType() == b2_dynamicBody; }
	// Test if a point is inside the box
	bool testPoint(glm::vec2 pos) const { return m_fixture->TestPoint(b2Vec2(pos.x, pos.y)); }

	inline b2Body* getBody() const { return m_body; }
	inline b2Fixture* getFixture() const { return m_fixture; }
	glm::vec2 getPosition() const { return glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y); }
	inline const glm::vec2& getDimentions() const { return m_dims; }
	inline glm::vec4 getDestRect() const;
	inline const ge::ColorRGBA8& getColor() const { return m_color; }

private:
	b2Body * m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dims;
	ge::ColorRGBA8 m_color;
	ge::GLTexture m_texture2D;
	glm::vec4 m_uvRect;
};

