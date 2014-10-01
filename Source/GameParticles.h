#ifndef GAMEPARTICLES_H
#define GAMEPARTICLES_H

#include <ObjectHandler.h>
#include <ColourBehaviour.h>
#include <ScaleBehaviour.h>

class ColourFade : public ColourBehaviour
{
public:
	ColourFade();
	virtual ~ColourFade();
	void CalculateParticle(D3DXCOLOR& colour, const Particle& particle);
	void Load(XMLRead* file, ParticleSystem* owner) override;
	void UpdateParticle(float deltaTime, Particle& particle);
private:
	D3DXCOLOR mStartColour;
	D3DXCOLOR mEndColour;
	D3DXCOLOR mDiff;
	float mFallOff;
};

class ScaleChange : public ScaleBehaviour
{
public:
	ScaleChange();
	virtual ~ScaleChange();
	void CalculateParticle(D3DXVECTOR2& scale, const Particle& particle);
	void Load(XMLRead* file, ParticleSystem* owner);
	void UpdateParticle(float deltaTime, Particle& particle);
private:
	D3DXVECTOR2 mStartScale;
	float mChange;

};

#endif