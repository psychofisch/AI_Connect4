#include "ai.h"

ai::ai(ConnectFour* gm, PlayerInfo rs, int depth, bool alphabeta, bool debug)
	:m_alive(true),
	m_depth(depth),
	m_game(gm),
	m_playerNo(rs),
	m_debug(debug),
	m_alphabeta(alphabeta),
	m_game_cpy(new ConnectFour(*gm))
{
}


ai::~ai()
{
	delete m_game_cpy;
}

void ai::run()
{
	while (m_alive)
	{
		if (m_game->calculateHeuristic == true)
		{
			std::cout << heuristic(m_game, m_game->currentPlayer()) << std::endl;
			m_game->printBoard();
			m_game->calculateHeuristic = false;
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
		if (m_game->currentPlayer() == m_playerNo)
		{
			m_game_cpy = new ConnectFour(*m_game);
			int move = think();
			if (m_game_cpy->addStone(move, m_playerNo))
			{
				m_game->addStone(move, m_playerNo);
				if (m_game->isFinished() || m_game->checkTie())
				{
					kill();//I WON SO I CAN DIE IN PEACE! *harakiri*
					break;
				}

				m_game->nextPlayer();
			}
		}

		if (m_game->currentPlayer() == P_NONE)
			kill();
	}
}

int ai::think()
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	m_isFinished = false;
	int column = 0;
	int score = negamax(m_game_cpy, m_depth, -INT_MAX, INT_MAX, m_playerNo, &column);

	m_isFinished = true;
	//std::cout << std::endl;
	//m_game_cpy->printBoard();
	//std::cout << m_playerNo << ": col " << column << " | score " << score << std::endl;

	std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> d = std::chrono::duration_cast<std::chrono::duration<float>>(stop - start);

	std::cout << "round_time," << d.count() << std::endl;

	//delete[] columns;
	//delete[] best;

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
		int h = 1000 * -player;
		if (finished)
		{
			/*std::cout << std::endl;
			m_game_cpy->printBoard();
			std::cout << h << std::endl;*/
			return h;
		}
		
		h = heuristic(game, player);

		/*std::cout << std::endl;
		m_game_cpy->printBoard();
		std::cout << h << std::endl;*/
		
		return h;
	}

	int last_free_col = 0;
	int full_col_cnt = 0;
	int best = -INT_MAX;
	for (int x = 0; x < m_game_cpy->getSize().x; ++x)
	{
		if (game->getBoard()[x][0] != P_NONE)
		{
			full_col_cnt++;
			continue;
		}

		last_free_col = x;
		if (column != nullptr && full_col_cnt == game->getSize().y)
		{
			*column = x;
			break;
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
		if (m_alphabeta && alpha >= beta)
			break;
	}

	return best;
}

int ai::heuristic(ConnectFour * game, int player)
{
	int score = 0;

	//game->printBoard();

	//VERTICAL
	score += heuristic_v(game, player);

	//HORIZONTAL
	score += heuristic_h(game, player);
	
	////DIAGONAL
	score += heuristic_d(game, player);

	//no need for "if (won)" -> this case gets handled in the negamax function
	return score;
}

int ai::heuristic_v(ConnectFour * game, int player)
{
	int score = 0;

	for (int x = 0; x < game->getSize().x; ++x)
	{
		int score_v = 0;

		if (game->getBoard()[x][game->getSize().y - 1] == P_NONE)
		{
			continue;
		}

		for (int y = 0; y < game->getSize().y - 3; ++y)
		{
			int score_v_i = 0;
			bool useless = false;

			/*if (y == 0 && game->getBoard()[x][1] == P_2 && game->getBoard()[x][2] != P_2)
			{
			score_v -= 1;
			break;
			}*/

			for (int yn = 3; yn >= 0; --yn)
			{
				if (game->getBoard()[x][y + yn] == player)
				{
					if (score_v_i == 0)
						score_v_i++;
					else
						score_v_i *= 2;
				}
				//else if (game->getBoard()[x][y + yn] == P_NONE)
				//	score_v_i++;
				else if (game->getBoard()[x][y + yn] == -player)
				{
					//score_v_i--;
					//useless = true;
					score_v_i = 0;
					break;
				}
			}

			/*if (useless && score_v_i < -2)
			score_v += 3;
			else*/
			score_v += score_v_i;
		}

		score += score_v;
	}

	return score;
}

int ai::heuristic_h(ConnectFour * game, int player)
{
	int score = 0;

	for (int y = game->getSize().y - 1; y >= 0; --y)
	{
		int score_h = 0;
		for (int x = 0; x < game->getSize().x - 3; ++x)
		{
			//SPECIAL-CASE: -xxx-
			if (x > 0 && x < game->getSize().x - 4
				&& game->getBoard()[x][y] == P_NONE
				&& game->getBoard()[x + 1][y] == player
				&& game->getBoard()[x + 2][y] == player
				&& game->getBoard()[x + 3][y] == player
				&& game->getBoard()[x + 4][y] == P_NONE)
			{
				score_h += 100;
				break;
			}
			//****

			int score_h_i = 0;
			for (int xn = 0; xn < 4; ++xn)
			{
				if (game->getBoard()[x + xn][y] == player)
				{
					if (score_h_i == 0)
						score_h_i = 1;
					else
						score_h_i *= 2;
				}
				else if (game->getBoard()[x + xn][y] == -player)
				{
					score_h_i = 0;
					break;
				}
			}
			score_h += score_h_i;
		}
		score += score_h;
	}

	return score;
}

int ai::heuristic_d(ConnectFour * game, int player)
{
	int score = 0;

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
			}
			score_d = score_d_i;
		}

		score += score_d;
	}

	return score;
}

