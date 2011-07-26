#include "Emittor.h"
#include "CBase.h"
#include "CCamera.h"
#include "CSGD_TextureManager.h"
#include "CBullet.h"
#include "CCar.h"
#include "CGame.h"


Emittor::Emittor()
{
	position.fX = 0.0f;
	position.fY = 0.0f;
	acceleration.fX = 0.0f;
	acceleration.fY = 0.0f;
	minVelocity.fX = 0.0f;
	minVelocity.fY = 0.0f;
	maxVelocity.fX = 0.0f;
	maxVelocity.fY = 0.0f;

	isActive = false;
	isDead = true;

	ID = 0;

	m_fCurrLife = 0.0f;
}

Emittor::~Emittor()
{
	for( unsigned int i = 0; i < m_vParticleList.size(); i++)
	{
		if( m_vParticleList[i] != NULL)
		{
			delete m_vParticleList[i];
			m_vParticleList[i] = NULL;
		}
	}
	m_vParticleList.clear();
}

void Emittor::Update(float fElapsedTime)
{
	m_fCurrLife += fElapsedTime;
	
        for (unsigned int i = 0; i < m_vParticleList.size(); i++)
        {
			m_vParticleList[i]->spawn_timer += fElapsedTime;

			if( IsBursting() == false )
			{
				if (m_vParticleList[i]->spawn_timer <= m_vParticleList[i]->spawnDelay)
					continue;
				else
				{
					m_vParticleList[i]->isDead = false;

				}
			}
			else
			{
					m_vParticleList[i]->isDead = false;

			}

			m_vParticleList[i]->position.fX += (m_vParticleList[i]->velocity.fX + (this->acceleration.fX * fElapsedTime));
			m_vParticleList[i]->position.fY += (m_vParticleList[i]->velocity.fY + (this->acceleration.fY * fElapsedTime));
            
			if( base )
			{
				if( base->GetType() == OBJECT_BULLET )
				{
					CBullet* bullet = (CBullet*)base;
					m_vParticleList[i]->position.fX += -(bullet->GetVelX() * 0.01f);
					m_vParticleList[i]->position.fY += -(bullet->GetVelY() * 0.01f);
				}
			}

			m_vParticleList[i]->currLife += fElapsedTime;
			m_vParticleList[i]->colorfade_timer += fElapsedTime;
			m_vParticleList[i]->scaleX_timer += fElapsedTime;
			m_vParticleList[i]->scaleY_timer += fElapsedTime;

				m_vParticleList[i]->rotation += this->rotation * fElapsedTime;
            if (m_vParticleList[i]->rotation >= 6.28f)
                m_vParticleList[i]->rotation = 0.0f;

               
            UpdateColor(i);
            UpdateScale(i);

			if (m_vParticleList[i]->colorfade_timer >= m_vParticleList[i]->maxlife)
                m_vParticleList[i]->colorfade_timer = 0.0f;
            if (m_vParticleList[i]->scaleX_timer >= m_vParticleList[i]->maxlife)
                m_vParticleList[i]->scaleX_timer = 0.0f;
            if (m_vParticleList[i]->scaleY_timer >= m_vParticleList[i]->maxlife)
                m_vParticleList[i]->scaleY_timer = 0.0f;

				
                if (m_vParticleList[i]->currLife > m_vParticleList[i]->maxlife)
                {
					if( m_isContinuous)
					{
						m_vParticleList[i]->currLife = 0.0f;

						m_vParticleList[i]->position = position;
						m_vParticleList[i]->velocity.fX = (RAND_FLOAT(minVelocity.fX, maxVelocity.fX)) * fElapsedTime;
						m_vParticleList[i]->velocity.fY = (RAND_FLOAT(minVelocity.fY, maxVelocity.fY)) * fElapsedTime;
						m_vParticleList[i]->acceleration = acceleration;
						m_vParticleList[i]->color = m_StartColor;
						m_vParticleList[i]->scaleX = m_StartScaleX;
						m_vParticleList[i]->scaleY = m_StartScaleY;
						m_vParticleList[i]->rotation = 0.0f;

						m_vParticleList[i]->spawn_timer = 0.0f;
						m_vParticleList[i]->isDead = true;
					}
					else
						m_vParticleList[i]->isDead = true;
				
				}
		}

}

void Emittor::Render(CCamera* camera)
{
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	if( (this->position.fX - camera->GetCamX()) < 0 || (this->position.fX - camera->GetCamX()) > camera->GetRenderPosX() + camera->GetWidth()
		|| ( this->position.fY - camera->GetCamY() + 2 < 0) || (this->position.fY - camera->GetCamY()) > camera->GetRenderPosY() + camera->GetHeight())
		return;

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	int temp_source = 0;
	int temp_destination = 0;
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND,(DWORD*)&temp_source);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND,(DWORD*)&temp_destination);

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, this->SourceBlend);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, this->DestinationBlend);

	for( unsigned int i = 0; i < m_vParticleList.size(); i++)
	{
		if( !m_vParticleList[i]->isDead)
		{
			
			// Renders each individual particle.
			pTM->Draw(m_nTextureID, (int)m_vParticleList[i]->position.fX - (int)camera->GetCamX() + (int)camera->GetRenderPosX(), (int)m_vParticleList[i]->position.fY - (int)camera->GetCamY()+ (int)camera->GetRenderPosY(), m_vParticleList[i]->scaleX, m_vParticleList[i]->scaleY,
				NULL, (float)pTM->GetTextureWidth(this->GetTextureID())*0.5f, (float)pTM->GetTextureHeight(this->GetTextureID()) *0.5f, m_vParticleList[i]->rotation, m_vParticleList[i]->color);
		}
	}

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, temp_source);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, temp_destination);
}

void Emittor::ClearParticleList()
{
	for(unsigned int i = 0; i < m_vParticleList.size(); i++)
	{
		if( m_vParticleList[i])
		{
			delete m_vParticleList[i];
			m_vParticleList[i] = NULL;
		}
	}

	m_vParticleList.clear();
}

void Emittor::UpdateColor(int i)
{


			tempA = ((m_StartColor >> 24) - (m_EndColor >> 24));
			tempR = ((m_StartColor >> 16) & 0xFF) -  ((m_EndColor >> 16) & 0xFF);
			tempG = ((m_StartColor >> 8) & 0xFF) - ((m_EndColor >> 8) & 0xFF);
			tempB = (m_StartColor & 0xFF) - (m_EndColor & 0xFF);


				tempA = ((m_StartColor >> 24) - (int)(tempA * (m_vParticleList[i]->colorfade_timer / m_fEndLife)));

                tempR = (((m_StartColor >> 16) & 0xFF) - (int)(tempR * (m_vParticleList[i]->colorfade_timer / m_fEndLife)));
																					
                tempG = (((m_StartColor >> 8 ) & 0xFF) - (int)(tempG * (m_vParticleList[i]->colorfade_timer / m_fEndLife)));                       
                                                  								
                tempB = ((m_StartColor & 0xFF) - (int)(tempB * (m_vParticleList[i]->colorfade_timer / m_fEndLife)));

                if (tempA > 255)
                    tempA = 255;
                if (tempR > 255)
                    tempR = 255;
                if (tempG > 255)
                    tempG = 255;
                if (tempB > 255)
                    tempB = 255;

                if (tempA < 0)
                    tempA = 0;
                if (tempR < 0)
                    tempR = 0;
                if (tempG < 0)
                    tempG = 0;
                if (tempB < 0)
                    tempB = 0;
            

				m_vParticleList[i]->color = D3DCOLOR_ARGB(tempA, tempR, tempG, tempB);
}

void Emittor::UpdateScale(int i)
{
			float scaleX = 0;

			scaleX = (m_StartScaleX - m_EndScaleX);
			scaleX = (m_StartScaleX - (scaleX * (m_vParticleList[i]->scaleX_timer / m_fEndLife)));

			m_vParticleList[i]->scaleX = scaleX;

            float scaleY = 0;

			scaleY = (m_StartScaleY - m_EndScaleY);
			scaleY = (m_StartScaleY - (scaleY * (m_vParticleList[i]->scaleY_timer / m_fEndLife)));

			m_vParticleList[i]->scaleY = scaleY;
}

void Emittor::SetBase(CBase* base)
{
	if( this != NULL)
	{
		this->base = base;
	}
}

void Emittor::InitializeEmittor()
{
	if( this == NULL)
	{
		MessageBox(0, "Failed to Initialize Emittor, emittor's value is NULL", 0, 0);
		return;
	}

	this->SetCurrLife(0.0f);
	this->SetTimeToDie(1.0f);

	std::string directory = "Resource/Graphics/";
	directory += imagename;
	SetTextureID( CSGD_TextureManager::GetInstance()->LoadTexture(directory.c_str(), D3DCOLOR_XRGB(0,0,0)));

	InitializeParticleList();
	
}

void Emittor::InitializeParticleList()
{
	m_vParticleList.clear();

	for (int i = 0; i < this->m_nMaxNumber; i++)
        {

            Particle* temp = new Particle();

			temp->position = this->position;
			temp->color = this->m_StartColor;

			temp->currLife = 0.0f;
			temp->maxlife = RAND_FLOAT(m_fMinLife, m_fEndLife);
			temp->spawnDelay = RAND_FLOAT(0.0f, 1.0f);
			
			temp->velocity.fX = (RAND_FLOAT(minVelocity.fX, maxVelocity.fX)) * CGame::GetInstance()->GetElapsedTime();
			temp->velocity.fY = (RAND_FLOAT(minVelocity.fY, maxVelocity.fY)) * CGame::GetInstance()->GetElapsedTime();
			temp->scaleX = m_StartScaleX;
			temp->scaleY = m_StartScaleY;
			temp->rotation = 0.0f;

			temp->colorfade_timer = 0.0f;
			temp->scaleX_timer = 0.0f;
			temp->scaleY_timer = 0.0f;
			temp->spawn_timer = 0.0f;

			temp->isDead = true;

			m_vParticleList.push_back(temp);
        }

	int count = 0;
	while( count < (int)(m_nMaxNumber * 0.4f) )
	{
		int index = rand()%m_nMaxNumber;
		m_vParticleList[index]->isDead = false;
		m_vParticleList[index]->spawn_timer = m_vParticleList[index]->spawnDelay;
		count++;
	}
}
