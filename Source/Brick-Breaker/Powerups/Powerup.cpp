#include "Powerup.h"

Powerup::Powerup() {}

Powerup::Powerup(std::string &name, glm::vec2 &pos)
	: Objects(std::string(name + "Power"), pos) {
	height = POWER_HEIGHT;
	id = 4;
	existence = false;
	initialPos.x = pos.x;
	initialPos.y = pos.y;
	double random = (double)rand() / RAND_MAX;
	if (random <= 0.5f) {
		powerType = GameState::PowerType::GUNS;
	} else {
		powerType = GameState::PowerType::SUPER_BALL;
	}
}

Powerup::~Powerup() {}

Mesh * Powerup::CreatePowerup(std::string &name, glm::vec3 & color) {
	glm::vec3 corner(0, 0, 0);
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(height, 0, 0), color),
		VertexFormat(corner + glm::vec3(height, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	Mesh* power = new Mesh(name);
	power->InitFromData(vertices, indices);
	return power;
}

glm::mat3 Powerup::CreateModelMatrix(glm::mat3 modelMatrix) {
	glm::mat3 matrix = modelMatrix;
	matrix *= Transform2D::Translate(pos.x + height / 2, pos.y + height / 2);
	matrix *= Transform2D::Rotate(angle);
	matrix *= Transform2D::Translate(-height / 2, -height / 2);
	return matrix;
}

void Powerup::Update() {
	if (pos.y + height <= 0) {
		existence = false;
		return;
	}
	angle += 0.15f;
	if (angle >= M_PI * 2)
		angle = 0;
	pos.y -= POWER_FALL_SPEED;
}

void Powerup::Reset() {
	ChangePos(initialPos);
	existence = true;
}

float Powerup::GetHeight() { return height; }
float Powerup::GetAngle() { return angle; }
GameState::PowerType Powerup::GetPowerType() { return powerType; }