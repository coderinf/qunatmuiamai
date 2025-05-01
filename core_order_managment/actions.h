#ifndef ACTIONS_H
#define ACTIONS_H

#include <ixwebsocket/IXWebSocket.h>
#include <string>

bool authenticate(ix::WebSocket& webSocket, const std::string& client_id, const std::string& client_secret);
void placeOrder(ix::WebSocket& webSocket);
void cancelOrder(ix::WebSocket& webSocket);
void modifyOrder(ix::WebSocket& webSocket);
void getOrderbook(ix::WebSocket& webSocket); // New function declaration
void viewPositions(ix::WebSocket& webSocket);

#endif // ACTIONS_H