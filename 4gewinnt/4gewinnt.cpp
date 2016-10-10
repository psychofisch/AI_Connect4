// 4gewinnt.cpp : Defines the entry point for the console application.
//
#include <SFML\Graphics.hpp>

#include <iostream>

#include "ConnectFour.h"

int main()
{
	sf::VideoMode vm;
	vm.width = 720;
	vm.height = 720;
	vm.bitsPerPixel = 32;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;

	sf::RenderWindow window(vm, "Connect Four", sf::Style::Titlebar | sf::Style::Close, settings);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);

	ConnectFour game(&window);

	game.run();

	return 0;
}
