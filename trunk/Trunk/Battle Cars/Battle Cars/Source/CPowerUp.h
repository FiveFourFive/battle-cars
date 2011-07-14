#ifndef POWERUP_H_
#define POWERUP_H_

#include "CBase.h"

enum PowerUp_Type{ SHIELD_POWERUP, HEALTH_POWERUP, WEAPONS_POWERUP, SPECIAL_POWERUP, MAX_POWERUPS};

class PowerUp : public CBase
{
private:
	float m_fRespawn;
	float m_fRespawnDelay;
	bool m_bActive;
	int m_nPtype;
public:
	PowerUp();
	~PowerUp();


	virtual void Update(float fElapsedTime);
	virtual void Render(CCamera* camera);

	virtual RECT GetRect();

	virtual bool CheckCollision(IBaseInterface* pBase);

	void SetPowerType(int type) { m_nPtype = type; }
};

#endif