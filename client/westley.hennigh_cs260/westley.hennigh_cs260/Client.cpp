// Westley Hennigh
// Client.cpp : the implementation of the client class.
// CS260 Assignment 1
// Jan 26th 2010

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

//need for getnameinfo() function
#include "Ws2tcpip.h"

//help the compiler figure out where winsock lives.  You could
//also add this to the linker but this means no 
//extra settings for the .csproj file
#pragma comment(lib, "ws2_32.lib")

#include "Client.hpp"
#include "Defines.hpp"

extern HWND output;


Client::Client( std::string server_ip, unsigned port /*void (*callback_)(IMessage)*/ ) : ipAddress(server_ip), remote_port(port), lost_server(false)
{}

Client::Client(std::string text_file) : lost_server(false)
{
	std::ifstream user_file(text_file.c_str());

	// read in the ip, port and username info
	std::string line;
	unsigned place;

	// while there is more text and we have not read in the three values we need, read in lines
	for (unsigned val_read = 0; std::getline(user_file, line) && val_read < 4; )
	{
		if((place = line.find("IP: ")) != line.npos)
		{
			ipAddress = line.c_str() + place + sizeof("IP: ") - 1;  // the ip address is everything after that place in the string (-1 for null)
			++val_read;
		}
		else if((place = line.find("Port: ")) != line.npos)
		{
			std::stringstream value;  // to extract the unsigned
			value << line.c_str() + place + sizeof("Port: ") - 1;  // -1 for null
			value >> remote_port;
			++val_read;
		}
		else if((place = line.find("Name: ")) != line.npos)
		{
			username = line.c_str() + place + sizeof("Name: ") - 1;  // -1 for null
			++val_read;
		}
		//else
			//^! bad value, print error
	}
}


int Client::Connect(std::string username_)
{
	int ret = 0;

	//start up winsock, asking for version 2.2

	//our WSAData structure.  This holds information about the version of winsock that we are using
	//not necessarily the version that we are requesting
	WSADATA wsaData;
	//let's start with zeroing wsaData, not a bad idea
	SecureZeroMemory(&wsaData, sizeof(wsaData));

	//you must call this function to initialize winsock.  You must call WSACleanup when you are finished.
	//this uses a reference counter, so for each call to WSAStartup, you must call WSACleanup or suffer 
	//memory issues
	ret = WSAStartup(MAKEWORD(2,2), &wsaData);

	//check that WSAStartup was successful.
	if(ret != 0)
	{
		std::cout << "Error initializing winsock!  Error was: " << ret << std::endl;
		return ret;
	}

	//a sockaddr_in specifies IP address and port information.  the address family tells what the add
	//address used to bind to socket, holds port, IP, and address family
	struct sockaddr_in socketAddress;
	//address family-  this identifies the addressing structure of this address structure.
	//for IP, it must always be set to AF_INET
	socketAddress.sin_family = AF_INET;

	//this is the port.  remember, networks are in network-order, which is big-endian.  we use htons() to ensure
	//that the number is in the correct "endian-ness" for the network regardless of what machine architecture we're on
	//for a local connection going out, pick 0 and winsock will assign a random port
	socketAddress.sin_port = 0; //htons(remote_port); //^? cant be remote_port, that would break everything when working on one machine

	//now we need to set the actual IP address.  this is an in_addr struct for IPv4.  for Ipv6, it's an in6_addr
	//it is represented as a ulong with each byte representing one part of the ip address.  we can convert an address in
	//it's dot-notation (i.e. 192.168.1.1) to this format using the inet_addr() function, OR we can just use ADDR_ANY which
	//will cause this address to listen to any available network interface (machines may have more than one network card)

	//get host.  hostent stores information about a host
	hostent* localhost;

	//gethostbyname- gets host information based on host name.
	//this allocates memory only once per host no matter how many times you call
	//gethostbyname().  
	//deprecated in favor of getaddrinfo

	//there are a lot of ways to do this
	localhost = gethostbyname("");
	char* localIP;

	//this will give us the four-byte representation of our ip address.
	localIP = inet_ntoa(*(in_addr*)*localhost->h_addr_list);

	//let's use the address that we got from gethostbyname for our local endpoint
	socketAddress.sin_addr.s_addr  = inet_addr(localIP);

	sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	remote.sin_port = htons(remote_port);

	//SOCKET clientSocket;	

	//create a socket
	//this socket is not bound to anything, so all it is is an empty socket at this point
	//AF_INET- this means that we want an IP socket.  If we wanted an IPv6 socket, we would use AF_INET6
	//if we wanted to talk to an infrared port, we'd use AF_IRDA
	//bluetooth is AF_BTM

	//SOCK_STREAM - tells us the type of socket.  SOCK_STREAM is two-way, reliable, connection-based byte streams
	//that can transmit OOB data.  uses TCP.  for UDP use SOCK_DGRAM, and for raw, use SOCK_RAW.  there are others also

	//IPPROTO_TCP- tells us to use the TCP protocol.  this parameter can only be used certain ways with
	//certain address family and type settings.  if we wanted UDP, we would have had to set our type to SOCK_DGRAM 
	//or we'll get an error.  passing 0 means that we dont' care about the protocol and the socket can choose

	//NULL-  we specified TCP, so we don't need this parameter, but it would contain protocol info normally.  if
	//we enumerate all the protocols supported on this machine, we could pick one of them and pass it in here to get
	//an explicit protocol implementation.  mostly, we don't care, but if we were using IPX (for example) then we would
	//care a lot

	//0-  this is reserved, always set it to 0.

	//0-  these are the options.  normally the only one you'd care about is creating an overlapped socket for
	//use with IO ports.  if you want to do that, send WSA_IO_OVERLAPPED here.  more on that in a future demo
	clientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	//error checking
	if(clientSocket == INVALID_SOCKET)
	{
		int errorcode = WSAGetLastError();
		std::cout << "Error on creating socket: " << errorcode << std::endl;
		return errorcode;
	}

	//so now we have a socket, and we have a sockaddr_in that describes an address and port, let's put them
	//together and send some data!

	//bind- binds a socket to an address.  
	//clientSocket- the socket to bind
	//socketAddress- the address structure to bind, need to recast to sockaddr here
	//size of the sockaddr struct
	ret = bind(clientSocket, (sockaddr*)&socketAddress, sizeof(socketAddress));
	if(ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		return ret;
	}

	//now we have a socket and we can do something to it.  let's connect to a server somewhere and 
	//send some data

	//we're going to connect.  for connection-oriented sockets (TCP) this will establish a connection.
	//for datagram sockets, it will only set up a default endpoint to send subsequent calls to SEND to

	//we're going connect to a remote host now.  
	//clientSocket is the socket that we've created for this end of the connection
	//remote is the endpoint that we're connection to.  We have to cast it as sockaddr for this
	//version of connect
	//we also pass the size of the sockaddr struct since this could potentially change based on 
	//what type of address we're using
	ret = connect(clientSocket, (sockaddr*)&remote, sizeof(remote));
	if(ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		return ret;
	}

  // now we have a connection made, but the server will need to know our name.
  // wait for a query of that sort and then reply
  char buffer [STD_BUFF_SIZE];
  ret = recv(clientSocket, buffer, STD_BUFF_SIZE, 0); 

  IMessage* kitten = ConstructMessage(buffer);

  if(kitten->my_type == RequestForUsername_Msg)
  {
    UsernameMsg username_message;
    username_message.myname = username;

    ret = username_message.WriteOut(buffer);
    ret = send(clientSocket, buffer, ret, 0);
  }
  else
  {
    // the server is not behaving as expected (were not in the servers vec yet,
    // so we shouldnt get any other message)
    return -1;
  }
  delete kitten;  // clean up!

	// now put the socket into non-blocking mode
	u_long val = 1;
	ioctlsocket(clientSocket, FIONBIO, &val);

	return 0;
}


int Client::ShutDown()
{
	int ret = 0;

	//shutdown the socket
	//clientSocket- the socket to shut down
	//SD_SEND-  how to shut it down.  SD_SEND means that
	//you will no longer send over this socket.  for TCP, it 
	//also means that you will send a FIN packet once all 
	//data has been received and acknowledged

	//you also have other options
	//SD_RECEIVE - no more receiving data.  triggers a RST if
	//data is received after this is done

	//SD_BOTH- no more send and receive
	ret = shutdown(clientSocket, SD_SEND);
	if(ret == SOCKET_ERROR){
		ret = WSAGetLastError();
		return ret;
	}

	//clean up the socket.  Technically, WSACleanup will do this for you
	//but it's good to get in the habit of closing your own sockets.
	ret = closesocket(clientSocket);
	if(ret == SOCKET_ERROR){
		ret = WSAGetLastError();
		return ret;
	}

	//don't forget to call this at the end.  in total, call it as many times as you have called
	//WSAStartup()
	WSACleanup();

	lost_server = true;

	return 0;
}


int Client::Send(IMessage& message_)
{
	char buffer [STD_BUFF_SIZE];
	int size = message_.WriteOut(buffer);

	return send(clientSocket, buffer, size + 1, 0);
}


IMessage* Client::Receive()
{
	int ret;
	char buffer [STD_BUFF_SIZE];

	// if we find data then make it into a nice message
	ret = recv(clientSocket, buffer, STD_BUFF_SIZE, 0);
	if(ret == SOCKET_ERROR)  // check for error
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
		{
			std::cout << "Error receiving data in accepting thread: " << WSAGetLastError() << std::endl;
			return NULL;
		}
		else  // if it was
			return NULL;
	}
	else if(ret == 0)  // means they are done playing around
	{
		// the server has shut down...
		lost_server = true;

		//^! this should be de-coupled
		SendMessage(output, WM_SETTEXT, 0, (LPARAM)"We are currently maintaining the server. Go away.");
	}

	// make the message (this is a good spot to check for errors)
	if (*reinterpret_cast<unsigned*>(buffer) > static_cast<unsigned>(ret))
	{
		std::cout << "There was a problem receiving a message in the client..." << std::endl;
	}
	return ConstructMessage(buffer);
}


bool Client::StillConnected()
{
	return !lost_server;
}

std::string Client::GetUsername()
{
	return username;
}
//int Client::Run()
//{
//	int ret = 0;
//
//	std::string SendBuffer;
//	std::string CommandBuffer;  // we want to receive case insensitive commands from the prompt
//	char RecieveBuffer[MAX_LEN];
//
//	std::cout << "---------" << std::endl;
//	std::getline(std::cin, SendBuffer);  // prime the buffer with their initial input so we can cin last in the loop and check quit before we network.
//
//	// uppercase everything to check for commands
//	CommandBuffer = SendBuffer;
//	std::transform(CommandBuffer.begin(), CommandBuffer.end(), CommandBuffer.begin(), toupper);
//
//	while (CommandBuffer != "QUIT")
//	{
//		// send the message -------
//		ret = send(clientSocket, SendBuffer.c_str(), SendBuffer.size(), 0);
//		if(ret == SOCKET_ERROR)
//		{
//			ret = WSAGetLastError();
//			return ret;
//		}
//		// ------------
//
//		// listen for a reply -------
//		memset(RecieveBuffer, 0, MAX_LEN);  // clear out the buffer
//
//		ret = recv(clientSocket, RecieveBuffer, MAX_LEN, 0);
//		if(ret == SOCKET_ERROR)
//		{
//			ret = WSAGetLastError();
//			return ret;
//		}
//		std::cout << std::string(RecieveBuffer) << std::endl << "---------" << std::endl;  // print out what we got
//		// ------------
//
//		std::getline(std::cin, SendBuffer);  // get some input from the user
//
//		// uppercase everything to check for commands
//		CommandBuffer = SendBuffer;
//		std::transform(CommandBuffer.begin(), CommandBuffer.end(), CommandBuffer.begin(), toupper);
//	}
//
//	return 0;
//}
