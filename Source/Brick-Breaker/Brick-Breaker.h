#pragma once
#pragma comment(lib, "irrKlang.lib")

#include <Component/SimpleScene.h>
#include <time.h>
#include "Transform2D.h"
#include "Objects/Ball.h"
#include "Powerups/GunPower.h"
#include "Powerups/Powerup.h"
#include "Objects/Platform.h"
#include "States.h"

#define Translate Transform2D::Translate
#define Rotate Transform2D::Rotate
#define Scale Transform2D::Scale
#define WType Wall::WallType
#define LIVES_HEIGHT 30

class BrickBreaker : public SimpleScene {
public:
	struct ViewportSpace {
		ViewportSpace() : x(0), y(0), width(1), height(1) {}
		ViewportSpace(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height) {}
		int x;
		int y;
		int width;
		int height;
	};

	struct LogicSpace {
		LogicSpace() : x(0), y(0), width(1), height(1) {}
		LogicSpace(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {}
		float x;
		float y;
		float width;
		float height;
	};
public:
	BrickBreaker(char);
	~BrickBreaker();
	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

	glm::mat3 VisualizationTransf2D(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);
	void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

	// Reset the ball and the platform at the center of the window,
	// reset the guns and the projectile and updates the state of the game
	void RestartGame();

	// Render the lifes at the bottom of the screen
	void DrawLives(glm::mat3 visMatrix);

	// Render the walls
	void DrawWalls(glm::mat3 visMatrix);

	// Render the ball when the game is not started
	void DrawBallPause(glm::mat3 visMatrix);

	// Render the ball when the game has started,
	// update the balls position, check if the powerup is active
	// and check for collisions with the walls and bricks
	void DrawBallPlay(glm::mat3 visMatrix);

	// Render the platform and check for collision with the ball
	void DrawPlatform(glm::mat3 visMatrix);

	// Check if the platform collided with the powerup
	// and activates the power if so.
	// Render the falling powerup and the bricks.
	void DrawBricks(glm::mat3 visMatrix);

	// Plays background soundtrack if the user pressed "y" or "Y".
	// Checks if game is finished.
	// Draws objects on the scene based on the current state of the game.
	void DrawScene(glm::mat3 visMatrix);
	
	// Check if guns are still active based on a timer.
	// Render the guns and the projectiles.
	// Check if the projectile collided with a brick and
	// if it did bring it back to the gun.
	void DrawPowerups(glm::mat3 visMatrix);

protected:
	// All the objects in the game
	Ball ball;
	Platform platform;
	Wall walls[WALL_NUM];
	Brick bricks[NR_LINE][NR_COL];
	Gun gun[GUN_NUM];
	Powerup power;

	int lifes = 3;
	GameState::GameState state = GameState::GameState::Not_Started;
	ViewportSpace viewSpace;
	LogicSpace logicSpace;
	glm::mat3 modelMatrix, visMatrix;

	// Variables used for sound
	ISoundEngine *soundEngine;
	char **soundPath;
	bool withSound;
	int soundLen;
};