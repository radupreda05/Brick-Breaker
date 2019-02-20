#include "Ball.h"

Ball::Ball() {}
Ball::~Ball() {}

Ball::Ball(glm::vec2 &translated)
	: Objects(std::string("Play ball"), translated){
	radius = 10.0f;
	velocity = glm::vec2(INITIAL_VEL_X, INITIAL_VEL_Y);
	initialPos = translated;
}

Mesh * Ball::CreateBall(std::string &name, glm::vec3 &color) {
	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
	for (float angle = 1.0f; angle <= 360; angle += 0.1f) {
		float x2 = sin(angle) * radius;
		float y2 = cos(angle) * radius;
		vertices.push_back(VertexFormat(glm::vec3(x2, y2, 0), color));
	}

	Mesh* ball = new Mesh(name);
	std::vector<unsigned short> indices;
	for (unsigned short i = 1; i < vertices.size() - 1; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	ball->InitFromData(vertices, indices);
	return ball;
}

void Ball::Collision(Objects *obj, bool withSound, char *path, ISoundEngine *se) {
	glm::vec2 center = obj->GetPos();
	float width, height;
	if (obj->id == 2) {
		Wall wall = *(Wall *) obj;
		if (wall.GetType() == Wall::WallType::UPPER) {
			width = wall.GetHeight() / 2;
			height = WALL_WIDTH / 2;
			center += glm::vec2(width, height);
		} else {
			width = WALL_WIDTH / 2;
			height = wall.GetHeight() / 2;
			center += glm::vec2(width, height);
		}
	} else if (obj->id == 3) {
		Brick brick = *(Brick *)obj;
		if (!brick.GetExistence() || brickHit)
			return;
		width = brick.GetWidth() / 2;
		height = brick.GetHeight() / 2;
		center += glm::vec2(width, height);
	}
	float dx = max(abs(pos.x - center.x) - width, 0);
	float dy = max(abs(pos.y - center.y) - height, 0);
	float distance = sqrt(dx * dx + dy * dy);
	
	if (distance <= radius) {
		if (withSound && se)
			se->play2D(path);
		if (obj->id == 2) {
			Wall wall = *(Wall *)obj;
			if (wall.GetType() == Wall::WallType::UPPER)
				velocity.y = -velocity.y;
			else {
				glm::vec2 corner = wall.GetPos();
				
				// The ball hits the bottom side of the left wall
				if (pos.x <= corner.x + WALL_WIDTH + radius * 2
						&& wall.GetType() == Wall::WallType::LEFT
						&& pos.y <= START_WALL_DRAW) {
					velocity = -velocity;
				}

				// The ball hits the bottom side of the right wall
				else if (pos.x >= corner.x - radius
						&& wall.GetType() == Wall::WallType::RIGHT
						&& pos.y <= START_WALL_DRAW)
					velocity = -velocity;
				
				// The ball hits the margins side of the walls
				else
					velocity.x = -velocity.x;
			}
		} else if (obj->id == 3) {
			Brick brick = *(Brick *)obj;
			glm::vec2 corner = brick.GetPos();
			((Brick *)obj)->SetExistence(false);
			brickHit = true;
			if (superBall)
				return;
			// Bottom hit
			if (pos.x >= corner.x - radius
				&& pos.x <= corner.x + brick.GetWidth() + radius
				&& pos.y <= corner.y)
				velocity.y = -velocity.y;
			// Left hit
			else if (pos.x <= corner.x
				&& pos.y >= corner.y - radius
				&& pos.y <= corner.y + brick.GetHeight() + radius)
				velocity.x = -velocity.x;
			// Roght hit
			else if (pos.x > corner.x + brick.GetWidth()
				&& pos.y >= corner.y - radius
				&& pos.y <= corner.y + brick.GetHeight() + radius)
				velocity.x = -velocity.x;
			// Upper hit
			else if (pos.x >= corner.x - radius
				&& pos.x <= corner.x + brick.GetWidth() + radius
				&& pos.y >= corner.y)
				velocity.y = -velocity.y;
		}
	}
}

void Ball::Reset() {
	superBall = false;
	pos = initialPos;
	velocity = glm::vec2(0, 5);
}

bool Ball::CheckPowerActive() {
	double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	if (duration > TIME_ACTIVE) {
		superBall = false;
		return false;
	}
	return true;
}

bool Ball::CheckDeath(float width) {
	return pos.y - radius <= 0
		|| pos.x - radius <= 0
		|| pos.x >= width;
}

void Ball::ActivateSuperBall() {
	superBall = true;
	start = clock();
}

void Ball::ResetBrickHit() { brickHit = !brickHit; }
bool Ball::GetBrickHit() { return brickHit; }
float Ball::GetRadius() { return radius; }
glm::vec2 Ball::GetVelocity() {	return velocity; }
void Ball::SetVelocity(glm::vec2 newVel) { velocity = newVel; }
void Ball::ChangePositionOnPlay() { pos += velocity; }