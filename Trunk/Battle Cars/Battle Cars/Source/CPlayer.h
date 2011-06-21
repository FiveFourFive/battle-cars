#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "CCar.h"
#include "CEventSystem.h"
#include "IListener.h"

class CXboxInput;
class CCamera;
class CMessageSystem;

class CPlayer : public CCar, public IListener
{
private:
	CCamera* m_pCamera;

	CEventSystem* m_pES;
	CXboxInput* m_pController1;



	float m_fCollisionDelay;
	int m_nBulletImageID;
	int m_nFirstWeaponID;
	int m_nSecondWeaponID;
	int m_nSpecialWeaponID;

	RECT m_CollisionRECT;

public:
	CPlayer(void);
	~CPlayer(void);
	void Update(float fElapsedTime);
	void Render(void);

	void SetCollisionRect (RECT collision) {m_CollisionRECT = collision;}
	RECT GetCollisionRect () { return m_CollisionRECT;}

	CCamera* GetCamera(void) { return m_pCamera; }

	int GetBulletImageID(void) { return m_nBulletImageID; }

	bool CheckCollision(IBaseInterface* pBase);

	void HandleEvent(CEvent* pEvent);
};




#endif