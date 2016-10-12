#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

#define Color_1 sf::Color(147, 69, 173)
#define Color_2 sf::Color(76, 150, 173)

/*
147,69,173
118,80,183
78,80,160
80,120,183
76,150,173
*/

enum FieldState { FIELD_FREE, FIELD_1, FIELD_2 };
enum GameState { GS_INIT, GS_1, GS_2, GS_WIN, GS_LOSE, GS_TIE, GS_UNCHANGED };
enum PlayerState{ P_GS_1, P_GS_2 };

class ConnectFour
{
public:
	ConnectFour(sf::RenderWindow* wndw);
	~ConnectFour();

	void run();
	bool addStone(int pos, FieldState player);
	GameState isFinished();
private:
	sf::Vector2i m_size;
	int** m_board;
	sf::RenderWindow* m_window;
	sf::Clock m_clock;
	int m_currentSelection;
	sf::CircleShape m_circle;
	GameState m_state;
};
