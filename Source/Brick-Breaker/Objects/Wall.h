#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "Objects.h"
#define START_WALL_DRAW 75
#define WALL_WIDTH 15
#define WALL_NUM 3

class Wall : public Objects {
public:
	// Used at collision to know how the velocity of the ball modifies
	enum class WallType {
		LEFT,
		UPPER,
		RIGHT
	};
public:
	Wall();
	Wall(std::string &name, float height, glm::vec2 &pos, WallType type);
	~Wall();
	
	// Create the wall using the given name and color
	Mesh* CreateWall(std::string &name, glm::vec3 &color);

	// Getters and setters
	WallType GetType();
	float GetHeight();
protected:
	WallType type;
	float height;
};