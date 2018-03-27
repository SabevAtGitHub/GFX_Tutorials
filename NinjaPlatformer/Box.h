#pragma once
#include <Box2D\Box2D.h>
#include <GameEngineOpenGL\Vertex.h>
#include <glm\glm.hpp>

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, glm::vec2 pos, glm::vec2 dims, 
		ge::ColorRGBA8 color = ge::ColorRGBA8(255, 0, 0, 255));

	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }
	const glm::vec2& getDimentions() const { return m_dims; }
	const ge::ColorRGBA8& getColor() const { return m_color; }
	
private:
	b2Body * m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dims;
	ge::ColorRGBA8 m_color;
};

