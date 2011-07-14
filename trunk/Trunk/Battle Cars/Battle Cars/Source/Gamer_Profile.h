#ifndef GAMER_PROFILE_H_
#define GAMER_PROFILE_H_

#include <cstring>
using namespace std;
class CKeyBinds;
class CKeyboardKeyBinds;
class Gamer_Profile
{
public:
	string m_sUserName;
	CKeyBinds* m_pKB;
	CKeyboardKeyBinds* m_pKKB;

	string GetUserName(){ return m_sUserName;}
	CKeyBinds* GetControllerBinds(){ return m_pKB;}
	CKeyboardKeyBinds* GetKeyboardBinds(){ return m_pKKB;}
};

#endif