#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "IGameState.h"

#include <vector>
using namespace std;

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;
class CPlayer;

class CCharacterSelection : public IGameState
{
private:
	CSGD_TextureManager*	m_pTM;
	CPrintFont*				m_pPF;
	CSGD_FModManager*		m_pFM;
	CSGD_DirectInput*		m_pDI;

	vector<CPlayer*> m_vPlayerList;
	bool isAvailable[4];
	CPlayer* currPlayer;
	CPlayer* m_player1;
	CPlayer* m_player2;

	int m_nBGImageID;
	int	m_nFontID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;
	int m_nIncorrectSelection;

	bool m_bPlayer1_turn;
	bool m_bPlayer2_turn;


	// Make it a Singleton
	CCharacterSelection();
	~CCharacterSelection();
	CCharacterSelection& operator=(const CCharacterSelection&);
	CCharacterSelection(const CCharacterSelection&);

public:
	
	static CCharacterSelection* GetInstance();

	void Enter(void);
	void Exit(void);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);

	bool LoadCharacters();
	bool IsPlayer1Selecting(){ return m_bPlayer1_turn; }
	bool IsPlayer2Selecting(){ return m_bPlayer2_turn; }
	CPlayer* GetPlayer1(){ return m_player1;}
	CPlayer* GetPlayer2(){ return m_player2;}

	vector<CPlayer*> GetList() { return m_vPlayerList;}

};

#endif