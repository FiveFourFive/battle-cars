
#ifndef _COPTIONSTATE_H
#define _COPTIONSTATE_H


#include "IGameState.h"

class CSGD_Direct3D;
class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class COptionState : public IGameState
{

private:
	CSGD_Direct3D* m_pD3D;
	CSGD_TextureManager* m_pTM;
	CPrintFont* m_pPF;
	CSGD_FModManager* m_pFM;
	CSGD_DirectInput* m_pDI;

	CXboxInput* m_pController1;
	CXboxInput* m_pController2;

	int m_nBackgroundMusicID;
	int m_nFontID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;
	COptionState(void);
	~COptionState(void);
	COptionState& operator=(const COptionState&);
	COptionState(const COptionState&);

	bool HandleEnter(void);
public:
	static COptionState* GetInstance(void);
	void Enter();
	bool Input();
	void Update(float fElapsedTime);
	void Render();
	void Exit();


};



#endif