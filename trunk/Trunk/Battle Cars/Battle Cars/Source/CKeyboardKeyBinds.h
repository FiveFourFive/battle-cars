#ifndef _CKEYBOARDKEYBINDS_H
#define _CKEYBAORDKEYBINDS_H

class CKeyboardKeyBinds
{
private:
	int shoot;
	int accept;
	int back;
	int change_weapon;
	int forward;
	int backward;
	int left;
	int right;

public:
	CKeyboardKeyBinds(void);
	~CKeyboardKeyBinds(void);
	int GetShoot(void) { return shoot; }
	int GetAccept(void) { return accept; }
	int GetBack(void) { return back; }
	int GetChangeWeapon(void) { return change_weapon; }
	int Getforward(void) { return forward; }
	int Getbackward(void) { return backward; }
	int GetLeft(void) { return left; }
	int GetRight(void) { return right; }


	void SetAccept(int newaccept) { accept = newaccept; }
	void SetShoot(int newshoot) { shoot = newshoot; }
	void SetBack(int newback) { back = newback; }
	void SetChangeWeapon(int newchange) { change_weapon = newchange; }
	void SetForward(int newforward) { forward = newforward; }
	void SetBackward(int newbackward) { backward = newbackward; }
	void SetLeft(int newleft) { left = newleft; }
	void SetRight(int newright) { right = newright; }

	void ResetDefaults(void);

};


#endif