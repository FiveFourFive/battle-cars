#ifndef _CHUD_H
#define _CHUD_H


class CPlayer;

class CHUD
{
private:
	CPlayer* m_pOwner;

public:
	CHUD(void);
	~CHUD(void);

	void Update(float fElapsedTime);
	void Render(void);

	void SetOwner(CPlayer* owner) { m_pOwner = owner; }

};


#endif