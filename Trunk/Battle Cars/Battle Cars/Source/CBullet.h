
#ifndef _CBULLET_H
#define _CBULLET_H

#include "CBase.h"


class CPlayer;
class CSGD_TextureManager;
class CCamera;
class Emittor;

enum bullets{PROJECTILE_BULLET, PROJECTILE_MISSILE, PROJECTILE_MINI_MISSILE, PROJECTILE_LANDMINE};
class CBullet : public CBase
{
private:

	CSGD_TextureManager* m_pTM;
	CPlayer* m_pOwner;
	float m_fVelX;
	float m_fVelY;
	float m_fRotation;
	float m_fScale;
	float m_nDamage;
	float m_fCurLife;
	float m_fMaxLife;
	float m_fBlastRadius;
	int m_nBulletType;
	float m_fSlowRate;

	bool m_bToRender;				// Used to not render flamethrower.
	
	int trace_particle;				// The index to the particle effect.

public:
	CBullet(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);


	float GetVelX(void)	{return m_fVelX;}
	float GetVelY(void)	{return m_fVelY;}
	float GetRotation(void) { return m_fRotation; }
	float GetScale(void) { return m_fScale; }
	float GetCurLife(void) { return m_fCurLife; }
	float GetMaxLife(void) { return m_fMaxLife; }
	float GetDamage() {return m_nDamage;}
	float GetBlastRadius() {return m_fBlastRadius;}
	float GetSlowRate() {return m_fSlowRate;}
	int GetBulletType() {return m_nBulletType;}
	bool ToRender(){ return m_bToRender;}
	CPlayer* GetOwner(void) { return m_pOwner; }
	CSGD_TextureManager* GetTextureManager() {return m_pTM;}

	void SetCurLife(float life) { m_fCurLife = life; }
	void SetMaxLife(float maxlife) { m_fMaxLife = maxlife; }
	void SetVelX(float velx) { m_fVelX = velx; }
	void SetVelY(float vely) { m_fVelY = vely; }
	void SetScale(float scale) { m_fScale = scale; }
	void SetRotation(float rot) { m_fRotation = rot; }
	void SetOwner(CPlayer* own) { m_pOwner = own; }
	void SetDamage(float dam) {m_nDamage = dam;}
	void SetBulletType(int type) {m_nBulletType = type;}
	void SetBlastRadius(float radius) {m_fBlastRadius = radius;}
	void SetSlowRate(float rate) {m_fSlowRate = rate;}
	void SetToRender(bool value){m_bToRender = value;}
	bool CheckCollision(IBaseInterface* pBase);

	void SetTracerEmittor(int index){ trace_particle = index;}
	int GetTracerEmittor(){ return trace_particle;}
};


#endif