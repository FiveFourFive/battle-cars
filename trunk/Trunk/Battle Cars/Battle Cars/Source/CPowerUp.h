#ifndef POWERUP_H_
#define POWERUP_H_

#include "CBase.h"

enum PowerUp_Type{ SHIELD_POWERUP, HEALTH_POWERUP, WEAPONS_POWERUP, SPECIAL_POWERUP, MAX_POWERUPS};

class PowerUp : public CBase
{
private:


public:
	PowerUp();
	~PowerUp();


	virtual void Update(float fElapsedTime);
	virtual void Render();

	virtual RECT GetRect();

	virtual bool CheckCollision(IBaseInterface* pBase);
};

#endif