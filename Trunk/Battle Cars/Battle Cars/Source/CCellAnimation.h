#ifndef _CCELLANIMATION_H
#define _CCELLANIMATION_H

#include <Windows.h>
using namespace std;
#include <vector>
//#include "SGD Wrappers\CSGD_TextureManager.h"


class CCellAnimation
{
private:
	struct tFrame
	{
		RECT rFrame;
		POINT ptAnchor;
		float fDuration; // how long this individual frame is
	};

	vector<tFrame> m_vFrames;
	int m_nFrameWidth;
	int m_nFrameHeight;
	int m_nNumCols;

	int m_nImageID;

	float m_fSpeed; // speed = 1.0f normal, 0.5f half as fast, 2.0f twice as fast

	int m_nCurrFrame;

	float m_fTimeWaited;
	
	bool m_bIsLooping; // mode: once, loop, oscillate
	// direction: forward backward
	bool m_bIsPlaying;


public:

	void Update(float fElapsedTime)
	{
		if(!m_bIsPlaying)
			return;

		m_fTimeWaited += fElapsedTime * m_fSpeed;		

		if(m_fTimeWaited > m_vFrames[m_nCurrFrame].fDuration)
		{
			m_fTimeWaited -= m_vFrames[m_nCurrFrame].fDuration;
			m_nCurrFrame++;

			if((size_t)(m_nCurrFrame) >= m_vFrames.size())
			{
				if(m_bIsLooping)
					Reset();
				else
				{
					Stop();
					m_nCurrFrame = m_vFrames.size()-1;
				}
			}

		}
	}
	void Render(int x, int y, float scale, bool bIsFlipped,DWORD color)
	{
		RECT rFrame = m_vFrames[m_nCurrFrame].rFrame;
		POINT ptAnchor = m_vFrames[m_nCurrFrame].ptAnchor;

		float fScalex = scale;
		if(bIsFlipped)
		{
			fScalex *= -1.0f;
			int width = rFrame.right - rFrame.left;
			x += (int)(width * scale);
			ptAnchor.x = width - ptAnchor.x;
		}
		
//		CSGD_TextureManager::GetInstance()->Draw(m_nImageID,x - (ptAnchor.x * scale),y - (ptAnchor.y * scale),fScalex,scale,&rFrame,0,0,0,color);

		
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


};



#endif