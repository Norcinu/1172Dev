#ifndef TEXTBOUNCEBEHAVIOUR_H
#define TEXTBOUNCEBEHAVIOUR_H

#include <Text.h>

class TextBounceBehaviour : public UpdateBehaviour
{
public:
	TextBounceBehaviour();
	TextBounceBehaviour(Text* text, const D3DXVECTOR2& startingScale, 
		const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement);
	~TextBounceBehaviour(){}
	void ResetValues();
	void SetTriggered(bool triggered);
	void SetValues(Text* text, const D3DXVECTOR2& startingScale, 
		const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement);
	void Update(float deltaTime);
	bool Load(XMLRead* file);
private:
	bool m_stop;
	bool m_valuesSet;
	D3DXVECTOR2 m_bounceMaxHeight;
	D3DXVECTOR2 m_startingScale;
	D3DXVECTOR2 m_shadowOffset;
	float m_bounceIncrement;
	float m_bounceValue;
	std::string m_textName;
	Text* m_text;
};

#endif TEXTBOUNCEBEHAVIOUR_H