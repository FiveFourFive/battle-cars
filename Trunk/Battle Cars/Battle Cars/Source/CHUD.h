#ifndef _CHUD_H
#define _CHUD_H


class CPlayer;
class CPrintFont;
class CSGD_TextureManager;
class CHUD
{
private:
	CPlayer* m_pOwner;
	CPrintFont* m_pPF;
	CSGD_TextureManager* m_pTM;

	int m_nMiniMapID;
	int m_nScoreBoardID;
	int m_nHealthID;
	int m_nPistolID;
	int m_nMissileID;

	float m_fMiniMapPosX;
	float m_fMiniMapPosY;
public:
	CHUD(void);
	~CHUD(void);

	void Update(float fElapsedTime);
	void Render(void);

	void SetOwner(CPlayer* owner) { m_pOwner = owner; }

	float GetMiniMapXPos(){ return m_fMiniMapPosX;}
	float GetMiniMapYPos(){ return m_fMiniMapPosY;}

};


#endif