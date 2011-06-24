#ifndef _CCAR_H
#define _CCAR_H

#include "CBase.h"
#include "SGD_Math.h"

class CCamera;

enum weapons { WEAPON_PISTOL, WEAPON_RPG, WEAPON_SPECIAL };
class CCar : public CBase
{

private:
	float m_fVelX;
	float m_fVelY;
	float m_fRotation;
	float m_fRotationRate;
	float m_fAccelerationRate;
	float m_fSpeed;
	float m_nHealth;
	float m_nMaxHealth;
	float m_fPowerUpBar;
	float m_fShieldBar;
	float m_fMaxShield;
	float m_fMaxPowerUp;
	float m_fMaxSpeed;
	int m_nCrashID;
	int m_nKillCount;
	float m_fFireDelay;
	int m_nBulletSound;
	int m_nSelectedWeapon;
	tVector2D m_tDirection;
	tVector2D m_tVelocity;
public:
	CCar(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);


	//getters
	tVector2D GetDirection(void) { return m_tDirection; }
	tVector2D GetVelocity(void) { return m_tVelocity; }
	float GetVelX(void)	{return m_fVelX;}
	float GetVelY(void)	{return m_fVelY;}
	float GetHealth(void) { return m_nHealth;}
	float GetMaxHealth(void) { return m_nMaxHealth; }
	float GetSpeed(void) { return m_fSpeed; }
	float GetAcceleration(void) { return m_fAccelerationRate; }
	float GetRotationRate(void) { return m_fRotationRate; }
	float GetRotation(void) { return m_fRotation; }
	float GetPowerUpBar(void) { return m_fPowerUpBar; }
	float GetShieldBar(void) { return m_fShieldBar; }
	float GetMaxPowerUp(void) { return m_fMaxPowerUp; }
	float GetMaxShield(void) { return m_fMaxShield; }
	float GetMaxSpeed(void) { return m_fMaxSpeed; }
	int GetKillCount(void) { return m_nKillCount; }
	float GetFireDelay(void) { return m_fFireDelay; }
	int GetSelectedWeapon(void) { return m_nSelectedWeapon; }
	//setters
	void SetPowerUpBar(float pbar) { m_fPowerUpBar = pbar; }
	void SetShieldBar(float sbar) { m_fShieldBar = sbar; }
	void SetMaxPowerUp(float max) { m_fMaxPowerUp = max; }
	void SetMaxShield(float max) { m_fMaxShield = max; }
	void SetVelX(float nVelX) {m_fVelX = nVelX;}
	void SetVelY(float nVelY) {m_fVelY = nVelY;}
	void SetHealth(float health) { m_nHealth = health;}
	void SetMaxHealth(float maxhp) { m_nMaxHealth = maxhp; }
	void SetSpeed(float speed) { m_fSpeed = speed; }
	void SetAcceleration(float acc) { m_fAccelerationRate = acc; }
	void SetRotation(float rot) { m_fRotation = rot; }
	void SetRotationRate(float rotrate) { m_fRotationRate = rotrate; }
	void SetDirection(tVector2D vec) { m_tDirection = vec; }
	void SetVelocity(tVector2D vec) { m_tVelocity = vec; }
	void SetMaxSpeed(float maxspeed) { m_fMaxSpeed = maxspeed; }
	void SetKillCount(int killcount) { m_nKillCount = killcount; }
	void SetFireDelay(float delay) { m_fFireDelay = delay; }
	void IncrementWeapon(void) { m_nSelectedWeapon++; }
	void DecrementWeapon(void) { m_nSelectedWeapon--; }
	void SetSelectedWeapon(int wepnum) { m_nSelectedWeapon = wepnum; }
	// plays the crash sound
	void PlayCrash(void);
	void PlayBullet(void);

	// checks to see if the car is within the camera (within screen limits atm)
	bool InBounds(void);
	bool CheckCollision(IBaseInterface* pBase);

};




#endif