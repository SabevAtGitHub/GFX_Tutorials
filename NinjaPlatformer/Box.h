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

	void draw(ge::SpriteBatch& spriteBatch);

	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }
	const glm::vec2& getDimentions() const { return m_dims; }
	glm::vec4 getDestRect() const;
	const ge::ColorRGBA8& getColor() const { return m_color; }

private:
	b2Body * m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dims;
	ge::ColorRGBA8 m_color;
	ge::GLTexture m_texture2D;
	glm::vec4 m_uvRect;
};
