// 4gewinnt.cpp : Defines the entry point for the console application.
//
#include <SFML\Graphics.hpp>

#include <iostream>
#include <thread>

#include "ConnectFour.h"
#include "ai.h"

int main()
{
	while (1)
	{
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

		std::cout << "give birth to npc...\n";
		ai npc(&game, P_2, false);
		//ai npc2(&game, P_2);

		std::cout << "starting artificial intelligence...\n";
		std::thread ai_thread(&ai::run, &npc);
		//std::thread ai_thread2(&ai::run, &npc2);

		std::cout << "starting game!\n";
		game.run();
		std::cout << "game ended...\n";

		std::cout << "killing ai...\n";
		npc.kill();
		//npc2.kill();

		std::cout << "waiting for ai to die...\n";
		//ai_thread.join();
		//ai_thread2.join();
	}

	std::cout << "bye.\n";
	return 0;
}
