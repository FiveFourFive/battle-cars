#ifndef _CKEYBINDS_H
#define _CKEYBINDS_H

//#include <Xinput.h>
class CKeyBinds
{
private:
	int shoot;
	bool shoot_trigger;
	int accept;
	bool accept_trigger;
	int back;
	bool back_trigger;
	int change_weapon;
	bool change_trigger;
	int accelerate;
	bool accel_trigger;
	int break_reverse;
	bool break_trigger;

public:
	CKeyBinds(void);
	~CKeyBinds(void);
	int GetShoot(void) { return shoot; }
	int GetAccept(void) { return accept; }
	int GetBack(void) { return back; }
	int GetChangeWeapon(void) { return change_weapon; }
	int GetAccelerate(void) { return accelerate; }
	int GetBreakReverse(void) { return break_reverse; }

	void SetAccept(int newaccept) { accept = newaccept; }
	void SetBack(int newback) { back = newback; }
	void SetShoot(int newshoot) { shoot = newshoot; }
	void SetChangeWeapon(int newchange) { change_weapon = newchange; }
	void SetAccelerate(int newaccel) { accelerate = newaccel; }
	void SetBreakReverse(int newbreak) { break_reverse = newbreak; }


	void ResetDefaults(void);

};


#endif