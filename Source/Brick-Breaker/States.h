#pragma once

namespace GameState {
	enum class GameState {
		Not_Started,
		Is_Running,
		You_Died,
		Game_Over,
		Game_Won
	};
	enum class PowerType {
		GUNS,
		SUPER_BALL
	};
}