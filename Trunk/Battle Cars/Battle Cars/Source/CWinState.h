#ifndef _CWINSTATE_H
#define _CWINSTATE_H

#include "IGameState.h"
#include <Windows.h>
class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class CPlayer;
class CSGD_Direct3D;

class CWinState : public IGameState
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

	float m_fSlotTime;
	float m_fSlotDelay;
	float m_fTotalTurns;
	float m_fDelay;
	int m_nNumSlots;
	int m_nSlot;
	int m_nBGImageID;
	bool m_bRoll;
	int miniID;
	int vetteID;
	int humveeID;
	int truckID;
	int id;
	int carsID;
	RECT mini;
	RECT vette;
	RECT humvee;
	RECT truck;

	CPlayer* m_pWinner;

	CWinState(void);
	~CWinState(void);
	CWinState(const CWinState&);
	CWinState& operator = (const CWinState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);

public:
	static CWinState* GetInstance();

	void Enter(void);
	void Exit(void);

	void SetWinner(CPlayer* player) { m_pWinner = player; }
};




#endif