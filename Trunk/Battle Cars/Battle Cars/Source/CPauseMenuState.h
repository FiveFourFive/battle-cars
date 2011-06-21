
#ifndef _CPAUSEMENUSTATE_H
#define _CPAUSEMENUSTATE_H

#include "IGameState.h"

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class CPauseMenuState : public IGameState
{
private:
	CSGD_TextureManager* m_pTM;
	CPrintFont* m_pPF;
	CSGD_FModManager* m_pFM;
	CSGD_DirectInput* m_pDI;

	CXboxInput* m_pController;

	int	m_nFontID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;

	CPauseMenuState(void);
	~CPauseMenuState(void){};
	CPauseMenuState(const CPauseMenuState&);
	CPauseMenuState& operator = (const CPauseMenuState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);

public:
	static CPauseMenuState* GetInstance();

	void SetController(CXboxInput* controller) { m_pController = controller; }

	void Enter(void);
	void Exit(void);
};

#endif