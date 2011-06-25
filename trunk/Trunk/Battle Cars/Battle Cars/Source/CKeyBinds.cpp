
#include "CKeyBinds.h"
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")
#include <XInput.h>
CKeyBinds::CKeyBinds(void)
{
	shoot_accept = XINPUT_GAMEPAD_X;
	change_weapon = XINPUT_GAMEPAD_LEFT_SHOULDER;
	/*accelerate ;
	break_reverse;*/

}

CKeyBinds::~CKeyBinds(void)
{

}

void CKeyBinds::ResetDefaults(void)
{
	shoot_accept = XINPUT_GAMEPAD_X;
	change_weapon = XINPUT_GAMEPAD_LEFT_SHOULDER;
}