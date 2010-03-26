// Westley Hennigh
// FileTransferThread.hpp: ActiveObject that runs a file transfer between clients.
// CS260 Assignment 3
// Mar 26th 2010

#ifndef FILET_H
#define FILET_H

#include <vector>
#include <string>

#include "ActiveObject.hpp"
#include "Mutex.hpp"

class FileTransferThread : public ActiveObject
{
public:
	FileTransferThread (std::string remote_ip, unsigned remote_port, std::string filename);
	virtual ~FileTransferThread ();

private:
	virtual void InitThread ();
	virtual void Run ();
	virtual void FlushThread ();

};

#endif
