#pragma once
#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <iostream>
#include <vector>
#include <time.h>
#include "Wall.h"
#include "Brick.h"
#include "Objects.h"
#include "../../include/irrKlang.h"
#define INITIAL_VEL_X 0
#define INITIAL_VEL_Y 5
#define TIME_ACTIVE 5
#define BALL_DRAW_HEIGHT 45

using namespace irrklang;

class Ball : public Objects {
public:
	Ball();
	Ball(glm::vec2 &pos);
	~Ball();
	// Activate the powerup and start the timer
	void ActivateSuperBall();

	// Create the ball with the given name and color
	Mesh* CreateBall(std::string &name, glm::vec3 &color);

	// Update the balls position with the velocity vector
	void ChangePositionOnPlay();

	// Reset the variables of the ball class
	void Reset();

	// Checks if the time for the powerup has expired or not.
	// If it has, deactivate the powerup
	bool CheckPowerActive();

	// Check for collisions based on the distance from ball to objects.
	// Objects can be Walls and Bricks.
	// The difference between the Walls and Bricks is made with the id variable
	// from the superclass "Objects" which is set in every constructor.
	void Collision(Objects* obj, bool withSound, char *path, ISoundEngine*);

	// Check if the ball exited the scene resolution
	bool CheckDeath(float);

	// Resets the variable brickHit
	void ResetBrickHit();

	// Getters and setters
	bool GetBrickHit();
	float GetRadius();
	glm::vec2 GetVelocity();
	void SetVelocity(glm::vec2 newVel);
protected:
	clock_t start;
	glm::vec2 initialPos;
	bool superBall = false;
	float radius;
	glm::vec2 velocity;
	bool brickHit = false;
};
