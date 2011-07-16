#ifndef LEVEL_H_
#define LEVEL_H_

#include "IGameState.h"

#include <string>

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;

enum {LEVEL_ONE, LEVEL_TWO, LEVEL_THREE, LEVEL_MAX};

struct SLevel
{
	int ImageID;
	std::string FileName;
};

class CLevelSelectionState : public IGameState
{
private:
	CSGD_TextureManager*	m_pTM;
	CPrintFont*				m_pPF;
	CSGD_FModManager*		m_pFM;
	CSGD_DirectInput*		m_pDI;

	int	m_nFontID;
	int m_nSelection;
	int m_nMenuSelect;
	int m_nMenuMove;

	int m_nBGImageID;

	SLevel* m_ListofLevels[LEVEL_MAX];

	CLevelSelectionState();
	~CLevelSelectionState();
	CLevelSelectionState& operator=(const CLevelSelectionState&);
	CLevelSelectionState( const CLevelSelectionState&);

public:
	static CLevelSelectionState* GetInstance();
	void Enter(void);
	void Exit(void);

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool HandleEnter(void);

	///////////////////////////////////////////////////
	// Loads the level filename and the level's image
	//////////////////////////////////////////////////
	bool LoadLevel(const char* szXmlFileName);

	int GetSelection(){ return m_nSelection;}
	SLevel* GetLevel(int index){ return m_ListofLevels[index];}

};

#endif