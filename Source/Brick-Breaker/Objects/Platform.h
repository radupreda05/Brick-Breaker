#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "Ball.h"
#include "../../include/irrKlang.h"

using namespace irrklang;
class Platform : public Objects {
public:
	Platform();
	Platform(glm::vec2&);
	~Platform();
	
	// Reset the current position to the initial one
	// from the begginng of the game
	void Reset();
	
	// Creates the platform Mesh using the given name and color
	Mesh* CreatePlatform(std::string name, glm::vec3 color);

	// Check for collision with an Object.
	// The objects can be the ball or the falling powerups
	bool Collision(Objects *obj, bool withSound, char * path, ISoundEngine * se);

	// Getters and setters
	float GetWidth();
	float GetHeight();
protected:
	glm::vec2 initialPos;
	float width;
	float height;
};