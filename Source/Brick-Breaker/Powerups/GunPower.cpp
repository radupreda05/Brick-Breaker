#include "GunPower.h"

Gun::Gun() {}
Gun::~Gun() {}

Gun::Gun(std::string &name, glm::vec2 & position) : Objects(name, position) {
	projectile = Projectile::Projectile(name, position);
	length = 10.0f;
	initialPos = position;
}

Mesh * Gun::CreateGun(std::string &name, glm::vec3 &color) {
	glm::vec3 corner(0, 0, 0);
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color),
		VertexFormat(corner + glm::vec3(2 * length, 0, 0), color),
		VertexFormat(corner + glm::vec3(2 * length, length, 0), color),
		VertexFormat(corner + glm::vec3(2 * length, 2 * length, 0), color),
		VertexFormat(corner + glm::vec3(length, 2 * length, 0), color),
		VertexFormat(corner + glm::vec3(3 * length, 0, 0), color),
		VertexFormat(corner + glm::vec3(3 * length, length, 0), color)
	};
	
	Mesh *gun = new Mesh(name);
	std::vector<unsigned short> indices = {
		0, 1, 2,
		0, 2, 3,
		1, 4, 6,
		1, 6, 7,
		4, 8, 9,
		4, 9, 5
	};
	
	gun->InitFromData(vertices, indices);
	return gun;
}

void Gun::CheckTimeActive() {
	double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	if (duration > TIME_ACTIVE) {
		SetInactive();
		projectile.ChangePos(pos + glm::vec2(GetLength() / 2, 0));
	}
}

void Gun::SetActive() {
	active = true;
	start = clock();
}

void Gun::SetInactive() {
	active = false;
	shoot = false;
	projectile.SetExistence(false);
}

void Gun::ShootGun() {
	if (shoot)
		projectile.ChangePos(projectile.GetPos()
			+ glm::vec2(0, PROJECTILE_SPEED));
}

void Gun::SetShoot(bool act) {
	shoot = act;
	projectile.SetExistence(true);
}

void Gun::Reset() {
	pos = initialPos;
	projectile.ChangePos(pos + glm::vec2(length * 3 / 2, 0));
	SetInactive();
}

Projectile& Gun::GetProjectile() { return projectile; }
float Gun::GetLength() { return 3 * length; }
bool Gun::IsActive() { return active; }
bool Gun::GetShoot() { return shoot; }