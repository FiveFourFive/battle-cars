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
	vector<int> templist;

	if( this == NULL)
		return;

	for( unsigned int j = 0; j < m_GameEmittors.size(); j++)
	{
		if( m_GameEmittors[j]->IsDead() == false)
		{
			if( m_GameEmittors[j]->IsActive() == false)
			{
				templist.push_back(j);
			}
		}
	}

	for( unsigned int j = 0; j < templist.size(); j++)
	{
		int index = templist[j];
		m_GameEmittors[index]->SetIsActive(true);
		m_ActiveEmittors.push_back(m_GameEmittors[index]);
	}

	for( unsigned int i = 0; i < m_ActiveEmittors.size(); i++)
	{
		if( m_ActiveEmittors[i])
		m_ActiveEmittors[i]->Update(fElapsedTime);
	}
}

void ParticleManager::RenderEmittors()
{
	for( unsigned int i = 0; i < m_ActiveEmittors.size(); i++)
	{
		m_ActiveEmittors[i]->Render();
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

	Emittor* temp = new Emittor();
#pragma region XML_Variables
	TiXmlElement* pNumParticles = pRoot->FirstChildElement("NumberofParticles");
	if( pNumParticles)
	{
		temp->SetMaxNumber(atoi(pNumParticles->GetText()));
	}
	else 
		MessageBox(0, "Failed to Load Number of Particles", 0,0);

	TiXmlElement* pMinLifeSpan = pRoot->FirstChildElement("MinLifeSpan");
	if( pMinLifeSpan)
	{
		temp->SetMinLife((float)atof(pMinLifeSpan->GetText()));
	}
	else 
		MessageBox(0, "Failed to Load Minimum Life Span of Particles", 0,0);

	TiXmlElement* pMaxLifeSpan = pRoot->FirstChildElement("MaxLifeSpan");
	if( pMaxLifeSpan)
	{
		temp->SetEndLife((float)atof(pMaxLifeSpan->GetText()));
	}
	else 
		MessageBox(0, "Failed to Load Maximum Life Span of Particles", 0,0);

	TiXmlElement* pStartColor = pRoot->FirstChildElement("StartColor");
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

	TiXmlElement* pEndColor = pRoot->FirstChildElement("EndColor");
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

	TiXmlElement* pStartXScale = pRoot->FirstChildElement("XStartScale");
	if(pStartXScale)
	{
		temp->SetStartScaleX( (float)atof(pStartXScale->GetText()));
	}
	else
		MessageBox(0, "Failed to load X Starting Scale", 0, 0);

	TiXmlElement* pEndXScale = pRoot->FirstChildElement("XEndScale");
	if(pEndXScale)
	{
		temp->SetEndScaleX( (float)atof(pEndXScale->GetText()));
	}
	else
		MessageBox(0, "Failed to load X Ending Scale", 0, 0);

	TiXmlElement* pStartYScale = pRoot->FirstChildElement("YStartScale");
	if(pStartXScale)
	{
		temp->SetStartScaleY( (float)atof(pStartYScale->GetText()));
	}
	else
		MessageBox(0, "Failed to load Y Starting Scale", 0, 0);

	TiXmlElement* pEndYScale = pRoot->FirstChildElement("YEndScale");
	if(pEndXScale)
	{
		temp->SetEndScaleY( (float)atof(pEndYScale->GetText()));
	}
	else
		MessageBox(0, "Failed to load Y Ending Scale", 0, 0);

	TiXmlElement* pMinXVelocity = pRoot->FirstChildElement("MinXVelocity");
	TiXmlElement* pMinYVelocity = pRoot->FirstChildElement("MinYVelocity");
	if(pMinXVelocity && pMinYVelocity)
	{
		temp->SetMinVelocity((float)atof(pMinXVelocity->GetText()), (float)atof(pMinYVelocity->GetText()));
	}
	else
		MessageBox(0, "Failed to load Min Velocity", 0, 0);

	TiXmlElement* pMaxXVelocity = pRoot->FirstChildElement("MaxXVelocity");
	TiXmlElement* pMaxYVelocity = pRoot->FirstChildElement("MaxYVelocity");
	if(pMaxXVelocity && pMaxYVelocity)
	{
		temp->SetMaxVelocity((float)atof(pMaxXVelocity->GetText()), (float)atof(pMaxYVelocity->GetText()));
	}
	else
		MessageBox(0, "Failed to load Max Velocity", 0, 0);

	TiXmlElement* pXAccel = pRoot->FirstChildElement("XAcceleration");
	TiXmlElement* pYAccel = pRoot->FirstChildElement("YAcceleration");
	if(pXAccel && pYAccel)
	{
		temp->SetAcceleration((float)atof(pXAccel->GetText()), (float)atof(pYAccel->GetText()));
	}
	else
		MessageBox(0, "Failed to load Acceleration", 0, 0);

	TiXmlElement* pRotation = pRoot->FirstChildElement("YAcceleration");
	if(pRotation)
	{
		temp->SetRotation((float)atof(pRotation->GetText()));
	}
	else
		MessageBox(0, "Failed to load Rotation for emittor", 0, 0);

	TiXmlElement* pSource = pRoot->FirstChildElement("SourceBlend");
	if(pSource)
	{
		temp->SetSourceBlend(atoi(pSource->GetText()));
	}
	else
		MessageBox(0, "Failed to load Source Blend", 0, 0);

	TiXmlElement* pDestination = pRoot->FirstChildElement("DestinationBlend");
	if(pDestination)
	{
		temp->SetDestinationBlend(atoi(pDestination->GetText()));
	}
	else
		MessageBox(0, "Failed to load Destination Blend", 0, 0);

	TiXmlElement* pImageName = pRoot->FirstChildElement("ImageName");
	if(pImageName)
	{
		temp->SetImageString(pImageName->GetText());
	}
	else
		MessageBox(0, "Failed to load Particle Image Name", 0, 0);

	TiXmlElement* pRepeat = pRoot->FirstChildElement("Continuous");
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


#pragma endregion

	temp->InitializeEmittor();
	temp->SetID(Count);

	Count += 1;
	m_GameEmittors.push_back(temp);	// Pushes temp onto the Game emittor.
	

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
	emittor->SetBase(base);
	emittor->SetPosition( base->GetPosX() + offsetX, base->GetPosY() + offsetY);

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