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
	
	delete[] m_pTiles;
	delete[] m_pCollisions;
	delete[] m_pEvents;
	delete[] m_pSpawns;

	m_pTiles = NULL;
	m_pCollisions = NULL;
	m_pEvents = NULL;
	m_pSpawns = NULL;
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

	m_pTiles		= new CTile[m_nMapHeight * m_nMapWidth];
	
	TiXmlElement* pTiles = pRoot->FirstChildElement ("Tiles");
	int i = 0;

	if (pTiles)
	{
		TiXmlElement* pTile = pTiles->FirstChildElement ("Tile");
		

		while (pTile)
		{
			CTile t;

			pTile->Attribute ("PickedY", &height);
			pTile->Attribute ("PickedX", &width);

			t.SetXPos (width);
			t.SetYPos (height);

			pTile->Attribute ("YPos", &height);
			pTile->Attribute ("XPos", &width);

			t.SetIndex ((height * m_nMapWidth) + width);

			int type;
			pTile->Attribute ("Type", &type);

			t.SetType (type);

			m_pTiles[t.GetIndex ()] = t;

			pTile = pTile->NextSiblingElement ();
			i++;
		}
	}

	return true;
}

bool CMap::LoadCollisions (const char* filename)
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

	m_pCollisions	= new CTile[m_nMapHeight * m_nMapWidth];
	

		TiXmlElement* pTile = pRoot->FirstChildElement ("Collision");

		int width, height;

		while (pTile)
		{
			CTile c;

			pTile->Attribute ("YPos", &height);
			pTile->Attribute ("XPos", &width);

			c.SetIndex ((height * m_nMapWidth) + width);

			int type;
			pTile->Attribute ("Type", &type);

			c.SetType (type);

			if (type != -1)
			{
				c.SetName (pTile->GetText ());
			}

			m_pCollisions[c.GetIndex ()] = c;

			pTile = pTile->NextSiblingElement ();
		}

	return true;
}

bool CMap::LoadSpawns (const char* filename)
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

	m_pSpawns		= new CTile[m_nMapHeight * m_nMapWidth];
	

		TiXmlElement* pTile = pRoot->FirstChildElement ("Spawn");

		int width, height;

		while (pTile)
		{
			CTile c;

			pTile->Attribute ("YPos", &height);
			pTile->Attribute ("XPos", &width);

			c.SetIndex ((height * m_nMapWidth) + width);

			int type;
			pTile->Attribute ("Type", &type);

			c.SetType (type);

			if (type != -1)
			{
				c.SetName (pTile->GetText ());
			}

			m_pSpawns[c.GetIndex ()] = c;

			pTile = pTile->NextSiblingElement ();
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

	m_pEvents		= new CTile[m_nMapHeight * m_nMapWidth];
	

		TiXmlElement* pTile = pRoot->FirstChildElement ("Spawn");

		int width, height;

		while (pTile)
		{
			CTile c;

			pTile->Attribute ("YPos", &height);
			pTile->Attribute ("XPos", &width);

			c.SetIndex ((height * m_nMapWidth) + width);

			int type;
			pTile->Attribute ("Type", &type);

			c.SetType (type);

			if (type != -1)
			{
				c.SetName (pTile->GetText ());
			}

			m_pSpawns[c.GetIndex ()] = c;

			pTile = pTile->NextSiblingElement ();
		}

	return true;
}

RECT CMap::GetCollisionRect (int IndexID)
{
	////////////fix so its using the indes
	RECT temp = {(m_pCollisions[IndexID].GetIndex() % GetMapWidth()) * GetPixelWidth() , (m_pCollisions[IndexID].GetIndex() / GetMapHeight()) * GetPixelHeight(), 
		(m_pCollisions[IndexID].GetIndex() % GetMapWidth()) * GetPixelWidth() + m_nPixelWidth, (m_pCollisions[IndexID].GetIndex() / GetMapHeight()) * GetPixelHeight() + m_nPixelHeight};

	return temp;
}
