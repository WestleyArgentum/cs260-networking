// Westley Hennigh
// Socket.hpp: Interface for sockets
// CS260 Assignment 3
// Mar 28th 2010

#ifndef SSOCKET_H
#define SSOCKET_H

#include <string>
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

#include "Message.hpp"

/*
Because I do not expect to have time to implement more than the reliable udp supersocket for this
assignment, I will just make send in supersocket take an id. I need to find a way to solve this
problem though because pretty much only the reliable udp socket is concerned with keeping track
of the people using it and I really cant bloat the base class with derived specific stuff.
*/
class SuperSocket
{
public:
	virtual ~SuperSocket () {}

	virtual int Send (IMessage* message) = 0;
	virtual IMessage* Recv () = 0;
};


/*
This is sort of a lie. I do not actually maintain a connection quite as
robustly as TCP might. For instance, I don't ever send keep alive packets, I just
assume that you are using this understanding how it actually works, not how it should.
*/
class ReliableUdpSocet : public SuperSocket
{
public:
	virtual ~ReliableUdpSocet ();

	virtual int Send (IMessage* message);
	virtual IMessage* Recv ();

	// We will maintain a connection over UDP
	int Connect (std::string remote_ip_, unsigned remote_port_);

private:
	std::string remote_ip;
	unsigned remote_port;

	sockaddr_in remoteAddress;
	SOCKET socket;

};


// Below are socket functions I think are useful.
int PollForAck( SOCKET sock, sockaddr_in remote, unsigned millisec );

#endif
