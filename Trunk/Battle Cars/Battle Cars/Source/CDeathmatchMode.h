#ifndef _CDEATHMATCHMODE_H
#define _CDEATHMATCHMODE_H

#include "IGameModeInterface.h"
class CCar;


class CDeathmatchMode : public IGameModeInterface
{
private:
	
	CDeathmatchMode(void);
	~CDeathmatchMode(void);
	CDeathmatchMode& operator=(const CDeathmatchMode&);
	CDeathmatchMode(const CDeathmatchMode&);
public:
	static CDeathmatchMode* GetInstance(void);
	void CheckCarStatus(CCar* car);

	void CheckWinLoss(void);
	//void (*HandleCar) (CCar* car);

};



#endif