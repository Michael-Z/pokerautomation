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

/**
Implements events for synchronizing between threads
*/
class win32Event
{
private:
	HANDLE eventHandle;
public:
	win32Event();
	~win32Event();
	/**
	Causes the event state to go to a signalled state
	*/
	void signal();
	/**
	Clears the signalled state of the event
	*/
	void clear();
	/**
	Causes the calling thread to wait on the event.
	*/
	void wait();
};
