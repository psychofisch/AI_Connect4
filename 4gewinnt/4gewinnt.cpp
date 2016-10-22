// 4gewinnt.cpp : Defines the entry point for the console application.
//
#include <SFML\Graphics.hpp>

#include <iostream>
#include <thread>

#include "ConnectFour.h"
#include "ai.h"

int main(int argc, char* argv[])
{
	int ai_depth = 5;
	int mode = 0;


	//while (1)
	//{
		sf::VideoMode vm;
		vm.width = 720;
		vm.height = 720;
		vm.bitsPerPixel = 32;

		sf::ContextSettings settings;
		settings.antialiasingLevel = 2;

		std::cout << "creating window...\n";
		sf::RenderWindow window(vm, "Connect Four", sf::Style::Titlebar | sf::Style::Close, settings);
		window.setVerticalSyncEnabled(false);
		window.setFramerateLimit(60);

		std::cout << "creating game...\n";
		ConnectFour game;
		game.setWindow(&window);

		ai* npc = nullptr;
		std::thread* ai_thread = nullptr;
		if (mode == 0 || mode == 2)
		{
			std::cout << "give birth to npc one...\n";
			npc = new ai(&game, P_2, ai_depth);
			std::cout << "starting first artificial intelligence...\n";
			ai_thread = new std::thread(&ai::run, npc);
			game.setHuman(P_1);
		}

		ai* npc2 = nullptr;
		std::thread* ai_thread2 = nullptr;
		if (mode == 1 || mode == 2)
		{
			std::cout << "give birth to npc two...\n";
			npc2 = new ai(&game, P_1, ai_depth);
			std::cout << "starting second artificial intelligence...\n";
			ai_thread2 = new std::thread(&ai::run, npc2);
			game.setHuman(P_2);
		}

		if (mode == 2)
			game.setHuman(P_NONE);

		std::cout << "starting game!\n";
		game.run();
		std::cout << "game ended...\n";

		std::cout << "killing ai...\n";
		if (npc != nullptr)
		{
			npc->kill();
			ai_thread->join();
		}

		if (npc2 != nullptr)
		{
			npc2->kill();
			ai_thread2->join();
		}

		std::cout << "cleaning up...\n";

		delete npc;
		delete npc2;
		delete ai_thread;
		delete ai_thread2;
	//}

	std::cout << "bye.\n";
	return 0;
}
