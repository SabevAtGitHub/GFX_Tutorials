#pragma once
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\GLTexture.h>
#include <GameEngineOpenGL\InputManager.h>
#include <GameEngineOpenGL\DebugRenderer.h>
#include "Box.h"
#include "Capsule.h"

class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, glm::vec2 pos, glm::vec2 dims,
		ge::ColorRGBA8 color, bool fixedRotation);

	void draw(ge::SpriteBatch& spriteBatch);
	void drawDebug(ge::DebugRenderer debugRenderer);

	void update(ge::InputManager inputManager);

	//const Box& getBox() const { return m_collisionBox; }
	const Capsule& getCapsule() const { return m_capsule; }
	
private:
	//Box m_collisionBox;
	Capsule m_capsule;
	ge::GLTexture m_texture2D;
	ge::ColorRGBA8 m_color;

private:
	// True if player's bottom has contact point
	bool CanJump();

};
