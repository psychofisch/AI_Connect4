#pragma once

#include "ConnectFour.h"

class ai
{
public:
	ai(ConnectFour* gm);
	~ai();
private:
	ConnectFour* m_game;
};

