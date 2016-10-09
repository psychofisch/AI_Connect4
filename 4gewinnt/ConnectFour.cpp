#include "ConnectFour.h"


ConnectFour::ConnectFour(sf::RenderWindow * wndw)
	:m_board(nullptr),
	size(7,6),
	m_currentSelection(0)
{
	m_window = wndw;

	m_board = new int*[size.x];

	for (int x = 0; x < 7; ++x)
	{
		m_board[x] = new int[size.y];
		for (int y = 0; y < 6; ++y)
		{
			m_board[x][y] = FIELD_FREE;
		}
	}
}

ConnectFour::~ConnectFour()
{
}

void ConnectFour::run()
{
	sf::Event eve;

	bool quit = false;
	while (quit == false)
	{
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
					//TODO: Menu...
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

		m_window->clear();
		//Render
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
