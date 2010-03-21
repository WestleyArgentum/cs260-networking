// Westley Hennigh
// AcceptActiveObj.hpp: ActiveObject for accepting new connections and putting them in the vector
// CS260 Assignment 2
// Feb 25th 2010

#ifndef ACCEPT_H
#define ACCEPT_H

#include <vector>

#include "ActiveObject.hpp"
#include "Mutex.hpp"

class AcceptingThread : public ActiveObject
{
public:
	typedef std::vector<std::pair<std::string, SOCKET> > NamedSockets;

	AcceptingThread (NamedSockets& clients_, Mutex& mutex_, SOCKET& listener_);
	virtual ~AcceptingThread ();

private:
	virtual void InitThread ();
	virtual void Run ();
	virtual void FlushThread ();

	NamedSockets&    clients;   // the vector of sockets and names
	Mutex&           mutex;     // for the clients
	SOCKET&          listener;  // we take a listening, non-blocking socket

	void AddNameSocketPair (std::string name, SOCKET& socket);

};

#endif
