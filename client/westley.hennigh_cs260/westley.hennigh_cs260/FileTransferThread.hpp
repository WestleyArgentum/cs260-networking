// Westley Hennigh
// FileTransferThread.hpp: ActiveObject that runs a file transfer between clients.
// CS260 Assignment 3
// Mar 26th 2010

#ifndef FILET_H
#define FILET_H

#include <vector>
#include <string>
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

#include "ActiveObject.hpp"
#include "Mutex.hpp"

class FileTransferThread : public ActiveObject
{
public:
	FileTransferThread (std::string remote_ip_, unsigned remote_port_, std::string filename_);
	virtual ~FileTransferThread ();

private:
	virtual void InitThread ();
	virtual void Run ();
	virtual void FlushThread ();

	std::string filename;
	std::string remote_ip;
	unsigned remote_port;

	SOCKET socket;
	sockaddr_in remote_address;

};

#endif
