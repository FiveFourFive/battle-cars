
/////////////////////////////////////////////////////////
//	File Name : "CGame.h"
//	Author Name : Wade Shwallon
//	Purpose : contains all game related code
/////////////////////////////////////////////////////////

#ifndef _CGAME
#define _CGAME

#include "CStopWatch.h"
#include <vector>
using namespace std;
class CSGD_Direct3D;
class CSGD_TextureManager;
class CMainMenuState;
class IGameState;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class CKeyboardKeyBinds;
class Gamer_Profile;

class CGame
{
private:

	// Singleton Pointers
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_FModManager*		m_pFM;
	CSGD_DirectInput*		m_pDI;

	//controller input
	CKeyboardKeyBinds*		m_pKeyboardKB;
	CXboxInput*				m_pController1;
	CXboxInput*				m_pController2;
	bool m_bController;

	Gamer_Profile* PlayerProfile;		// the profile the player is using;

	// States
	IGameState*				m_pGameState;
	CMainMenuState*			m_pMainMenuState;
	vector<IGameState*>		m_vGameStates;
	float m_fInputDelay;
	float m_fThumbDelay;

	// Sound
	float						m_nSoundAVolume;
	float						m_nSoundBVolume;
	float						m_panning;

	// Time
	CStopWatch				m_stopWatch;
	float					m_fElapsedTime;

	//Screen stuffs
	int						m_ScreenWidth;
	int						m_ScreenHeight;
	HWND					hWnd;
	bool					isWindowedMode;

	int						m_nScore;

	CGame();
	~CGame();
	CGame(const CGame& game);
	CGame& operator =(const CGame& game);

	// Utility functions
	bool Input();
	void Update(float fElapsedTime);
	void Render();

public:
	static CGame* GetInstance();

	// Initialization
	void Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed);

	// manage states
	void ChangeState(IGameState* state);
	void AddState(IGameState* state);
	void RemoveState(IGameState* state);
	void ClearStates(IGameState* state);

	CXboxInput* GetController1(void) { return m_pController1; }
	CXboxInput* GetController2(void) { return m_pController2; }
	bool ControllerInput(void) { return m_bController; }
	float GetInputDelay(void) { return m_fInputDelay; }
	void ResetInputDelay(void) { m_fInputDelay = 0.0f; }
	float GetThumbDelay(void) { return m_fThumbDelay; }
	void ResetThumbDelay(void) { m_fThumbDelay = 0.0f; }
	int GetScreenWidth() { return m_ScreenWidth;}
	int GetScreenHeight(){ return m_ScreenHeight;}
	void SetControllerInput(bool input) { m_bController = input; }
	void SetKB(CKeyboardKeyBinds* newKB) { m_pKeyboardKB = newKB; }
	CKeyboardKeyBinds* GetKeyboardKeyBinds(void) { return m_pKeyboardKB; }
	float getSoundAVolume();
	float getSoundBVolume();

	void SetSoundAVolume(float newvol);
	void SetSoundBVolume(float newvol);

	float getPanning();

	void SetPanning(float left);

	// Execution
	// returns false when game should quit
	bool Main();

	void SetScore(int score) { m_nScore = score; }
	int GetScore() { return m_nScore; }

	HWND GetHWND(){ return hWnd;}
	bool GetIsWindowed(){ return isWindowedMode;}

	Gamer_Profile* GetPlayerProfile() { return PlayerProfile;}
	void SetPlayerProfile(Gamer_Profile* profile){ PlayerProfile = profile;}

	// Clean Up
	void Shutdown();	
};

#endif