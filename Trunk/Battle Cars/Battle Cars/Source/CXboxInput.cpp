/////////////////////////////////////////////////////////
//	File Name : "CXboxInput.cpp"
//	Author Name : Wade Shwallon
//  Created On: 6-17-2011
//	Purpose : To contain the functionality of gamepad input
/////////////////////////////////////////////////////////


#include "CXboxInput.h"

CXboxInput::CXboxInput(int PlayerNum)
{
	m_nPlayerNum = PlayerNum -1;
	XInputEnable(true);
}

XINPUT_STATE CXboxInput::ReadInputState(void)
{
	ZeroMemory(&m_xState, sizeof(XINPUT_STATE));

	XInputGetState(m_nPlayerNum, &m_xState);

	return m_xState;
}

void CXboxInput::Vibrate(int left, int right)
{
	XINPUT_VIBRATION Vibrate;

	ZeroMemory(&Vibrate, sizeof(XINPUT_VIBRATION));

	Vibrate.wLeftMotorSpeed = left;
	Vibrate.wRightMotorSpeed = right;

	XInputSetState(m_nPlayerNum, &Vibrate);

}

bool CXboxInput::Connected(void)
{
	ZeroMemory(&m_xState, sizeof(XINPUT_STATE));

	DWORD con = XInputGetState(m_nPlayerNum, &m_xState);

	if(con == ERROR_SUCCESS)
		return true;
	else
		return false;
}
