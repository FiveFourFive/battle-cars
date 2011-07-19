#ifndef _CHIGHSCORESTATE_H
#define _CHIGHSCORESTATE_H

#include "IGameState.h"
#include <vector>
using namespace std;
class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;

struct HighScore
{
	char* name;
	int score;

};
class CHighscoreState : public IGameState
{

private:
	CSGD_TextureManager* m_pTM;
	CPrintFont* m_pPF;
	CSGD_FModManager* m_pFM;
	CSGD_DirectInput* m_pDI;
	HighScore* highscores[10];
	vector<HighScore> m_vScoreList;
	CXboxInput* m_pController1;

	int	m_nFontID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;

	CHighscoreState(void);
	~CHighscoreState(void);
	CHighscoreState(const CHighscoreState&);
	CHighscoreState& operator = (const CHighscoreState&);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);



public:

	static CHighscoreState* GetInstance();

	void Enter(void);
	void Exit(void);

	void SortScores(void);
	void SaveScores(void);
	bool AddScore(HighScore addme);
	HighScore* LoadScores(void);
	HighScore* GetScores(void);

};


#endif