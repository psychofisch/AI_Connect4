#pragma once

#include <SFML\Graphics.hpp>

enum FieldState { FIELD_FREE, FIELD_1, FIELD_2 };

class ConnectFour
{
public:
	ConnectFour(sf::RenderWindow* wndw);
	~ConnectFour();
	void run();
private:
	sf::Vector2i size;
	int** m_board;
	sf::RenderWindow* m_window;
	sf::Clock m_clock;
	int m_currentSelection;
};
