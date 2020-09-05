#ifndef WINDOWS_H
#define WINDOWS_H
#include "TGUI/TGUI.hpp"
#include "SFML/Network.hpp"
namespace windows {
	class Chess {
	public:
		void showMenu();
		void showNetworkMenu(sf::RenderWindow& window, tgui::Gui& gui);
		void showChatWindow(sf::TcpSocket& socket);
		//void playGame();
	private:
		void addMessageToScreen(std::string message, tgui::Gui& gui, const sf::RenderWindow& window, bool position); // bool position false == left and true == right
	};

	

	class NetworkTest {
	public:
		void showConnectMenu();
	};
}
#endif
