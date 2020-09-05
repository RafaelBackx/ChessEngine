#ifndef WINDOWS_H
#define WINDOWS_H
#include "TGUI/TGUI.hpp"
namespace windows {
	class Chess {
	public:
		void showMenu();
		void showNetworkMenu(sf::RenderWindow& window, tgui::Gui& gui);
		//void playGame();
	};

	

	class NetworkTest {
	public:
		void showConnectMenu();
	};
}
#endif
