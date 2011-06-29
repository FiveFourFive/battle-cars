#ifndef _CKEYBINDSSTATE_H
#define _CKEYBINDSSTATE_H

#include "IGameState.h"
class CSGD_Direct3D;
class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class CKeyBindsState : public IGameState
{
private:
	CSGD_Direct3D* m_pD3D;
	CSGD_TextureManager* m_pTM;
	CPrintFont* m_pPF;
	CSGD_FModManager* m_pFM;
	CSGD_DirectInput* m_pDI;
	CXboxInput* m_pController;
	int m_nBackgroundMusicID;
	int m_nFontID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;
	int m_nSoundA;
	int m_nSoundB;
	float m_fDelay;
	CKeyBindsState(void);
	~CKeyBindsState(void);
	CKeyBindsState& operator=(const CKeyBindsState&);
	CKeyBindsState(const CKeyBindsState&);

	bool HandleEnter(void);

public:
	static CKeyBindsState* GetInstance(void);
	void Enter();
	bool Input();
	void Update(float fElapsedTime);
	void Render();
	void Exit();

	void SetController(CXboxInput* pController) { m_pController = pController; }

};



#endif