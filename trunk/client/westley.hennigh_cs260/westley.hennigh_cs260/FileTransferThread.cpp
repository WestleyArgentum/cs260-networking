// Westley Hennigh
// FileTransferThread.cpp: ActiveObject that runs a file transfer between clients.
// CS260 Assignment 3
// Mar 26th 2010

#include <iostream>

#include "FileTransferThread.hpp"


FileTransferThread::FileTransferThread( std::string remote_ip_, unsigned remote_port_, std::string filename_ )
	: remote_ip(remote_ip_), remote_port(remote_port_), filename(filename_)
{}

FileTransferThread::~FileTransferThread()
{}

void FileTransferThread::InitThread()
{
	int ret;

	// set up the local socket -------
	struct sockaddr_in socketAddress;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = 0;

	hostent* localhost;
	localhost = gethostbyname("");
	char* localIP = inet_ntoa(*(in_addr*)*localhost->h_addr_list);

	socketAddress.sin_addr.s_addr  = inet_addr(localIP);

	socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

	if(socket == INVALID_SOCKET)
	{
		int errorcode = WSAGetLastError();
		std::cout << "Error on creating socket: " << errorcode << std::endl;
	}

	// bind
	ret = bind(socket, (sockaddr*)&socketAddress, sizeof(socketAddress));
	if(ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
	}
	// --------------

	// set up the remote address ----------
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(8000u);  //^! need to specify a range for this in the config file
	remote_address.sin_addr.s_addr  = inet_addr(localIP);
	// --------------
}

void FileTransferThread::Run()
{
	// set up a data object to get chunks of data

	
}

void FileTransferThread::FlushThread()
{
	// This seems like a terrible idea, but I'm going to try anyway.
	// While the return of Resume is positive the thread is not going...
	// We need the thread to go so it can be killed.
	while(mythread.Resume());
}
