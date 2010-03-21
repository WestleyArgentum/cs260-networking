// Westley Hennigh
// ActiveObject.hpp: Interface for working with threads.
// CS260 Assignment 2
// Feb 25th 2010

#ifndef ACTIVEO_H
#define ACTIVEO_H

#include "Thread.hpp"

/*
ActiveObject is basically a cookie cutter thread.
Derive from ActiveObject and you can specify init, run and
flush for your own thread.
*/
class ActiveObject
{
public:
	ActiveObject ();
	virtual ~ActiveObject ();

	void Wake ();  // I like the idea of telling the thread to go rather than having it do
								 // so automatically after construction.

	void Kill ();  //^? must call before exiting... perhaps I should do an isDying check in 
								 // the destructor and clean up (just in case)

protected:
	virtual void InitThread () = 0;
	virtual void Run () = 0;
	virtual void FlushThread () = 0;

	// thread function will call virtual functions from derived class
	static DWORD WINAPI ThreadFunc (void *pArg);

	bool    isDying;
	Thread  mythread;
};

#endif
