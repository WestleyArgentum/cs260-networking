// Westley Hennigh
// Socket.hpp: Interface for sockets
// CS260 Assignment 3
// Mar 28th 2010

#ifndef SSOCKET_H
#define SSOCKET_H

#include <string>

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
	virtual int Send (unsigned id, IMessage* message) = 0;
	virtual IMessage* Recv (unsigned id) = 0;
};


class ReliableUdpSocet : public SuperSocket
{
public:
	virtual int Send (unsigned id, IMessage* message);
	virtual IMessage* Recv (unsigned id);

	// Sets up a "connection" with another host and returns an id that will be your
	// identification for send / recv calls (save that id!).
	unsigned RegisterConnection (std::string remote_ip, unsigned remote_port);
	void DropConnection (unsigned id);

private:
	unsigned connection_id;
};

#endif
