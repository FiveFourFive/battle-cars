#ifndef CAMERA_H_
#define CAMERA_H_

#include <Windows.h>

class CBase;

class CCamera
{
private:
	CBase* owner;

	float camPosX;
	float camPosY;

	float offX;
	float offY;

	float m_fRenderPosX;
	float m_fRenderPosY;

	int m_nWidth;
	int m_nHeight;
	
public:

	CCamera();
	~CCamera();

	float GetCamX(){ return camPosX;}			// Get Camera's X Postition
	float GetCamY(){ return camPosY;}			// Get Camera's Y Position
	
	void SetCamX( float posX ) { camPosX = posX; }
	void SetCamY( float posY ) { camPosY = posY; }

	void Update(void);
	void AttachTo( CBase* camera_owner) { owner = camera_owner;}
	//////////////////////////////////////////////////////////////////
	// The offset is how many units to move the cameras intial position over.
	//		ie: if you pass in 300 the camera's top left corner will move over 300
	//			units in which ever direction you specified.
	//////////////////////////////////////////////////////////////////
	void AttachTo( CBase* camera_owner, float offsetX, float offsetY);
	/////////////////////////////////////////////////////////////////
	//	Sets the owner to null nothing special.
	//////////////////////////////////////////////////////////////////
	void RemoveOwner() { owner = NULL;}

	float GetRenderPosX(){ return m_fRenderPosX; }
	float GetRenderPosY(){ return m_fRenderPosY; }
	int GetWidth(){ return m_nWidth;}
	int GetHeight() { return m_nHeight;}

	void SetRenderPosX(float xPos){ m_fRenderPosX = xPos; }
	void SetRenderPosY(float yPos){ m_fRenderPosY = yPos; }

	CBase* GetOwner() { return owner;}
	
	RECT GetRect();
};

#endif