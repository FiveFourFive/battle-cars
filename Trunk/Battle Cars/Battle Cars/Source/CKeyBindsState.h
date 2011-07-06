#ifndef _CKEYBINDSSTATE_H
#define _CKEYBINDSSTATE_H

#include "IGameState.h"
class CSGD_Direct3D;
class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class CKeyBinds;
class CKeyboardKeyBinds;
class CKeyBindsState : public IGameState
{
private:
	CSGD_Direct3D* m_pD3D;
	CSGD_TextureManager* m_pTM;
	CPrintFont* m_pPF;
	CSGD_FModManager* m_pFM;
	CSGD_DirectInput* m_pDI;
	CXboxInput* m_pController;
	CKeyBinds* m_pKB;
	CKeyboardKeyBinds* m_pKeyboardKB;
	int m_nBackgroundMusicID;
	int m_nFontID;
	int m_nSelection;
	int m_nMaxOptions;
	int m_nMenuSelect;
	int m_nMenuMove;
	int m_nSoundA;
	int m_nSoundB;
	float m_fDelay;
	bool m_bSelected;
	int m_nSelected;
	CKeyBindsState(void);
	~CKeyBindsState(void);
	CKeyBindsState& operator=(const CKeyBindsState&);
	CKeyBindsState(const CKeyBindsState&);

	bool HandleEnter(void);

	void Accept(int keycode);
	void Back(int keycode);
	void Shoot(int keycode);
	void ChangeWeapon(int keycode);
	void Forward(int keycode);
	void Backward(int keycode);
	void Left(int keycode);
	void Right(int keycode);


public:
	static CKeyBindsState* GetInstance(void);
	void Enter();
	bool Input();
	void Update(float fElapsedTime);
	void Render();
	void Exit();

	void SetController(CXboxInput* pController) { m_pController = pController; }
	char* GetKeyBound(int keycode);
	bool SendKeyBinds(void);
	bool SetKeyBind(int keycode);
};



#endif