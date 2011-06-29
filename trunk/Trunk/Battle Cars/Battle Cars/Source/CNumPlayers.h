#ifndef NUMPLAYER_H_
#define NUMPLAYER_H_

#include "IGameState.h"

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;

class CNumPlayers : public IGameState
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

	int m_nNumberOfPlayers;


	CNumPlayers(void);
	~CNumPlayers(void);
	CNumPlayers(const CNumPlayers&);
	CNumPlayers& operator = (const CNumPlayers&);
public:
	static CNumPlayers* GetInstance();

	void Enter(void);
	void Exit(void);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);
	
	int GetNumberOfPlayers() { return m_nNumberOfPlayers; }


};

#endif