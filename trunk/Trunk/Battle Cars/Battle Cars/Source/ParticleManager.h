#ifndef PARTICLEMANAGER_H_
#define PARTICLEMANAGER_H_

#include <vector>
using std::vector;

class Emittor;
class CBase;
class CCamera;

enum emittor_types{ COLLISION_EMITTOR, MISSLE_EMITTOR, EXPLOSION_SMOKE_EMITTOR, EXPLOSION_FIREBURST1_EMITTOR,
	EXPLOSION_FIREBURST2_EMITTOR, EXPLOSION_FIREBURST3_EMITTOR, EXPLOSION_FLAME_EMITTOR, AFTEREXPLOSION_SMOKE_EMITTOR, AFTEREXPLOSION_FLAME_EMITTOR,
	FLAMETHROWER_EMITTOR, BARREL_EMITTOR, TOTAL_EMITTOR};

class ParticleManager
{
private:

	static ParticleManager* instance;	// make it a singleton
	vector<Emittor*> m_GameEmittors;	// All the emittors used for the game will be contained here.
	vector< Emittor*> m_ActiveEmittors;	// Active Emittors currently being used.

	int Count;
	int ActiveCount;

	// Constructor
	ParticleManager();

	// Trilogy of Evil:
	//				Destructor
	//				Assignment Operator
	//				Copy Constructor
	~ParticleManager();
	ParticleManager& operator=(const ParticleManager& );
	ParticleManager(const ParticleManager& );

public:
	
	////////////////////////////////////////////////
	// 
	// Return: The Instance of the Particle Manager
	//
	// Input : Void
	//
	// Used to grab instance of particle manager to make function calls.
	////////////////////////////////////////////////
	static ParticleManager* GetInstance();

	static void DeleteInstance();	// Never actually used, do not call this function without the right authority.


	////////////////////////////////////////////////
	// 
	// Return: void
	//
	// Input : delta time
	//
	// Calls each emittors individual update.
	////////////////////////////////////////////////
	void UpdateEmittors(float fElapsedTime);

	////////////////////////////////////////////////
	// 
	// Return: void
	//
	// Input : void
	//
	// Calls each emittors individual render.
	////////////////////////////////////////////////
	void RenderEmittors(CCamera* camera);

	////////////////////////////////////////////////
	// 
	// Return: true if load was succesful, false otherwise
	//
	// Input : The name of the file to load from
	//
	// Will load the emittors data and then add it to the list
	//	if everything was loaded correctly.
	////////////////////////////////////////////////
	bool LoadEmittor(const char* FileName);

	//////////////////////////////////////////////////
	//
	// Return: void
	//
	// Input: the base object to attach to, and the emittor that will be attaching
	//
	// This will Set the emittors position to the base position and "attach" it 
	//////////////////////////////////////////////////////////
	void AttachToBase(CBase* base, Emittor* emittor);

	//////////////////////////////////////////////////
	//
	// Return: void
	//
	// Input: the offset
	//
	// This will set the emittors position to an offset of the Base,
	//	as well as set the base as the emittors base.
	//////////////////////////////////////////////////
	void AttachToBasePosition(CBase* base, Emittor* emittor, float offsetX = 0, float offsetY = 0);

	///////////////////////////////////////////////////
	//
	// Return: Emittor*
	//
	// Input: Emittor*
	//
	// This will create a new emittor without overriding the default loaded onces.
	// Purpose is to reuse the effect and create multiple effects without overriding each other.
	////////////////////////////////////////////////////
	Emittor* CreateEffect( Emittor* temp_emittor, float posX, float posY, float accelX = 0.0f, float accelY = 0.0f);

	void ShutDownParticleManager();

	Emittor* GetEmittor(int id);
	Emittor* GetActiveEmittor(int id);

};

#endif