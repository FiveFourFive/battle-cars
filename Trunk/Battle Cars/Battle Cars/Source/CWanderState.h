/////////////////////////////////////////////////
//	File	:	"IAIBaseState.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Pure virtual class which all AI
//				states inheriet from
//				
/////////////////////////////////////////////////

#ifndef _CWANDERSTATE_H_
#define _CWANDERSTATE_H_

//#include <Windows.h>
#include "IAIBaseState.h"
#include "SGD_Math.h"
#include "CPowerUp.h"
#include "CSpeedRamp.h"
class CEnemy;
class CPlayer;
class CCar;

class CWanderState : public IAIBaseState
{
private:
	/*tVector2D m_vPredictMove1;
	tVector2D m_vPredictMove2;
	tVector2D m_vPredictMove3;
	tVector2D m_vPredictMove4;*/

	//RECT m_rtPredictMove;

	//tVector2D m_vPredictCircle;
	//float	m_fPredictRadius;

	//int m_nThreatDistance;

	/*tVector2D m_vVelocity;
	tVector2D m_vTurn;*/

	//////float m_fMainCircleRadius;
	//////float m_fNewPointRadius;

	//////int m_nCounter;
	//////tVector2D m_vDirectionCenter;
	//////tVector2D m_vMainCenter;

	bool m_bHasTargets;
	float m_fTargetX;
	float m_fTargetY;
	float m_fRotationAngle;
	float m_fturnLeftOrRight;
	tVector2D m_vTargetWanderDirection;
	CEnemy* m_Owner;
	CCar* m_Target1;
	CCar* m_Target2;
	PowerUp* m_PowerUpTarget;
	CSpeedRamp* m_SpeedRampTarget;
	float m_fAggroRadius;
	void Wander(float fElapsedTime);
	bool FindThreat ();
	bool FindPowerUps(float fElapsedTime);
	void GrabPowerUp(float fElapsedTime);
	void FindSpeedRamp(float fElapsedTime);
	void UseSpeedRamp(float fElapsedTime);

	
	CWanderState(const CWanderState&);
	CWanderState& operator = (const CWanderState&);

public:
	CWanderState(void){};
	~CWanderState(void){};

	void Update (float fElapsedTime);
	void Render ();
	void Enter ();
	void Exit ();
	void SetOwner (CEnemy* owner) {m_Owner = owner;}
	void SetTarget1(CCar* pTarget) {m_Target1 = pTarget;}
	void SetTarget2(CCar* pTarget) {m_Target2 = pTarget;}
	CCar* GetTarget1() {return m_Target1;}
	CCar* GetTarget2() {return m_Target2;}
	void SetHasTarget (bool target) {m_bHasTargets = target;}
	
};

#endif