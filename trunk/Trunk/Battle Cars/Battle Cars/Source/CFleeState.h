/////////////////////////////////////////////////
//	File	:	"CFleeState.h"
//
//	Author	:	John Rostick
//
//	Purpose	:	Enemy Flee State declarations
//				
//				
/////////////////////////////////////////////////

#ifndef _CFLEESTATE_H_
#define _CFLEESTATE_H_

//#include <Windows.h>
#include "IAIBaseState.h"
#include "SGD_Math.h"
class CEnemy;
class CPlayer;

class CFleeState : public IAIBaseState
{
private:
	// member variables
	CEnemy* m_Owner;
	CPlayer* m_Target;
	bool Damaged(void);
	void Flee(float fElapsedTime);
	bool Escaped();

	CFleeState(void){};
	~CFleeState(void){};
	CFleeState(const CFleeState&);
	CFleeState& operator = (const CFleeState&);

public:
	static CFleeState* GetInstance ();

	void Update (float fElapsedTime);
	void Render ();
	void Enter ();
	void Exit ();

	//Accessors
	CEnemy* GetOwner() {return m_Owner;}
	CPlayer* GetTarget() {return m_Target;}
	//Mutators
	void SetOwner(CEnemy* pOwner) {m_Owner = pOwner;}
	void SetTarget(CPlayer* pTarget) {m_Target = pTarget;}
};

#endif