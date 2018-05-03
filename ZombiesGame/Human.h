#pragma once
#include "Agent.h"

class Human :
	public Agent
{
public:
	Human();
	~Human();

	void init(glm::vec2 initialPos, float initialSpeed);

	virtual void update(const std::vector<std::string>& lvlData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;
private:
	int m_numFrames;
};

