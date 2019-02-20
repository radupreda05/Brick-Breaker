#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "../Objects/Objects.h"
#include "../Objects/Brick.h"
#include "../Objects/Ball.h"
#define PROJECTILE_HEIGHT 20
#define PROJECTILE_SPEED 20

class Projectile : public Objects {
public:
	Projectile();
	Projectile(std::string &name, glm::vec2 &pos);
	~Projectile();

	// Create the projectile with the given name and color
	Mesh* CreateProjectile(std::string &name, glm::vec3 &color);

	// Check if the projectile has collided with a brick
	bool Collision(Brick *brick, bool sound, char *path, ISoundEngine *se);

	// Getter
	float GetHeight();
protected:
	float height;
};