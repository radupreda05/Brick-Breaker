#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "../Objects/Objects.h"
#include "../Transform2D.h"
#include "../States.h"

#define POWER_HEIGHT 17.5
#define POWER_FALL_SPEED 2

class Powerup : public Objects {
public:
	Powerup();
	Powerup(std::string &name, glm::vec2 &pos);
	~Powerup();
	
	// Reset the variables. Useful when the game has ended
	// or when the player has died
	void Reset();

	Mesh* CreatePowerup(std::string &name, glm::vec3 &color);

	// Create the new modelMatrix to create the animation
	glm::mat3 CreateModelMatrix(glm::mat3 modelMatrix);
	
	// Update the power position
	void Update();
	
	// Getterts and setters
	float GetHeight();
	float GetAngle();
	GameState::PowerType GetPowerType();
protected:
	glm::vec2 initialPos;
	GameState::PowerType powerType;
	float height;
	float angle = 0;
};