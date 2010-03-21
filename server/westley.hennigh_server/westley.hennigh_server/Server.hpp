// Westley Hennigh
// Server.hpp : the interface to the server class
// CS260 Assignment 1
// Jan 26th 2010

#ifndef SERVER_H
#define SERVER_H

#include "winsock2.h" // winsock
#pragma comment(lib, "ws2_32.lib")

#include <vector>
#include <string>

#include "Mutex.hpp"
#include "AcceptActiveObj.hpp"
#include "EchoActiveObj.hpp"

//class AcceptingThread;  // accepting thread needs to know about server

/*
With the server we have the same problem as with the threads, and I'm
tempted to solve it in the same way. With threads you have no good way
of specifying a callback function that works cleanly, and especially with
a thread you need to wrap up data (that would otherwise be global) in a
nice way that the specified function can still use. To solve those issues
with threads I employ a strategy Robert found online and showed me, where
you wrap up the whole thread within an ActiveObject and then derive a new
class every time you need a specific thread. This is clean and elegant
but a little lame in that at runtime you could not just come up with
things that should be done on threads and go...

Same with the server.  We need a good way to specify a function for the
server to run, but data needs to be taken care of. For now I will just
make a server take in two active objects and deal with it... Maybe later
I will find an even better way.
*/
class Server
{
public:
	// actually I changed my mind, I'm just going to get it working.
	Server( unsigned port_ );

	int Shutdown ();  // you must call this to shut a server down

	int Run ();

	typedef std::vector<std::pair<std::string, SOCKET> > NamedSockets;

private:
	unsigned      port;
	SOCKET        listenerSocket;
	NamedSockets  clients;
	Mutex         clients_mutex;

	AcceptingThread  accepting_thread;
	EchoingThread    echoing_thread;

	Server( const Server& rhs );  // DO NOT CALL!

};

#endif
