#include "CKeyboardKeyBinds.h"
#include "CSGD_DirectInput.h"
CKeyboardKeyBinds::CKeyboardKeyBinds(void)
{
	ResetDefaults();
}
CKeyboardKeyBinds::~CKeyboardKeyBinds(void)
{

}

void CKeyboardKeyBinds::ResetDefaults(void)
{
	shoot = DIK_SPACE;
	accept = DIK_RETURN;
	back = DIK_ESCAPE;
	change_weapon = DIK_LCONTROL;
}