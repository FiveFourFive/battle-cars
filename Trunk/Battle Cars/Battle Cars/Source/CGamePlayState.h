
#ifndef _CGAMEPLAYSTATE_H
#define _CGAMEPLAYSTATE_H

#include "IGameState.h"
#include "CBase.h"

#include "CObjectFactory.h"
#include "CMessageSystem.h"
#include "CEventSystem.h"

#include <string>
using std::string;
#include <vector>
using namespace std;
#include <list>


#include "CLevel.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_FModManager;
class CObjectManager;
class CPrintFont;
class CCar;
class CBullet;
class CPlayer;
class CXboxInput;
class CSpeedRamp;
class ParticleManager;
class CEnemy;
class PowerUp;

class CGamePlayState : public IGameState
{

private:
	CSGD_Direct3D* m_pD3D;
	CSGD_DirectInput* m_pDI;
	CSGD_TextureManager* m_pTM;
	CSGD_FModManager* m_pFM;
	CMessageSystem* m_pMS;
	CEventSystem* m_pES;

	CObjectManager* m_pOM;
	CObjectFactory<string,CBase>* m_pOF;

	CXboxInput* m_pController1;
	CXboxInput* m_pController2;

	ParticleManager* m_pPM;		// The pointer to the particle manager.

	// font
	CPrintFont* m_pPF;
	CLevel* Level;
	RECT temp; // this is a temp rect that can be deleted when the camera is done

	// list for scores;
	std::vector<CCar*> m_lScores;
	
	// objects
	CPlayer* player;
	CEnemy* dummy;
	PowerUp* power_up;
	CSpeedRamp* speedy;
	CCar* dummy2;
	CPlayer* player2;
	// sound
	int m_nBackgroundMusicID;
	int m_nCountDown;
	int m_nCountDownEnd;

	bool m_bPlaying;
	float m_fCountDown;
	float m_fEnlarge;
	bool m_bCountDown;

	int time;		// total time, convert everything into seconds: 2 minutes = 120 seconds.
	float m_fElapsedSecond;		// determine if a second has passed.

	int score;		// players score;
	float m_ftimer;	// timer before score is incremented.

	vector<CPlayer*> characters;

	CGamePlayState(void);
	~CGamePlayState(void){};
	CGamePlayState(const CGamePlayState&);
	CGamePlayState& operator = (const CGamePlayState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);
public:

	CLevel* GetLevel () {return Level;}

	void Enter(void);
	void Exit(void);

	std::vector<CCar*>* GetList(void) { return &m_lScores; }

	static CGamePlayState* GetInstance();

	static void MessageProc(CBaseMessage* pMsg);

	vector<CPlayer*> GetCharacters() {return characters;}
	void SetCharacters(vector<CPlayer*> players) {characters = players;}
	CObjectManager* GetObjectManager() {return m_pOM;}

};


#endif