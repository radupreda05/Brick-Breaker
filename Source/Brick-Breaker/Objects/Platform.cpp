#include "Platform.h"

Platform::Platform() {}
Platform::~Platform() {}

Platform::Platform(glm::vec2& translated)
	: Objects(std::string("Platform"), translated) {
	width = 200.0f;
	height = 7.5f;
	id = 1;
	initialPos = translated;
	initialPos.x -= width / 2;
}

Mesh * Platform::CreatePlatform(std::string name, glm::vec3 color) {
	glm::vec3 corner(0, 0, 0);
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};
	
	Mesh* platform = new Mesh(name);
	std::vector<unsigned short> indices = {0, 1, 2, 3, 0, 2};

	platform->InitFromData(vertices, indices);
	return platform;
}

bool Platform::Collision(Objects *obj, bool withSound, char *path, ISoundEngine *se) {
	glm::vec2 center = pos;
	center += glm::vec2(width / 2, height / 2);
	glm::vec2 objectPos = obj->GetPos();
	float radius;
	if (obj->id == 0)
		radius = (*(Ball *)obj).GetRadius();
	else if (obj->id == 4)
		radius = (*(Powerup *)obj).GetHeight();
	else
		return false;

	float dx = max(abs(objectPos.x - center.x) - width / 2, 0);
	float dy = max(abs(objectPos.y - center.y) - height / 2, 0);
	float distance = sqrt(dx * dx + dy * dy);

	if (distance <= radius) {
		if (withSound && se)
			se->play2D(path);

		if (obj->id == 0) {
			Ball ball = *(Ball *)obj;
			float cosine = (objectPos.x - center.x) / width;
			if (cosine < -1.0f)
				cosine = -1.0f;
			else if (cosine > 1.0f)
				cosine = 1.0f;
			glm::vec2 velocity = ball.GetVelocity();
			velocity.y = -velocity.y;
			velocity.x = (float)sin(cosine * M_PI / 2) * 4;
			((Ball *)obj)->SetVelocity(glm::vec2(velocity));
			return true;
		} else if (obj->id == 4) {
			((Powerup *)obj)->SetExistence(false);
			return true;
		}
	}
	return false;
}

float Platform::GetWidth() { return width; }
float Platform::GetHeight() { return height; }
void Platform::Reset() { pos = initialPos; }