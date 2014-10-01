#ifndef IMAGEPATTERNBEHAVIOUR_H
#define IMAGEPATTERNBEHAVIOUR_H

#include <Object2D.h>

struct PosLinePair
{
	PosLinePair(D3DXVECTOR2 position, unsigned int line)
	{
		m_position = position;
		m_line = line;
		m_inPosition = false;
	}
	bool m_inPosition;
	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_velocity;
	float m_fallSpeed;
	unsigned int m_increment;
	unsigned int m_line;
};

class ImagePatternBehaviour// : public UpdateBehaviour
{
public:
	ImagePatternBehaviour();
	~ImagePatternBehaviour();
	void SetTriggered(bool triggered);
	void Update(float deltaTime);
	bool Load(XMLRead* file);
private:
	typedef std::vector<PosLinePair> PositionPair;	
	bool m_allInPosition;
	D3DXVECTOR2 m_startingPosition;
	float m_fallSpeed;
	float m_holdTime;
	float m_runningTime;
	float m_rotatingInterval;
	float m_rotatingTimeAccumulated;
	Object2D* m_object;
	PositionPair m_positions;
	std::string m_patternTextureName;
	unsigned int m_coinSpread;
	unsigned int m_droppingLines;
	unsigned int m_moveSpeed;
	unsigned int m_spread;
	unsigned int m_totalLines;
};

#endif IMAGEPATTERNBEHAVIOUR_H