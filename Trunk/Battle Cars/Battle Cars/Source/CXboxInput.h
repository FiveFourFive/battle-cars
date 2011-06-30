/////////////////////////////////////////////////////////
//	File Name : "CXboxInput.h"
//	Author Name : Wade Shwallon
//  Created On: 6-17-2011
//	Purpose : To contain the functionality of gamepad input
/////////////////////////////////////////////////////////


#ifndef _CXBOXINPUT_H
#define _CXBOXINPUT_H
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")


class CKeyBinds;
class CXboxInput
{
private:

	XINPUT_STATE m_xState;
	int m_nPlayerNum;
	CKeyBinds* m_pKB;

public:
	
	CXboxInput(int PlayerNum);
	~CXboxInput(void);
	XINPUT_STATE ReadInputState(void);
	XINPUT_STATE GetState(void);
	bool Connected(void);
	void Vibrate(int left = 0, int right = 0);
	CKeyBinds* GetKB(void) { return m_pKB; }


	bool CheckShoot(void);
	bool CheckAccept(void);
	bool CheckBack(void);
	bool CheckChangeWeapon(void);
	bool CheckAccelerate(void);
	bool CheckBreak_reverse(void);

};

#endif