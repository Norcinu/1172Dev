#ifndef TEXTAWAYBEHAVIOUR_H
#define TEXTAWAYBEHAVIOUR_H

#include <Text.h>

class TextAwayBehaviour : public UpdateBehaviour
{
public:
	TextAwayBehaviour();
	TextAwayBehaviour(Text* text, const D3DXVECTOR2& startingScale, const D3DXVECTOR2& moveAmount, 
		const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement, unsigned int numFrames);
	~TextAwayBehaviour();
	void ResetValues();
	void SetValues(Text* text, const D3DXVECTOR2& startingScale, const D3DXVECTOR2& moveAmount,
		const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement, unsigned int numFrames);
	void Update(float deltaTime);
	bool Load(XMLRead* file);
private:
	bool m_valuesSet;
	D3DXVECTOR2 m_bounceMaxHeight;
	D3DXVECTOR2 m_moveAmount;
	D3DXVECTOR2 m_currentMoveAmount;
	D3DXVECTOR2 m_velocity;
	D3DXVECTOR2 m_startingScale;
	float m_bounceIncrement;
	unsigned int m_frameCount;
	unsigned int m_numFrames;
	std::string m_textName;
	Text* m_text;	
};

#endif TEXTAWAYBEHAVIOUR_H