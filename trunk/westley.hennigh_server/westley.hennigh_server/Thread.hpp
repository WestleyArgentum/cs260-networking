// Westley Hennigh
// Thread.hpp: Interface for working with threads.
// CS260 Assignment 2
// Feb 25th 2010

#ifndef THREAD_H
#define THREAD_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Thread
{
public:
	Thread ( DWORD (WINAPI* pFun) (void* arg), void* pArg)
	{
		thread_handle = CreateThread (
			0,  // Security attributes
			0,  // Stack size
			pFun,
			pArg,
			CREATE_SUSPENDED,  // Start in suspended state, resume later
			&id );
	}
	~Thread () { CloseHandle (thread_handle); }

	int Resume () { return ResumeThread (thread_handle); }

	void WaitForDeath ()
	{
		WaitForSingleObject (thread_handle, INFINITE);  // Wait forever for the thread to close...
	}
private:
	HANDLE thread_handle;
	DWORD  id;
};

#endif
