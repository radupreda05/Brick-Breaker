#pragma once
#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "Objects.h"
#include "../Powerups/Powerup.h"
#include "../Transform2D.h"
#include "../States.h"

#define SCALE_REDUCTION 0.02f
#define NR_LINE 10
#define NR_COL 12
#define BRICK_HEIGHT_GAP 10
#define BRICK_WIDTH_GAP 15
#define BRICK_START_X 190
#define BRICK_START_Y 100

class Brick : public Objects {
public:
	Brick();
	Brick(std::string &name, glm::vec2 &pos, float, float);
	~Brick();

	// Reset the bricks variables or destroy the falling
	// powerups depending on the current state of the game.
	void Reset(GameState::GameState);

	// Creates the bricks Mesh using the given name and color
	Mesh* CreateBrick(std::string &name, glm::vec3 &color);

	// Create the new modelMatrix to make the animation
	glm::mat3 CreateModelMatrix(glm::mat3 modelMatrix);

	// Reduces the scale if the brick was hit
	void ReduceScale(float);

	// Getters and setters
	float GetHeight();
	float GetWidth();
	float GetScale();
	Powerup& GetPower();
	bool HasPower();
protected:
	bool hasPower = false;
	Powerup power;
	float height, width;
	float scale;
};
