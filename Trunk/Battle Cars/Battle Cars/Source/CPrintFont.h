
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
		if(ch=='p' || ch=='j' || ch=='y' || ch=='g')
			m_pTM->Draw(m_nImageID,x,y+3,scale,scale,&rLetter,0,0,0,color);
		else
			m_pTM->Draw(m_nImageID,x,y,scale,scale,&rLetter,0,0,0,color);
		x += (int)(m_nCharWidth * scale);
		}					
	}
	
	void PrintCentered(const char* szTextToPrint, int nPosX, int nPosY, float fScale, DWORD dwColor)
	{
		int nOffsetX = 0;
		//loop through the string 1 char at a time
		int len = strlen(szTextToPrint);
		for(int i=0; i < len ; i++)
		{
			//char /find the id by offsetting the startchar
			char ch=szTextToPrint[i];
		
			nOffsetX = (m_nCharWidth*i*(int)fScale);
			if(ch == ' ')
			{
				nOffsetX = (m_nCharWidth*i*(int)fScale);
				continue;
			}
			else if (ch=='\n')
			{
				nOffsetX = 0;
				nPosY+= m_nCharHeight *(int)fScale;
			
				continue;
			}
		
			int id = (int)(ch - m_cStartChar);

			//use CellAlgorithm to get RECT for Letter
		
			RECT rLetter = CellAlgorithm(id);
			//Draw to the screen	

			CSGD_TextureManager::GetInstance()->Draw(m_nImageID, (int((nPosX-((m_nCharWidth*len)/2))+nOffsetX)*fScale), nPosY, fScale, fScale, &rLetter, 0,0,0, dwColor);
		}
	}


};


#endif