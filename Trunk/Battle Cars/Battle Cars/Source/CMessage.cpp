/////////////////////////////////////////////////
//	File	:	"CMessage.cpp"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the information for creating
//				a message
//				
/////////////////////////////////////////////////

#include "CMessage.h"

CCreateLevelMessage::CCreateLevelMessage(void) : CBaseMessage(MSG_CREATE_LEVEL)
{

}

CDestroyLevelMessage::CDestroyLevelMessage (CLevel* pLevel) : CBaseMessage(MSG_DESTROY_LEVEL)
{
	m_pLevel = pLevel;
}

CLevel* CDestroyLevelMessage::GetLevel ()
{
	return m_pLevel;
}

CCreatePlayerBulletMessage::CCreatePlayerBulletMessage(CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_PLAYER_BULLET)
{
	m_pPlayer = pPlayer;
}

CPlayer* CCreatePlayerBulletMessage::GetPlayer(void)
{
	return m_pPlayer;
}

CDestroyBulletMessage::CDestroyBulletMessage(CBullet* pBullet) : CBaseMessage(MSG_DESTROY_BULLET)
{
	m_pBullet = pBullet;
}

CBullet* CDestroyBulletMessage::GetBullet(void)
{

	return m_pBullet;
}

CCreatePlayerMissileMessage::CCreatePlayerMissileMessage(CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_PLAYER_MISSILE)
{
	m_pPlayer = pPlayer;
}

CPlayer* CCreatePlayerMissileMessage::GetPlayer(void)
{
	return m_pPlayer;
}