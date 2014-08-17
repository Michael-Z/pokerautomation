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


class win32Thread
{
private:
	HANDLE threadHandle;
	DWORD threadId;
public:
	win32Thread(DWORD (WINAPI * functionP) (void* arg), void* argP);
	~win32Thread();
	void resume();
	void suspend();
};
