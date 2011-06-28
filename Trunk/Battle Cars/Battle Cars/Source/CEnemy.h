/////////////////////////////////////////////////
//	File	:	"CEnemy.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains all functions needed for
//				a simple AI
//				
/////////////////////////////////////////////////

#ifndef _CENEMY_H
#define _CENEMY_H

#include <vector>

#include "IAIBaseState.h"
#include "CCar.h"
#include "IListener.h"
#include "CEventSystem.h"
#include "SGD_Math.h"
//#include "CMessageSystem.h"

class CCamera;

class CEnemy : public CCar, public IListener
{
private:
	IAIBaseState*  m_AICurrentState;

	CEventSystem*	m_pES;

	std::vector<RECT> m_CollisionRECT;

	float m_fViewRadius;

public:
	CEnemy();
	~CEnemy();


	float GetViewRadius () {return m_fViewRadius;}
	void SetViewRadius (float radius) {m_fViewRadius = radius;}

	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	void ChangeState (IAIBaseState* state);

	void EnterState ();

	void AddCollisionRect (RECT collision);
	std::vector<RECT> GetCollisionRects () { return m_CollisionRECT;}

	void HandleEvent(CEvent* pEvent);
	bool CheckCollision(IBaseInterface* pBase);
};



#endif