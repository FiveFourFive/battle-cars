/////////////////////////////////////////////////
//	File	:	"CAttackState.h"
//
//	Author	:	John Rostick
//
//	Purpose	:	Enemy Attack State declarations
//				
//				
/////////////////////////////////////////////////

#ifndef _CATTACKSTATE_H_
#define _CATTACKSTATE_H_

//#include <Windows.h>
#include "IAIBaseState.h"
#include "SGD_Math.h"
class CEnemy;
class CCar;

class CAttackState : public IAIBaseState
{
private:
	// member variables
	CEnemy* m_Owner;
	CCar* m_Target;
	float m_fAggroRadius;
	float m_fFireRate;
	float m_fFireTimer;
	void Chase(float fElapsedTime);
	bool StillThreat(void);
	bool Damaged(void);

	CAttackState(void){};
	~CAttackState(void){};
	CAttackState(const CAttackState&);
	CAttackState& operator = (const CAttackState&);

public:
	static CAttackState* GetInstance ();

	void Update (float fElapsedTime);
	void Render ();
	void Enter ();
	void Exit ();

	//Accessors
	CEnemy* GetOwner() {return m_Owner;}
	CCar* GetTarget() {return m_Target;}
	//Mutators
	void SetOwner(CEnemy* pOwner) {m_Owner = pOwner;}
	void SetTarget(CCar* pTarget) {m_Target = pTarget;}
};

#endif