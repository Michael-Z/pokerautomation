/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#include "win32Thread.h"

win32Thread::win32Thread(DWORD (WINAPI * functionP) (void* arg), void* argP)
{
	threadHandle = CreateThread (
		0, // Security attributes
        0, // Stack size
        functionP, 
        argP, 
        CREATE_SUSPENDED, 
        &threadId);
}

win32Thread::~win32Thread()
{
	CloseHandle(threadHandle);
}
void win32Thread::resume()
{
	ResumeThread(threadHandle);
}
void win32Thread::suspend()
{
	SuspendThread(threadHandle);
}
