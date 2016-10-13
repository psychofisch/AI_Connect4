#pragma once

#include <thread>
#include <chrono>

#include "ConnectFour.h"
#include "winning_fields.h"

class ai
{
public:
	ai(ConnectFour* gm, RoundState rs);
	~ai();
	void run();
	int think();
	bool isFinished();
	void kill();
private:
	bool m_alive;
	ConnectFour* m_game;
	const int** m_board;
	bool m_isFinished;
	RoundState m_playerNo;
	bool m_thinking;
};
