#ifndef _CLOADINGSTATE_H
#define _CLOADINGSTATE_H

#include "IGameState.h"
#include "CMessageSystem.h"

class CLoadingState : public IGameState
{

private:
	CLoadingState(void){}
	~CLoadingState(void){}
	CLoadingState(const CLoadingState&);
	CLoadingState& operator = (const CLoadingState&);
	
	float timeStamp;
	bool HandleEnter(void);
public:
	
	static CLoadingState* GetInstance();

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	void Enter(void);
	void Exit(void);
	static void MessageProc(CBaseMessage* pMsg);

};



#endif