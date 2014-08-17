/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#include "win32ActiveObject.h"

win32ActiveObject::win32ActiveObject()
	: isDying(0),
	#pragma warning(disable: 4355) // 'this' used before initialized
	thread (threadEntry, this)
	#pragma warning(default: 4355)
{
}

win32ActiveObject::~win32ActiveObject()
{
}

void win32ActiveObject::kill()
{
	isDying++;
	flushThread();
}

DWORD WINAPI win32ActiveObject::threadEntry (void* argP)
{
    win32ActiveObject *activeP = (win32ActiveObject *) argP;
    activeP->initThread();
    activeP->run();
    return 0;
}
DWORD win32ActiveObject::getId()
{
	return GetCurrentThreadId();
}
