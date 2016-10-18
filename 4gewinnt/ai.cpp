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
	sf::Event eve;

	while (m_alive)
	{
		if (m_game->calculateHeuristic == true)
		{
			std::cout << heuristic(m_game) << std::endl;
			m_game->calculateHeuristic = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	m_isFinished = false;
	int column = 0;
	int pos = negamax(m_game_cpy, 6, -INT_MAX, INT_MAX, m_playerNo, &column);
	m_isFinished = true;
	std::cout << m_playerNo << ": col " << column << " | heur " << pos << std::endl;

	std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> d = std::chrono::duration_cast<std::chrono::duration<float>>(stop - start);

	std::cout << "time needed = " << d.count() << std::endl;

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
	bool finished = game->isFinished();
	if (depth == 0 || finished)
	{
		if (finished)
		{
			return player * 10000;
		}
		return player * heuristic(game);
	}

	int best = -INT_MAX;
	for (int x = 0; x < m_game_cpy->getSize().x; ++x)
	{
		if (game->getBoard()[x][0] != P_NONE)
		{
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
	int connected = 0;
	PlayerInfo current = P_NONE;
	bool won = false;

	//VERTICAL
	for (int x = 0; x < game->getSize().x; ++x)
	{
		int connected_v = 0;
		for (int y = game->getSize().y - 1; y >= 0; --y)
		{
			if (game->getBoard()[x][y] != P_NONE)
			{
				if (game->getBoard()[x][y] == current)
					connected_v++;
				else
				{
					current = static_cast<PlayerInfo>(game->getBoard()[x][y]);
					connected_v = 1;
				}
			}
			else
				break;
				//y = -1; //break condition
		}
		
		if (connected_v >= 4)
		{
			won = true;
			break;
		}
		
		if (connected_v > connected)
			connected = connected_v;
	}

	//HORIZONTAL
	for (int y = game->getSize().y - 1 && !won; y >= 0; --y)
	{
		int connected_h = 0;
		current = P_NONE;
		for (int x = 0; x < game->getSize().x; ++x)
		{
			if (game->getBoard()[x][y] != P_NONE)
			{
				if (game->getBoard()[x][y] == current)
					connected_h++;
				else
				{
					current = static_cast<PlayerInfo>(game->getBoard()[x][y]);
					connected_h = 1;
				}
			}
			else {
				current = P_NONE;
				connected_h = 1;
			}

			if (connected_h >= 4)
			{
				won = true;
				break;
			}
			
			if (connected_h > connected)
				connected = connected_h;
		}
	}

	//DIAGONAL
	for (int i = 0; i < 12 && !won; ++i)
	{
		int connected_d = 0;
		PlayerInfo current = P_NONE;
		for (int j = 0; j < 6; ++j)
		{
			if (winning_fields[i][j] == 99)
				break;

			int x = (winning_fields[i][j] / 10) % 10;
			int y = winning_fields[i][j] % 10;

			if (game->getBoard()[x][y] != P_NONE)
			{
				if (game->getBoard()[x][y] == current)
					connected_d++;
				else
				{
					current = static_cast<PlayerInfo>(game->getBoard()[x][y]);
					connected_d = 1;
				}
			}
			else {
				current = P_NONE;
				connected_d = 1;
			}

			if (connected_d == 4)
			{
				won = true;
				break;
			}

			if (connected_d > connected)
				connected = connected_d;
		}
	}

	int result;

	//no need for "if (won)" -> this case gets handled in the negamax function
	if (connected == 3)
		result = 100;
	else if (connected == 2)
		result = 50;
	else if (connected <= 1)
		result = 10;

	return result;
}

