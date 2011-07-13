#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "CCar.h"
#include "CEventSystem.h"
#include "IListener.h"

class CXboxInput;
class CMessageSystem;
class CHUD;

enum characters{CAR_MINI, CAR_VETTE, CAR_HUMMER, CAR_TRUCK, CAR_NUM};
class CPlayer : public CCar
{
private:
	CCamera* m_pCamera;
	CHUD* m_pHUD;
	CEventSystem* m_pES;
	CXboxInput* m_pController1;
	float m_fFireTimer;
	int m_nPlayerType;
	RECT m_CollisionRECT;

	int m_nPlayerNum;

public:
	CPlayer(CXboxInput* pController);
	~CPlayer(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	void SetController(CXboxInput* pController) { m_pController1 = pController; }
	CXboxInput* GetController(void) { return m_pController1; }
	void SetCollisionRect (RECT collision) {m_CollisionRECT = collision;}
	RECT GetCollisionRect () { return m_CollisionRECT;}

	CCamera* GetCamera(void) { return m_pCamera; }

	int GetPlayerNum(void) { return m_nPlayerNum; }
	void SetPlayerNum(int num) { m_nPlayerNum = num; }


	bool CheckCollision(IBaseInterface* pBase);
	int GetPlayerType() {return m_nPlayerType;}
	void SetPlayerType(int type) {m_nPlayerType = type;}
	void HandleEvent(CEvent* pEvent);
};




#endif