#include "Emittor.h"
#include "CBase.h"
#include "CCamera.h"
#include "CSGD_TextureManager.h"

namespace temp_variables
{
	int temp_alpha;
	int temp_red;
	int temp_green;
	int temp_blue;

	bool isSet = false;
}

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

	m_fCurrLife = 0.0f;
}

Emittor::~Emittor()
{
	for( unsigned int i = 0; i < m_vParticleList.size(); i++)
	{
		delete m_vParticleList[i];
		m_vParticleList[i] = NULL;
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
					m_vParticleList[i]->isDead = false;
			}
			else
					m_vParticleList[i]->isDead = false;

			m_vParticleList[i]->position.fX += (m_vParticleList[i]->velocity.fX + acceleration.fX * fElapsedTime);
			m_vParticleList[i]->position.fY += (m_vParticleList[i]->velocity.fY + acceleration.fY * fElapsedTime);
                

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
						m_vParticleList[i]->color = m_StartColor;
						m_vParticleList[i]->scaleX = m_StartScaleX;
						m_vParticleList[i]->scaleY = m_StartScaleY;

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
			pTM->Draw(m_nTextureID, (int)m_vParticleList[i]->position.fX - (int)(pTM->GetTextureWidth(this->GetTextureID())*0.5f) - (int)camera->GetCamX(), (int)m_vParticleList[i]->position.fY - (int)(pTM->GetTextureHeight(this->GetTextureID())*0.5f) - (int)camera->GetCamY(), m_vParticleList[i]->scaleX, m_vParticleList[i]->scaleY,
				NULL, (float)pTM->GetTextureWidth(this->GetTextureID()), (float)pTM->GetTextureHeight(this->GetTextureID()), m_vParticleList[i]->rotation, m_vParticleList[i]->color);
		}
	}

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, temp_source);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, temp_destination);
}

void Emittor::UpdateColor(int i)
{
			int tempA = 0;
            int tempR = 0;
            int tempG = 0;
            int tempB = 0;

	// Possible way to get color values.
	if( temp_variables::isSet == false)
	{
		temp_variables::isSet = true;
		temp_variables::temp_alpha = ((m_StartColor >> 24) - (m_EndColor >> 24));
		temp_variables::temp_red = ((m_StartColor >> 16) & 0xFF) -  ((m_EndColor >> 16) & 0xFF);
		temp_variables::temp_green = ((m_StartColor >> 8) & 0xFF) - ((m_EndColor >> 8) & 0xFF);
		temp_variables::temp_blue = (m_StartColor & 0xFF) - (m_EndColor & 0xFF);
	}

				tempA = ((m_StartColor >> 24) - (int)(temp_variables::temp_alpha * (m_vParticleList[i]->colorfade_timer / m_fEndLife)));

                tempR = (((m_StartColor >> 16) & 0xFF) - (int)(temp_variables::temp_red * (m_vParticleList[i]->colorfade_timer / m_fEndLife)));
																					
                tempG = (((m_StartColor >> 8 ) & 0xFF) - (int)(temp_variables::temp_green * (m_vParticleList[i]->colorfade_timer / m_fEndLife)));                       
                                                  								
                tempB = ((m_StartColor & 0xFF) - (int)(temp_variables::temp_blue * (m_vParticleList[i]->colorfade_timer / m_fEndLife)));

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
	this->base = base;;
}

void Emittor::InitializeEmittor()
{
	if( this == NULL)
	{
		MessageBox(0, "Failed to Initialize Emittor, emittor's value is NULL", 0, 0);
		return;
	}
	
	std::string directory = "Resource/Graphics/";
	directory += imagename;
	SetTextureID( CSGD_TextureManager::GetInstance()->LoadTexture(directory.c_str(), D3DCOLOR_XRGB(0,0,0)));

	m_vParticleList.clear();

	for (int i = 0; i < this->m_nMaxNumber; i++)
        {

            Particle* temp = new Particle();

			temp->position = this->position;
			temp->color = this->m_StartColor;

			temp->currLife = 0.0f;
			temp->maxlife = RAND_FLOAT(m_fMinLife, m_fEndLife);
			temp->spawnDelay = RAND_FLOAT(0.0f, 1.0f);
			
			temp->velocity.fX = (RAND_FLOAT(minVelocity.fX, maxVelocity.fX)) * 0.01f;
			temp->velocity.fY = (RAND_FLOAT(minVelocity.fY, maxVelocity.fY)) * 0.01f;
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
}