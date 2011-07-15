#ifndef _CTIMECHALLENGE_H
#define _CTIMECHALLENGE_H

#include "IGameModeInterface.h"
class CCar;


class CTimeChallengeMode : public IGameModeInterface
{
private:
	bool m_bGameOver;

	CTimeChallengeMode(void);
	~CTimeChallengeMode(void);
	CTimeChallengeMode& operator=(const CTimeChallengeMode&);
	CTimeChallengeMode(const CTimeChallengeMode&);
public:
	static CTimeChallengeMode* GetInstance(void);
	void CheckCarStatus(CCar* car);

	void CheckWinLoss(void);
	//void (*HandleCar) (CCar* car);

};



#endif