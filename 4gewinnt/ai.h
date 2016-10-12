#pragma once

#include "ConnectFour.h"
#include "winning_fields.h"

class ai
{
public:
	ai(ConnectFour* gm);
	~ai();
	int think();
private:
	ConnectFour* m_game;
};
