/////////////////////////////////////////////////
//	File	:	"CMap.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains all the information 
//				for making a level.
//				
/////////////////////////////////////////////////

#ifndef _CMAP_H_
#define _CMAP_H_

#include <Windows.h>
#include <string>

class CTile;
class CCollisions;
class CEvents;

class CMap
{
private:
	CTile*	m_pTiles;
	CTile*	m_pCollisions;
	CTile*	m_pEvents;
	CTile*	m_pSpawns;

	int m_nMapWidth; // array width of the map
	int m_nMapHeight; // array height of the map
	int m_nTileWidth; // array width of the tiles
	int m_nTileHeight; // array height of the tiles
	int m_nPixelWidth;
	int m_nPixelHeight;
	int m_nTileImageID; // Tile map Texture

	std::string m_stMapName;

public:
	CMap ();
	~CMap ();

	bool LoadMap (const char* filename);
	bool LoadTiles (const char* filename);
	bool LoadCollisions (const char* filename);
	bool LoadSpawns (const char* filename);
	bool LoadEvents (const char* filename);

	//Accessors
	CTile* GetTileList () {return m_pTiles;}
	CTile* GetCollisionList () {return m_pCollisions;}
	CTile* GetEventsList () {return m_pEvents;}
	CTile* GetSpawnList () {return m_pSpawns;}
	int GetMapWidth () {return m_nMapWidth;}
	int GetMapHeight () {return m_nMapHeight;}
	int GetTileWidth () {return m_nTileWidth;}
	int GetTileHeight () {return m_nTileHeight;}
	int GetPixelWidth () {return m_nPixelWidth;}
	int GetPixelHeight () {return m_nPixelHeight;}
	int GetTileImageID () {return m_nTileImageID;}
	int GetTotalIndexSize () {return (m_nMapHeight  * m_nMapWidth);}
	const char* GetMapName () {return m_stMapName.c_str();}

	//Mutators
	void SetTileList (CTile* TileList) {m_pTiles = TileList;}
	void SetCollisionList (CTile* CollisionList) {m_pCollisions = CollisionList;}
	void SetEventList (CTile* EventsList) {m_pEvents = EventsList;}
	void SetSpawnList (CTile* SpawnList) {m_pSpawns = SpawnList;}
	void SetMapWidth (int Width) {m_nMapWidth = Width;}
	void SetMapHeight (int Height) {m_nMapHeight = Height;}
	void SetPixelWidth (int Width) {m_nPixelWidth = Width;}
	void SetPixelHeight (int Height) {m_nPixelHeight = Height;}
	void SetTileWidth (int Width) {m_nTileWidth = Width;}
	void SetTileHeight (int Height) {m_nTileHeight = Height;}
	void SetTileImageID (int ID) {m_nTileImageID = ID;}
	void SetMapName (const char* Name) {m_stMapName = Name;}

	RECT GetCollisionRect (int IndexID);
};

#endif