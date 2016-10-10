#include "ai.h"

ai::ai(ConnectFour* gm)
{
	m_game = gm;
}


ai::~ai()
{
}

int ai::think()
{
	return rand()%7;
}
