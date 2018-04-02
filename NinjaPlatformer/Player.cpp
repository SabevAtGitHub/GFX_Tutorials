#include "Player.h"
#include <SDL\SDL.h>
#include <GameEngineOpenGL\ResourceManager.h>
#include <iostream>


Player::Player() { /* empty */ }

Player::~Player() { /* empty */ }

void Player::init(b2World* world, glm::vec2 pos, glm::vec2 drawDims,
	glm::vec2 collitionDims, ge::ColorRGBA8 color, bool fixedRotation)
{
	// loading player's texture
	m_drawDims = drawDims;
	m_texture2D = ge::ResourceManager::getTexture("Assets/blue_ninja.png");
	m_color = color;

	//m_collisionBox.init(world, pos, dims, m_texture2D, color, fixedRotation, m_uvRect);

	m_capsule.init(world, pos, collitionDims, 0.9f, 0.1f, fixedRotation);
}

void Player::draw(ge::SpriteBatch& spriteBatch)
{
	auto uvRect = glm::vec4(0.f, 0.f, 0.1f, 0.5f);

	b2Body * m_body = m_capsule.getBody();
	auto destRect = glm::vec4(
		m_body->GetPosition().x - m_drawDims.x / 2.f,
		m_body->GetPosition().y - m_capsule.getDimentions().y / 2.f,
		m_drawDims.x,
		m_drawDims.y);

	//m_collisionBox.draw(spriteBatch);
	spriteBatch.draw(destRect, uvRect,
		m_texture2D.id, 0.f, m_color, m_capsule.getBody()->GetAngle());
}

void Player::drawDebug(ge::DebugRenderer& debugRenderer)
{
	m_capsule.drawDebug(debugRenderer);
}

void Player::update(ge::InputManager inputManager)
{
	const float SIDE_IMPULSE = 90.f;
	const float JUMP_IMPULSE = 40.f;
	const float MAX_SPEED = 5.f;
	const float SIDE_DAMP_RATE = 0.96f;

	//auto body = m_collisionBox.getBody();
	auto body = m_capsule.getBody();

	// Side movement
	if (inputManager.isKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-SIDE_IMPULSE, 0.f), true);
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		body->ApplyForceToCenter(b2Vec2(SIDE_IMPULSE, 0.f), true);
	}
	else { // Apply damping
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * SIDE_DAMP_RATE, body->GetLinearVelocity().y));
	}

	// Limiting max side speed
	if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}

	// Jumps
	if (inputManager.isKeyPressed(SDLK_w) && CanJump()) {
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
				if (manifold.points[i].y < body->GetPosition().y - m_capsule.getDimentions().y / 2.f + 0.01f) {
					return true;
				}
			}
		}
		ce = ce->next;
	}
	return false;
}
