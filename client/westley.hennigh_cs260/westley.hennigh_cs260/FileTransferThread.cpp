// Westley Hennigh
// FileTransferThread.cpp: ActiveObject that runs a file transfer between clients by processing jobs.
// CS260 Assignment 3
// Mar 26th 2010

#include <iostream>
#include <algorithm>
#include <functional>

#include "FileTransferThread.hpp"
#include "Mutex.hpp"

FileTransferThread* FileTransferThread::me = NULL;

/*
No longer in use, info on connection will come in with jobs (for now just the first one will be looked at).
*/
FileTransferThread::FileTransferThread() : quitflag(false)
{}

FileTransferThread::~FileTransferThread()
{}

void FileTransferThread::InitThread()
{
	// no init, file transfer sits and waits for jobs, then uses supersocket
}

void FileTransferThread::Run()
{
	while (!quitflag)
	{
		{
			Lock lock(jobs_mutex);  // acquire
			for (unsigned i = 0; i < activejobs.size(); ++i)
				activejobs[i]->update();
		}

		// do
	}
}

void FileTransferThread::FlushThread()
{
	// This seems like a terrible idea, but I'm going to try anyway.
	// While the return of Resume is positive the thread is not going...
	// We need the thread to go so it can be killed.
	while(mythread.Resume());
}

void FileTransferThread::AddJob( jobs* job )
{
	// append the socket pointer into the job, the job will set up the socket for it's
	// remote host and stuff

	job->SetSocket(&socket);
}

FileTransferThread* FileTransferThread::GetInstance()
{
	if (!me)
		me = new FileTransferThread;

	return me;
}