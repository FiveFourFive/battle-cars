#ifndef _CLOADINGSTATE_H
#define _CLOADINGSTATE_H

#include "IGameState.h"
#include "CPrintFont.h"

class CLoadingState : public IGameState
{

private:
	CPrintFont * pFont;
	int m_nBGImageID;
	int m_nAnimID;
	int m_nBackgroundMusicID;
	int m_nCountDown;
	int m_nCountDownEnd;

	CLoadingState(void){}
	~CLoadingState(void){}
	CLoadingState(const CLoadingState&);
	CLoadingState& operator = (const CLoadingState&);
	
	float timeStamp;
	bool HandleEnter(void);
public:
	bool loading;
	static CLoadingState* GetInstance();
	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	void Enter(void);
	void Exit(void);
};



#endif