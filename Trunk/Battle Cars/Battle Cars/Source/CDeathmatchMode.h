#ifndef _CDEATHMATCHMODE_H
#define _CDEATHMATCHMODE_H

class CCar;


class CDeathmatchMode
{
private:


public:
	CDeathmatchMode(void);
	~CDeathmatchMode(void);
	void CheckCarStatus(CCar* car);

	void CheckWinLoss(void);
	//void (*HandleCar) (CCar* car);

};



#endif