#include "CAnimation.h"

CAnimation::CAnimation(int nwidth, int nheight, int ncols, int nimage, float fspeed, int nmaxframes, float ntimeperframe, bool blooping)
{
	m_nFrameWidth = nwidth;
	m_nFrameHeight = nheight;
	m_nNumCols = ncols;
	
	m_nImageID = nimage;
	
	m_fSpeed = fspeed;
	m_nMaxFrames = nmaxframes;
	m_fTimePerFrame = ntimeperframe;
	
	m_bIsLooping = blooping;
	Reset();
}

RECT CAnimation::CellAlgorithm(int id)
{
	RECT rCell;
	rCell.left = (id % m_nNumCols) * m_nFrameWidth;
	rCell.top = (id / m_nNumCols) * m_nFrameHeight;
	rCell.right = rCell.left + m_nFrameWidth;
	rCell.bottom = rCell.top + m_nFrameHeight;

	return rCell;

}