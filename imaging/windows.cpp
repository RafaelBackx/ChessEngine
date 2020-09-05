#include "windows.h"
#include "imaging.h"
#include <thread>
#include <iostream>

void windows::Chess::showChatWindow(sf::TcpSocket& socket)
{
    sf::RenderWindow window(sf::VideoMode(300, 600), "Chat - " + socket.getRemoteAddress().toString());
    sf::Event event;
    tgui::Gui gui(window);
    tgui::TextBox::Ptr messageBox = tgui::TextBox::create();
    messageBox->setDefaultText("Message...");
    messageBox->setSize(200, 50);
    messageBox->setPosition(0, window.getSize().y - messageBox->getSize().y);
    tgui::Button::Ptr sendButton = tgui::Button::create();
    sendButton->setText("Send");
    sendButton->setSize(100, 50);
    sendButton->setPosition(messageBox->getSize().x, window.getSize().y - sendButton->getSize().y);
    sendButton->connect("Clicked", [&]()
        {
            std::string message = messageBox->getText();
            sf::Packet packet;
            packet.append(message.c_str(), sizeof(message));
            socket.send(packet);
            // add the message to the screen
            addMessageToScreen(message, gui, window, true);
            messageBox->setText("");
        });
    gui.add(messageBox,"MessageBox");
    gui.add(sendButton,"SendButton");
    while (window.isOpen())
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
        auto status = socket.receive(receivedPacket);
        if (status == sf::Socket::Status::Done)
        {
            char* message = (char*)receivedPacket.getData();
            addMessageToScreen(message, gui, window, false);
        }
        window.clear(sf::Color::White);
        gui.draw();
        window.display();
    }
}

void windows::Chess::addMessageToScreen(std::string message, tgui::Gui& gui, const sf::RenderWindow& window, bool position)
{
    int margin = 10;
    tgui::Label::Ptr messageLabel = tgui::Label::create();
    messageLabel->setText(message);
    tgui::Padding padding(5);
    auto messageLabelStyle = messageLabel->getRenderer();
    messageLabelStyle->setBorderColor(tgui::Color::Black);
    messageLabelStyle->setPadding(padding);
    if (position)
    {
        messageLabel->setPosition(window.getSize().x - messageLabel->getSize().x-margin, window.getSize().y - 50 - messageLabel->getSize().y - margin);
        messageLabelStyle->setBackgroundColor(sf::Color::Color(3, 152, 252));
    }
    else
    {
        messageLabel->setPosition(0+margin, window.getSize().y - 50 - messageLabel->getSize().y - margin);
        messageLabelStyle->setBackgroundColor(sf::Color::Color(147,157,163));
    }
    // move up all other messages
    for (tgui::Widget::Ptr widget : gui.getWidgets())
    {
        std::string name = widget->getWidgetName();
        std::cout << name << std::endl;
        if (widget->getWidgetName() == "")
        {
            auto position = widget->getPosition();
            widget->setPosition(position.x, position.y - messageLabel->getSize().y - margin);
        }
    }
    gui.add(messageLabel);
}

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
                socket.setBlocking(false);
                std::thread chatThread(&Chess::showChatWindow, this, std::ref(socket));
                chatThread.join();
                //this->showChatWindow(socket);
            }
        });
    tgui::Button::Ptr listenButton = tgui::Button::create();
    listenButton->setText("Start listening for clients");
    listenButton->setPosition(window.getSize().x * 0.5 - listenButton->getSize().x * 0.5, window.getSize().y * 0.6667);
    sf::TcpListener listener;
    //listener.setBlocking(false);
    listenButton->connect("Clicked", [&]()
        {
            listener.listen(53000);
            auto status = listener.accept(socket);
            if (status == sf::Socket::Status::Done)
            {
                std::cout << "Client succesfully connected with address " << socket.getRemoteAddress() << std::endl;
                server = true;
                socket.setBlocking(false);
                this->showChatWindow(socket);
            }
        });
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