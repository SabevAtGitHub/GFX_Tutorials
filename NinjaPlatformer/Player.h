#pragma once
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\TileSheet.h>
#include <GameEngineOpenGL\InputManager.h>
#include <GameEngineOpenGL\DebugRenderer.h>
#include "Box.h"
#include "Capsule.h"

enum class PlayerMoveState
{
	STANDING, RUNNING, PUNCHING, IN_AIR
};

class Player
{

public:
	Player();
	~Player();

	void init(b2World* world, glm::vec2 pos, glm::vec2 drawDims,
		glm::vec2 collitionDims, ge::ColorRGBA8 color, bool fixedRotation);
	void destroy(b2World* world);
	void draw(ge::SpriteBatch& spriteBatch);
	void drawDebug(ge::DebugRenderer& debugRenderer);

	void update(ge::InputManager inputManager);

	glm::vec2 getPos() const {
		return glm::vec2(m_capsule.getBody()->GetPosition().x,
						 m_capsule.getBody()->GetPosition().y);
	}
	const Capsule& getCapsule() const { return m_capsule; }
	
private:
	//Box m_collisionBox;
	glm::vec2 m_drawDims;
	Capsule m_capsule;
	ge::TileSheet m_tileSheet;
	ge::ColorRGBA8 m_color;

	PlayerMoveState m_moveState = PlayerMoveState::STANDING;
	float m_animTime = 0.0f;
	int m_dir = 1;
	bool m_onGround = false;
	bool m_isPunching = false;

private:
	// True if player's bottom has contact point
	bool CanJump();
};
