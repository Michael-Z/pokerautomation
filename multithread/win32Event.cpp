/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#include "win32Event.h"

win32Event::win32Event()
{
	//Non signaled state
	eventHandle = CreateEvent (0, TRUE, FALSE, 0);
}

win32Event::~win32Event()
{
	CloseHandle(eventHandle);
}

void win32Event::signal()
{
	SetEvent(eventHandle);
}

void win32Event::clear()
{
	ResetEvent(eventHandle);
}
void win32Event::wait()
{
	WaitForSingleObject(eventHandle,INFINITE);
}
