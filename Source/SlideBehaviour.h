#ifndef SLIDEBEHAVIOUR_H
#define SLIDEBEHAVIOUR_H

#include <ObjectHandler.h>
#include <TextureManager.h>
#include <UpdateBehaviour.h>

class SlideBehaviour : public UpdateBehaviour
{
public:
	SlideBehaviour();
	void SetAtEnd(bool toEnd);
	void SetTriggered(bool triggered);
	void Update(float deltaTime);
	bool GetTriggered(){return m_triggered;};
	bool Load(XMLRead* file);
private:
	bool CheckWithEpsilon(D3DXVECTOR2 vec, D3DXVECTOR2 comparsionVec);
private:
	bool m_init;
	bool m_slideToEnd;	
	bool m_textInEndPos;
	D3DXVECTOR2 m_currentAdjustment;
	D3DXVECTOR2 m_endAdjustment;
	D3DXVECTOR2 m_floatEpsilon;
	D3DXVECTOR2 m_numbersStartPos;	
	D3DXVECTOR2 m_startingPosition;
	D3DXVECTOR2 m_titleStartPos;
	float m_velocity;	
	Object2D* m_object;
	std::string m_numbersName;
	std::string m_titleName;
	Text* m_numbersText;
	Text* m_titleText;
};

#endif SLIDEBEHAVIOUR_H