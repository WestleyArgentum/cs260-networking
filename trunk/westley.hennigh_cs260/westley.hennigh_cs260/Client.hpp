// Westley Hennigh
// Client.hpp : the interface to the client class
// CS260 Assignment 1
// Jan 26th 2010

#ifndef CLIENT_H
#define CLIENT_H

#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

#include "Message.hpp"

/*
The client class is responsible for making a connection to a server and relaying information between the user
and the server via messages.

To send a message you call the send function with a message.

In theory, upon construction you will give the client a callback function that it will use to report back.
In reality, I'm out of time so your just going to call receive and if you get something back there you go.
*/
class Client
{
public:
	// upon creation we need at least an ip, a port, and the callback function to reach you
	Client( std::string server_ip, unsigned port /*void (*callback_)(IMessage)*/ );

	int Connect(std::string username_ = std::string());  // call to initiate a connection
	int ShutDown(void);  // user MUST call to cleanup the connection (maybe this should just be called in the destructor)

	int Send(IMessage& message_);  // call to send data to the server
	IMessage* Receive ();

	bool StillConnected ();

	//grr
	int GetRdy () { return rdy; }
	
private:
	std::string username;    // the username for this client
	std::string ipAddress;  // the ip address of the server we are connecting to
	unsigned remote_port;  // the port that server will be listening on
	SOCKET clientSocket;  // the socket
	bool lost_server;    // the server has gone away 

	Client( const Client& do_not_call );  // DO NOT CALL!

	// One of the many reasons I despise Microsoft is that I feel they, if anything,
	// encourage such horrible hacks as the one below:
	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	int rdy;

};

#endif
