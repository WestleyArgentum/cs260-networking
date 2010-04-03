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
#include "Socket.hpp"
#include "Jobs.h"

class FileTransferThread : public ActiveObject
{
public:
	FileTransferThread ();
	virtual ~FileTransferThread ();

	// singleton
	static FileTransferThread* GetInstance ();

	void AddJob (jobs* job);

	// these are just hacks to get this working ----
	std::vector<sendJob*> pending_sendjobs;
	std::vector<recJob*> pending_recvjobs;
	// -------

private:
	virtual void InitThread ();
	virtual void Run ();
	virtual void FlushThread ();

	static FileTransferThread* me;

	ReliableUdpSocet socket;  // this will eventually be multiplexed. as of now if more than one job uses it it will break.
	//sockaddr_in remote_address;

	std::vector<jobs*> activejobs;
	Mutex jobs_mutex;
	bool quitflag;  // if true, were shutting down

};

#endif
