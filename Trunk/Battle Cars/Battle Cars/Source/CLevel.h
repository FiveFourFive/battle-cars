/////////////////////////////////////////////////
//	File	:	"CLevel.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the functions for rendering
//				and checking map collisions and sending events.
//				
/////////////////////////////////////////////////

#ifndef _CLEVEL_H_
#define _CLEVEL_H_

#include <Windows.h>
#include <vector>
using namespace std;

class CSGD_TextureManager;
class CSGD_Direct3D;
class CMap;
class CBase;
class CEventSystem;
class CCamera;
class PowerUp;
class CObstacle;
class CSpeedRamp;

class CLevel
{
private:
	CMap*					LevelMap;
	CSGD_TextureManager*	m_pTM;
	CEventSystem*			m_pES;
	CSGD_Direct3D*			m_pD3D;

	CLevel(void);
	~CLevel(void){};
	CLevel(const CLevel&);
	CLevel& operator = (const CLevel&);

public:
	static CLevel* GetInstance ();

	void Shutdown ();
	bool Load (const char* filename);
	
	void Render (CCamera* camera);
	//void Update (float fElapsedTime);

	bool CheckPlayerCollision (CBase* pBase, CCamera* camera);
	bool CheckCameraCollision (CCamera* camera);
	bool CheckEnemyCollision (CBase* pBase);
	bool CheckObstacleCollision (CBase* pBase);
	bool CheckBulletCollision (CBase* pBase);

	vector<CBase*>  SetCarSpawn (vector<CBase*> pBases);
	vector<CSpeedRamp*> SetSpeedRampSpawn ();
	vector<PowerUp*> SetPowerUpSpawn ();
	vector<CObstacle*> SetObstacleSpawn ();
	void ResetCarSpawn (CBase* pBase);

	void ResetSpawns ();

	CMap* GetMap(void) { return LevelMap; }
};

#endif