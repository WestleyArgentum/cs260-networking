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
*/
FileTransferThread::FileTransferThread() : quitflag(false)
{
	// actually I decided that for this one we can just wake pronto
	Wake();
}

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
			{
				if(activejobs[i]->update())
				{
					delete activejobs[i];
					activejobs.erase(activejobs.begin() + i);
				}
			}
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

	// now add the job to the list of things to be updated!
	Lock lock(jobs_mutex);
	activejobs.push_back(job);
}

FileTransferThread* FileTransferThread::GetInstance()
{
	if (!me)
		me = new FileTransferThread;

	return me;
}