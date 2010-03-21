// Westley Hennigh
// Server.cpp : the implementation of the server class
// CS260 Assignment 1
// Jan 26th 2010

#include <iostream>
#include <string>

#include "Server.hpp"
#include "AcceptActiveObj.hpp"

const unsigned BUFF_SIZE = 4096;


Server::Server( unsigned port_ ) : port(port_), accepting_thread(clients, clients_mutex, listenerSocket), echoing_thread(clients, clients_mutex)
{
	int ret;
	WSADATA wsData;

	// initialize winsock
	ret = WSAStartup(MAKEWORD(2,2), &wsData);
	if(ret)
	{
		std::cout << "Error starting winsock: " << ret << std::endl;
		return;
	}

	// host info
	char* localIP;
	hostent* localhost;

	// get local host IP, same as in client demo
	localhost = gethostbyname("");
	localIP = inet_ntoa(*(in_addr*)*localhost->h_addr_list);

	std::cout << "Locaohost: " << localhost->h_name << std::endl;
	std::cout << "Local IP: " << localIP << std::endl;

	// our listener socket
	struct sockaddr_in socketAddress;

	// create local endpoint for listening.
	socketAddress.sin_family = AF_INET;
	// set the port to what was passed in
	socketAddress.sin_port = htons(port);
	socketAddress.sin_addr.s_addr  = inet_addr(localIP);

	// create our socket
	listenerSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	// handle errors
	if(listenerSocket == INVALID_SOCKET)
	{
		std::cout << "WSASocket call failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();	
		return;
	}

	// bind our listener socket to local endpoint
	ret = bind(listenerSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
	if(ret == SOCKET_ERROR)
		return;

	// now we listen for someone to connect.  This will put the socket into a listening
	// state, but will not automatically accept a connection
	ret = listen(listenerSocket, 10);	
	if(ret == SOCKET_ERROR)
		return;

	// make socket non-blocking
	u_long val = 1;
	ioctlsocket(listenerSocket, FIONBIO, &val);

	// from here when we call run we will use the listening, non blocking socket in a thread
	// and we will echo data to any clients that socket picks up and passes along
}


int Server::Shutdown()
{
	// kill our threads!
	accepting_thread.Kill();
	echoing_thread.Kill();

	// shut down our sockets (including the vector of them)
	for (unsigned i = 0; i < clients.size(); ++i)
		shutdown(clients[i].second, SD_BOTH);

	shutdown(listenerSocket, SD_BOTH);

	// close the sockets (including the vector of them)
	for (unsigned i = 0; i < clients.size(); ++i)
		closesocket(clients[i].second);

	closesocket(listenerSocket);

	WSACleanup();

	return 0;
}


int Server::Run()
{
	accepting_thread.Wake();
	echoing_thread.Wake();




	return 0;
}