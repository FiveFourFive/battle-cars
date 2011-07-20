/////////////////////////////////////////////////
//	File	:	"CMap.cpp"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains all the information
//				for making a level.
//				
/////////////////////////////////////////////////

#include <string>
#include "CSGD_TextureManager.h"
#include "CMap.h"
#include "CTile.h"
#include "tinyxml.h"
#include "CLevelSelectionState.h"

CMap::CMap ()
{
	SetMapWidth (0);
	SetMapHeight (0);
	SetTileWidth (0);
	SetTileHeight (0);
	SetTileImageID (-1);
	SetMapName ("NoName");
}

CMap::~CMap ()
{
	for (int i = 0; i < m_nMapHeight; i++)
	{
		delete[] m_pTiles[i];
	}
	delete[] m_pTiles;

	for (int i = 0; i < m_nMapHeight; i++)
	{
		delete[] m_pEvents[i];
	}
	delete[] m_pEvents;

	m_pTiles = NULL;
	m_pEvents = NULL;
}

bool CMap::LoadMap (const char* filename)
{
	return false;
}

bool CMap::LoadTiles (const char* filename)
{
	TiXmlDocument doc;
	std::string path = "Resource/Data/";
	char buffer[32];
	sprintf_s(buffer,32, "level%i/", CLevelSelectionState::GetInstance()->GetSelection() + 1);
	path += buffer;

	std::string filepath = path + filename;

	if (doc.LoadFile (filepath.c_str()) == false)
		return false;

	TiXmlElement* pRoot = doc.RootElement ();

	if (!pRoot)
		return false;
	
	
	int width;
	int height;

	pRoot->Attribute ("Width", &width);
	pRoot->Attribute ("Height", &height);
	this->SetTileWidth (width);
	SetTileHeight (height);

	TiXmlElement* pBitmapName = pRoot->FirstChildElement ("BitmapName");

	if (pBitmapName)
	{
		path = "Resource/Graphics/";
		filepath = path + pBitmapName->GetText();

		this->SetTileImageID (CSGD_TextureManager::GetInstance ()->LoadTexture (filepath.c_str ()));
	}

	//int Index = (1 * m_nTileWidth) + 1;

	m_pTiles		= new CTile*[m_nMapHeight];

	for (int index = 0; index < m_nMapHeight; index++)
	{
		m_pTiles[index] = new CTile[m_nMapWidth];
	}

	TiXmlElement* pTiles = pRoot->FirstChildElement ("Tiles");
	int i = 0;

	if (pTiles)
	{
		TiXmlElement* pTile = pTiles->FirstChildElement ("Tile");
		

		int xpos = 0, ypos = 0;
		while (pTile)
		{
			CTile t;

			pTile->Attribute ("PickedY", &ypos);
			pTile->Attribute ("PickedX", &xpos);

			t.SetXPicked (xpos);
			t.SetYPicked (ypos);

			pTile->Attribute ("YPos", &ypos);
			pTile->Attribute ("XPos", &xpos);

			t.SetXPos (xpos);
			t.SetYPos (ypos);

			int type;
			pTile->Attribute ("Type", &type);

			t.SetType (type);

			m_pTiles[ypos][xpos] = t;

			pTile = pTile->NextSiblingElement ();
			i++;
		}
	}

	return true;
}

bool CMap::LoadEvents (const char* filename)
{
	TiXmlDocument doc;
	std::string path = "Resource/Data/";
	char buffer[32];
	sprintf_s(buffer,32, "level%i/", CLevelSelectionState::GetInstance()->GetSelection() + 1);
	path += buffer;

	std::string filepath = path + filename;

	if (doc.LoadFile (filepath.c_str()) == false)
		return false;

	TiXmlElement* pRoot = doc.RootElement ();

	if (!pRoot)
		return false;

	m_pEvents		= new CTile*[m_nMapHeight];

	for (int index = 0; index < m_nMapHeight; index++)
	{
		m_pEvents[index] = new CTile[m_nMapWidth];
	}
	

		TiXmlElement* pTile = pRoot->FirstChildElement ("Event");

		int xpos = 0, ypos = 0;

		while (pTile)
		{
			CTile c;

			pTile->Attribute ("YPos", &ypos);
			pTile->Attribute ("XPos", &xpos);

			c.SetXPos (xpos);
			c.SetYPos (ypos);

			int type;
			pTile->Attribute ("Type", &type);

			c.SetType (type);

			if (type != -1)
			{
				c.SetName (pTile->GetText ());
			}

			m_pEvents[ypos][xpos] = c;

			pTile = pTile->NextSiblingElement ();
		}

	return true;
}

RECT CMap::GetCollisionRect (int XPos, int YPos)
{
	RECT temp = {(m_pEvents[YPos][XPos].GetXPos() * GetPixelWidth()) , (m_pEvents[YPos][XPos].GetYPos()) * GetPixelHeight(), 
		(m_pEvents[YPos][XPos].GetXPos() * GetPixelWidth()) + m_nPixelWidth, (m_pEvents[YPos][XPos].GetYPos() * GetPixelHeight()) + m_nPixelHeight};

	return temp;
}
