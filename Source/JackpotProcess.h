#ifndef JACKPOTPROCESS_H
#define JACKPOTPROCESS_H

#include <GameProcess.h>

class JackpotProcess : public GameProcess
{
public:
	JackpotProcess();
	~JackpotProcess();
	void Complete();	
	void Update();
private:
	Object2D* mImage;

	float mInitialX;
	float mInitialY;
	float mInitialXScale;
	float mInitialYScale;

	float mX;
	float mY;
	float mXScale;
	float mYScale;

	float mImageHeight;
	float mImageWidth;
	float mNumSteps;
	float mFinalHeight; 
	float mFinalWidth;
	float mStepHeight;
	float mStepWidth;

	float mCurrentStep;

	float mJackpotDelayTimer;
};

#endif JACKPOTPROCESS_H