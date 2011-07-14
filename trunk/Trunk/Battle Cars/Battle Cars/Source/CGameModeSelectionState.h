#ifndef _CGAMEMODESELECTIONSTATE_H
#define _CGAMEMODESELECTIONSTATE_H


#include "IGameState.h"

#include <vector>
using namespace std;

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class CPlayer;

class CGameModeSelectionState : public IGameState
{
private:
	CSGD_TextureManager*	m_pTM;
	CPrintFont*				m_pPF;
	CSGD_FModManager*		m_pFM;
	CSGD_DirectInput*		m_pDI;

	int	m_nFontID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;

	CGameModeSelectionState();
	~CGameModeSelectionState();
	CGameModeSelectionState& operator=(const CGameModeSelectionState&);
	CGameModeSelectionState(const CGameModeSelectionState&);

public:

	void Enter(void);
	void Exit(void);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);

};



#endif