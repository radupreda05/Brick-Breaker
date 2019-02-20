#include "Brick.h"

Brick::Brick() {}
Brick::~Brick() {}

Brick::Brick(std::string &name, glm::vec2 &translated, float w, float h) {
	this->name.assign(name);
	this->pos = translated;
	this->width = w;
	this->height = h;
	id = 3;
	scale = 1;
	existence = true;
	double random = (double) rand() / RAND_MAX;
	if (random <= 0.1f) {
		glm::vec2 powerPos = translated;
		powerPos += glm::vec2(width / 2 - POWER_HEIGHT / 2,
			height / 2 - POWER_HEIGHT / 2);
		power = Powerup::Powerup(name, powerPos);
		power.SetExistence(true);
		hasPower = true;
	}
}

void Brick::Reset(GameState::GameState state) {
	if (state == GameState::GameState::Game_Over
		|| state == GameState::GameState::Game_Won) {
		scale = 1;
		existence = true;
		if (hasPower)
			power.Reset();
	}
	else if (state == GameState::GameState::You_Died) {
		if (!existence) {
			power.SetExistence(false);
		}
	}
}

Mesh *Brick::CreateBrick(std::string &name, glm::vec3 &color) {
	glm::vec3 corner(0, 0, 0);
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	Mesh* brick = new Mesh(name);
	brick->InitFromData(vertices, indices);
	return brick;
}

glm::mat3 Brick::CreateModelMatrix(glm::mat3 modelMatrix) {
	glm::mat3 matrix = modelMatrix;
	matrix = Transform2D::Translate(pos.x + width / 2, pos.y + height / 2);
	matrix *= Transform2D::Scale(scale, scale);
	matrix *= Transform2D::Translate(-width / 2, -height / 2);
	return matrix;
}

void Brick::ReduceScale(float fact) {
	if (scale <= 0) {
		scale = 0;
		return;
	}
	scale -= fact;
}

float Brick::GetWidth() { return width; }
float Brick::GetScale() { return scale; }
Powerup& Brick::GetPower() {	return power; }
bool Brick::HasPower() { return hasPower; }
float Brick::GetHeight() { return height; }

