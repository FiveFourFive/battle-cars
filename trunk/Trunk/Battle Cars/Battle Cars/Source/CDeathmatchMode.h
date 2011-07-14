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

	//void (*HandleCar) (CCar* car);

};



#endif