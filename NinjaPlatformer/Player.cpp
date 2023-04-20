#include "Player.h"
#include <SDL2/SDL.h>
#include <GameEngineOpenGL/ResourceManager.h>
#include <iostream>


Player::Player() { /* empty */ }

Player::~Player() { /* empty */ }

void Player::init(b2World* world, glm::vec2 pos, glm::vec2 drawDims,
	glm::vec2 collitionDims, ge::ColorRGBA8 color, bool fixedRotation)
{
	// loading player's texture
	m_drawDims = drawDims;
	auto texture = ge::ResourceManager::getTexture("Assets/blue_ninja.png");
	m_color = color;

	m_capsule.init(world, pos, collitionDims, 1.0f, 0.1f, fixedRotation);
	m_tileSheet.init(texture, glm::ivec2(10, 2));
}

void Player::destroy(b2World * world)
{
	m_capsule.destroy(world);
}

void Player::draw(ge::SpriteBatch& spriteBatch)
{
	b2Body * m_body = m_capsule.getBody();
	auto destRect = glm::vec4(
		m_body->GetPosition().x - m_drawDims.x / 2.f,
		m_body->GetPosition().y - m_capsule.getDimentions().y / 2.f,
		m_drawDims.x,
		m_drawDims.y);

	// Animation...
	int tileIndex = 0;
	int numTiles = 1;
	auto vel = glm::vec2(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
	float animationSpeed = 0.2f;

	if (m_onGround) { // ...on the ground..

		if (m_isPunching) { // ...punching
			numTiles = 4;
			tileIndex = 1;

			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_moveState = PlayerMoveState::PUNCHING;
				m_animTime = 0.0f;
			}
		}
		else if (abs(vel.x) > 1.0f && (vel.x < 0.0f && m_dir < 0) || (vel.x > 0.0f && m_dir > 0)) {
			tileIndex = 10;	 // ...running
			numTiles = 6;
			animationSpeed = abs(vel.x) * 0.025f;
			if (m_moveState != PlayerMoveState::RUNNING) {
				m_moveState = PlayerMoveState::RUNNING;
				m_animTime = 0.0f;
			}
		}
		else { // ...standing still
			m_moveState = PlayerMoveState::STANDING;
			tileIndex = 0;
			numTiles = 1;
		}
	}
	else { // ...in the air..
		if (m_isPunching) { //.. kicking
			numTiles = 1;
			tileIndex = 18;
			animationSpeed *= 0.5f;
			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_moveState = PlayerMoveState::PUNCHING;
				m_animTime = 0.0f;
			}
		}
		else if (vel.y <= 0.0f) {
			tileIndex = 17; // ...falling
			numTiles = 1;
			m_moveState = PlayerMoveState::IN_AIR;
		}
		else {
			tileIndex = 16; // ...rising
			numTiles = 1;
			m_moveState = PlayerMoveState::IN_AIR;
		}
	}

	// increment time
	m_animTime += animationSpeed;

	// Check for punch end
	if (m_animTime > numTiles)
		m_isPunching = false;

	// apply animation
	tileIndex = tileIndex + (int)m_animTime % numTiles;

	// get uv coordinates from the tile index
	auto uvRect = m_tileSheet.getUVs(tileIndex);

	// left <-> right
	if (m_dir == -1) {
		uvRect.x += 1.0f / m_tileSheet.dims.x;
		uvRect.z *= -1.0f;
	}

	// draw the sprite
	spriteBatch.draw(destRect, uvRect, m_tileSheet.texture.id,
		0.f, m_color, m_capsule.getBody()->GetAngle());
}

void Player::drawDebug(ge::DebugRenderer& debugRenderer)
{
	m_capsule.drawDebug(debugRenderer);
}

void Player::update(ge::InputManager inputManager)
{
	const float SIDE_IMPULSE = 65.f;
	const float JUMP_IMPULSE = 50.f;
	const float MAX_SPEED = 7.f;
	const float SIDE_DAMP_RATE = 0.96f;

	//auto body = m_collisionBox.getBody();
	auto body = m_capsule.getBody();

	// Side movement
	if (inputManager.isKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-SIDE_IMPULSE, 0.f), true);
		m_dir = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		body->ApplyForceToCenter(b2Vec2(SIDE_IMPULSE, 0.f), true);
		m_dir = 1;
	}
	else { // Apply damping
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * SIDE_DAMP_RATE, body->GetLinearVelocity().y));
	}

	//cheking for punch
	if (inputManager.isKeyPressed(SDLK_SPACE)) {
		m_isPunching = true;
	}

	// Limiting max side speed
	if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}

	// Jumps
	m_onGround = CanJump();
	if (inputManager.isKeyPressed(SDLK_w) && m_onGround) {
		body->ApplyLinearImpulse(b2Vec2(0.f, JUMP_IMPULSE), b2Vec2(0.f, 0.f), true);
	}
}

bool Player::CanJump() {
	//auto body = m_collisionBox.getBody();
	auto body = m_capsule.getBody();
	auto ce = body->GetContactList();

	// Loop through all the contact points
	while (ce != nullptr)
	{
		if (ce->contact->IsTouching()) {
			b2WorldManifold manifold;
			ce->contact->GetWorldManifold(&manifold);

			for (size_t i = 0; i < b2_maxManifoldPoints; i++) {
				// Check if the points are below
				if (manifold.points[i].y < body->GetPosition().y - 
					m_capsule.getDimentions().y / 2.f + 
					m_capsule.getDimentions().x / 2.f + 0.01f) {
					return true;
				}
			}
		}
		ce = ce->next;
	}
	return false;
}
