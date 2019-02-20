#include "Brick-Breaker.h"

char *path_to_sound[] = { "\\breakout.mp3", "\\bleep.wav", "\\solid.wav",
"\\bleep.mp3", "\\powerup.wav", "\\firegun.wav" };

BrickBreaker::BrickBreaker(char answer) {
	if (answer == 'y' || answer == 'Y')
		withSound = true;
	else
		withSound = false;

	if (withSound) {
		// Create the sound engine using irrKlang library
		soundEngine = createIrrKlangDevice();
		if (!soundEngine) {
			std::cout << "Error! Sound engine down!" << std::endl;
		}
	}
	// Initialize the array of paths to the sound files
	soundLen = sizeof(path_to_sound) / sizeof(path_to_sound[0]);
	soundPath = new char*[soundLen];
	for (int i = 0; i < soundLen; i++)
		soundPath[i] = new char[MAX_PATH];

	glm::vec2 res = window->GetResolution();
	ball = Ball::Ball(glm::vec2(res.x / 2 - ball.GetRadius() / 2, BALL_DRAW_HEIGHT));
	platform = Platform::Platform(glm::vec2(res.x / 2, 15));
	float width = platform.GetWidth() / 2;
	platform.ChangePos(glm::vec2(res.x / 2 - width, platform.GetPos().y));
	
	// Create all the bricks
	float widthB, heightB;
	widthB = res.x - 2 * BRICK_START_X - (NR_COL - 1) * BRICK_WIDTH_GAP;
	widthB /= NR_COL;
	heightB = res.y - BRICK_START_Y - (NR_LINE - 1) * BRICK_HEIGHT_GAP;
	heightB /= NR_LINE;
	heightB /= 2;
	glm::vec2 startBrick(BRICK_START_X, res.y - BRICK_START_Y);
	bricks[0][0] = Brick::Brick(std::string("brick00"), startBrick, widthB, heightB);
	for (int i = 0; i < NR_LINE; i++) {
		for (int j = (i == 0 ? 1 : 0); j < NR_COL; j++) {
			glm::vec2 prevPos;
			if (j == 0) {
				prevPos.x = bricks[0][0].GetPos().x;
				prevPos.y = bricks[i - 1][j].GetPos().y - BRICK_HEIGHT_GAP
					- bricks[i - 1][j].GetHeight();
			}
			else {
				prevPos = bricks[i][j - 1].GetPos();
				prevPos.x += bricks[i][j - 1].GetWidth() + BRICK_WIDTH_GAP;
			}
			std::string str = std::to_string(i) + std::to_string(j);
			bricks[i][j] = Brick::Brick(std::string("brick" + str), prevPos, widthB, heightB);
		}
	}

	// Create walls
	float wallLeftLen = res.y - START_WALL_DRAW - WALL_WIDTH / 2;
	glm::vec2 wallPosLeft(0, START_WALL_DRAW);
	std::string wallLeftName = std::string("Wall1");
	walls[0] = Wall::Wall(wallLeftName, wallLeftLen, wallPosLeft, WType::LEFT);
	
	glm::vec2 wallPosUp(0, res.y - WALL_WIDTH);
	std::string wallUpName = std::string("Wall2");
	walls[1] = Wall::Wall(wallUpName, res.x, wallPosUp, WType::UPPER);

	glm::vec2 wallPosR(res.x - WALL_WIDTH, START_WALL_DRAW);
	float wallRightLen = res.y - START_WALL_DRAW;
	std::string wallRName = std::string("Wall3");
	walls[2] = Wall::Wall(wallRName, wallRightLen, wallPosR, WType::RIGHT);

	// Create guns
	std::string gunName("Gun0");
	glm::vec2 gunPos = platform.GetPos() + glm::vec2(0, platform.GetHeight());
	gun[0] = Gun::Gun(gunName, gunPos);

	gunName = std::string("Gun1");
	float height = platform.GetHeight();
	gunPos = platform.GetPos() + glm::vec2(height, height);
	gun[1] = Gun::Gun(gunName, gunPos);
}

BrickBreaker::~BrickBreaker() {
	// Delete the sound engine
	if (withSound && soundEngine)
		soundEngine->drop();
	for (int i = 0; i < soundLen; i++)
		delete[] soundPath[i];
	delete soundPath;
}

void BrickBreaker::Init() {
	
	glm::vec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	
	// Create array of paths to every sound file
	if (withSound) {
		for (int i = 0; i < soundLen; i++) {
			GetCurrentDirectoryA(MAX_PATH, soundPath[i]);
			strcat(soundPath[i], "\\Resources\\Soundtracks");
			strcat(soundPath[i], path_to_sound[i]);
		}
	}

	// Initialize logicSpace
	logicSpace.x = 0;
	logicSpace.y = 0;
	logicSpace.width = resolution.x;
	logicSpace.height = resolution.y;

	// Create meshes and add the to the list
	Mesh* platformMesh = platform.CreatePlatform(
		platform.GetName(), glm::vec3(0, 1, 0));
	Mesh* ballMesh = ball.CreateBall(ball.GetName(), glm::vec3(1));
	Mesh* lives = ball.CreateBall(std::string("lives"), glm::vec3(1));
	glm::vec3 color(
		(double)rand() / RAND_MAX,
		(double)rand() / RAND_MAX,
		(double)rand() / RAND_MAX
	);

	for (int i = 0; i < NR_LINE; i++) {
		for (int j = 0; j < NR_COL; j++) {
			Brick& brick = bricks[i][j];
			Mesh* brickMesh = brick.CreateBrick(brick.GetName(), color);
			AddMeshToList(brickMesh);
			if (brick.GetPower().GetExistence()) {
				Powerup& power = brick.GetPower();
				if (power.GetPowerType() == GameState::PowerType::GUNS)
					AddMeshToList(power.CreatePowerup(
						power.GetName(), glm::vec3(0, 0, 1)));
				else
					AddMeshToList(power.CreatePowerup(
						power.GetName(), glm::vec3(1, 0, 1)));
			}
		}
	}

	for (int i = 0; i < WALL_NUM; i++) {
		Mesh *wall = walls[i].CreateWall(
			walls[i].GetName(), glm::vec3(1, 0, 0));
		AddMeshToList(wall);
	}

	for (int i = 0; i < GUN_NUM; i++) {
		AddMeshToList(gun[i].CreateGun(
			gun[i].GetName(), glm::vec3(0, 0, 1)));
		Projectile& projectile = gun[i].GetProjectile();
		AddMeshToList(projectile.CreateProjectile(
			projectile.GetName(), glm::vec3(1, 1, 0)));
	}
	AddMeshToList(platformMesh);
	AddMeshToList(ballMesh);
	AddMeshToList(lives);

	if (withSound && soundEngine) {
		soundEngine->play2D(soundPath[0], true);
	}
}

glm::mat3 BrickBreaker::VisualizationTransf2D(const LogicSpace &logicSpace,
	const ViewportSpace &viewSpace) {
	
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;
	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

void BrickBreaker::SetViewportArea(const ViewportSpace & viewSpace,
	glm::vec3 colorColor, bool clear) {
	
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	GetSceneCamera()->SetOrthographic((float)viewSpace.x,
		(float)(viewSpace.x + viewSpace.width), (float)viewSpace.y,
		(float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void BrickBreaker::FrameStart() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BrickBreaker::Update(float deltaTimeSeconds) {
	glm::ivec2 resolution = window->GetResolution();
	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0), true);
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);
	DrawScene(visMatrix);
}

void BrickBreaker::DrawLives(glm::mat3 visMatrix) {
	for (int i = 1; i <= lifes; i++) {
		modelMatrix = visMatrix;
		modelMatrix *= Translate((float) i * LIVES_HEIGHT, LIVES_HEIGHT);
		RenderMesh2D(meshes["lives"], shaders["VertexColor"], modelMatrix);
	}
	modelMatrix = visMatrix;
}

void BrickBreaker::DrawWalls(glm::mat3 visMatrix) {
	for (int i = 0; i < WALL_NUM; i++) {
		modelMatrix = visMatrix;
		modelMatrix *= Translate(walls[i].GetPos().x, walls[i].GetPos().y);
		std::string name(walls[i].GetName());
		RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
	}
	modelMatrix = visMatrix;
}

void BrickBreaker::DrawBallPause(glm::mat3 visMatrix) {
	modelMatrix = visMatrix * Translate(ball.GetPos().x, ball.GetPos().y);
	RenderMesh2D(meshes[ball.GetName()], shaders["VertexColor"], modelMatrix);
}

void BrickBreaker::DrawPlatform(glm::mat3 visMatrix) {
	platform.Collision(&ball, withSound, soundPath[3], soundEngine);
	modelMatrix = visMatrix;
	modelMatrix *= Translate(platform.GetPos().x, platform.GetPos().y);
	std::string name(platform.GetName());
	RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
}

void BrickBreaker::DrawBallPlay(glm::mat3 visMatrix) {
	ball.ChangePositionOnPlay();
	DrawBallPause(visMatrix);
	ball.CheckPowerActive();
	for (int i = 0; i < WALL_NUM; i++)
		ball.Collision(&walls[i], withSound, soundPath[2], soundEngine);
	for (int i = 0; i < NR_LINE; i++) {
		for (int j = 0; j < NR_COL; j++) {
			Brick& brick = bricks[i][j];
			ball.Collision(&brick, withSound, soundPath[1], soundEngine);
		}
	}

	if (ball.CheckDeath((float) window->GetResolution().x))
		state = GameState::GameState::You_Died;
	if (ball.GetBrickHit())
		ball.ResetBrickHit();
}

void BrickBreaker::RestartGame() {
	glm::vec2 resolution = window->GetResolution();
	ball.Reset();
	platform.Reset();
	
	for (int i = 0; i < NR_LINE; i++) {
		for (int j = 0; j < NR_COL; j++) {
			bricks[i][j].Reset(state);
		}
	}
	for (int i = 0; i < GUN_NUM; i++)
		gun[i].Reset();

	if (state == GameState::GameState::Game_Over) {
		state = GameState::GameState::Not_Started;
		lifes = 3;
	} else if (state == GameState::GameState::You_Died) {
		--lifes;
		if (lifes == 0)
			state = GameState::GameState::Game_Over;
		else
			state = GameState::GameState::Not_Started;
	} else {
		lifes = 3;
		state = GameState::GameState::Not_Started;
	}
}

void BrickBreaker::DrawBricks(glm::mat3 visMatrix) {
	for (int i = 0; i < NR_LINE; i++) {
		for (int j = 0; j < NR_COL; j++) {
			if (!bricks[i][j].GetExistence()) {
				Powerup &power = bricks[i][j].GetPower();
				if (bricks[i][j].HasPower()) {
					if (power.GetExistence()) {
						bool hit = platform.Collision(&power, withSound,
							soundPath[4], soundEngine);
						GameState::PowerType powerType = power.GetPowerType();
						if (powerType == GameState::PowerType::GUNS && hit) {
							for (int i = 0; i < GUN_NUM; i++)
								gun[i].SetActive();
						}
						if (powerType == GameState::PowerType::SUPER_BALL
							&& hit) {
								ball.ActivateSuperBall();
						}
						power.Update();
						modelMatrix = power.CreateModelMatrix(visMatrix);
						RenderMesh2D(meshes[power.GetName()],
							shaders["VertexColor"], modelMatrix);
					}
				}
				bricks[i][j].ReduceScale(SCALE_REDUCTION);
			}
			modelMatrix = bricks[i][j].CreateModelMatrix(visMatrix);
			RenderMesh2D(meshes[bricks[i][j].GetName()],
				shaders["VertexColor"], modelMatrix);
		}
	}
	modelMatrix = visMatrix;
}

void BrickBreaker::DrawPowerups(glm::mat3 visMatrix) {
	if (gun[0].IsActive()) {
		for (int i = 0; i < GUN_NUM; i++) {
			gun[i].CheckTimeActive();
			modelMatrix = visMatrix;
			modelMatrix *= Translate(gun[i].GetPos().x, gun[i].GetPos().y);
			RenderMesh2D(meshes[gun[i].GetName()],
				shaders["VertexColor"], modelMatrix);
			Projectile& projectile = gun[i].GetProjectile();
			if (gun[i].GetShoot()) {
				if (projectile.GetExistence()) {
					modelMatrix = visMatrix;
					glm::vec2 pos = projectile.GetPos();
					modelMatrix *= Translate(pos.x, pos.y);
					RenderMesh2D(meshes[projectile.GetName()],
						shaders["VertexColor"], modelMatrix);
					gun[i].ShootGun();
					for (int j = 0; j < NR_LINE; j++) {
						for (int k = 0; k < NR_COL; k++) {
							if (projectile.Collision(&bricks[j][k], withSound,
									soundPath[1], soundEngine)
								|| projectile.GetPos().y > walls[1].GetPos().y) {
								glm::vec2 newPos = gun[i].GetPos();
								newPos += gun[i].GetLength();
								projectile.ChangePos(newPos);
								projectile.SetExistence(false);
								break;
							}
						}
					}
				}
			}
		}
		modelMatrix = visMatrix;
	}
}

void BrickBreaker::DrawScene(glm::mat3 visMatrix) {
	bool gameWon = true;
	for (int i = 0; i < NR_LINE; i++) {
		for (int j = 0; j < NR_COL; j++) {
			if (bricks[i][j].GetExistence())
				gameWon = false;
		}
	}
	if (gameWon)
		state = GameState::GameState::Game_Won;

	DrawWalls(visMatrix);
	DrawLives(visMatrix);
	DrawPlatform(visMatrix);
	DrawBricks(visMatrix);
	switch (state) {
	case GameState::GameState::Not_Started:
		DrawBallPause(visMatrix);
		break;
	case GameState::GameState::Is_Running:
		DrawPowerups(visMatrix);
		DrawBallPlay(visMatrix);
		break;
	case GameState::GameState::Game_Won:
		RestartGame();
		DrawBallPause(visMatrix);
		break;
	default:
		RestartGame();
		break;
	}
}

void BrickBreaker::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
	glm::ivec2 resolution = window->GetResolution();
	float platfWidth = platform.GetWidth();
	float platfHeight = platform.GetHeight();
	// Change position of the ball based on mouse if the game is on pause
	if (state == GameState::GameState::Not_Started) {
		ball.ChangePos(glm::vec2((float) mouseX, ball.GetPos().y));
	}
	// Change platform, guns and projectile positions based on mouse position
	platform.ChangePos(glm::vec2(mouseX - platfWidth / 2, platform.GetPos().y));
	gun[0].ChangePos(platform.GetPos() + glm::vec2(0, platfHeight));
	gun[1].ChangePos(platform.GetPos()
		+ glm::vec2(platfWidth - gun[1].GetLength(), platfHeight));
	for (int i = 0; i < GUN_NUM; i++) {
		Projectile& projectile = gun[i].GetProjectile();
		if (!projectile.GetExistence())
			projectile.ChangePos(gun[i].GetPos() + gun[i].GetLength() / 2);
	}
}

void BrickBreaker::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	// The game has started, change the state
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)
		&& state == GameState::GameState::Not_Started)
			state = GameState::GameState::Is_Running;
	
	// Shooting the guns
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)
		&& state == GameState::GameState::Is_Running) {
		if (gun[0].IsActive()) {
			for (int i = 0; i < GUN_NUM; i++) {
				Projectile& projectile = gun[i].GetProjectile();
				if (!projectile.GetExistence()) {
					if (withSound && soundEngine) {
						soundEngine->play2D(soundPath[5]);
					}
					projectile.SetExistence(true);
				}
				gun[i].SetShoot(true);
			}
		}
	}
}