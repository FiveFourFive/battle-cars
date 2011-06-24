/////////////////////////////////////////////////////////
//	File Name : "IBaseInterface.h"
//	Author Name : James Brown
//	Purpose : Interface
/////////////////////////////////////////////////////////

#ifndef _IBASEINTERFACE
#define _IBASEINTERFACE

#include <Windows.h>

class CCamera;

class IBaseInterface
{
public:
	virtual ~IBaseInterface() = 0 {};

	virtual void Update(float fElapsedTime) = 0;
	virtual void Render(CCamera* camera) = 0;

	virtual void AddRef() = 0;
	virtual void Release() = 0;

	virtual int GetType() = 0;
	
	virtual RECT GetRect() = 0;

	virtual bool CheckCollision(IBaseInterface* pBase) = 0;
};

#endif