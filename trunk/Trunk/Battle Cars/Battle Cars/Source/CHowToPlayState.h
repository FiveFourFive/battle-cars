
#ifndef _CHOWTOPLAYSTATE_H
#define _CHOWTOPLAYSTATE_H

#include "IGameState.h"

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;

class CHowToPlayState : public IGameState
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
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;

	CHowToPlayState(void);
	~CHowToPlayState(void);
	CHowToPlayState(const CHowToPlayState&);
	CHowToPlayState& operator = (const CHowToPlayState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);

public:
	static CHowToPlayState* GetInstance();

	void Enter(void);
	void Exit(void);
};

#endif