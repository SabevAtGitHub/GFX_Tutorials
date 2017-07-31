#pragma once
#include "Agent.h"

class Zombie : 
	public Agent 
{
public:
	Zombie();
	~Zombie();

	void init(glm::vec2 initialPos, float initialSpeed);

	void update(const std::vector<std::string>& lvlData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;
private:
	Human* getNearestHuman(std::vector<Human*>& humans);
};

