#ifndef _CLOSSSTATE_H
#define _CLOSSSTATE_H

#include "IGameState.h"

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class CSGD_Direct3D;

class CLossState : public IGameState
{
private:
	CSGD_TextureManager* m_pTM;
	CPrintFont* m_pPF;
	CSGD_FModManager* m_pFM;
	CSGD_DirectInput* m_pDI;
	CSGD_Direct3D* m_pD3D;
	//controllers
	CXboxInput* m_pController1;
	CXboxInput* m_pController2;

	int	m_nFontID;
	int m_nBackgroundMusicID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;


	CLossState(void);
	~CLossState(void);
	CLossState(const CLossState&);
	CLossState& operator = (const CLossState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);

public:
	static CLossState* GetInstance();

	void Enter(void);
	void Exit(void);


};



#endif