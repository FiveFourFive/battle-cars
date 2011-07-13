#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "CCar.h"
#include "CEventSystem.h"
#include "IListener.h"

class CXboxInput;
class CMessageSystem;
class CHUD;


class CPlayer : public CCar
{
private:
	CCamera* m_pCamera;
	CHUD* m_pHUD;
	CEventSystem* m_pES;
	CXboxInput* m_pController1;
	float m_fFireTimer;
	bool m_bIsFlameThrowerOn;
	bool m_bIsIcyGatlingOn;
	float m_fIcyBullets;
	float m_fFlames;
	RECT m_CollisionRECT;
	bool Collision_effect;			//Check whether effect has been applied yet or not.

	int m_nPlayerNum;

public:
	CPlayer(CXboxInput* pController);
	~CPlayer(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	void SetController(CXboxInput* pController) { m_pController1 = pController; }
	CXboxInput* GetController(void) {return m_pController1; }
	void SetCollisionRect (RECT collision) {m_CollisionRECT = collision;}
	RECT GetCollisionRect () { return m_CollisionRECT;}

	CCamera* GetCamera(void) { return m_pCamera; }

	int GetPlayerNum(void) { return m_nPlayerNum; }
	void SetPlayerNum(int num) { m_nPlayerNum = num; }


	bool CheckCollision(IBaseInterface* pBase);
	bool IsFlameThrowerOn() {return m_bIsFlameThrowerOn;}
	void SetFlameThrower(bool flame) {m_bIsFlameThrowerOn = flame;}
	void HandleEvent(CEvent* pEvent);
};




#endif