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
#include "CWanderState.h"
#include "CFleeState.h"
#include "CAttackState.h"
#include "CCollectState.h"

#include "CPlayer.h"
#include "IListener.h"
#include "CEventSystem.h"
#include "SGD_Math.h"
#include "CPowerUp.h"
#include "CSpeedRamp.h"
//#include "CMessageSystem.h"

class CCamera;

class CEnemy : public CPlayer
{
private:
	IAIBaseState*  m_AICurrentState;
	CWanderState* m_WanderState;
	CFleeState*   m_FleeState;
	CAttackState* m_AttackState;
	CCollectState* m_CollectState;

	CEventSystem*	m_pES;
	std::vector<RECT> m_CollisionRECT;
	std::vector<PowerUp*> powerUps;
	std::vector<CSpeedRamp*> speedRamps;
	float m_fViewRadius;
	bool m_bHasCollidedWithSpeedRamp;
	float m_fSpeedRampTimer;

public:
	CEnemy(CXboxInput* pController);
	~CEnemy();

	//Accessors
	float GetViewRadius () {return m_fViewRadius;}
	std::vector<RECT> GetCollisionRects () { return m_CollisionRECT;}
	std::vector<PowerUp*> GetPowerUps() {return powerUps;}
	std::vector<CSpeedRamp*> GetSpeedRamps() {return speedRamps;}
	bool HasCollidedWithSpeedRamp() {return m_bHasCollidedWithSpeedRamp;}
	float GetSpeedRampTimer() {return m_fSpeedRampTimer;}
	IAIBaseState* GetCurrentState() {return m_AICurrentState;}
	CEventSystem* GetEventSystem() {return m_pES;}
	CWanderState* GetWanderState () {return m_WanderState;}
	CFleeState* GetFleeState () {return m_FleeState;}
	CAttackState* GetAttackState () {return m_AttackState;}
	CCollectState* GetCollectState () {return m_CollectState;}

	//Mutators
	void SetPowerUps(std::vector<PowerUp*> powerUp) {powerUps = powerUp;}
	void SetSpeedRamps(std::vector<CSpeedRamp*> ramps) {speedRamps = ramps;}
	void SetCollisionRects(std::vector<RECT> rects) {m_CollisionRECT = rects;}
	void SetCollidedWithSpeedRamp(bool collide) {m_bHasCollidedWithSpeedRamp = collide;}
	void SetSpeedRampTimer(float timer) {m_fSpeedRampTimer = timer;}
	void SetViewRadius (float radius) {m_fViewRadius = radius;}
	void SetCurrentState(IAIBaseState* state) {m_AICurrentState = state;}
	void SetEventSystem(CEventSystem* pES) {m_pES = pES;}

	void Update(float fElapsedTime);
	void Render(CCamera* camera);
	void ChangeState (IAIBaseState* state);
	void EnterState ();
	void AddCollisionRect (RECT collision);
	void HandleEvent(CEvent* pEvent);
	bool CheckCollision(IBaseInterface* pBase);

};



#endif