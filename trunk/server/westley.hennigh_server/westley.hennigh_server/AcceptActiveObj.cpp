// Westley Hennigh
// AcceptActiveObj.cpp: ActiveObject for accepting new connections and putting them in the vector
// CS260 Assignment 2
// Feb 25th 2010

#include <iostream>
#include "winsock2.h" // winsock
#pragma comment(lib, "ws2_32.lib")

#include "AcceptActiveObj.hpp"
#include "Message.hpp"

static const unsigned BUFF_SIZE = 4096;

AcceptingThread::AcceptingThread( NamedSockets& clients_, Mutex& mutex_, SOCKET& listener_ )
	: clients(clients_), mutex(mutex_), listener(listener_)
{}

AcceptingThread::~AcceptingThread()
{}

void AcceptingThread::InitThread()
{
	// we don't need to do anything here for now
}

void AcceptingThread::Run()
{
	int ret;
	char buffer [BUFF_SIZE];

	// we will have a socket that we are waiting with
	sockaddr_in remoteEndpoint;
	int endpointSize = sizeof(remoteEndpoint);

	while(!isDying)
	{
		memset(&remoteEndpoint, 0, sizeof(remoteEndpoint));
		memset(buffer, 0, BUFF_SIZE);

		// now we call accept on the listening socket.
		SOCKET clientSocket = accept(listener, (sockaddr*)&remoteEndpoint, &endpointSize);
		if(clientSocket == INVALID_SOCKET)
		{
			ret = WSAGetLastError();
			if (ret != WSAEWOULDBLOCK)
			{
				std::cout << "error accepting: " << ret << std::endl;
				std::cout << "please shutdown and try again."  << std::endl;
				return;
			}
		}
		else
		{
			// we now have a valid client socket

			// first we will query them for some basic information (name)
			std::string name;

			// first send ------------------
			IMessage msg;
			ret = msg.WriteOut(buffer);
			ret = send(clientSocket, buffer, ret, 0);
			// -----------------------

			// then receive ----------------
			char* curr_place = buffer;   // buffer for received data
			unsigned byte_count = 0;     // number of bytes read in so far
			IMessage* received_message;  // pointer to message constructed from data
			while(true)
			{
				// read in to the current place in the buffer and no more than the space left in the buffer
				ret = recv(clientSocket, curr_place, BUFF_SIZE - byte_count, 0);
				if(ret == SOCKET_ERROR)  // check for error
				{
					if(WSAGetLastError() != WSAEWOULDBLOCK)
					{
						std::cout << "Error receiving data in accepting thread: " << WSAGetLastError() << std::endl;
						return;
					}
				}
				else if(ret == 0)  // means something funky is going on. they don't want to connect anymore...
					break;
				else
					byte_count += ret;  // inc the number of bytes we have read in so far

				// now check to see if we have a good message.  if we do, at this time we are not concerned with
				// receiving multiple messages so just set it up and break out.
				if (byte_count <= sizeof(unsigned))
					continue;
				if(*(reinterpret_cast<unsigned*>(buffer)) <= byte_count)
				{
					received_message = ConstructMessage(buffer);
					break;
				}
			}

			// quick check for the client ending before sending us a username
			if (received_message == NULL)
				continue;  // we don't want to go on and parse the message because we didn't actually get one

			// now we have a message, lets check its type and hopefully get a good username
			if (received_message->my_type == Username_Msg)
				name = static_cast<UsernameMsg*>(received_message)->myname;
			else
			{
				std::cout << "They are not properly responding to a request for a username. Dropping them..." << std::endl;

				shutdown(clientSocket, SD_BOTH);
				closesocket(clientSocket);

				continue;  // we want to start over waiting for a connection
			}
			// ------------------------

			// tell the new socket about all it's predecessors
			{
				Lock lock (mutex);

				char msgbuff [BUFF_SIZE];
				UsernameMsg name_msg;

				for (unsigned i = 0; i < clients.size(); ++i)
				{
					name_msg.myname = clients[i].first;
					int size = name_msg.WriteOut(msgbuff);
					send(clientSocket, msgbuff, size, 0);
					Sleep(1);  //^! temp fix
				}
			}

			// now put the socket into non-blocking mode and put it in the vector with the name
			u_long val = 1;
			ioctlsocket(clientSocket, FIONBIO, &val);

			AddNameSocketPair(name, clientSocket);

			// a new member has arrived. we need to tell everyone (including them) about the new username
			{
				Lock lock (mutex);

				for (unsigned i = 0; i < clients.size(); ++i)
				{
					send(clients[i].second, buffer, *reinterpret_cast<unsigned*>(buffer), 0);  //^* this code relies on the fact that the buffer has not changed from above
																																										 // and that the buffer has the correct size written into it
				}
				delete received_message;  // we have to clean up the dynamically allocated messages returned from ConstructMessage
				received_message = NULL;  // make sure we set it to NULL so that if the next client exits without giving us a name we break and don't
																	// get confused with an old message
			}
		}

		Sleep(1);  // sleep for a little while, no need to destroy the processor
	}

	// We are dying. Oh, no!
}

void AcceptingThread::FlushThread()
{
	// This seems like a terrible idea, but I'm going to try anyway.
	// While the return of Resume is positive the thread is not going...
	// We need the thread to go so it can be killed.
	while(mythread.Resume());

	// theres not much else we can do to prod it along...
}

void AcceptingThread::AddNameSocketPair( std::string name, SOCKET& socket )
{
	Lock lock (mutex);
	clients.push_back(std::pair<std::string, SOCKET> (name, socket));
}
