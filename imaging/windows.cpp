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
            if (status == sf::Socket::Done )
            {
                std::cout << "connected succesfully with server" << std::endl;
                sf::Packet data;
                std::string message = "hello from client";
                data.append(message.c_str(), sizeof(message));
                socket.send(data);
                socket.receive(data);
                void* receivedData = const_cast<void*>(data.getData());
                std::string* receivedMessage = reinterpret_cast<std::string*>(receivedData);
                std::cout << *receivedMessage << std::endl;
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
        //std::cout << status << std::endl;
        if (status == sf::Socket::Status::Done)
        {
            std::cout << "Client has connected " << client.getRemoteAddress() << std::endl;
            sf::Packet receivedPacket;
            client.receive(receivedPacket);
            const void* data = receivedPacket.getData();
            char* message = (char*)data;
            std::cout << message << std::endl;
            std::string response = "Hello from server";
            sf::Packet packet;
            packet.append(response.c_str(), sizeof(response));
            client.send(packet);
        }
        window.clear(sf::Color::White);
        gui.draw();
        window.display();
    }
}