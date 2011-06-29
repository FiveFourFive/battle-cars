
#include "CKeyBinds.h"
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")
#include <XInput.h>
CKeyBinds::CKeyBinds(void)
{
	ResetDefaults();

}

CKeyBinds::~CKeyBinds(void)
{

}

void CKeyBinds::ResetDefaults(void)
{
	shoot = XINPUT_GAMEPAD_X;
	accept = XINPUT_GAMEPAD_A;
	change_weapon = XINPUT_GAMEPAD_LEFT_SHOULDER;
	back = XINPUT_GAMEPAD_B;
	shoot_trigger = false;
	accept_trigger = false;
	back_trigger = false;
	change_trigger = false;
	accel_trigger = true;
	break_trigger = true;
}