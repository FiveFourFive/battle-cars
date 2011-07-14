#include "CPrintFont.h"

CPrintFont::CPrintFont(int imageid)
{
	m_nCharWidth = 25;
	m_nCharHeight = 25;
	m_cStartChar = ' ';

	m_nImageID = imageid;
}


RECT CPrintFont::CellAlgorithm(int id)
{
	
	RECT rCell;
	rCell.left = (id % 11) * m_nCharWidth;
	rCell.top = (id / 11) * m_nCharHeight;
	rCell.right = rCell.left + m_nCharWidth;
	rCell.bottom = rCell.top + m_nCharHeight;

	return rCell;

}