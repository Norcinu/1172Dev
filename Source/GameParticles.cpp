
#include <ParticleFactories.h>
#include "GameParticles.h"

ColourBehaviour* CreateFadeColour()
{
	return new ColourFade;
}

static bool registered = TheColourBehaviourFactory::Instance()->AddType("Fade", CreateFadeColour);

ColourFade::ColourFade()
{
	mStartColour = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	mEndColour = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	mFallOff = 0.1f;
}

ColourFade::~ColourFade()
{

}

void ColourFade::CalculateParticle(D3DXCOLOR& colour, const Particle& particle)
{
	colour = mStartColour;
}

void ColourFade::Load(XMLRead* file, ParticleSystem* owner)
{
	m_owner = owner;

	file->GetAttributeFloat("Colour", "FallOff", &mFallOff);

	while(!file->GetNodeComplete("Colour"))
	{
		if(file->IsNodeName("StartColour"))
		{
			file->GetAttributeD3DXCOLOR("StartColour", &mStartColour);
		}
		if(file->IsNodeName("EndColour"))
		{
			file->GetAttributeD3DXCOLOR("EndColour", &mEndColour);
		}
		file->Read();
	}

	mDiff = mStartColour - mEndColour;
	mDiff *= mFallOff;
}

void ColourFade::UpdateParticle(float deltaTime, Particle& particle)
{
	D3DXCOLOR colour = particle.GetColour();
	float decrease = mFallOff* deltaTime;

	//D3DXCOLOR change = mDiff* deltaTime;
	if(colour.r != mEndColour.r)
	{
		colour -= (mDiff*deltaTime);
	}
	//colour -= mDiff;
	/*colour.r -= decrease;
	colour.g -= decrease;
	colour.b -= decrease;
	colour.a -= decrease;*/
	particle.SetColour(colour);
}


ScaleBehaviour* CreateScaleChange()
{
	return new ScaleChange;
}

static bool scaleReg = TheScaleBehaviourFactory::Instance()->AddType("Change", CreateScaleChange);

ScaleChange::ScaleChange()
{
	mChange = 0.0f;
}

ScaleChange::~ScaleChange()
{
	mStartScale = D3DXVECTOR2(1.0f,1.0f);
	mChange = 0.0f;
}

void ScaleChange::CalculateParticle(D3DXVECTOR2& scale, const Particle& particle)
{
	scale = mStartScale;
}

void ScaleChange::Load(XMLRead* file, ParticleSystem* owner)
{
	m_owner = owner;

	file->GetAttributeFloat("Scale", "Change", &mChange);

	while(!file->GetNodeComplete("Scale"))
	{
		if(file->IsNodeName("StartScale"))
		{
			file->GetAttributeD3DXVECTOR2("StartScale", &mStartScale);
		}
		file->Read();
	}
}

void ScaleChange::UpdateParticle(float deltaTime, Particle& particle)
{
	D3DXVECTOR2 scale = particle.GetScale();
	
	if(scale.x > 0.0f)
	{
		float change = mChange*deltaTime;
		scale.x += change;
		scale.y += change;
		particle.SetScale(scale);
	}
}