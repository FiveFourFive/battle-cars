#ifndef CBOSS_H_
#define CBOSS_H_

#include "CEnemy.h"
class CCamera;

class CBoss : public CEnemy
{
private:
	CPlayer* m_Player1;
	CPlayer* m_Player2;
	bool m_bHasTarget;
	int m_nTarget;
	float m_fFireTimer;
	bool m_bGatlingIsOn;
	int m_nIceBullets;
	bool m_bFlameThrowerIsOn;
	int m_nFireBullets;
	float m_fIceTimer;
	bool m_bIsEnraged;
	bool m_bIsMiniBoss;

	void SelectTarget();
	void ChaseTarget(float fElapsedTime);
	void FireAtTarget(float fElapsedTime);

public:
	CBoss(CXboxInput* pController);
	~CBoss();

	//Accessors
	CPlayer* GetPlayer1() {return m_Player1;}
	CPlayer* GetPlayer2() {return m_Player2;}
	bool HasTargets() {return m_bHasTarget;}
	int GetTarget() {return m_nTarget;}
	float GetFireTimer() {return m_fFireTimer;}
	bool IsMiniBoss() {return m_bIsMiniBoss;}

	//Mutators
	void SetPlayer1(CPlayer* player) {m_Player1 = player;}
	void SetPlayer2(CPlayer* player) {m_Player2 = player;}
	void SetHasTargets(bool targets) {m_bHasTarget = targets;}
	void SetTarget(int target) {m_nTarget = target;}
	void SetFireTimer(float timer) {m_fFireTimer = timer;}
	void SetMiniBoss(bool boss) {m_bIsMiniBoss = boss;}

	void Update(float fElapsedTime);
	void Render(CCamera* camera);
	void AddCollisionRect (RECT collision);
	void HandleEvent(CEvent* pEvent);
	bool CheckCollision(IBaseInterface* pBase);

};
#endif