#ifndef GAMERPROFILE_H_
#define GAMERPROFILE_H_

#include "IGameState.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_FModManager;
class CPrintFont;

class Gamer_Profile;

#include <vector>
using namespace std;


class CGamerProfile : public IGameState
{
private:
	static CGamerProfile* instance;		// the instance of the gamer profile.

	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_FModManager*		m_pFM;
	CPrintFont*				m_pPF;

	vector<Gamer_Profile*> m_vUserProfiles;		// The vector of user profiles.
	Gamer_Profile* activeProfile;				// the current profile to save or edit;

	int BGMusicID;		// ID for the music background.

	int PosX;			// Position to render profiles at.
	int PosY[5];		// Position to render profiles at.

	int CurrPos;		// The current Position of the active menu item.
	bool editProfile;	// Editing profile?

	char temp;

	CGamerProfile();
	~CGamerProfile();
	CGamerProfile& operator = (const CGamerProfile&);
	CGamerProfile(const CGamerProfile&);

public:

	//////////////////////////////////////////////
	// retrieve the instance of the game state
	//////////////////////////////////////////////
	static CGamerProfile* GetInstance();

	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);

	void Enter();		// Load up everything needed for this game state.
	void Exit();		// Shutdown everything and deallocate memory that this program started.

	Gamer_Profile* GetActiveProfile(){ return activeProfile;}

	////////////////////////////////////////////////////////
	//	Loads up the Gamer profiles xml file. Only three profiles will be saved at once.
	////////////////////////////////////////////////////////
	bool LoadProfiles(const char* szXmlFileName);

	///////////////////////////////////////////////////////
	// Saves out the Gamer profile system. Only three profiles will be saved at once.
	///////////////////////////////////////////////////////
	bool SaveProfiles(const char* szXmlFileName);

	
	bool HandleEnter(void);
};

#endif