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
enum MsgTypes { MSG_NULL = 0, MSG_CREATE_LEVEL, MSG_DESTROY_LEVEL, MSG_CREATE_PLAYER_BULLET,MSG_DESTROY_BULLET, MSG_MAX };

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

#endif