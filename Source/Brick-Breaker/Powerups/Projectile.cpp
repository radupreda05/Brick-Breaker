#include "Projectile.h"

Projectile::Projectile() {}
Projectile::~Projectile() {}

Projectile::Projectile(std::string & name, glm::vec2 & pos)
	: Objects(std::string("Projectile" + name), pos) {
	this->height = PROJECTILE_HEIGHT;
	existence = false;
}

Mesh * Projectile::CreateProjectile(std::string &name, glm::vec3 & color) {
	glm::vec3 corner(0, 0, 0);
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};
	
	for (float angle = 0.0f; angle <= 180; angle += 0.1f) {
		float x2 = sin(angle) * height / 10;
		float y2 = cos(angle) * height / 10;
		vertices.push_back(VertexFormat(glm::vec3(x2, y2, 0), color));
	}

	vertices.push_back(VertexFormat(corner + glm::vec3(-height / 10, height, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(height / 10, height, 0), color));
	vertices.push_back(VertexFormat(corner, color));

	Mesh* projectile = new Mesh(name);	
	std::vector<unsigned short> indices;
	for (unsigned short i = 1; i < vertices.size() - 4; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);

	projectile->InitFromData(vertices, indices);
	return projectile;
}

bool Projectile::Collision(Brick *brick, bool withSound, char *path, ISoundEngine *se) {
	glm::vec2 center = brick->GetPos();
	center += glm::vec2(brick->GetWidth() / 2, brick->GetHeight() / 2);
	glm::vec2 rightCorner = brick->GetPos() + glm::vec2(brick->GetWidth(), 0);
	if (!brick->GetExistence())
		return false;
	float dx = max(abs(pos.x - center.x) - brick->GetWidth() / 2, 0);
	float dy = max(abs(pos.y - center.y) - brick->GetHeight() / 2, 0);
	float distance = sqrt(dx * dx + dy * dy);

	if (distance <= 0) {
		if (withSound && se)
			se->play2D(path);
		brick->SetExistence(false);
		return true;
	}
	return false;
}

float Projectile::GetHeight() { return height; }