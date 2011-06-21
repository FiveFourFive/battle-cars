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
	//SetWidth (0);
	//SetHeight (0);
	SetIndex (-1);
	SetName ("NoName");
	SetType (-1);
	SetInUse (false);
}

CTile::CTile (int XPos, int YPos, int Width, int Height, int Index, const char* Name, int type)
{
	SetXPos (XPos);
	SetYPos (YPos);
	//SetWidth (Width);
	//SetHeight (Height);
	SetIndex (Index);
	SetName (Name);
	SetType (type);
}

CTile::~CTile()
{

}
