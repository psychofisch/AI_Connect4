#include "ai.h"

ai::ai(ConnectFour* gm, PlayerInfo rs)
	:m_thinking(false),
	m_alive(true)
{
	m_game = gm;
	//m_board = m_game->getBoard();
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
			if (m_game->addStone(think(), m_playerNo))
			{
				if (m_game->isFinished())
				{
					kill();//I WON SO I CAN DIE IN PEACE! *harakiri*
					break;
				}
				m_game->nextPlayer();
			}
		}
	}
}

int ai::think()
{
	m_isFinished = false;
	int pos = negamax(m_game, 5, -INFINITY, INFINITY, m_playerNo);
	m_isFinished = true;
	return pos;
}

bool ai::isFinished()
{
	return m_isFinished;
}

void ai::kill()
{
	m_alive = false;
}

/*
WIKIPEDIA: Negamax
01 function negamax(node, depth, α, β, color)
02     if depth = 0 or node is a terminal node
03         return color * the heuristic value of node
04     childNodes := GenerateMoves(node)
05     childNodes := OrderMoves(childNodes)
06     bestValue := −∞
07     foreach child in childNodes
08         v := −negamax(child, depth − 1, −β, −α, −color)
09         bestValue := max( bestValue, v )
10         α := max( α, v )
11         if α ≥ β
12             break
13     return bestValue
*/

int ai::negamax(ConnectFour * game, int depth, int alpha, int beta, int player)
{
	if (depth == 0 || game->isFinished())
		return player * heuristic(game);

	int best = 0;
	for (int x = 0; x < m_game->getSize().x; ++x)
	{
		if (game->getBoard()[best][5] != P_NONE)
			continue;

		int v = -negamax(game, depth - 1, -beta, -alpha, -player);
		best = std::max(0, v);
	}

	return best;
}

int ai::heuristic(ConnectFour * game)
{
	return rand() % 100;
}

