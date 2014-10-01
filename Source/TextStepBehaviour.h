#ifndef TEXTSTEPBEHAVIOUR_H
#define TEXTSTEPBEHAVIOUR_H

#include <Text.h>

class TextStepBehaviour : public UpdateBehaviour
{
public:
	TextStepBehaviour();
	TextStepBehaviour(Text* text, float bounceMaxHeight, float bounceIncrement, unsigned int numFrames);
	~TextStepBehaviour();
	void ResetValues();
	void SetTriggered(bool triggered);
	void SetValues(Text* text, float bounceMaxHeight, float bounceIncrement, unsigned int numFrames);
	void Update(float deltaTime);
	bool Load(XMLRead* file);
private:
	bool m_valuesSet;
	D3DXVECTOR2 m_startingScale;
	float m_bounceIncrement;
	float m_bounceMaxHeight;
	float m_bounceValue;
	unsigned int m_frameCount;
	unsigned int m_numFrames;
	std::string m_textName;
	Text* m_text;
};

#endif TEXTSTEPBEHAVIOUR_H