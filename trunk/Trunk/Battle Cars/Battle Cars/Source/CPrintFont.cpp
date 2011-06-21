#include "CPrintFont.h"

CPrintFont::CPrintFont(int imageid)
{
	m_nCharWidth = 32;
	m_nCharHeight = 32;
	m_cStartChar = 32;

	m_nImageID = imageid;
}


RECT CPrintFont::CellAlgorithm(int id)
{
	
	RECT rCell;
	rCell.left = (id % 10) * m_nCharWidth;
	rCell.top = (id / 10) * m_nCharHeight;
	rCell.right = rCell.left + m_nCharWidth;
	rCell.bottom = rCell.top + m_nCharHeight;

	return rCell;

}