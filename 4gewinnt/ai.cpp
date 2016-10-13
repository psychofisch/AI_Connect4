#include "ai.h"

ai::ai(ConnectFour* gm, RoundState rs)
	:m_thinking(false),
	m_alive(true)
{
	m_game = gm;
	m_board = m_game->getBoard();
	m_playerNo = rs;
}


ai::~ai()
{
}

void ai::run()
{
	while (m_alive)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (m_game->currentPlayer() == m_playerNo)
		{
			if (m_game->addStone(think(), (m_playerNo == P_1)?FIELD_1:FIELD_2))
			{
				if (m_game->isFinished())
				{
					kill();//I WON SO I CAN DIE IN PEACE! *harakiri*
				}
				m_game->nextPlayer();
			}
		}
	}
}

int ai::think()
{
	m_isFinished = false;
	m_isFinished = true;
	return rand()%7;
}

bool ai::isFinished()
{
	return m_isFinished;
}

void ai::kill()
{
	m_alive = false;
}
