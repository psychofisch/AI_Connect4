#include "ConnectFour.h"


ConnectFour::ConnectFour(sf::RenderWindow * wndw)
	:m_board(nullptr),
	m_size(7,6),
	m_currentSelection(0),
	m_state(GS_INIT)
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
	m_state = GS_1;

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
					

					std::cout << mousePos.x << ":" << mousePos.y << " - " << floor((mousePos.x/720)*7) << std::endl;
					FieldState player = FIELD_FREE;
					if (m_state == GS_1)
						player = FIELD_1;
					else if (m_state == GS_2)
						player = FIELD_2;

					if (player == FIELD_FREE)
						break;
					bool added = addStone(currentColumn, player);

					if (added)
						isFinished();

					if (added && m_state == GS_1)
						m_state = GS_2;
					else
						m_state = GS_1;
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

		m_window->clear(sf::Color::White);
		//Render
		sf::CircleShape arrow(46, 3);
		arrow.setRotation(180);
		if(m_state == GS_1)
			arrow.setFillColor(Color_1);
		else if(m_state == GS_2)
			arrow.setFillColor(Color_2);
		arrow.setPosition(sf::Vector2f((720.0f / 7) + floor((mousePos.x / 720) * 7) * (720.0f / 7), 100));
		m_window->draw(arrow);

		for (int x = 0; x < 7; ++x)
		{
			//m_board[x] = new int[m_size.y];
			for (int y = 0; y < 6; ++y)
			{
				m_circle.setPosition(sf::Vector2f(23 + (x*100), 110.0f + (y * 100)));
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
		/*
		m_window->draw(background);

		element* tiles = m_board->getElements();
		for (int i = 0; i < m_board->getSize() * m_board->getSize(); ++i)
		{
			if (tiles[i].isEnabled())
			{
				sf::Vector2f pos(70.0f * tiles[i].getX(), 70.0f * tiles[i].getY());
				tileSprite.setPosition(pos);
				m_window->draw(tileSprite);

				values.setString(std::to_string(tiles[i].getValue()));
				values.setPosition(pos + (sf::Vector2f(values.getGlobalBounds().width / 2, values.getGlobalBounds().height / 2)));
				m_window->draw(values);
			}
		}
		//****
		*/
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

GameState ConnectFour::isFinished()
{
	int count;
	FieldState player;

	for (int x = 0; x < m_size.x; ++x)
	{
		int connected[2] = { 0, 0};
		for (int y = m_size.y - 1; y >= 0; --y)
		{
			std::cout << m_board[x][y];
			if (m_board[x][y] == FIELD_1)
				connected[0]++;
			else if (m_board[x][y] == FIELD_2)
				connected[1]++;
			else if (m_board[x][y] == FIELD_FREE)
			{
				break;
			}
		}
		std::cout << std::endl;
	}

	return GS_UNCHANGED;
}
