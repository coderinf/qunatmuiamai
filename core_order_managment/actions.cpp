#ifndef ACTIONS_H
#define ACTIONS_H
#include <iostream>
#include <nlohmann/json.hpp>
#include <ixwebsocket/IXWebSocket.h>
#include "actions.h"

using json = nlohmann::json;

bool authenticate(ix::WebSocket& webSocket, const std::string& client_id, const std::string& client_secret)
{
    bool isAuthenticated = false;
    bool responseReceived = false;

    webSocket.setOnMessageCallback([&webSocket, &isAuthenticated, &responseReceived, client_id, client_secret](const ix::WebSocketMessagePtr& msg)
    {
        if (msg->type == ix::WebSocketMessageType::Open)
        {
            std::cout << "Connection established!" << std::endl;

            // Prepare the authentication message
            json auth_msg = {
                {"jsonrpc", "2.0"},
                {"id", 9929},
                {"method", "public/auth"},
                {"params", {
                    {"grant_type", "client_credentials"},
                    {"client_id", client_id},
                    {"client_secret", client_secret}
                }}
            };

            // Send the authentication message
            webSocket.send(auth_msg.dump());
        }
        else if (msg->type == ix::WebSocketMessageType::Message)
        {
            std::cout << "Received: " << msg->str << std::endl;

            // Check if authentication was successful
            if (msg->str.find("\"result\"") != std::string::npos)
            {
                std::cout << "Authentication successful!" << std::endl;
                isAuthenticated = true;
            }
            else
            {
                std::cerr << "Authentication failed!" << std::endl;
            }
            responseReceived = true;
        }
        else if (msg->type == ix::WebSocketMessageType::Error)
        {
            std::cerr << "Error: " << msg->errorInfo.reason << std::endl;
            responseReceived = true;
        }
    });

    // Start the WebSocket connection
    webSocket.start();

    // Wait for the authentication response
    while (!responseReceived)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return isAuthenticated;
}

// Function to place a buy order
void placeOrder(ix::WebSocket& webSocket)
{
    json buy_msg = {
        {"jsonrpc", "2.0"},
        {"id", 5275},
        {"method", "private/buy"},
        {"params", {
            {"instrument_name", "ETH-PERPETUAL"},
            {"amount", 40},
            {"type", "market"},
            {"label", "market0000234"}
        }}
    };

    // Send the buy order message
    webSocket.send(buy_msg.dump());
    std::cout << "Buy order placed!" << std::endl;
}

// Function to place a sell order
void sellOrder(ix::WebSocket& webSocket)
{
    json sell_msg = {
        {"jsonrpc", "2.0"},
        {"id", 2148},
        {"method", "private/sell"},
        {"params", {
            {"instrument_name", "ETH-PERPETUAL"},
            {"amount", 123},
            {"type", "stop_limit"},
            {"price", 145.61},
            {"trigger_price", 145},
            {"trigger", "last_price"}
        }}
    };

    // Send the sell order message
    webSocket.send(sell_msg.dump());
    std::cout << "Sell order placed!" << std::endl;
}

// Function to cancel an order
void cancelOrder(ix::WebSocket& webSocket)
{
    // Ask the user for the order ID
    std::string order_id;
    std::cout << "Enter the order ID to cancel: ";
    std::cin >> order_id;

    // Prepare the cancel order message
    json cancel_msg = {
        {"jsonrpc", "2.0"},
        {"id", 4214},
        {"method", "private/cancel"},
        {"params", {
            {"order_id", order_id}
        }}
    };

    // Send the cancel order message
    webSocket.send(cancel_msg.dump());
    std::cout << "Cancel order request sent for order ID: " << order_id << std::endl;
}

// Function to modify an order
void modifyOrder(ix::WebSocket& webSocket)
{
    // Ask the user for the order details
    std::string order_id;
    int amount;
    double price;

    std::cout << "Enter the order ID to modify: ";
    std::cin >> order_id;
    std::cout << "Enter the new amount: ";
    std::cin >> amount;
    std::cout << "Enter the new price: ";
    std::cin >> price;

    // Prepare the modify order message
    json modify_msg = {
        {"jsonrpc", "2.0"},
        {"id", 3725},
        {"method", "private/edit"},
        {"params", {
            {"order_id", order_id},
            {"amount", amount},
            {"price", price},
            {"advanced", "implv"}
        }}
    };

    // Send the modify order message
    webSocket.send(modify_msg.dump());
    std::cout << "Modify order request sent for order ID: " << order_id << std::endl;
}

// Function to get current positions
void  viewPositions(ix::WebSocket& webSocket)
{
    // Ask the user for the currency and kind
    std::string currency, kind;
    std::cout << "Enter the currency (e.g., ETH): ";
    std::cin >> currency;
    std::cout << "Enter the kind (e.g., future, option): ";
    std::cin >> kind;

    // Prepare the get positions message
    json positions_msg = {
        {"jsonrpc", "2.0"},
        {"id", 2236},
        {"method", "private/get_positions"},
        {"params", {
            {"currency", currency},
            {"kind", kind}
        }}
    };

    // Send the get positions message
    webSocket.send(positions_msg.dump());
    std::cout << "Get positions request sent for currency: " << currency << " and kind: " << kind << std::endl;

    // Wait for the response
    webSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
    {
        if (msg->type == ix::WebSocketMessageType::Message)
        {
            std::cout << "Received positions data: " << msg->str << std::endl;
        }
        else if (msg->type == ix::WebSocketMessageType::Error)
        {
            std::cerr << "Error: " << msg->errorInfo.reason << std::endl;
        }
    });
}

// Function to get the order book
void getOrderbook(ix::WebSocket& webSocket)
{
    // Ask the user for the instrument name and depth
    std::string instrument_name;
    int depth;
    std::cout << "Enter the instrument name (e.g., BTC-PERPETUAL): ";
    std::cin >> instrument_name;
    std::cout << "Enter the depth (e.g., 5): ";
    std::cin >> depth;

    // Prepare the get order book message
    json orderbook_msg = {
        {"jsonrpc", "2.0"},
        {"id", 8772},
        {"method", "public/get_order_book"},
        {"params", {
            {"instrument_name", instrument_name},
            {"depth", depth}
        }}
    };

    // Send the get order book message
    webSocket.send(orderbook_msg.dump());
    std::cout << "Get order book request sent for instrument: " << instrument_name << " with depth: " << depth << std::endl;

    // Wait for the response
    webSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
    {
        if (msg->type == ix::WebSocketMessageType::Message)
        {
            std::cout << "Received order book data: " << msg->str << std::endl;
        }
        else if (msg->type == ix::WebSocketMessageType::Error)
        {
            std::cerr << "Error: " << msg->errorInfo.reason << std::endl;
        }
    });
}

// Placeholder implementations for other actions

#endif // ACTIONS_H