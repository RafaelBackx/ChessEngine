#include "imaging/imaging.h"
#include "imaging/windows.h"
#include "SFML/Graphics.hpp"

int main()
{
	windows::NetworkTest test;
	test.showConnectMenu();

	/*windows::Chess chess;
	chess.showMenu();*/

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