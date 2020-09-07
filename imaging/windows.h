#ifndef WINDOWS_H
#define WINDOWS_H
#include "TGUI/TGUI.hpp"
#include "SFML/Network.hpp"
#include "imaging.h"
namespace windows {
	class Chess {
		StyleManager style;
	public:
		void showMenu(sf::RenderWindow& window,tgui::Gui& gui);
		void showNetworkMenu(sf::RenderWindow& window, tgui::Gui& gui);
		void showChatWindow(sf::TcpSocket& socket);
		void startGame(sf::RenderWindow& window, tgui::Gui& gui, sf::TcpSocket& socket, bool color);
		void showCustomizeStyleWindow(sf::RenderWindow& window, tgui::Gui& gui);
		//void playGame();
	private:
		void addMessageToScreen(std::string message, tgui::Gui& gui, const sf::RenderWindow& window, bool position); // bool position false == left and true == right
	};

	/*class NetworkTest {
	public:
		void showConnectMenu();
	};*/
}
#endif
