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
#include "CPowerUp.h"
#include <vector>
using namespace std;
class CEnemy;
class CCar;

class CFleeState : public IAIBaseState
{
private:
	// member variables
	CEnemy* m_Owner;
	CCar* m_Target;
	bool m_bFindingHealth;

	bool Damaged(void);
	void Flee(float fElapsedTime);
	void FindHealth(float fElapsedTime);
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
	CCar* GetTarget() {return m_Target;}
	bool IsGettingHealth() {return m_bFindingHealth;}
	//Mutators
	void SetOwner(CEnemy* pOwner) {m_Owner = pOwner;}
	void SetTarget(CCar* pTarget) {m_Target = pTarget;}
	void SetFindingHealth(bool health) {m_bFindingHealth = health;}
};

#endif