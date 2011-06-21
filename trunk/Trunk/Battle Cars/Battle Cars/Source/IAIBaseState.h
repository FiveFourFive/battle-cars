/////////////////////////////////////////////////
//	File	:	"IAIBaseState.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Pure virtual class which all AI
//				states inheriet from
//				
/////////////////////////////////////////////////

#ifndef _IAIBASESTATE_H_
#define _IAIBASESTATE_H_

class CEnemy;

class IAIBaseState
{
private:

public:

	virtual void Update (float fElapsedTime) = 0;
	virtual void Enter () = 0;
	virtual void Exit () = 0;
	virtual void SetOwner (CEnemy* owner) = 0;
};

#endif