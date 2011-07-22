#ifndef CCOLLECTSTATE_H_
#define CCOLLECTSTATE_H_


#include "IAIBaseState.h"
#include <vector>
using namespace std;
class CCollectable;

class CCollectState : public IAIBaseState
{
private:
	bool m_bHasTargets;
	vector<CCollectable*> collectables;
	int m_nTarget;
	CEnemy* m_Owner;

	CCollectState(const CCollectState&);
	CCollectState& operator=(const CCollectState&);
	
public:
	
	CCollectState(){};
	~CCollectState(){};

	void Update(float fElapsedTime);
	void Render();
	void Enter();
	void Exit();

	void SetOwner(CEnemy* owner) {m_Owner = owner;}
	void SetHasTargets(bool target) {m_bHasTargets = target;}
	void SetCollectables(vector<CCollectable*> col) {collectables = col;}
	void SetTarget(int target) {m_nTarget = target;}

	bool HasTargets() {return m_bHasTargets;}
	vector<CCollectable*> GetCollectables() {return collectables;}
	int GetTarget() {return m_nTarget;}
	CEnemy* GetOwner() {return m_Owner;}

};
#endif