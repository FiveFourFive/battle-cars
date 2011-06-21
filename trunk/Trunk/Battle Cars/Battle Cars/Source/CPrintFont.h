
#ifndef _CPRINTFONT_H
#define _CPRINTFONT_H

#include <Windows.h>
#include "CSGD_TextureManager.h"


class CPrintFont
{

private:
	CSGD_TextureManager*	m_pTM;
	//	Cell Algorithm
	int 	m_nCharWidth;
	int	m_nCharHeight;
	char	m_cStartChar;

	int	m_nImageID;

	RECT CellAlgorithm(int id);
public:
	CPrintFont(int imageid);
	void SetImageID(int nImageID) { m_nImageID = nImageID; }
	void Print(const char* szText, int x, int y, float scale, DWORD color)
	{
		m_pTM = CSGD_TextureManager::GetInstance();
		// loop through text one char at a time
		int len = strlen(szText);
		int xorig = x;
		for(int i = 0; i < len; i++)
		{
		// get letter
		char ch = szText[i];

		if(ch == ' ')
		{
			x += (int)(m_nCharWidth * scale);
			continue;
		}
		else if(ch == '\n')
		{
			x = xorig;
			y += (int)(m_nCharHeight * scale);	
			continue;
		}
		// get image id
		int id = ch - m_cStartChar;
		// get rect of letter
		RECT rLetter = CellAlgorithm(id);
		// draw to screen
		m_pTM->Draw(m_nImageID,x,y,scale,scale,&rLetter,0,0,0,color);
		x += (int)(m_nCharWidth * scale);
		}					
	}


};


#endif