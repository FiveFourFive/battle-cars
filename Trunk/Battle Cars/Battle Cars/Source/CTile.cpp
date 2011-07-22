/////////////////////////////////////////////////
//	File	:	"CTile.cpp"
//
//	Author	:	Josh Fields
//
//	Purpose	:	 Defines what the tiles are.
//				
/////////////////////////////////////////////////

#include "CTile.h"

CTile::CTile ()
{
	SetXPos (0);
	SetYPos (0);
	SetXPicked (-1);
	SetYPicked (-1);
	SetName ("NoName");
	SetType (-1);
	SetInUse (false);
	SetID (-1);
}

CTile::CTile (int XPos, int YPos, int XPicked, int YPicked, int id, const char* Name, int type)
{
	SetXPos (XPos);
	SetYPos (YPos);
	SetXPicked (XPicked);
	SetYPicked (YPicked);
	SetName (Name);
	SetType (type);
	SetID (id);
}

CTile::~CTile()
{

}
