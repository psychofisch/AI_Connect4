#include "ConnectFour.h"


ConnectFour::ConnectFour(sf::RenderWindow * wndw)
	:m_board(nullptr),
	m_size(7,6),
	m_currentSelection(0),
	m_gamestate(GS_INIT),
	m_roundstate(P_1)
{
	m_window = wndw;

	m_board = new int*[m_size.x];

	for (int x = 0; x < 7; ++x)
	{
		m_board[x] = new int[m_size.y];
		for (int y = 0; y < 6; ++y)
		{
			m_board[x][y] = FIELD_FREE;
		}
	}

	m_circle.setRadius(46.0f);
	m_circle.setFillColor(sf::Color::Black);
}

ConnectFour::~ConnectFour()
{
	delete[] m_board;
}

void ConnectFour::run()
{
	m_gamestate = GS_RUNNING;

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
					if (m_gamestate != GS_RUNNING || m_roundstate != P_1)
						break;
					FieldState player = FIELD_1;
					//std::cout << mousePos.x << ":" << mousePos.y << " - " << floor((mousePos.x/720)*7) << std::endl;
					/*FieldState player = FIELD_FREE;
					if (m_roundstate == P_1)
						player = FIELD_1;
					else
						player = FIELD_2;

					if (player == FIELD_FREE)
						break;*/

					if (addStone(currentColumn, player))
					{
						isFinished();
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
		sf::CircleShape arrow(46, 3);
		arrow.setRotation(180);
		if(m_roundstate == P_1)
			arrow.setFillColor(Color_1);
		else if(m_roundstate == P_2)
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

				if (m_board[x][y] == FIELD_FREE)
					current = sf::Color::Black;
				else if (m_board[x][y] == FIELD_1)
					current = Color_1;
				else if (m_board[x][y] == FIELD_2)
					current = Color_2;

				m_circle.setFillColor(current);
				m_window->draw(m_circle);
			}
		}
		m_window->display();
		dt = m_clock.getElapsedTime().asSeconds();
	}
}

bool ConnectFour::addStone(int pos, FieldState player)
{
	bool done = false;
	for (int y = m_size.y - 1; y >= 0; --y)
	{
		if (m_board[pos][y] == FIELD_FREE)
		{
			done = true;
			m_board[pos][y] = player;
			break;
		}
	}
	
	return done;
}

bool ConnectFour::isFinished()
{
	int connected;
	FieldState current = FIELD_FREE;
	bool isFinished = false;
	//VERTICAL
	for (int x = 0; x < m_size.x; ++x)
	{
		connected = 0;
		for (int y = m_size.y - 1; y >= 0; --y)
		{
			if (m_board[x][y] != FIELD_FREE)
			{
				if (m_board[x][y] == current)
					connected++;
				else
				{
					current = static_cast<FieldState>(m_board[x][y]);
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
	for (int y = m_size.y - 1; y >= 0; --y)
	{
		connected = 0;
		current = FIELD_FREE;
		for (int x = 0; x < m_size.x; ++x)
		{
			if (m_board[x][y] != FIELD_FREE)
			{
				if (m_board[x][y] == current)
					connected++;
				else
				{
					current = static_cast<FieldState>(m_board[x][y]);
					connected = 1;
				}
			}

			if (connected == 4 || connected == -4)
			{
				isFinished = GS_END;
				break;
			}
		}
	}

	//DIAGONAL
	for (int i = 0; i < 12; ++i)
	{
		int connected = 0;
		FieldState current = FIELD_FREE;
		for (int j = 0; j < 6; ++j)
		{
			if (winning_fields[i][j] == 99)
				break;

			int x = (winning_fields[i][j] / 10) % 10;
			int y = winning_fields[i][j] % 10;

			if (m_board[x][y] != FIELD_FREE)
			{
				if (m_board[x][y] == current)
					connected++;
				else
				{
					current = static_cast<FieldState>(m_board[x][y]);
					connected = 1;
				}
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
	else
		m_roundstate = P_2;

	return isFinished;
}

void ConnectFour::nextPlayer()
{
	if (m_roundstate == P_1)
		m_roundstate = P_2;
	else
		m_roundstate = P_1;
}

RoundState ConnectFour::currentPlayer() const
{
	return m_roundstate;
}

const int ** ConnectFour::getBoard() const
{
	return const_cast<const int**>(m_board);
}
