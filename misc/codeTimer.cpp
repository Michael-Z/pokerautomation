/*
$Revision: 49 $
$Date: 2008-10-03 00:41:07 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#include "codeTimer.h"
#include <stdio.h>

// initialize static member vars
__int64 codeTimer::m_Freq = 0; 
__int64 codeTimer::m_Adjust = 0; 

void test_timer(){
	codeTimer mytimer;
	
	mytimer.Start();
	for(int i(0);i<1000;i++);
	mytimer.Stop();
	printf("\nTimer supported = %d",mytimer.IsSupported());
	printf("\nResolution = %f us",mytimer.Resolutionus());
	printf("\nElapsed = %f us",mytimer.Elapsedus());
}


bool codeTimer::IsSupported()
{ 
  return (m_Freq > 1);
}

const double codeTimer::Resolution()   
{ // Timer resolution in seconds
  return 1.0/(double)m_Freq; 
}

const double codeTimer::Resolutionms() 
{ // Timer resolution in milliseconds
  return 1000.0/(double)m_Freq; 
}

const double codeTimer::Resolutionus() 
{ // Timer resolution in microseconds
  return 1000000.0/(double)m_Freq; 
}