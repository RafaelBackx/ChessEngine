#include "windows.h"
#include "TGUI/TGUI.hpp"
#include "imaging.h"
#include <iostream>
void windows::Chess::showMenu() {
    sf::RenderWindow window( {800, 600}, "Window" );
    tgui::Gui gui{ window }; // Create the gui and attach it to the window
    tgui::Button::Ptr playButton = tgui::Button::create();
    playButton->setText("PLAY");
    playButton->setPosition(window.getSize().x / 2, window.getSize().y * 0.333);
    auto buttonstyle = playButton->getSharedRenderer();
    buttonstyle->setBackgroundColor(tgui::Color::Black);
    buttonstyle->setTextColor(tgui::Color::White);
    playButton->connect("Clicked", [&]() 
        {
            std::cout << "the playbutton was pressed" << std::endl;
            ChessGame game;
            game.run();
        });
    gui.add(playButton);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            gui.handleEvent(event); // Pass the event to the widgets
        }
        window.clear(sf::Color::White);
        gui.draw(); // Draw all widgets
        window.display();
    }
}