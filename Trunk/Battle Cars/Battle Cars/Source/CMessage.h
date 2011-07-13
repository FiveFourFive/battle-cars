/////////////////////////////////////////////////
//	File	:	"CMessage.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the information for creating
//				a message
//				
/////////////////////////////////////////////////

#ifndef _CMESSAGE_H_
#define _CMESSAGE_H_

class CLevel;
class CBullet;
class CPlayer;
class CEnemy;
enum MsgTypes { MSG_NULL = 0, MSG_CREATE_LEVEL, MSG_DESTROY_LEVEL, MSG_CREATE_PLAYER_BULLET,MSG_DESTROY_BULLET, MSG_CREATE_PLAYER_MISSILE, 
	MSG_CREATE_MINI_SPECIAL, MSG_CREATE_TRUCK_SPECIAL, MSG_CREATE_VETTE_SPECIAL, MSG_CREATE_HUMMER_SPECIAL, MSG_CREATE_ENEMY_BULLET_MESSAGE, MSG_MAX };

class CBaseMessage
{
private:
	int	m_msgID;

public:
	CBaseMessage(int msgID)
	{
		m_msgID = msgID;
	}

	virtual ~CBaseMessage(void) {}

	int GetMsgID(void)	{ return m_msgID; }
};

class CCreateLevelMessage : public CBaseMessage
{
private:


public:
	CCreateLevelMessage(void);
};

class CDestroyLevelMessage : public CBaseMessage
{
private:
	CLevel* m_pLevel;

public:
	CDestroyLevelMessage(CLevel* pLevel);
	CLevel* GetLevel();
};

class CCreatePlayerBulletMessage : public CBaseMessage
{
private:
	CPlayer* m_pPlayer;

public:
	CCreatePlayerBulletMessage(CPlayer* pPlayer);
	CPlayer* GetPlayer(void);

};

class CDestroyBulletMessage : public CBaseMessage
{

private:
	CBullet* m_pBullet;
public:
	CDestroyBulletMessage(CBullet* pBullet);
	CBullet* GetBullet(void);
};

class CCreatePlayerMissileMessage : public CBaseMessage
{
private:
	CPlayer* m_pPlayer;

public:
	CCreatePlayerMissileMessage(CPlayer* pPlayer);
	CPlayer* GetPlayer(void);

};

class CCreateMiniSpecialMessage : public CBaseMessage
{
private:
	CPlayer* m_pPlayer;

public:
	CCreateMiniSpecialMessage(CPlayer* pPlayer);
	CPlayer* GetPlayer() {return m_pPlayer;}
};

class CCreateTruckSpecialMessage : public CBaseMessage
{
private:
	CPlayer* m_pPlayer;

public:
	CCreateTruckSpecialMessage(CPlayer* pPlayer);
	CPlayer* GetPlayer() {return m_pPlayer;}

};

class CCreateVetteSpecialMessage : public CBaseMessage
{
private:
	CPlayer* m_pPlayer;

public:
	CCreateVetteSpecialMessage(CPlayer* pPlayer);
	CPlayer* GetPlayer() {return m_pPlayer;}
};

class CCreateHummerSpecialMessage : public CBaseMessage
{
private:
	CPlayer* m_pPlayer;

public:
	CCreateHummerSpecialMessage(CPlayer* pPlayer);
	CPlayer* GetPlayer() {return m_pPlayer;}
};

class CCreateEnemyBulletMessage : public CBaseMessage
{
private:
	CEnemy* m_pEnemy;

public:
	CCreateEnemyBulletMessage(CEnemy* pEnemy);
	CEnemy* GetEnemy(void) {return m_pEnemy;}

};

#endif


