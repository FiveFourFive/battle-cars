#ifndef _CCAR_H
#define _CCAR_H

#include "CBase.h"
#include "SGD_Math.h"
#include "IListener.h"
class CCamera;

enum weapons { WEAPON_PISTOL, WEAPON_RPG, WEAPON_SPECIAL };
enum characters{CAR_MINI, CAR_VETTE, CAR_HUMMER, CAR_TRUCK, CAR_NUM};
class CCar : public CBase, public IListener
{

private:
	float m_fVelX;					// overall velocity in the x direction
	float m_fVelY;					// overall velocity in the y direction
	float m_nCollisionX1;				// center of first collision circle
	float m_nCollisionY1;				// -------------------------------
	float m_nCollisionX2;				// center of second collision circle
	float m_nCollisionY2;				// -------------------------------
	float m_nCollisionRadius;			// the radius of the collision circles
	bool m_bAccelerating;				// whether or not the car is accelerating
	float m_fRotation;				// the amount the car is rotated
	float m_fRotationRate;			// the rate at which the car rotates
	float m_fAccelerationRate;		// the rate at which the car accelerates
	float m_fSpeed;					// the speed the car is currently moving
	float m_fArmor;					// the extra health stat for certain cars
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
	float m_fFireDelayMissiles;		// the delay between firing missiles
	int m_nBulletSound;				// the id for the sound of firing a bullet
	int m_nSelectedWeapon;			// the currently selected weapon (0-2) 
	tVector2D m_tDirection;			// the forward direction that speed is applied to
	tVector2D m_tVelocity;			// outside forces that effect which way the overall velocity of the car is
	tVector2D m_tOverallVelocity;
	float m_fCollisionDelay;
	int m_nBulletImageID;
	int m_nMissileImageID;
	int m_nSpecialWeaponImageID;
	int m_nSpecialLevel;
	int m_nPlayerType;
	bool m_bIsAlive;
	float m_fCollisionEffect;		// used to create a single particle effect with the send event rather than creating more than what was desired.
	float m_fRespawnTimer;			// The elapsed time until the object respawns again.

	bool Collision_effect;			//Check whether effect has been applied yet or not.

public:
	CCar(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);


	//getters
	tVector2D GetDirection(void) { return m_tDirection; }
	tVector2D GetVelocity(void) { return m_tVelocity; }
	float GetCX1(void) { return m_nCollisionX1; }
	float GetCY1(void) { return m_nCollisionY1; }
	float GetCX2(void) { return m_nCollisionX2; }
	float GetCY2(void) { return m_nCollisionY2; }
	int GetSpecialLevel(void) { return m_nSpecialLevel; }
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
	float GetArmor(void) {return m_fArmor;}
	float GetFireDelayMissile(void) {return m_fFireDelayMissiles;}
	int GetBulletImageID(void) { return m_nBulletImageID; }
	int GetMissileImageID(void) {return m_nMissileImageID;}
	int GetSpecialImageID(void) {return m_nSpecialWeaponImageID;}
	float GetCollisionDelay(void) {return m_fCollisionDelay;}
	float GetEffectTimer(){ return m_fCollisionEffect;}
	tVector2D GetOverallVelocity(void) { return m_tOverallVelocity; }
	int GetPlayerType() {return m_nPlayerType;}
	bool GetIsAlive(void) { return m_bIsAlive; }
	float GetRespawnTimer(){ return m_fRespawnTimer;}
	bool GetCollisionEffect(){ return Collision_effect; }
	//setters
	void SetOverallVelocity(tVector2D vel) { m_tOverallVelocity = vel; }
	void SetSpecialLevel(int level) { m_nSpecialLevel = level; }
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
	void SetAccelerating(bool accel) { m_bAccelerating = accel; }
	void SetCollisionInfo(float x1, float y1, float radius) { m_nCollisionX1 = x1; m_nCollisionY1 = y1; m_nCollisionRadius = radius; }
	void Rotate(float angle);
	void SetArmor(float fArmor) {m_fArmor = fArmor;}
	void SetFireDelayMissile(float delay) {m_fFireDelayMissiles = delay;}
	void SetBulletImageID(int id) {m_nBulletImageID = id;}
	void SetMissileImageID(int id) {m_nMissileImageID = id;}
	void SetSpecialWeaponImageID(int id) {m_nSpecialWeaponImageID = id;}
	void SetCollisionDelay(float delay) {m_fCollisionDelay = delay;}
	void SetPlayerType(int type) {m_nPlayerType = type;}
	void ResetEffectTimer(){ m_fCollisionEffect = 0.0f;}
	void SetIsAlive(bool alive) { m_bIsAlive = alive; }
	void SetRespawnTimer(float respawn){ m_fRespawnTimer = respawn;}
	void SetCollisionEffect(bool value){ Collision_effect = value;}
	void SetCarId(int id) {m_nCarID = id;}
	void SetRadius(float radius) {m_nCollisionRadius = radius;}
	void SetCollisionX1(float x) {m_nCollisionX1 = x;}
	void SetCollisionY1(float y) {m_nCollisionY1 = y;}
	void SetCollisionX2(float x) {m_nCollisionX2 = x;}
	void SetCollisionY2(float y) {m_nCollisionY2 = y;}


	// plays the crash sound
	void PlayCrash(void);
	void PlayBullet(void);

	// checks to see if the car is within the camera (within screen limits atm)
	bool InBounds(float fElapsedTime);
	bool CheckCollision(IBaseInterface* pBase);

	void HandleEvent(CEvent* pEvent);

	RECT GetRect();

};




#endif