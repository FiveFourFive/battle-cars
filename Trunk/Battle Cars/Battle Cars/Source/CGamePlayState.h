
#ifndef _CGAMEPLAYSTATE_H
#define _CGAMEPLAYSTATE_H

#include "IGameState.h"
#include "IGameModeInterface.h"
#include "CBase.h"

#include "CObjectFactory.h"
#include "CMessageSystem.h"
#include "CEventSystem.h"

#include <string>
using std::string;
#include <vector>
using namespace std;
#include <list>
#include "SGD_Math.h"

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
class CDeathmatchMode;
class CKeyboardKeyBinds;
class CKeyBinds;
class CBoss;

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

	IGameModeInterface* m_pMode;

	CKeyBinds*				m_pPlayer1KB;
	CKeyboardKeyBinds*		m_pPlayer1KeyboardKB;

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
	vector<PowerUp*> power_ups;
	vector<CSpeedRamp*> ramps;
	vector<CBoss*> bosses;
	PowerUp* power_up_power;
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

	int m_nMiniMapOverlayIndex;
	int m_nMiniMapMiddlelayIndex;
	int m_nMiniMapUnderlayIndex;

	int time;		// total time, convert everything into seconds: 2 minutes = 120 seconds.
	float m_fElapsedSecond;		// determine if a second has passed.

	int score;		// players score;
	float m_ftimer;	// timer before score is incremented.
	bool m_bBossHasSpawned;
	float m_fRespawnBossTimer;
	vector<CPlayer*> characters;
	bool m_bBossHasDied;
	bool m_bMiniBossHasSpawned;
	bool m_bMiniBossHasDied;
	float m_fRespawnMiniBossTimer;
	bool m_bTimeTrial;
	bool m_bCollectionChallenge;

	CGamePlayState(void);
	~CGamePlayState(void){};
	CGamePlayState(const CGamePlayState&);
	CGamePlayState& operator = (const CGamePlayState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);
	void SortScores(int left, int right); // move to HUD class if implemented
public:

	CLevel* GetLevel () {return Level;}

	void Enter(void);
	void Exit(void);

	std::vector<CCar*>* GetList(void) { return &m_lScores; }
	vector<CCar*> GetScores(void) { return m_lScores; }
	static CGamePlayState* GetInstance();

	static void MessageProc(CBaseMessage* pMsg);
	//Accessors
	vector<CPlayer*> GetCharacters() {return characters;}
	CPlayer* GetPlayer1() {return player;}
	CPlayer* GetPlayer2() {return player2;}
	CObjectManager* GetObjectManager() {return m_pOM;}
	vector<CSpeedRamp*> GetSpeedRamps() {return ramps;}
	IGameModeInterface* GetMode(void) { return m_pMode; }
	int GetTimeLeft(void) { return time; }
	vector<CBoss*> GetBosses() {return bosses;}
	//Mutators
	void SetCharacters(vector<CPlayer*> players) {characters = players;}
	void SetSpeedRamps(vector<CSpeedRamp*> speedRamps) {ramps = speedRamps;}
	void SetBossHasDied(bool died) {m_bBossHasDied = died;}
	void SetGameMode(IGameModeInterface* mode) { m_pMode = mode; }
	IGameModeInterface* GetGameMode(){ return m_pMode;}
	void Setvolume(void);
	void SetTimeTrial(bool trial) {m_bTimeTrial = trial;}
	void SetMiniBossHasDied(bool died) {m_bMiniBossHasDied = died;}
	void SetCollectionChallenge(bool challenge) {m_bCollectionChallenge = challenge;}
};


#endif