#include "windows.h"
#include "TGUI/TGUI.hpp"
#include "imaging.h"
#include <iostream>
#include "SFML/Network.hpp"

void windows::Chess::showMenu() 
{
    sf::RenderWindow window( {400, 600}, "Window" );
    tgui::Gui gui{ window }; // Create the gui and attach it to the window
    tgui::Button::Ptr playButton = tgui::Button::create();
    playButton->setText("PLAY");
    playButton->setPosition(window.getSize().x * 0.5, window.getSize().y * 0.333);
    auto buttonstyle = playButton->getSharedRenderer();
    buttonstyle->setBackgroundColor(tgui::Color::Black);
    buttonstyle->setTextColor(tgui::Color::White);
    playButton->connect("Clicked", [&]() 
        {
            std::cout << "the playbutton was pressed" << std::endl;
            ChessGame game;
            game.run();
        });
    tgui::Button::Ptr network = tgui::Button::create();
    network->setText("Play over Network");
    network->setPosition(window.getSize().x * 0.5, window.getSize().y * 0.5);
    network->connect("Clicked", &Chess::showNetworkMenu, this,std::ref(window), std::ref(gui));
    gui.add(playButton);
    gui.add(network);
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

void windows::Chess::showNetworkMenu(sf::RenderWindow& window, tgui::Gui& gui)
{
    bool server;
    window.clear();
    gui.removeAllWidgets();
    sf::Event event;
    tgui::TextBox::Ptr serverAdress = tgui::TextBox::create();
    serverAdress->setDefaultText("Server ip-address 0.0.0.0");
    serverAdress->setSize(200, 50);
    serverAdress->setPosition(window.getSize().x * 0.5 - serverAdress->getSize().x*0.5, window.getSize().y * 0.3334);
    tgui::Button::Ptr connectButton = tgui::Button::create();
    connectButton->setText("Connect");
    connectButton->setPosition(window.getSize().x * 0.5 - connectButton->getSize().x*0.5, window.getSize().y * 0.5);
    sf::TcpSocket socket;
    connectButton->connect("Clicked", [&]()
        {
            // get ip addres from textbox
            std::string ipaddress = serverAdress->getText();
            std::cout << ipaddress << std::endl;
            auto status = socket.connect(ipaddress, 53000);
            if (status == sf::Socket::Status::Done)
            {
                std::cout << "Connected succesfully to " << ipaddress << std::endl;
                server = false;
            }
        });
    tgui::Button::Ptr listenButton = tgui::Button::create();
    listenButton->setText("Start listening for clients");
    listenButton->setPosition(window.getSize().x * 0.5 - listenButton->getSize().x * 0.5, window.getSize().y * 0.6667);
    sf::TcpListener listener;
    sf::TcpSocket client;
    listenButton->connect("Clicked", [&]()
        {
            listener.setBlocking(false);
            listener.listen(53000);
            auto status = listener.accept(client);
            if (status == sf::Socket::Status::Done)
            {
                std::cout << "Client succesfully connected with address " << client.getRemoteAddress() << std::endl;
                server = true;
            }
        });
    tgui::TextBox::Ptr message = tgui::TextBox::create();
    message->setDefaultText("message...");
    message->setSize(100, 40);
    message->setPosition(0, window.getSize().y - message->getSize().y);
    tgui::Button::Ptr sendButton = tgui::Button::create();
    sendButton->setText("Send");
    sendButton->setPosition(message->getSize().x, window.getSize().y - sendButton->getSize().y);
    sendButton->connect("Clicked", [&]()
        {
            std::string data = message->getText();
            sf::Packet packet;
            packet.append(data.c_str(), sizeof(data));
            if (server)
            {
                client.send(packet);
            }
            else
            {
                socket.send(packet);
            }
        });
    gui.add(sendButton);
    gui.add(message);
    gui.add(listenButton);
    gui.add(serverAdress);
    gui.add(connectButton);
    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            gui.handleEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        
        sf::Packet receivedPacket;
        auto receiveStatus = client.receive(receivedPacket);
        if  (receiveStatus == sf::Socket::Status::Done)
        {
            char* receivedData = (char*)receivedPacket.getData();
            std::cout << receivedData << std::endl;
        }
        window.clear(sf::Color::White);
        gui.draw();
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