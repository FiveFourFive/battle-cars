/////////////////////////////////////////////////
//	File	:	"CTile.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	 Defines what the tiles are.
//				
/////////////////////////////////////////////////
#ifndef _CTILE_H_
#define _CTILE_H_

#include <string>

class CTile
{
private:
	int m_nXPos; // used for finding image on the tile sheet
	int m_nYPos; // used for finding image on the tile sheet
	//int m_nWidth; 
	//int m_nHeight;
	int m_nTileIndex; // Index on the Map
	bool m_bInUse;

	int m_nType;  // this tells what type of tile it is (either spawn for players, power ups, if its a collision tile ......)

	std::string  m_stName; // for spawns, collisions and events

public:
	CTile ();
	CTile (int XPos, int YPos, int Width, int Height, int Index, const char* Name, int type);
	~CTile();

	//Accessors
	int GetXPos () {return m_nXPos; }
	int GetYPos () {return m_nYPos; }
	//int GetWidth () {return m_nWidth; }
	//int GetHeight () {return m_nHeight; }
	int GetIndex () {return m_nTileIndex; }
	const char* GetName () {return m_stName.c_str(); }
	int GetType() {return m_nType;}
	bool InUse () {return m_bInUse;}

	//Mutators
	void SetXPos (int XPos) {m_nXPos = XPos;}
	void SetYPos (int YPos) {m_nYPos = YPos;}
	//void SetWidth (int Width) {m_nWidth = Width;}
	//void SetHeight (int Height) {m_nHeight = Height;}
	void SetIndex (int Index) {m_nTileIndex = Index;}
	void SetName (const char* Name) {m_stName = Name;}
	void SetType (int type) {m_nType = type;}
	void SetInUse (bool inUse) {m_bInUse = inUse;}

};

#endif