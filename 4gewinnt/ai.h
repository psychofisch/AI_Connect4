#pragma once

#include "ConnectFour.h"

class ai
{
public:
	ai(ConnectFour* gm);
	~ai();
	int think();
private:
	ConnectFour* m_game;
};

