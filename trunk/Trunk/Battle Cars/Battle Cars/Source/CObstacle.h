#ifndef _COBSTACLE_H
#define _COBSTACLE_H

#include "CBase.h"

#include "SGD_Math.h"

enum {CRATE_OBSTACLES, BARREL_OBSTACLES, MAX_OBSTACLE_TYPES};

class IBaseInterface;
class CObstacle : public CBase
{
private:
	tVector2D m_vCollisionVel;

	RECT m_rectWall;
	RECT m_rectWallCollision;

	int m_nObstacleType;	// Determine whether it is a crate or a barrel.

	bool m_bHit;		// Determine whether the object was hit.
	float m_fBlowUpTimer;	// Time until it blows up.
	float m_fRespawnTimer;	// Time until barrel respawns.
	bool ParticleCreated;	// Whether or not particle burst is showing.
	int m_nBarrelBurstingID;	// The id of the bursting barrel particle effect.
	bool m_bActive;				// Is the obstacle active.

	tVector2D m_SpawnPosition;	// The spawn position of the obstacle.

public:
	CObstacle(void);
	~CObstacle(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	bool CheckCollision(IBaseInterface* pBase);

	void SetVel(tVector2D vel) { m_vCollisionVel = vel; }
	tVector2D GetVel(void) { return m_vCollisionVel; }

	void SetWallRect (RECT collision) {m_rectWall = collision;}
	void SetWallCollisionRect (RECT collision) {m_rectWallCollision = collision;}

	tVector2D GetSpawnPosition(){ return m_SpawnPosition;}
	void SetSpawnPosX(float x){ m_SpawnPosition.fX = x;}
	void SetSpawnPosY(float y){ m_SpawnPosition.fY = y;}

	void SetObstacleType(int type){ m_nObstacleType = type;}
	int GetObstacleType(){ return m_nObstacleType;}

	bool IsActive(){ return m_bActive;}
	bool IsHit(){ return m_bHit;}
	void SetHit(bool value){ m_bHit = value;}
};



#endif