#include "ParticleManager.h"
#include "Emittor.h"
#include "CBase.h"
#include "CSGD_TextureManager.h"

#include <stdio.h>
#include "tinyxml.h"

ParticleManager* ParticleManager::instance = NULL;

ParticleManager::ParticleManager()
{
	Count = 0;
}

ParticleManager::~ParticleManager()
{
	
}

ParticleManager* ParticleManager::GetInstance()
{
	if( !instance )
			instance = new ParticleManager();
		return instance;
}

void ParticleManager::DeleteInstance()
{
	if( instance != NULL )
	{
		delete instance;
		instance = NULL;
	}
}

void ParticleManager::UpdateEmittors(float fElapsedTime)
{
	if( this == NULL)
		return;

	for( unsigned int i = 0; i < m_ActiveEmittors.size(); i++)
	{
		if( m_ActiveEmittors[i])
		{
			m_ActiveEmittors[i]->Update(fElapsedTime);

			if( m_ActiveEmittors[i]->GetCurrentLife() >= m_ActiveEmittors[i]->GetTimeToDie())
			{
				delete m_ActiveEmittors[i];
				m_ActiveEmittors.erase(m_ActiveEmittors.begin() + i, m_ActiveEmittors.begin() + i + 1);
			}
		}
	}

	for( unsigned int i = 0; i < m_GameEmittors.size(); i++)
	{
		if( m_GameEmittors[i] )
			
			if( m_GameEmittors[i]->GetCurrentLife() >= m_GameEmittors[i]->GetTimeToDie())
			{
				m_GameEmittors.erase(m_GameEmittors.begin() + i, m_GameEmittors.begin() + i+1);
				Count--;
			}
	}
}

void ParticleManager::RenderEmittors(CCamera* camera)
{
	for( unsigned int i = 0; i < m_ActiveEmittors.size(); i++)
	{
		m_ActiveEmittors[i]->Render(camera);
	}
}

bool ParticleManager::LoadEmittor( const char* FileName)
{
	// Temporarily load up in XML unless process is slow.
	TiXmlDocument doc;

	if(doc.LoadFile(FileName) == false)
	{
		MessageBox(0, "Failed to Load File", 0,0);
		return false;
	}

	TiXmlElement* pRoot = doc.RootElement();
	if( !pRoot)
	{
		MessageBox(0, "Failed to load XML Root", 0,0);
		return false;
	}

#pragma region XML_Variables
	TiXmlElement* xEmittor = pRoot->FirstChildElement("Emittor");

	while( xEmittor != NULL)
	{
		Emittor* temp = new Emittor();

		TiXmlElement* pNumParticles = xEmittor->FirstChildElement("NumberofParticles");
		if( pNumParticles)
		{
			temp->SetMaxNumber(atoi(pNumParticles->GetText()));
		}
		else 
			MessageBox(0, "Failed to Load Number of Particles", 0,0);

		TiXmlElement* pMinLifeSpan = xEmittor->FirstChildElement("MinLifeSpan");
		if( pMinLifeSpan)
		{
			temp->SetMinLife((float)atof(pMinLifeSpan->GetText()));
		}
		else 
			MessageBox(0, "Failed to Load Minimum Life Span of Particles", 0,0);

		TiXmlElement* pMaxLifeSpan = xEmittor->FirstChildElement("MaxLifeSpan");
		if( pMaxLifeSpan)
		{
			temp->SetEndLife((float)atof(pMaxLifeSpan->GetText()));
		}
		else 
			MessageBox(0, "Failed to Load Maximum Life Span of Particles", 0,0);

		TiXmlElement* pStartColor = xEmittor->FirstChildElement("StartColor");
		if( pStartColor)
		{
			int col[4] = {-1, -1, -1, -1};

			pStartColor->Attribute("A", &col[0]);
			pStartColor->Attribute("R", &col[1]);
			pStartColor->Attribute("G", &col[2]);
			pStartColor->Attribute("B", &col[3]);

			for( int i = 0; i < 4; i++)
			{
				if( col[i] == -1)
				{
					MessageBox(0, "ERROR: Failed to load color values",0,0);
				}
			}

			temp->SetStartColor(D3DCOLOR_ARGB(col[0], col[1], col[2], col[3]));
		}
		else 
			MessageBox(0, "Failed to Load Starting Color of Emittor", 0,0);

		TiXmlElement* pEndColor = xEmittor->FirstChildElement("EndColor");
		if( pEndColor)
		{
			int col[4] = {-1, -1, -1, -1};

			pEndColor->Attribute("A", &col[0]);
			pEndColor->Attribute("R", &col[1]);
			pEndColor->Attribute("G", &col[2]);
			pEndColor->Attribute("B", &col[3]);

			for( int i = 0; i < 4; i++)
			{
				if( col[i] == -1)
				{
					MessageBox(0, "ERROR: Failed to load color values",0,0);
				}
			}

			temp->SetEndColor(D3DCOLOR_ARGB(col[0], col[1], col[2], col[3]));
		}
		else 
			MessageBox(0, "Failed to Load Ending Color of Emittor", 0,0);

		TiXmlElement* pStartXScale = xEmittor->FirstChildElement("XStartScale");
		if(pStartXScale)
		{
			temp->SetStartScaleX( (float)atof(pStartXScale->GetText()));
		}
		else
			MessageBox(0, "Failed to load X Starting Scale", 0, 0);

		TiXmlElement* pEndXScale = xEmittor->FirstChildElement("XEndScale");
		if(pEndXScale)
		{
			temp->SetEndScaleX( (float)atof(pEndXScale->GetText()));
		}
		else
			MessageBox(0, "Failed to load X Ending Scale", 0, 0);

		TiXmlElement* pStartYScale = xEmittor->FirstChildElement("YStartScale");
		if(pStartXScale)
		{
			temp->SetStartScaleY( (float)atof(pStartYScale->GetText()));
		}
		else
			MessageBox(0, "Failed to load Y Starting Scale", 0, 0);

		TiXmlElement* pEndYScale = xEmittor->FirstChildElement("YEndScale");
		if(pEndXScale)
		{
			temp->SetEndScaleY( (float)atof(pEndYScale->GetText()));
		}
		else
			MessageBox(0, "Failed to load Y Ending Scale", 0, 0);

		TiXmlElement* pMinXVelocity = xEmittor->FirstChildElement("MinXVelocity");
		TiXmlElement* pMinYVelocity = xEmittor->FirstChildElement("MinYVelocity");
		if(pMinXVelocity && pMinYVelocity)
		{
			temp->SetMinVelocity((float)atof(pMinXVelocity->GetText()), (float)atof(pMinYVelocity->GetText()));
		}
		else
			MessageBox(0, "Failed to load Min Velocity", 0, 0);

		TiXmlElement* pMaxXVelocity = xEmittor->FirstChildElement("MaxXVelocity");
		TiXmlElement* pMaxYVelocity = xEmittor->FirstChildElement("MaxYVelocity");
		if(pMaxXVelocity && pMaxYVelocity)
		{
			temp->SetMaxVelocity((float)atof(pMaxXVelocity->GetText()), (float)atof(pMaxYVelocity->GetText()));
		}
		else
			MessageBox(0, "Failed to load Max Velocity", 0, 0);

		TiXmlElement* pXAccel = xEmittor->FirstChildElement("XAcceleration");
		TiXmlElement* pYAccel = xEmittor->FirstChildElement("YAcceleration");
		if(pXAccel && pYAccel)
		{
			temp->SetAcceleration((float)atof(pXAccel->GetText()), (float)atof(pYAccel->GetText()));
		}
		else
			MessageBox(0, "Failed to load Acceleration", 0, 0);

		TiXmlElement* pRotation = xEmittor->FirstChildElement("YAcceleration");
		if(pRotation)
		{
			temp->SetRotation((float)atof(pRotation->GetText()));
		}
		else
			MessageBox(0, "Failed to load Rotation for emittor", 0, 0);

		TiXmlElement* pSource = xEmittor->FirstChildElement("SourceBlend");
		if(pSource)
		{
			temp->SetSourceBlend(atoi(pSource->GetText()));
		}
		else
			MessageBox(0, "Failed to load Source Blend", 0, 0);

		TiXmlElement* pDestination = xEmittor->FirstChildElement("DestinationBlend");
		if(pDestination)
		{
			temp->SetDestinationBlend(atoi(pDestination->GetText()));
		}
		else
			MessageBox(0, "Failed to load Destination Blend", 0, 0);

		TiXmlElement* pImageName = xEmittor->FirstChildElement("ImageName");
		if(pImageName)
		{
			temp->SetImageString(pImageName->GetText());
		}
		else
			MessageBox(0, "Failed to load Particle Image Name", 0, 0);

		TiXmlElement* pRepeat = xEmittor->FirstChildElement("Continuous");
		if(pRepeat)
		{
			const char* repeat = pRepeat->GetText();

			if( strcmp(repeat, "true"))
				temp->SetIsContinuous(false);
			else
				temp->SetIsContinuous(true);
		}
		else
			MessageBox(0, "Failed to load if this was a Continuous Effect or not", 0, 0);

		TiXmlElement* xBursting = xEmittor->FirstChildElement("Bursting");
		if(pRepeat)
		{
			const char* burst = xBursting->GetText();

			if( strcmp(burst, "true"))
				temp->SetIsBursting(false);
			else
				temp->SetIsBursting(true);
		}
		else
			MessageBox(0, "Failed to load if this was a Bursting Effect or not", 0, 0);
	


#pragma endregion

		temp->SetBase(NULL);
		temp->InitializeEmittor();
		temp->SetID(Count);

		Count += 1;
		m_GameEmittors.push_back(temp);	// Pushes temp onto the Game emittor.

		xEmittor = xEmittor->NextSiblingElement();

	}
	

	return true; // If successful.
}

void ParticleManager::AttachToBase(CBase* base, Emittor* emittor)
{
	if( emittor == NULL)
		MessageBox(0, "ERROR: Emittor was never initialized",0,0);

	emittor->SetBase(base);
}

void ParticleManager::AttachToBasePosition(CBase* base, Emittor* emittor, float offsetX, float offsetY)
{
	if( base )
	{
		emittor->SetBase(base);
		emittor->SetPosition( base->GetPosX() + offsetX, base->GetPosY() + offsetY);
	}
	else
	{
		emittor->SetPosition(offsetX, offsetY);
	}

}

void ParticleManager::ShutDownParticleManager()
{
	for(size_t i = 0; i <m_GameEmittors.size(); i++)
	{
		delete m_GameEmittors[i];
		m_GameEmittors[i] = NULL;
		Count--;
	}
	m_GameEmittors.clear();

	/*for( unsigned int i = 0; i < m_ActiveEmittors.size(); i++)
	{
		if( m_ActiveEmittors[i] != NULL)
			delete m_ActiveEmittors[i];
	}*/
	m_ActiveEmittors.clear();
	
	if( Count != 0)
		MessageBox(0, "Count does not equal 0, possible leak of emittor or my terrible coding", 0,0);
	DeleteInstance();
}

Emittor* ParticleManager::GetEmittor(int id)
{
	for( unsigned int i = 0; i < m_GameEmittors.size(); i++)
	{
		if( m_GameEmittors[i]->GetID() == id)
			return m_GameEmittors[i];
	}

	return NULL;
}

Emittor* ParticleManager::CreateEffect( Emittor* temp_emittor, float posX, float posY)
{
	Emittor* new_emittor = new Emittor();
	new_emittor->SetPosition(posX, posY);
	new_emittor->SetMaxNumber(temp_emittor->GetMaxNumber());
	new_emittor->SetMinLife(temp_emittor->GetMinLife());
	new_emittor->SetEndLife(temp_emittor->GetEndLife());
	new_emittor->SetStartColor(temp_emittor->GetStartColor());
	new_emittor->SetEndColor(temp_emittor->GetEndColor());
	new_emittor->SetStartScaleX(temp_emittor->GetStartScaleX());
	new_emittor->SetEndScaleX(temp_emittor->GetEndScaleX());
	new_emittor->SetStartScaleY(temp_emittor->GetStartScaleY());
	new_emittor->SetEndScaleY(temp_emittor->GetEndScaleY());
	new_emittor->SetMinVelocity(temp_emittor->GetMinVelocity().fX, temp_emittor->GetMinVelocity().fY );
	new_emittor->SetMaxVelocity(temp_emittor->GetMaxVelocity().fX, temp_emittor->GetMaxVelocity().fY );
	new_emittor->SetAcceleration(temp_emittor->GetAcceleration().fX, temp_emittor->GetAcceleration().fY );
	new_emittor->SetRotation(temp_emittor->GetRotation());
	new_emittor->SetSourceBlend(temp_emittor->GetSourceBlend());
	new_emittor->SetDestinationBlend(temp_emittor->GetDestinationBlend());
	new_emittor->SetImageString(temp_emittor->GetImageString());
	new_emittor->SetIsContinuous(temp_emittor->GetIsContinuous());
	new_emittor->SetTextureID(temp_emittor->GetTextureID());
	new_emittor->SetIsDead(false);
	new_emittor->SetIsBursting(temp_emittor->IsBursting());
	Count++;
	new_emittor->SetBase(NULL);
	new_emittor->SetID(Count);
	m_ActiveEmittors.push_back(new_emittor);
	m_GameEmittors.push_back(new_emittor);

	new_emittor->ClearParticleList();

	for (int i = 0; i < new_emittor->GetMaxNumber(); i++)
        {

            Particle* temp = new Particle();

			temp->position = new_emittor->GetPosition();
			temp->color = new_emittor->GetStartColor();

			temp->currLife = 0;
			temp->maxlife = RAND_FLOAT(new_emittor->GetMinLife(), new_emittor->GetEndLife());

			temp->spawnDelay = RAND_FLOAT(0.0f, 1.0f);
			
			temp->velocity.fX = (RAND_FLOAT(new_emittor->GetMinVelocity().fX,new_emittor->GetMaxVelocity().fX)) * 0.01f;
			temp->velocity.fY = (RAND_FLOAT(new_emittor->GetMinVelocity().fY, new_emittor->GetMaxVelocity().fY)) * 0.01f;
			temp->scaleX = new_emittor->GetStartScaleX();
			temp->scaleY = new_emittor->GetStartScaleY();

			temp->rotation = 0.0f;

			temp->colorfade_timer = 0.0f;
			temp->scaleX_timer = 0.0f;
			temp->scaleY_timer = 0.0f;
			temp->spawn_timer = 0.0f;

			temp->isDead = true;

			new_emittor->AddToParticleList(temp);
        }

	return new_emittor;
}