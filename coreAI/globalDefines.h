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

//#define DEBUG ///< Debug mode
#define DEBUGASSERT ///< Enable code assertions
#define CODETIMER ///< Enable code timing

typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#ifdef DEBUG
	#define DBGPR(x) std::cout << #x " = " << x << "\n"; 
#else
	#define	DBGPR(x)
#endif

#define Stringize( L ) #L 
#define MakeString( M, L ) M(L) 
#define $Line MakeString( Stringize, __LINE__ ) 
#define Reminder __FILE__ "(" $Line ") : Reminder: " 

//Tricks for enums
#define enumTricks(eEnumType) \
inline void operator++(eEnumType& eVal) \
{ \
    eVal = eEnumType(eVal+1); \
} \
\
inline void operator++(eEnumType& eVal, int)  \
{ \
    eVal = eEnumType(eVal+1); \
} \
\
inline void operator--(eEnumType& eVal) \
{ \
    eVal = eEnumType(eVal-1); \
} \
\
inline void operator--(eEnumType& eVal, int)  \
{ \
    eVal = eEnumType(eVal-1); \
} \
\
inline eEnumType eEnumMax(eEnumType) \
{ \
    return eEnumType##_Max; \
} \
\
inline eEnumType eEnumMin(eEnumType) \
{ \
	return eEnumType##_Min; \
}

