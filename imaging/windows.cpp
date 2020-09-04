#include "windows.h"
#include "TGUI/TGUI.hpp"
#include "imaging.h"
#include <iostream>
#include "SFML/Network.hpp"
void windows::Chess::showMenu() 
{
    sf::RenderWindow window( {800, 600}, "Window" );
    tgui::Gui gui{ window }; // Create the gui and attach it to the window
    tgui::Button::Ptr playButton = tgui::Button::create();
    playButton->setText("PLAY");
    playButton->setPosition(window.getSize().x / 2, window.getSize().y * 0.333);
    auto buttonstyle = playButton->getRenderer();
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

void windows::NetworkTest::showConnectMenu() 
{
    // start listening for tcp connections on the main pc on port 53000
    sf::TcpListener listener;
    listener.listen(53000);
    listener.setBlocking(false);
    sf::TcpSocket client;
    std::cout << "waiting for a connection..." << std::endl;



    sf::RenderWindow window({ 800,800 }, "Network test");
    tgui::Gui gui;
    gui.setTarget(window);
    sf::Event event;
    tgui::Button::Ptr clientConnect = tgui::Button::create();
    clientConnect->setText("Connect to server");
    auto buttonstyle = clientConnect->getRenderer();
    buttonstyle->setBackgroundColor(tgui::Color::Black);
    buttonstyle->setTextColor(tgui::Color::White);
    clientConnect->connect("Clicked", [&]()
        {
            sf::TcpSocket socket;
            sf::Socket::Status status = socket.connect("192.168.0.125", 53000);
            if (status != sf::Socket::Done )
            {
                std::cout << "an error has occured | clientside" << std::endl;
            }
        });
    gui.add(clientConnect);
    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            gui.handleEvent(event);
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        sf::Socket::Status status = listener.accept(client);
        if (status == sf::Socket::Status::Done)
        {
            std::cout << "Client has connected " << client.getRemoteAddress() << std::endl;
        }
        window.clear(sf::Color::White);
        gui.draw();
        window.display();
    }
}