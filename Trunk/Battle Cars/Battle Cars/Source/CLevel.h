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
class CSGD_TextureManager;
class CSGD_Direct3D;
class CMap;
class CBase;
class CEventSystem;
class CCamera;

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

	void SetCarSpawn (CBase* pBase);
	void SetSpeedRampSpawn (CBase* pBase);
	void SetPowerUpSpawn (CBase* pBase);
	void SetObstacleSpawn (CBase* pBase);
	
	CMap* GetMap(void) { return LevelMap; }
};

#endif