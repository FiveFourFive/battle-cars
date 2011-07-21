#include "Gamer_Profile.h"

Gamer_Profile::Gamer_Profile()
{

}

Gamer_Profile::~Gamer_Profile()
{
	if( m_pKB )
	{
		delete this->m_pKB;
		m_pKB = NULL;
	}

	if( m_pKKB)
	{
		delete this->m_pKKB;
		m_pKKB = NULL;
	}
}

Gamer_Profile& Gamer_Profile::operator()(const Gamer_Profile& temp )
{
	if( this != &temp)
	{
		this->m_sUserName = temp.m_sUserName;
		this->m_sfx = temp.m_sfx;
		this->m_background = temp.m_background;

		CKeyBinds* temp_controllerbinds = new CKeyBinds();
		temp_controllerbinds->SetAccelerate(temp.m_pKB->GetAccelerate());
		temp_controllerbinds->SetAccept(temp.m_pKB->GetAccept());
		temp_controllerbinds->SetBack(temp.m_pKB->GetBack());
		temp_controllerbinds->SetBreakReverse(temp.m_pKB->GetBreakReverse());
		temp_controllerbinds->SetChangeWeapon(temp.m_pKB->GetChangeWeapon());
		temp_controllerbinds->SetShoot(temp.m_pKB->GetShoot());

		this->m_pKB = temp_controllerbinds;

		CKeyboardKeyBinds* temp_keyboardbinds = new CKeyboardKeyBinds();
		temp_keyboardbinds->SetAccept(temp.m_pKKB->GetAccept());
		temp_keyboardbinds->SetBack(temp.m_pKKB->GetBack());
		temp_keyboardbinds->SetBackward(temp.m_pKKB->Getbackward());
		temp_keyboardbinds->SetChangeWeapon(temp.m_pKKB->GetChangeWeapon());
		temp_keyboardbinds->SetForward(temp.m_pKKB->Getforward());
		temp_keyboardbinds->SetLeft(temp.m_pKKB->GetLeft());
		temp_keyboardbinds->SetRight(temp.m_pKKB->GetRight());
		temp_keyboardbinds->SetShoot(temp.m_pKKB->GetShoot());

		this->m_pKKB = temp_keyboardbinds;
	}

	return *this;
}