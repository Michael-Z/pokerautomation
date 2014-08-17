/*
$Revision: 49 $
$Date: 2008-10-03 00:41:07 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#pragma once
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

void test_timer();
class codeTimer
{
public:
  codeTimer() {Init();}                // Init timer on creation

  void Start();						 // Start timer
  void Stop();                       // Stop timer
  bool IsRunning();                  // Returns FALSE if stopped.
  bool IsSupported();                // Returns FALSE if performance counter not supported.

  const double Resolution();         // Returns timer resolution in seconds
  const double Resolutionms();       // Returns timer resolution in milliseconds
  const double Resolutionus();       // Returns timer resolution in microseconds
  
  const double Elapsed();            // Returns elapsed time in seconds
  const double Elapsedms();          // Returns elapsed time in milliseconds 
  const double Elapsedus();          // Returns elapsed time in microseconds
  const __int64 Elapsedticks();		 // Returns elapsed ticks

private:
	void Init();
  __int64 m_Start;
  static __int64 m_Freq;   // does not change
  static __int64 m_Adjust; // Adjustment time it takes to Start and Stop
};

inline void codeTimer::Init()
{
  if (!m_Freq) 
  { // Initialization runs once
    QueryPerformanceFrequency((LARGE_INTEGER *)&m_Freq); 
    if (!m_Freq)
      m_Freq = 1; // Timer will be useless but will not cause divide by zero
    m_Start = 0; 
    m_Adjust = 0; 
    Start();            // Time a Stop
    Stop(); 
    m_Adjust = m_Start;
  }
  //Reset counter to 0
  m_Start = 0;  
}

inline void codeTimer::Start() 
{ 
  __int64 i;
  QueryPerformanceCounter((LARGE_INTEGER *)&i);
  m_Start = i;
} 

inline void codeTimer::Stop() 
{
  if (m_Start <= 0)
  {
    return;          // Timer wasnt started
  }
  __int64 i;
  QueryPerformanceCounter((LARGE_INTEGER *)&i); 
  m_Start += -i;          // Stopped timer keeps elapsed timer ticks as a negative 
  if (m_Start < m_Adjust)
    m_Start -= m_Adjust;  // Adjust for time timer code takes to run
  else 
    m_Start = 0;          // Nothing in between start and stop
} 

inline bool codeTimer::IsRunning() 
{ 
  return (m_Start > 0); // -ve value means timer has stopped   
}
 inline const double codeTimer::Elapsed()
{ // Returns elapsed time in seconds
  Stop();
  return (double)(-m_Start)/(double)m_Freq; 
}

inline const double codeTimer::Elapsedms() 
{ 
  Stop();
  return (-m_Start*1000.0)/(double)m_Freq; 
}

inline const double codeTimer::Elapsedus() 
{
  Stop();
  return (-m_Start * 1000000.0)/(double)m_Freq; 
}

inline const __int64 codeTimer::Elapsedticks()
{
	Stop();
	return (-m_Start); 
}

