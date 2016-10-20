#include "ai.h"

ai::ai(ConnectFour* gm, PlayerInfo rs, bool debug)
	:m_thinking(false),
	m_alive(true)
{
	m_game = gm;
	m_game_cpy = new ConnectFour(*gm);
	//m_board = m_game_cpy.getBoard();
	m_playerNo = rs;
	m_debug = debug;
}


ai::~ai()
{
	delete m_game_cpy;
}

void ai::run()
{
	srand(123);

	while (m_alive)
	{
		if (m_game->calculateHeuristic == true)
		{
			std::cout << heuristic(m_game) << std::endl;
			m_game->printBoard();
			m_game->calculateHeuristic = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		if (!m_debug && m_game->currentPlayer() == m_playerNo)
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
	int pos = negamax(m_game_cpy, 7, -INT_MAX, INT_MAX, m_playerNo, &column);
	m_isFinished = true;
	std::cout << m_playerNo << ": col " << column << " | heur " << pos << std::endl;
	m_game_cpy->printBoard();

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
		//m_game_cpy->printBoard();
		if (finished)
		{
			return -player * 1000;
		}
		int h = heuristic(game);
		return -player * h;
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

		alpha = std::max(alpha, v);
		if (alpha >= beta)
			break;
	}

	return best;
}

int ai::heuristic(ConnectFour * game)
{
	int score = 0;

	//game->printBoard();

	//VERTICAL
	for (int x = 0; x < game->getSize().x; ++x)
	{
		int score_v = 0;

		/*if (x > 2 && x < game->getSize().x - 2)
			score_v++;*/

		for (int y = 0; y <= game->getSize().y - 3; ++y)
		{
			int score_v_i = 0;
			for (int yn = 0; yn <= 4; ++yn)
			{
				if (game->getBoard()[x][y + yn] == P_1)
					score_v_i++;
				//else if (game->getBoard()[x][y + yn] == P_NONE)
				//	score_v_i++;
				else if (game->getBoard()[x][y + yn] == P_2)
				{
					score_v_i = 0;
					break;
				}
			}
			score_v += score_v_i;
		}

		score += score_v;
	}

	//HORIZONTAL
	for (int y = game->getSize().y - 1; y >= 0; --y)
	{
		int score_h = 0;
		int connected = 0;
		bool gap = false;
		PlayerInfo last = P_NONE;
		for (int x = 0; x < game->getSize().x; ++x)
		{
			if (game->getBoard()[x][y] == P_NONE)
			{
				if (last == P_1)
					gap = true;
				else
					gap = false;
				continue;
			}
			else if (game->getBoard()[x][y] == P_1)
			{
				last = P_1;
				connected++;
			}
			else if (game->getBoard()[x][y] == P_2)
			{
				last = P_2;
			}
		}
		score_h = connected;
		score += score_h;
	}
	
	//DIAGONAL
	for (int win_x = 0; win_x < sizeof(winning_fields) / sizeof(winning_fields[0]); ++win_x)
	{
		int score_d = 0;

		for (int yn = 0; yn < 4; ++yn)
		{
			int score_d_i = 0;
			for (int win_y = yn; win_y < yn + 4; ++win_y)
			{
				if (winning_fields[win_x][win_y] == 99)
					break;

				int x = (winning_fields[win_x][win_y] / 10) % 10;
				int y = winning_fields[win_x][win_y] % 10;

				if (game->getBoard()[x][y] == P_1)
					score_d_i++;
				else if (game->getBoard()[x][y] == P_2)
				{
					score_d_i = 0;
					break;
				}

				//std::cout << x << ":" << y << std::endl;
			}
			score_d = score_d_i;
		}

		score += score_d;
	}

	//no need for "if (won)" -> this case gets handled in the negamax function
	return score;
}

