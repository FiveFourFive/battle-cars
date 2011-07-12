#ifndef EMITTOR_H_
#define EMITTOR_H_

#include "SGD_Math.h"
#include "CSGD_Direct3D.h"

#include <vector>
#include <cstring>
using std::vector;

class CBase;
class CCamera;


#define RAND_FLOAT(min,max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))

struct Particle
{
	tVector2D position;
	tVector2D velocity;
	D3DCOLOR color;

	float rotation;
	float currLife;
	float maxlife;
	float scaleX;
	float scaleY;
	float spawnDelay;

	float spawn_timer;
	float colorfade_timer;
	float scaleX_timer;
	float scaleY_timer;

	bool isDead;
};

class Emittor
{
private:

	vector<Particle*> m_vParticleList;		//the list of particles in the emittor.

	int m_nTextureID;			// The Image of the texture;

	float m_fCurrLife;			// the current life of the emittor.
	float m_fMinLife;			// the min value for how long the emittor is alive for
	float m_fEndLife;			// the max value for how long the emittor is alive for.

	D3DCOLOR m_StartColor;		// The starting color of the emittor to be set for all particles.
	D3DCOLOR m_EndColor;		// The ending color of the emittor for the particles to blend to.

	int m_nMaxNumber;			// the max Number of particles this emittor will have.
								//		same as number of current particles in emittor.

	tVector2D position;			// The Emittors position.
	tVector2D acceleration;		// The emittors Acceleration.

	tVector2D minVelocity;		// the emittors min velocity.
	tVector2D maxVelocity;		// the emittors max velocity.

	float rotation;				// the emittors rotation.

	CBase* base;				// The base object itself that the particle may attach itself to.
								// if there is no base objects then the emittor uses its own position.
								// If there is a base object then the emittor's position is the base' position.

	bool isDead;				// Is the emittor itself Dead/InActive or is it Undead/Active.
	bool isActive;				// Is the emittor itself Active, if not then we can add it on to active list if its alive.
	
	float m_fTimeToDie;			// Variable used to store the time until the emittor dies. 

	float m_StartScaleX;				// The scale for each particle
	float m_StartScaleY;				// The scale for each particle
	float m_EndScaleX;					// The scale for each particle
	float m_EndScaleY;					// The scale for each particle

	bool m_isContinuous;				// Is the particle continuous
	bool m_isBursting;					// Is the particle Bursting

	int SourceBlend;					// Blend state for the Source
	int DestinationBlend;				// Blend state for the Destination
	std::string imagename;					// The name of the particle image

	int ID;								// the ID of the emittor


public:

	Emittor();
	~Emittor();

	///////////////////////////////////////
	//
	// Input: The time elapsed since last frame.
	//
	// Return: void
	//
	// This handles the update of all particles in the emittor.
	//////////////////////////////////////
	void Update(float fElapsedTime);

	///////////////////////////////////////////////
	//
	// Input: void
	//
	// Return: void
	//
	// This handles all the rendering of each particle.
	///////////////////////////////////////////////
	void Render(CCamera* camera);

	void UpdateColor(int i);

	void UpdateScale(int i);


	bool IsDead()
	{
		return isDead;
	}
	bool IsActive()
	{
		return isActive;
	}

	bool IsBursting()
	{
		return m_isBursting;
	}

	// Accessors
	int GetTextureID() { return m_nTextureID;}
	float GetCurrentLife() { return m_fCurrLife;}
	float GetTimeToDie() { return m_fTimeToDie;}
	int GetID(){ return ID;}
	tVector2D GetPosition(){ return position;}
	int GetMaxNumber(){ return m_nMaxNumber;}
	float GetMinLife(){ return m_fMinLife;}
	float GetEndLife(){ return m_fEndLife;}
	D3DCOLOR GetStartColor(){ return m_StartColor;}
	D3DCOLOR GetEndColor(){ return m_EndColor;}
	tVector2D GetAcceleration(){ return acceleration;}
	tVector2D GetMinVelocity(){ return minVelocity;}
	tVector2D GetMaxVelocity(){ return maxVelocity;}
	float GetRotation() { return rotation;}
	float GetStartScaleX(){ return m_StartScaleX;}
	float GetEndScaleX(){ return m_EndScaleX;}
	float GetStartScaleY(){ return m_StartScaleY;}
	float GetEndScaleY(){ return m_EndScaleY;}
	bool GetIsContinuous(){ return m_isContinuous;}
	int GetSourceBlend() { return SourceBlend;}
	int GetDestinationBlend() { return DestinationBlend;}
	std::string GetImageString(){ return imagename;}


	// Mutators
	void SetBase(CBase* base);
	void SetPosition(float x, float y) { position.fX = x; position.fY = y;}
	void SetTextureID(int id) { m_nTextureID = id;}
	void SetMinLife(float life) { m_fMinLife = life;}
	void SetEndLife(float life) { m_fEndLife = life;}
	void SetCurrLife(float life) { m_fCurrLife = life;}
	void SetStartColor(D3DCOLOR color) {m_StartColor = color;}
	void SetEndColor(D3DCOLOR color) { m_EndColor = color;}
	void SetMaxNumber(int num) { m_nMaxNumber = num;}
	void SetAcceleration(float x, float y) { acceleration.fX = x; acceleration.fY = y;}
	void SetMinVelocity(float x, float y) { minVelocity.fX = x; minVelocity.fY = y; }
	void SetMaxVelocity(float x, float y) { maxVelocity.fX = x; maxVelocity.fY = y;}
	void SetRotation(float rot) { rotation = rot;}
	void SetStartScaleX(float scale) { m_StartScaleX = scale;}
	void SetEndScaleX(float scale) { m_EndScaleX = scale; }
	void SetStartScaleY(float scale) { m_StartScaleY = scale;}
	void SetEndScaleY( float scale) { m_EndScaleY = scale;}
	void SetIsContinuous(bool value) { m_isContinuous = value;}
	void SetIsDead(bool value) { isDead = value;}
	void SetIsActive(bool value) { isActive = value;}
	void SetSourceBlend( int value){ SourceBlend = value;}
	void SetDestinationBlend(int value){ DestinationBlend = value;}
	void SetImageString( std::string filename) { imagename = filename;}
	void SetID( int id) { ID = id;}
	void SetTimeToDie(float time) { m_fTimeToDie = time;}
	void SetIsBursting(bool value){ m_isBursting = value;}

	void ResetElapsedEmitterTime() { m_fCurrLife = 0.0f;}
	void ClearParticleList(){ m_vParticleList.clear();}
	void AddToParticleList(Particle* particle){ m_vParticleList.push_back(particle);}

	////////////////////////////////////////////////////////////////////////
	//
	// Call this only after all of the emittors values have been initialized.
	//
	// Usually gets called if a load of a particle effect was succesful.
	//
	// On Second thought dont ever call this function
	///////////////////////////////////////////////////////////////////////
	void InitializeEmittor();
};

#endif