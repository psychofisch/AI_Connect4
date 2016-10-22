#pragma once

#include <thread>
#include <chrono>
#include <algorithm>
#include <chrono>

#include "ConnectFour.h"
#include "winning_fields.h"

class ai
{
public:
	ai(ConnectFour* gm, PlayerInfo rs, int depth, bool debug = false);
	~ai();
	void run();
	int think();
	bool isFinished();
	void kill();
private:
	bool m_alive;
	ConnectFour* m_game;
	ConnectFour* m_game_cpy;
	//const int** m_board;
	bool m_isFinished;
	PlayerInfo m_playerNo;
	bool m_thinking;
	int m_depth;
	int negamax(ConnectFour* game, int depth, int alpha, int beta, int player, int* column);
	int heuristic(ConnectFour* game, int player);
	int heuristic_v(ConnectFour* game, int player);
	int heuristic_h(ConnectFour* game, int player);
	int heuristic_d(ConnectFour* game, int player);
	bool m_debug;
};
