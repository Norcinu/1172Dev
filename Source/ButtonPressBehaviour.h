#ifndef BUTTONPRESS_H
#define BUTTONPRESS_H

#include <ObjectHandler.h>
#include <TextureManager.h>
#include <UpdateBehaviour.h>

class ButtonPressBehaviour : public UpdateBehaviour
{
public:
	ButtonPressBehaviour();
	~ButtonPressBehaviour(){}
	void SetTriggered(bool triggered) override;
	void Update(float deltaTime) override;
	bool Load(XMLRead *file);
private:
	bool m_reversing;
	int m_numFrames;
	Object2D* m_object;
};

#endif BUTTONPRESS_H