#include "ConnectFour.h"


ConnectFour::ConnectFour()
	:m_board(nullptr),
	m_size(7, 6),
	m_currentSelection(0),
	m_gamestate(GS_INIT),
	m_playerInfo(P_1)
{
	m_board = new int*[m_size.x];

	for (int x = 0; x < m_size.x; ++x)
	{
		m_board[x] = new int[m_size.y];
		for (int y = 0; y < m_size.y; ++y)
		{
			m_board[x][y] = P_NONE;
		}
	}

	m_circle.setRadius(46.0f);
	m_circle.setFillColor(sf::Color::Black);
}

ConnectFour::~ConnectFour()
{
	for (int x = 0; x < m_size.x; ++x)
	{
		delete m_board[x];
	}

	delete[] m_board;
}

ConnectFour::ConnectFour(ConnectFour & other)
	:m_size(other.m_size),
	m_window(other.m_window),
	m_currentSelection(other.m_currentSelection),
	m_gamestate(other.m_gamestate),
	m_playerInfo(other.m_playerInfo),
	m_lastMoves(other.m_lastMoves)
{
	m_board = new int*[m_size.x];

	for (int x = 0; x < m_size.x; ++x)
	{
		m_board[x] = new int[m_size.y];
		for (int y = 0; y < m_size.y; ++y)
		{
			m_board[x][y] = static_cast<PlayerInfo>(other.m_board[x][y]);
		}
	}

	m_circle.setRadius(46.0f);
	m_circle.setFillColor(sf::Color::Black);
}

void ConnectFour::run()
{
	m_gamestate = GS_RUNNING;
	calculateHeuristic = false;

	sf::Event eve;

	bool quit = false;
	while (quit == false)
	{
		sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));

		int currentColumn = floor((mousePos.x / 720) * 7);

		float dt;
		m_clock.restart();

		//Keyboard
		while (m_window->pollEvent(eve))
		{
			if (eve.type == sf::Event::Closed)
			{
				quit = true;
				break;
			}

			if (eve.type == sf::Event::MouseButtonPressed)
			{
				if (eve.mouseButton.button == sf::Mouse::Left)
				{
					if (m_gamestate != GS_RUNNING || m_playerInfo != P_1)
						break;
					//std::cout << mousePos.x << ":" << mousePos.y << " - " << floor((mousePos.x/720)*7) << std::endl;
					/*FieldState player = P_NONE;
					if (m_roundstate == P_1)
						player = FIELD_1;
					else
						player = FIELD_2;

					if (player == P_NONE)
						break;*/

					if (addStone(currentColumn, m_playerInfo))
					{
						if (!isFinished())
							m_playerInfo = static_cast<PlayerInfo>(-m_playerInfo);
					}
					/*if (added)
						if (m_gamestate == GS_END)
							break;

					if (added && m_roundstate == P_1)
						m_roundstate = P_2;
					else
						m_roundstate = P_1;*/
				}
				break;
			}

			if (eve.type == sf::Event::KeyPressed)
			{
				switch (eve.key.code)
				{
				/*case sf::Keyboard::W:
					moveTo(Direction::Up);
					break;

				case sf::Keyboard::A:
					moveTo(Direction::Left);
					break;

				case sf::Keyboard::S:
					moveTo(Direction::Down);
					break;

				case sf::Keyboard::D:
					moveTo(Direction::Right);
					break;
					*/
				case sf::Keyboard::H:
					calculateHeuristic = true;
					break;
				case sf::Keyboard::R:
					removeLastStone();
					break;
				case sf::Keyboard::P:
					//m_board->addElement(rand() % m_board->getSize(), rand() % m_board->getSize());
					break;

				case sf::Keyboard::Escape:
					quit = true;
					break;
				}
			}
		}
		//****

		if(m_gamestate == GS_END)
			m_window->clear(sf::Color::Black);
		else
			m_window->clear(sf::Color::White);
		//Render
		sf::CircleShape arrow(36, 3);
		arrow.setRotation(180);
		if(m_playerInfo == P_1)
			arrow.setFillColor(Color_1);
		else if(m_playerInfo == P_2)
			arrow.setFillColor(Color_2);
		arrow.setPosition(sf::Vector2f((720.0f / 7) + floor((mousePos.x / 720) * 7) * (720.0f / 7), 100));
		m_window->draw(arrow);

		for (int x = 0; x < 7; ++x)
		{
			//m_board[x] = new int[m_size.y];
			for (int y = 0; y < 6; ++y)
			{
				m_circle.setPosition(sf::Vector2f(23.0f + (x*100), 110.0f + (y * 100)));
				sf::Color current(sf::Color::Red);

				if (m_board[x][y] == P_NONE)
					current = sf::Color::Black;
				else if (m_board[x][y] == P_1)
					current = Color_1;
				else if (m_board[x][y] == P_2)
					current = Color_2;

				m_circle.setFillColor(current);
				m_window->draw(m_circle);
			}
		}
		m_window->display();
		dt = m_clock.getElapsedTime().asSeconds();
	}
}

bool ConnectFour::addStone(int pos, PlayerInfo player)
{
	bool done = false;
	for (int y = m_size.y - 1; y >= 0; --y)
	{
		if (m_board[pos][y] == P_NONE)
		{
			done = true;
			m_board[pos][y] = player;
			m_lastMoves.push(pos);
			break;
		}
	}
	
	return done;
}

bool ConnectFour::isFinished()
{
	int connected;
	PlayerInfo current = P_NONE;
	bool isFinished = false;

	//VERTICAL
	for (int x = 0; x < m_size.x; ++x)
	{
		connected = 0;
		for (int y = m_size.y - 1; y >= 0; --y)
		{
			if (m_board[x][y] != P_NONE)
			{
				if (m_board[x][y] == current)
					connected++;
				else
				{
					current = static_cast<PlayerInfo>(m_board[x][y]);
					connected = 1;
				}
			}
			else
				y = -1; //break condition
		}

		if (connected == 4 || connected == -4)
		{
			isFinished = true;
			break;
		}
	}

	//HORIZONTAL
	for (int y = m_size.y - 1; y >= 0 && !isFinished; --y)
	{
		connected = 0;
		current = P_NONE;
		for (int x = 0; x < m_size.x; ++x)
		{
			if (m_board[x][y] != P_NONE)
			{
				if (m_board[x][y] == current)
					connected++;
				else
				{
					current = static_cast<PlayerInfo>(m_board[x][y]);
					connected = 1;
				}
			}
			else {
				current = P_NONE;
				connected = 1;
			}

			if (connected == 4 || connected == -4)
			{
				isFinished = GS_END;
				break;
			}
		}
	}

	//DIAGONAL
	for (int i = 0; i < 12 && !isFinished; ++i)
	{
		int connected = 0;
		PlayerInfo current = P_NONE;
		for (int j = 0; j < 6; ++j)
		{
			if (winning_fields[i][j] == 99)
				break;

			int x = (winning_fields[i][j] / 10) % 10;
			int y = winning_fields[i][j] % 10;

			if (m_board[x][y] != P_NONE)
			{
				if (m_board[x][y] == current)
					connected++;
				else
				{
					current = static_cast<PlayerInfo>(m_board[x][y]);
					connected = 1;
				}
			}
			else {
				current = P_NONE;
				connected = 1;
			}

			if (connected == 4 || connected == -4)
			{
				isFinished = true;
				break;
			}
		}
	}

	if (isFinished)
	{
		m_gamestate = GS_END;
	}

	return isFinished;
}

void ConnectFour::nextPlayer()
{
	m_playerInfo = static_cast<PlayerInfo>(-m_playerInfo);
}

PlayerInfo ConnectFour::currentPlayer() const
{
	return m_playerInfo;
}

const int ** const ConnectFour::getBoard() const
{
	return const_cast<const int**>(m_board);
	//return (m_board);
}

int ConnectFour::getLastMove()
{
	return (m_lastMoves.size() <= 0) ? -1 : m_lastMoves.top();
}

bool ConnectFour::removeLastStone()
{
	if (m_lastMoves.size() == 0)
		return false;

	int last = m_lastMoves.top();

	for (int y = 0; y < m_size.y; ++y)
	{
		if (m_board[last][y] != P_NONE)
		{
			m_board[last][y] = P_NONE;
			m_lastMoves.pop();
			return true;
		}
	}

	return false;
}

sf::Vector2i ConnectFour::getSize()
{
	return m_size;
}

void ConnectFour::setWindow(sf::RenderWindow * wndw)
{
	m_window = wndw;
}
