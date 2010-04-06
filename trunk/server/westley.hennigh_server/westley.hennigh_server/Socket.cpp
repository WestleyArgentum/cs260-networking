// Westley Hennigh
// Socket.cpp: Interface for sockets
// CS260 Assignment 3
// Mar 28th 2010

#include "Socket.hpp"
#include "Defines.hpp"
#include "Message.hpp"

const int SEND_TIMEOUT = 20;  // seconds

ReliableUdpSocet::~ReliableUdpSocet()
{}

int ReliableUdpSocet::Send( IMessage* message )
{
	int ret;
	unsigned time = GetTickCount();
	char buffer [STD_BUFF_SIZE];
  char muffler [STD_BUFF_SIZE];

	// write message into buffer
	unsigned msg_size = message->WriteOut(buffer);

	// loop until sent
  /*while(1)
  {
    ret = sendto(socket, buffer, msg_size, 0, (sockaddr*)&remoteAddress, sizeof(remoteAddress));
		if(ret == SOCKET_ERROR)
		{
			ret = WSAGetLastError();
			return ret;
		}

    int remoteAddresslength = sizeof(remoteAddress);
    int count = recvfrom(socket, muffler, STD_BUFF_SIZE, 0, (SOCKADDR*)&remoteAddress, &remoteAddresslength);
		if(count == SOCKET_ERROR)
			return -1;
    else if(count)
    {
      Sleep(1);
      return 0;
    }
  }*/
	while (GetTickCount() < time + (SEND_TIMEOUT * 1000))
	{
		ret = sendto(socket, buffer, msg_size, 0, (sockaddr*)&remoteAddress, sizeof(remoteAddress));
		if(ret == SOCKET_ERROR)
		{
			ret = WSAGetLastError();
			return ret;
		}

		if(PollForAck(socket, remoteAddress, 500) == 1)
			return 0;  // that means the packet was sent :)
		//else
			//return -1;  // there was a problem while polling
	}

	return -1;
}

IMessage* ReliableUdpSocet::Recv( )
{
	//^! there is going to be a potential problem in here because if someone (not another client)
	//	 were to just try and send random data... well I would crash like hell...

	char buffer [STD_BUFF_SIZE];

	sockaddr_in remoteAddress;
	SecureZeroMemory(&remoteAddress, sizeof(remoteAddress));
	int remoteAddresslength = sizeof(remoteAddress);

	int count = recvfrom(socket, buffer, STD_BUFF_SIZE, 0, (SOCKADDR*)&remoteAddress, &remoteAddresslength);
	if(count == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			return NULL;  //^! something went wrong
	}
	else if (count)
		return ConstructMessage(buffer);

	return NULL;
}

int ReliableUdpSocet::Connect( unsigned local_port_, std::string remote_ip_, unsigned remote_port_ )
{
	int ret;

	// save these just in case
	remote_ip = remote_ip_;
	remote_port = remote_port_;
	local_port = local_port_;

	struct sockaddr_in socketAddress;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(local_port);

	hostent* localhost;
	localhost = gethostbyname("");
	char* localIP;
	localIP = inet_ntoa(*(in_addr*)*localhost->h_addr_list);

	socketAddress.sin_addr.s_addr  = inet_addr(localIP);

	remoteAddress.sin_family = AF_INET;
	remoteAddress.sin_port = htons(remote_port);
	remoteAddress.sin_addr.s_addr  = inet_addr(remote_ip.c_str());

	// sock-et!
	socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

	if(socket == INVALID_SOCKET)
	{
		int errorcode = WSAGetLastError();
		//cout << "Error on creating socket: " << errorcode << endl;
		return errorcode;
	}

	//^? apparently for "clients" binding is "discouraged" ...
	ret = bind(socket, (sockaddr*)&socketAddress, sizeof(socketAddress));
	if(ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		return ret;
	}

	// now put the socket into non-blocking mode
	u_long val = 1;
	ioctlsocket(socket, FIONBIO, &val);

	// everything is a-ok *wink*
	return 0;
}

unsigned ReliableUdpSocet::GetLocalPort()
{
	return local_port;
}

/*
Returns 1 if they ack-ed, 0 if they did not, and -1 if there was an error.

//^! In the future ack params should be put in and we could then poll for the right ack.

//^! I should also make an overload that takes a session instead of a remote socket so that
		 I can match that instead.
*/
int PollForAck( SOCKET sock, sockaddr_in remote, unsigned millisec )
{
	sockaddr_in remoteAddress;
	SecureZeroMemory(&remoteAddress, sizeof(remoteAddress));
	int remoteAddresslength = sizeof(remoteAddress);

	// right now the buffer will not be used, it will just get the data
	char buffer [STD_BUFF_SIZE];

	unsigned breaktime = GetTickCount() + millisec;
	while (GetTickCount() < breaktime)
	{
		int count = recvfrom(sock, buffer, STD_BUFF_SIZE, 0, (SOCKADDR*)&remoteAddress, &remoteAddresslength);
		if(count == SOCKET_ERROR)
			return -1;
		else
		{
			//^! right now we do not have an ack param to check with so we assume they wanted to confirm delivery.

			//^! check to see that it came from the right remote socket
			if(1)
				return 1;
		}
	}
	return 0;
}