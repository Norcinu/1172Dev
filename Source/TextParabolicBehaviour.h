#ifndef TEXTPARABOLICBEHAVIOUR_H
#define TEXTPARABOLICBEHAVIOUR_H

#include <Text.h>

class TextParabolicBehaviour : public UpdateBehaviour
{
public:
	TextParabolicBehaviour();
	TextParabolicBehaviour(Text* text, const D3DXVECTOR2& startingScale, const D3DXVECTOR2& moveAmount, 
		const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement, unsigned int numFrames);
	~TextParabolicBehaviour();
	void ResetValues();
	void SetTriggered(bool triggered);
	void SetValues(Text* text, const D3DXVECTOR2& startingScale, const D3DXVECTOR2& moveAmount, 
		const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement, unsigned int numFrames);
	void Update(float deltaTime);
	bool Load(XMLRead* file);
private:
	bool m_valuesSet;
	D3DXVECTOR2 m_bounceMaxHeight;
	D3DXVECTOR2 m_moveAmount;
	D3DXVECTOR2 m_startingScale;
	float m_bounceIncrement;
	float m_bounceValue;
	unsigned int m_frameCount;
	unsigned int m_numFrames;
	std::string m_textName;
	Text* m_text;
};

#endif TEXTPARABOLICBEHAVIOUR_H