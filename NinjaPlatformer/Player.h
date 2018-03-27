#pragma once
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\GLTexture.h>
#include <GameEngineOpenGL\InputManager.h>
#include "Box.h"

class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, glm::vec2 pos, glm::vec2 dims,
		ge::ColorRGBA8 color, bool fixedRotation);

	void draw(ge::SpriteBatch& spriteBatch);

	void update(ge::InputManager inputManager);

	const Box& getBox() const { return m_collisionBox; }

private:
	Box m_collisionBox;
	ge::GLTexture m_texture2D;
	glm::vec4 m_uvRect;

private:
	// True if player's bottom has contact point
	bool CanJump();

};
