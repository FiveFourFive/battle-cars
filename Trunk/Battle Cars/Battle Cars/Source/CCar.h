#ifndef _CCAR_H
#define _CCAR_H

#include "CBase.h"
#include "SGD_Math.h"

class CCamera;

enum weapons { WEAPON_PISTOL, WEAPON_RPG, WEAPON_SPECIAL };
class CCar : public CBase
{

private:
	float m_fVelX;					// overall velocity in the x direction
	float m_fVelY;					// overall velocity in the y direction
	float m_nCollisionX1;				// center of first collision circle
	float m_nCollisionY1;				// -------------------------------
	float m_nCollisionX2;				// center of second collision circle
	float m_nCollisionY2;				// -------------------------------
	float m_nCollisionRadius;			// the radius of the collision circles
	float m_fRotation;				// the amount the car is rotated
	float m_fRotationRate;			// the rate at which the car rotates
	float m_fAccelerationRate;		// the rate at which the car accelerates
	float m_fSpeed;					// the speed the car is currently moving
	float m_nHealth;				// the current health of the car
	float m_nMaxHealth;				// the maximum amount of health for this car
	float m_fPowerUpBar;			// the current amount of power this car has
	float m_fShieldBar;				// the current amount of shield this car has
	float m_fMaxShield;				// the maximum amount of shield this car can have
	float m_fMaxPowerUp;			// the maximum amount of power this car can ahve
	float m_fMaxSpeed;				// the maximum speed this car can travel
	int m_nCrashID;					// the id for the crash sound to be played when crashing
	int m_nKillCount;				// the current amount of kills this car has
	int m_nCarID;					// the id for the car texture
	float m_fFireDelay;				// the delay between firing bullets
	int m_nBulletSound;				// the id for the sound of firing a bullet
	int m_nSelectedWeapon;			// the currently selected weapon (0-2) 
	tVector2D m_tDirection;			// the forward direction that speed is applied to
	tVector2D m_tVelocity;			// outside forces that effect which way the overall velocity of the car is
public:
	CCar(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);


	//getters
	tVector2D GetDirection(void) { return m_tDirection; }
	tVector2D GetVelocity(void) { return m_tVelocity; }
	float GetCX1(void) { return m_nCollisionX1; }
	float GetCY1(void) { return m_nCollisionY1; }
	float GetRadius(void) { return m_nCollisionRadius; }
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
	void SetCollisionInfo(int x1, int y1, int radius) { m_nCollisionX1 = x1; m_nCollisionY1 = y1; m_nCollisionRadius = radius; }
	void Rotate(float angle);
	
	
	// plays the crash sound
	void PlayCrash(void);
	void PlayBullet(void);

	// checks to see if the car is within the camera (within screen limits atm)
	bool InBounds(void);
	bool CheckCollision(IBaseInterface* pBase);

};




#endif