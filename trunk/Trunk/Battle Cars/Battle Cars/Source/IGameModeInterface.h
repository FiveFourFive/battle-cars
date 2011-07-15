#ifndef _IGAMEMODEINTERFACE_H
#define _IGAMEMODEINTERFACE_H

class CCar;
class IGameModeInterface
{
private:


public:

	virtual ~IGameModeInterface(void) = 0 {};

	virtual void CheckCarStatus(CCar* car) = 0;
	virtual void CheckWinLoss(void) = 0;

};



#endif