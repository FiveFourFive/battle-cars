
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
class CObstacle;
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
class CCollectable;

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
	vector<CBase*> cars;
	vector<CObstacle*> obstacles;
	vector<PowerUp*> power_ups;
	vector<CSpeedRamp*> ramps;
	vector<CBoss*> bosses;
	PowerUp* power_up_power;
	CEnemy* dummy2;
	CEnemy* dummy3;
	CEnemy* dummy4;
	CEnemy* dummy5;
	CPlayer* player2;
	CObstacle* crate1;
	CObstacle* crate2;
	CObstacle* crate3;
	CObstacle* barrel1;
	CObstacle* barrel2;

	int m_nCrateID;
	int m_nBarrelID;

	// sound
	int m_nBgMusicID;
	int m_nCountDown;
	int m_nCountDownEnd;

	bool m_bPlaying;
	float m_fCountDown;
	float m_fEnlarge;
	bool m_bCountDown;
	float scoretimer;

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
	int m_nCollectableTotalPlayer;
	int m_nCollectableTotalComputer;
	vector<CCollectable*> collectables;
	CEnemy* collectionChallengeBoss;

	CGamePlayState(void);
	~CGamePlayState(void){};
	CGamePlayState(const CGamePlayState&);
	CGamePlayState& operator = (const CGamePlayState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);
	 // move to HUD class if implemented
public:

	CLevel* GetLevel () {return Level;}

	void Enter(void);
	void Exit(void);

	std::vector<CCar*>* GetList(void) { return &m_lScores; }
	vector<CCar*> GetScores(void) { return m_lScores; }
	void SetScores(vector<CCar*> vscores) { m_lScores = vscores; }
	void SortScores(void);
	static CGamePlayState* GetInstance();

	static void MessageProc(CBaseMessage* pMsg);
	//Accessors
	vector<CPlayer*> GetCharacters() {return characters;}
	vector<CBase*> GetCars () {return cars;}
	vector<PowerUp*> GetPowerUps () {return power_ups;}

	CPlayer* GetPlayer1() {return player;}
	CPlayer* GetPlayer2() {return player2;}
	CObjectManager* GetObjectManager() {return m_pOM;}
	vector<CSpeedRamp*> GetSpeedRamps() {return ramps;}
	IGameModeInterface* GetMode(void) { return m_pMode; }
	int GetTimeLeft(void) { return time; }
	vector<CBoss*> GetBosses() {return bosses;}
	int GetComputerCollectables() {return m_nCollectableTotalComputer;}
	int GetPlayerCollectables() {return m_nCollectableTotalPlayer;}
	vector<CCollectable*> GetCollectables() {return collectables;}
	CEnemy* GetCollectionBoss() {return collectionChallengeBoss;}
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
	void SetComputerCollectables(int collectables) {m_nCollectableTotalComputer = collectables;}
	void SetPlayerCollectables(int collectables) {m_nCollectableTotalPlayer = collectables;}

	int GetBarrelImageID () {return m_nBarrelID;}
	int GetCrateImageID () {return m_nCrateID;}

	int GetBackgroundMusicID(void) { return m_nBgMusicID; }
};


#endif