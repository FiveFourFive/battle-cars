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

CCreateMiniSpecialMessage::CCreateMiniSpecialMessage(CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_MINI_SPECIAL)
{
	m_pPlayer = pPlayer;
}

CCreateTruckSpecialMessage::CCreateTruckSpecialMessage(CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_TRUCK_SPECIAL)
{
	m_pPlayer = pPlayer;
}

CCreateVetteSpecialMessage::CCreateVetteSpecialMessage(CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_VETTE_SPECIAL)
{
	m_pPlayer = pPlayer;
}

CCreateHummerSpecialMessage::CCreateHummerSpecialMessage(CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_HUMMER_SPECIAL)
{
	m_pPlayer = pPlayer;
}

CCreateEnemyBulletMessage::CCreateEnemyBulletMessage(CEnemy* pEnemy) : CBaseMessage(MSG_CREATE_ENEMY_BULLET_MESSAGE)
{
	m_pEnemy = pEnemy;
}


CCreateBossMiniSpecial::CCreateBossMiniSpecial(CBoss* pBoss) : CBaseMessage(MSG_CREATE_BOSS_MINI_SPECIAL)
{
	m_pBoss = pBoss;
}

CCreateBossVetteSpecial::CCreateBossVetteSpecial(CBoss* pBoss) : CBaseMessage(MSG_CREATE_BOSS_VETTE_SPECIAL)
{
	m_pBoss = pBoss;
}

CCreateBossHummerSpecial::CCreateBossHummerSpecial(CBoss* pBoss) : CBaseMessage(MSG_CREATE_BOSS_HUMMER_SPECIAL)
{
	m_pBoss = pBoss;
}

CCreateBossTruckSpecial::CCreateBossTruckSpecial(CBoss* pBoss) : CBaseMessage(MSG_CREATE_BOSS_TRUCK_SPECIAL)
{
	m_pBoss = pBoss;
}

CCreateMiniBossSpecial::CCreateMiniBossSpecial(CBoss* pBoss) : CBaseMessage(MSG_CREATE_MINIBOSS_SPECIAL)
{
	m_pBoss = pBoss;
}

CDestroyBossMessage::CDestroyBossMessage(CBoss* pBoss) : CBaseMessage(MSG_DESTROY_BOSS)
{
	m_pBoss = pBoss;
}