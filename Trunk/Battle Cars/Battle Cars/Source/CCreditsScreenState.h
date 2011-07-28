#ifndef	CREDITSSCREEN_H_
#define CREDITSSCREEN_H_

#include "IGameState.h"

class CSGD_Direct3D;
class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;

class ParticleManager;

#define PPLTOCREDIT 21

class CCreditsScreenState : public IGameState
{
private:
	CCreditsScreenState();
	~CCreditsScreenState();
	CCreditsScreenState& operator=(const CCreditsScreenState&);
	CCreditsScreenState(const CCreditsScreenState&);

	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CPrintFont*				m_pPF;
	CSGD_FModManager*		m_pFM;
	CSGD_DirectInput*		m_pDI;
	CXboxInput*				m_pController1;


	int m_nImageID;
	int m_nFontID;
	int m_nSoundID;

	float y[PPLTOCREDIT];

	ParticleManager* m_pPM;

public:
	static CCreditsScreenState* GetInstance();
	void Enter();
	bool Input();
	void Update(float fElapsedTime);
	void Render();
	void Exit();

	bool HandleEnter();
};

#endif