#include "Wall.h"

Wall::Wall() {}
Wall::~Wall() {}

Wall::Wall(std::string &name, float height, glm::vec2 &pos, WallType type)
	: Objects(name, pos) {
	this->height = height;
	this->type = type;
	id = 2;
}

Mesh * Wall::CreateWall(std::string &name, glm::vec3 &color) {
	glm::vec3 corner(0, 0, 0);

	std::vector<VertexFormat> vertices;
	if (type == Wall::WallType::UPPER)
		vertices = {
			VertexFormat(corner, color),
			VertexFormat(corner + glm::vec3(height, 0, 0), color),
			VertexFormat(corner + glm::vec3(height, WALL_WIDTH, 0), color),
			VertexFormat(corner + glm::vec3(0, WALL_WIDTH, 0), color)
		};
	else
		vertices = {
			VertexFormat(corner, color),
			VertexFormat(corner + glm::vec3(WALL_WIDTH, 0, 0), color),
			VertexFormat(corner + glm::vec3(WALL_WIDTH, height, 0), color),
			VertexFormat(corner + glm::vec3(0, height, 0), color)
		};

	Mesh* wall = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2};
	wall->InitFromData(vertices, indices);
	return wall;
}

Wall::WallType Wall::GetType() { return type; }
float Wall::GetHeight() { return height; }