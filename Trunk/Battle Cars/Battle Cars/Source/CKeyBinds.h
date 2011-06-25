#ifndef _CKEYBINDS_H
#define _CKEYBINDS_H

//#include <Xinput.h>
class CKeyBinds
{
private:
	int shoot_accept;
	bool shoot_trigger;
	int change_weapon;
	bool change_trigger;
	int accelerate;
	bool accel_trigger;
	int break_reverse;
	bool break_trigger;

public:
	CKeyBinds(void);
	~CKeyBinds(void);
	int GetShootAccept(void) { return shoot_accept; }
	int GetChangeWeapon(void) { return change_weapon; }
	int GetAccelerate(void) { return accelerate; }
	int GetBreakReverse(void) { return break_reverse; }


	void SetShootAccept(int newshoot) { shoot_accept = newshoot; }
	void SetChangeWeapon(int newchange) { change_weapon = newchange; }
	void SetAccelerate(int newaccel) { accelerate = newaccel; }
	void SetBreakReverse(int newbreak) { break_reverse = newbreak; }


	void ResetDefaults(void);

};


#endif