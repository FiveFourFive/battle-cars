
///////////////////////////////////////////////////////
// File Name	:	"CSTOPWATCH.h"
//
// Author		:	Zach Bolthouse
//
// Purpose		:	Hi-res timer that emulates a stopwatch
///////////////////////////////////////////////////////

#include <Windows.h>
#pragma comment(lib, "winmm.lib")

#ifndef __STOPWATCH_H
#define __STOPWATCH_H

class CStopWatch
{
private:
	LONGLONG	m_llStartTick;
	LONGLONG	m_llFrequency;	//# Ticks per second
	double		m_dFrequencyScale; //to save dividing
	double		m_dElapsedTime;
	bool		m_bIsRunning;

public:
	CStopWatch();

	//Start Timing
	void Start();

	//Stop Timining
	void Stop();

	//Reset Elapsed Time to 0
	void Reset();

	//Return the elapsed time
	double GetElapsedTime();
};


#endif