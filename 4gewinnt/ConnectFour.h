#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <stack>

#include "winning_fields.h"

#define Color_1 sf::Color(147, 69, 173)
#define Color_2 sf::Color(76, 150, 173)

/*
147,69,173
118,80,183
78,80,160
80,120,183
76,150,173
*/

enum GameState { GS_INIT, GS_RUNNING, GS_END, GS_UNCHANGED };
enum PlayerInfo{ P_1 = -1, P_2 = 1, P_NONE = 0 };

class ConnectFour
{
public:
	ConnectFour();
	~ConnectFour();
	ConnectFour(ConnectFour& other);
	void run();
	bool addStone(int pos, PlayerInfo player);
	bool isFinished();
	void nextPlayer();
	PlayerInfo currentPlayer() const;
	const int** const getBoard() const;
	int getLastMove();
	bool removeLastStone();
	sf::Vector2i getSize();
	void setWindow(sf::RenderWindow* wndw);
private:
	sf::Vector2i m_size;
	int** m_board;
	sf::RenderWindow* m_window;
	sf::Clock m_clock;
	int m_currentSelection;
	sf::CircleShape m_circle;
	GameState m_gamestate;
	PlayerInfo m_playerInfo;
	std::stack<int> m_lastMoves;
};
