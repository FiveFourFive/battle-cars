#ifndef LEVEL_H_
#define LEVEL_H_

#include "IGameState.h"
#include "CSGD_Direct3D.h"

#include <string>

class CSGD_TextureManager;
class CPrintFont;
class CSGD_FModManager;
class CSGD_DirectInput;
class CXboxInput;

enum {LEVEL_ONE, LEVEL_TWO, LEVEL_THREE, LEVEL_FOUR,LEVEL_MAX};

struct SLevel
{
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
	
	int m_nMovieID[12];		// The image ID's for the video being played
	int render_ids[3];		// The id's currently being rendered.
	float m_fMovieTimer;	// The timer until the movie image changes.
	D3DCOLOR color;			// The fading color value.
	bool isSet;				// Tell whether to load video or not.
	int m_nCurrentFrame;	// Current video frame to render.

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
	void LoadVideo();

	///////////////////////////////////////////////////
	// Loads the level filename and the level's image
	//////////////////////////////////////////////////
	bool LoadLevel(const char* szXmlFileName);

	int GetSelection(){ return m_nSelection;}
	SLevel* GetLevel(int index){ return m_ListofLevels[index];}

};

#endif