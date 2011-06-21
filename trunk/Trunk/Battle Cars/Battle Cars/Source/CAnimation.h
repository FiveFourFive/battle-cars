#ifndef _CANIMATION_H
#define _CANIMATION_H

#include <Windows.h>
//#include "SGD Wrappers\CSGD_TextureManager.h"

class CAnimation
{
	private:
	// cell algorithm
	int m_nFrameWidth;
	int m_nFrameHeight;
	int m_nNumCols;

	int m_nImageID;

	float m_fSpeed; // speed = 1.0f normal, 0.5f half as fast, 2.0f twice as fast

	int m_nCurrFrame;
	int m_nMaxFrames;

	float m_fTimeWaited;
	float m_fTimePerFrame;

	bool m_bIsLooping; // mode: once, loop, oscillate
	// direction: forward backward
	bool m_bIsPlaying;

	

	RECT CellAlgorithm(int id);

public:
	CAnimation(int nwidth, int nheight, int ncols, int nimage, float speed, int maxframes, float timeperframe, bool looping);
	void SetImageID(int imageID)
	{
		m_nImageID = imageID;
	}
	void SetSpeed(float speed) { m_fSpeed = speed; }
	void SetCols(int cols) { m_nNumCols = cols; }
	void SetMaxFrames(int frames) { m_nMaxFrames = frames; }
	void Update(float fElapsedTime)
	{
		if(!m_bIsPlaying)
			return;

		m_fTimeWaited += fElapsedTime * m_fSpeed;		

		if(m_fTimeWaited > m_fTimePerFrame)
		{
			m_fTimeWaited = 0.0f;
			m_nCurrFrame++;

			if(m_nCurrFrame >= m_nMaxFrames)
			{
				if(m_bIsLooping)
					Reset();
				else
				{
					Stop();
					m_nCurrFrame = m_nMaxFrames-1;
				}
			}

		}
	}
	void Render(int x, int y, float scale, DWORD color)
	{
		if(m_bIsPlaying)
		{
			RECT rFrame = CellAlgorithm(m_nCurrFrame);
		//	CSGD_TextureManager::GetInstance()->Draw(m_nImageID,x,y,scale,scale,&rFrame,0,0,0,color);
	
		}
	}

	// start animation from beginning
	void Play()
	{
		Reset();
		m_bIsPlaying = true;
	}
	// pause
	void Stop()
	{
		m_bIsPlaying = false;
	}
	// unpause
	void Resume()
	{
		m_bIsPlaying = true;
	}
	void Reset()
	{
		m_nCurrFrame = 0;
		m_fTimeWaited = 0.0f;
	}
	bool IsPlaying() { return m_bIsPlaying; }

};


#endif