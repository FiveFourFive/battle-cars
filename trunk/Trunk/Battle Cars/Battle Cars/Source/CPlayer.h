#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "CCar.h"
#include "CEventSystem.h"
#include "IListener.h"

class CXboxInput;
class CMessageSystem;

class CPlayer : public CCar, public IListener
{
private:
	CCamera* m_pCamera;

	CEventSystem* m_pES;
	CXboxInput* m_pController1;





	RECT m_CollisionRECT;

public:
	CPlayer(CXboxInput* pController);
	~CPlayer(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	void SetController(CXboxInput* pController) { m_pController1 = pController; }
	void SetCollisionRect (RECT collision) {m_CollisionRECT = collision;}
	RECT GetCollisionRect () { return m_CollisionRECT;}

	CCamera* GetCamera(void) { return m_pCamera; }



	bool CheckCollision(IBaseInterface* pBase);

	void HandleEvent(CEvent* pEvent);
};




#endif