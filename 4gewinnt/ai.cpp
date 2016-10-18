#include "ai.h"

ai::ai(ConnectFour* gm, PlayerInfo rs)
	:m_thinking(false),
	m_alive(true)
{
	m_game = gm;
	m_game_cpy = new ConnectFour(*gm);
	//m_board = m_game_cpy.getBoard();
	m_playerNo = rs;
}


ai::~ai()
{
}

void ai::run()
{
	srand(123);
	while (m_alive)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (m_game->currentPlayer() == m_playerNo)
		{
			m_game_cpy = new ConnectFour(*m_game);
			int move = think();
			if (m_game_cpy->addStone(move, m_playerNo))
			{
				m_game->addStone(move, m_playerNo);
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
	int column = 0;
	int pos = negamax(m_game_cpy, 6, -INT_MAX, INT_MAX, m_playerNo, &column);
	m_isFinished = true;
	return column;
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

int ai::negamax(ConnectFour * game, int depth, int alpha, int beta, int player, int* column)
{
	if (depth == 0 || game->isFinished())
		return player * heuristic(game);

	int best = -INT_MAX;
	for (int x = 0; x < m_game_cpy->getSize().x; ++x)
	{
		if (game->getBoard()[x][0] != P_NONE)
		{
			/*best = -INT_MAX;
			if (column != nullptr)
			{
				if(x == m_game_cpy->getSize().x)

				*column = ;
			}*/
			continue;
		}

		game->addStone(x, static_cast<PlayerInfo>(player));
		int v = -negamax(game, depth - 1, -beta, -alpha, -player, nullptr);
		game->removeLastStone();
		//best = std::max(best, v);
		if (v > best)
		{
			best = v;
			if(column != nullptr)
				*column = x;
		}
	}

	return best;
}

int ai::heuristic(ConnectFour * game)
{
	return rand() % 100;
}

