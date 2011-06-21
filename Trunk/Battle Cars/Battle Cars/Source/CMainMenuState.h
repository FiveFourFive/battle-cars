

#ifndef _CMAINMENUSTATE_H
#define _CMAINMENUSTATE_H

#include "IGameState.h"

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;

class CMainMenuState : public IGameState
{

private:
	CSGD_TextureManager* m_pTM;
	CPrintFont* m_pPF;
	CSGD_FModManager* m_pFM;
	CSGD_DirectInput* m_pDI;

	//controllers
	CXboxInput* m_pController1;
	CXboxInput* m_pController2;


	int	m_nFontID;
	int m_nBackgroundMusicID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;


	CMainMenuState(void);
	~CMainMenuState(void);
	CMainMenuState(const CMainMenuState&);
	CMainMenuState& operator = (const CMainMenuState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);

public:
	static CMainMenuState* GetInstance();

	void Enter(void);
	void Exit(void);

	int GetFontID(void) { return m_nFontID; }
	int GetBackgroundMusicID(void) { return m_nBackgroundMusicID; }

};



#endif