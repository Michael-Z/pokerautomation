/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#pragma once
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "win32Thread.h"

/**
Base class used to derive classes that can have threadable member functions
The derived constructor should call thread.resume() because the thread is started in a suspended state
*/
class win32ActiveObject
{
protected:
	/**
	Thread initialization routines
	*/
	virtual void initThread() = 0;
	/**
	The main function to be run by the thread
	*/
    virtual void run() = 0;
	/**
	Thread cleanup routines
	*/
    virtual void flushThread() = 0;

    int isDying;
    static DWORD WINAPI threadEntry ( void *argP);
    win32Thread thread;
public:
	win32ActiveObject();
	~win32ActiveObject();
	void kill();
	DWORD getId();
};
