#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, const glm::vec2& pos, const glm::vec2 dims);


private:

};

