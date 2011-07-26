#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "CCar.h"
#include "CEventSystem.h"
#include "IListener.h"
#include <vector>

class CXboxInput;
class CMessageSystem;
class CHUD;


class CPlayer : public CCar
{
private:
	CCamera* m_pCamera;
	CHUD* m_pHUD;
	int m_nHealth_Overlay_Index;
	int m_nHealth_Underlay_Index;

	int m_nScore_Overlay_Index;

	std::vector<int> Weapons;

	//int m_nWeaponImageIndex;
	//int m_nPistoalImageID;
	//int m_nRocketImageID;

	CEventSystem* m_pES;
	CXboxInput* m_pController1;
	float m_fFireTimer;
	bool m_bIsFlameThrowerOn;
	bool m_bIsIcyGatlingOn;
	float m_fIcyBullets;
	float m_fFlames;
	RECT m_rectWall;
	RECT m_rectWallCollision;

	float m_fBurstDelay;		// Delay until flamethrower bullet burst.

	int m_nFlameThrower;		// The Emittor ID to the flamethrower particle effect.
	bool isSet;					// Is the emittor Set?
	bool controller;
	int m_nPlayerNum;

public:
	CPlayer(CXboxInput* pController);
	~CPlayer(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	void SetController(CXboxInput* pController) { m_pController1 = pController; }
	CXboxInput* GetController(void) {return m_pController1; }

	void SetWallRect (RECT collision) {m_rectWall = collision;}
	RECT GetWallRect () { return m_rectWall;}

	void SetWallCollisionRect (RECT collision) {m_rectWallCollision = collision;}
	RECT GetWallCollisionRect () { return m_rectWallCollision;}

	void AddWeaponIcon(int icon){Weapons.push_back(icon);}
	int GetWeaponIcon(){return Weapons[GetSelectedWeapon ()];}

	CHUD* GetHudItem(){ return m_pHUD;}

	CCamera* GetCamera(void) { return m_pCamera; }

	int GetPlayerNum(void) { return m_nPlayerNum; }
	void SetPlayerNum(int num) { m_nPlayerNum = num; }

	int GetFlameThrowerEmittorID(){ return m_nFlameThrower; }
	void SetFlameThrowerEmittorID(int value){ m_nFlameThrower = value;}


	bool CheckCollision(IBaseInterface* pBase);
	bool IsFlameThrowerOn() {return m_bIsFlameThrowerOn;}
	void SetFlameThrower(bool flame) {m_bIsFlameThrowerOn = flame;}
	void HandleEvent(CEvent* pEvent);
};




#endif