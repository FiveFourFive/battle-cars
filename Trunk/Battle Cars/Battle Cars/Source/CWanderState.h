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

#include <Windows.h>
#include "IAIBaseState.h"
#include "SGD_Math.h"
class CEnemy;


class CWanderState : public IAIBaseState
{
private:
	/*tVector2D m_vPredictMove1;
	tVector2D m_vPredictMove2;
	tVector2D m_vPredictMove3;
	tVector2D m_vPredictMove4;*/

	//RECT m_rtPredictMove;

	tVector2D m_vPredictCircle;
	float	m_fPredictRadius;

	int m_nThreatDistance;

	/*tVector2D m_vVelocity;
	tVector2D m_vTurn;*/

	CEnemy* m_Owner;
public:

	CWanderState();
	~CWanderState();

	void Update (float fElapsedTime);
	void Enter ();
	void Exit ();
	void SetOwner (CEnemy* owner) {m_Owner = owner;}
	bool FindThreat ();
};

#endif