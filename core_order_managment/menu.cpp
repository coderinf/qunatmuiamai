#include <iostream>
#include <nlohmann/json.hpp> // Include the JSON library
#include "actions.h"         // Include the header file for actions
#include <winsock2.h>        // Include Winsock2 for WSAStartup and WSACleanup

using json = nlohmann::json; // Declare the alias for JSON

void displayMenu(ix::WebSocket& webSocket)
{
    while (true)
    {
        std::cout << "\nSelect an option:\n";
        std::cout << "1. Place Buy Order\n";
        std::cout << "2. Cancel Order\n";
        std::cout << "3. Modify Order\n";
        std::cout << "4. Get Orderbook\n";
        std::cout << "5. View Current Positions\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            placeOrder(webSocket);
            break;
        case 2:
            cancelOrder(webSocket);
            break;
        case 3:
            modifyOrder(webSocket);
            break;
        case 4:
            getOrderbook(webSocket); // Call the new function
            break;
        case 5:
            viewPositions(webSocket);
            break;
        case 6:
            std::cout << "Exiting...\n";
            webSocket.stop();
            return;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main()
{
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed. Exiting...\n";
        return 1;
    }

    // Display a welcome message
    std::cout << "Welcome to the Deribit Trading System!" << std::endl;

    // Ask for client_id and client_secret
    std::string client_id, client_secret;
    std::cout << "Enter your client ID: ";
    std::cin >> client_id;
    std::cout << "Enter your client secret: ";
    std::cin >> client_secret;

    ix::WebSocket webSocket;

    // Set the WebSocket URL
    webSocket.setUrl("wss://test.deribit.com/ws/api/v2");

    // Authenticate the API credentials
    if (!authenticate(webSocket, client_id, client_secret))
    {
        std::cerr << "Authentication failed. Exiting...\n";
        WSACleanup(); // Clean up Winsock
        return 1;
    }

    // Display the menu for further actions
    displayMenu(webSocket);

    // Clean up Winsock
    WSACleanup();

    return 0;
}