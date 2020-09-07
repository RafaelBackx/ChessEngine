#include "imaging/imaging.h"
#include "imaging/windows.h"
#include "SFML/Graphics.hpp"

int main()
{
	//windows::NetworkTest test;
	//test.showConnectMenu();

	/*sf::RenderWindow window;
	window.create({ 500,500 }, "Test");
	sf::ContextSettings settings = window.getSettings();
	std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
	sf::RectangleShape rect;
	sf::RectangleShape rect2;
	rect.setSize({ 100, 100 });
	rect.setPosition(100, 100);
	rect2.setSize({ 100, 100 });
	rect2.setPosition(100, 100);
	sf::Texture texture;
	texture.loadFromFile("sprites/king_b.png");
	rect.setTexture(&texture);
	rect2.setFillColor(sf::Color::Blue);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear(sf::Color::White);
		window.draw(rect2);
		window.draw(rect);
		window.display();
	}*/
	sf::RenderWindow window;
	window.create({ 400,600 }, "Chess");
	tgui::Gui gui(window);
	windows::Chess chess;
	chess.showMenu(window,gui);

	/*ChessGame* game = new ChessGame();
	game->run();
	delete game;*/


	/*sf::RenderWindow window;
	TextureManager tmanager;
	sf::Event event;
	sf::RectangleShape blackPawn(sf::Vector2f(50, 50));
	blackPawn.setFillColor(sf::Color::Blue);
	blackPawn.setTexture(&tmanager.pawn_black);
	blackPawn.setPosition(100, 100);
	window.create(sf::VideoMode(500, 500), "Test");
	while (window.isOpen())
	{
		while(window.pollEvent(event))
		{	
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
			default:
				break;
			}
			window.clear(sf::Color::Green);
			window.draw(blackPawn);
			window.display();
		}
	}*/
}