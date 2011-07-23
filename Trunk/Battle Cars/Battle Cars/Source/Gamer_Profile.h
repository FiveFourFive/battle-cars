#ifndef GAMER_PROFILE_H_
#define GAMER_PROFILE_H_


#include "CKeyBinds.h"
#include "CKeyboardKeyBinds.h"

#include <string>
using namespace std;

class Gamer_Profile
{
public:
	Gamer_Profile();
	~Gamer_Profile();

	string m_sUserName;
	CKeyBinds* m_pKB;
	CKeyboardKeyBinds* m_pKKB;
	float m_sfx;
	float m_background;
	int cars[4];
	int index; // the 0-2 value of which profile slot its saved in

	string GetUserName(){ return m_sUserName;}
	CKeyBinds* GetControllerBinds(){ return m_pKB;}
	CKeyboardKeyBinds* GetKeyboardBinds(){ return m_pKKB;}

	Gamer_Profile& operator()(const Gamer_Profile& temp);

};

#endif