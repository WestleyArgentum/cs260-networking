// Westley Hennigh
// EchoActiveObj.cpp: ActiveObject for receiving messages from clients and echoing them back to everyone
// CS260 Assignment 2
// Feb 27th 2010

#include <iostream>
#include "winsock2.h" // winsock
#pragma comment(lib, "ws2_32.lib")

#include "EchoActiveObj.hpp"
#include "Message.hpp"

static const unsigned BUFF_SIZE = 4096;

EchoingThread::EchoingThread( NamedSockets& clients_, Mutex& mutex_ ) :clients(clients_), mutex(mutex_)
{}

EchoingThread::~EchoingThread()
{}

void EchoingThread::InitThread()
{
  // for now we don't need any init
}

void EchoingThread::Run()
{
	char buffer [BUFF_SIZE] = {0};
	IMessage* received_message;  // pointer to message constructed from data
	int ret;

	while (!isDying)
	{
		// this scope is a quick fix for locking the vector.  we don't want run to grab and never let go of
		// the mutex so we lock it in this scope and wait a little after each loop though the vector.
		{
			Lock lock(mutex);

			// loop through the sockets looking on each for data
			for (unsigned i = 0; i < clients.size(); ++i)
			{
				memset(buffer, 0, BUFF_SIZE);

				// if we find data then make it into a nice message and switch on the message type
				ret = recv(clients[i].second, buffer, BUFF_SIZE, 0);
				if(ret == SOCKET_ERROR)  // check for error
				{
					ret = WSAGetLastError();
					if(ret != WSAEWOULDBLOCK)
					{
						std::cout << "Error receiving data in the echoing thread: " << ret << std::endl;
						return;
					}
					else  // if it was
						continue;
				}
				else if(ret == 0)  // means they are done playing around
				{
					// remove them from the list and post a message to everyone that they are gone
					RemoveUserMsg msg (RemoveUser_Msg);
					strcpy(msg.user, clients[i].first.c_str());

					char tempkitty [BUFF_SIZE];
					int kittysize = msg.WriteOut(tempkitty);

					for (unsigned j = 0; j < clients.size(); ++j)
						send(clients[j].second, tempkitty, kittysize, 0);

					clients.erase(clients.begin() + i);

					continue;
				}

				// make the message (this is a good spot to check for errors)
				if (*reinterpret_cast<unsigned*>(buffer) > static_cast<unsigned>(ret))
				{
					std::cout << "There was a problem recieving a message in the echo thread..." << std::endl;
				}
				received_message = ConstructMessage(buffer);

				switch(received_message->my_type)
				{
				case Invalid_Type:
				case NUM_TYPES:
				case Username_Msg:
					{
						// something has gone terribly wrong
						std::cout << "Invalid message type received from client..." << std::endl;
						break;
					}

				// if the message is chat then echo it around
				case ChatData_Msg:
					{
						// echo... echo... echo...
						for (unsigned kitty = 0; kitty < clients.size(); ++kitty)
						{
							ret = send(clients[kitty].second, buffer, ret, 0);
							//^! check for send error
						}
						break;
					}
				}

				delete received_message;  // remember to clean up that message we made
			}
		}

		Sleep(1);
	}
}

void EchoingThread::FlushThread()
{
	// This seems like a terrible idea, but I'm going to try anyway.
	// While the return of Resume is positive the thread is not going...
	// We need the thread to go so it can be killed.
	while(mythread.Resume());

	// theres not much else we can do to prod it along...
}
