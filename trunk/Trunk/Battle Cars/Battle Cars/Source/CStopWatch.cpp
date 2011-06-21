
///////////////////////////////////////////////////////
// File Name	:	"CSTOPWATCH.cpp"
//
// Author		:	Zach Bolthouse
//
// Purpose		:	Hi-res timer that emulates a stopwatch
///////////////////////////////////////////////////////

#include "CStopWatch.h"


CStopWatch::CStopWatch()
{
	LONGLONG llFrequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&llFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);

	m_dFrequencyScale = 1.0 / (double)llFrequency;
	m_dElapsedTime = 0.0;
	m_bIsRunning = false;
}

//Start Timing
void CStopWatch::Start()
{
	if(!m_bIsRunning)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);
		m_bIsRunning = true;
	}
}

//Stop Timining
void CStopWatch::Stop()
{
	if(m_bIsRunning)
	{
		m_dElapsedTime += GetElapsedTime();
		m_bIsRunning = false;
	}
}

//Reset Elapsed Time to 0
void CStopWatch::Reset()
{
	m_dElapsedTime = 0.0;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);
}

//Return the elapsed time
double CStopWatch::GetElapsedTime()
{
	double dTimeDiff = 0.0;
	if(m_bIsRunning)
	{
		LONGLONG llStopTick;
		QueryPerformanceCounter((LARGE_INTEGER*)&llStopTick);

		dTimeDiff = (double)((llStopTick - m_llStartTick) * m_dFrequencyScale);
		dTimeDiff += m_dElapsedTime;
	}
	else
		dTimeDiff = m_dElapsedTime;

	return dTimeDiff;
}