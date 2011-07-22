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
	int m_nXPos; 
	int m_nYPos; 
	int m_nXPicked;// used for finding image on the tile sheet
	int m_nYPicked;// used for finding image on the tile sheet
	bool m_bInUse;

	int m_nType;  // this tells what type of tile it is (either spawn for players, power ups, if its a collision tile ......)

	std::string  m_stName; // for spawns, collisions and events

	int m_nID;

public:
	CTile ();
	CTile (int XPos, int YPos, int XPicked, int YPicked, int id, const char* Name, int type);
	~CTile();

	//Accessors
	int GetXPos () {return m_nXPos; }
	int GetYPos () {return m_nYPos; }
	int GetXPicked () {return m_nXPicked; }
	int GetYPicked () {return m_nYPicked; }
	int GetID () {return m_nID;}

	const char* GetName () {return m_stName.c_str(); }
	int GetType() {return m_nType;}
	bool InUse () {return m_bInUse;}

	//Mutators
	void SetXPos (int XPos) {m_nXPos = XPos;}
	void SetYPos (int YPos) {m_nYPos = YPos;}
	void SetXPicked (int XPos) {m_nXPicked = XPos;}
	void SetYPicked (int YPos) {m_nYPicked = YPos;}
	void SetName (const char* Name) {m_stName = Name;}
	void SetType (int type) {m_nType = type;}
	void SetInUse (bool inUse) {m_bInUse = inUse;}
	void SetID (int id) {m_nID = id;}

};

#endif