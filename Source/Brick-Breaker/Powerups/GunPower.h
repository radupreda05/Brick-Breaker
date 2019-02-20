#pragma once
#include "../Objects/Objects.h"
#include <time.h>
#include <iostream>
#include "Projectile.h"

#define GUN_NUM 2
#define TIME_ACTIVE 5
#define PROJECTILE_NUM 10

class Gun : public Objects {
public:
	Gun();
	Gun(std::string &name, glm::vec2 &position);
	~Gun();

	// Resets the positions of the guns and the projectiles.
	// It also sets the projectile existence to false.
	void Reset();
	
	// Create gun with given name and color
	Mesh* CreateGun(std::string &name, glm::vec3 &color);
	
	// Start the clock and let the guns be active for 5 seconds
	void CheckTimeActive();

	// Update the projectile position
	void ShootGun();

	// Getters and setters
	bool GetShoot();
	float GetLength();
	bool IsActive();
	void SetActive();
	void SetShoot(bool act);
	void SetInactive();
	Projectile& GetProjectile();
protected:
	glm::vec2 initialPos;
	Projectile projectile;
	float length;
	bool active = false;
	bool shoot = false;
	clock_t start;
};
