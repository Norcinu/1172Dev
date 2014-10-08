/*
Buttons.h 2012
Handles all button deck functions
*/
#include <NonCopyable.h>
#include <Singleton.h>
#include <string>
#include <Windows.h>
#include <map>
#include "bo.h"


#ifndef _BUTTONS_H
#define _BUTTONS_H

#define START_BUTTON LH4_BUTTON
#define AUTOPLAY_BUTTON LH3_BUTTON
#define MAINMENU_BUTTON LH2_BUTTON
#define COLLECT_BUTTON LH1_BUTTON

#define START_LAMP LH4_LAMP
#define AUTOPLAY_LAMP LH3_LAMP
#define MENU_LAMP LH2_LAMP
#define COLLECT_LAMP LH1_LAMP

#define BUTTONPRESS 0
#define BUTTONACTIVE 1

class Object2D;
class Instance2D;

class HardwareButton
{
public:
	HardwareButton(const char* BtnName, unsigned char offset, unsigned char mask, unsigned char lamp);
	~HardwareButton();
	bool CheckPressed();
	bool IsActive(){return mActive;}
	bool IsPressed(){return mButtonPressed;}
	bool IsReleased(){return mButtonReleased;}
	void Clear();
	void Release();
	void SetActive(bool active, unsigned char state = LAMP_OFF) ;
	void SetPressed(){mButtonPressed = true;}
	void SetReleased(){ mButtonReleased = true;}
	const char* GetName(){return mName;}
protected:
	bool mActive;
	bool mButtonReleased;
	bool mButtonPressed;
	const char* mName;
	unsigned char mOffset;
	unsigned char mMask;
	unsigned char mLamp;
};

class OSButton
{
public:
	OSButton() {}
	OSButton(const char *objName,const char* objLlegend,const char * objUlegend);
	//OSButton(const char *objName, bool instance = false, int instanceNum = 0){}
	~OSButton();
	
	bool CheckPressed();
	bool IsActive(){return mActive;}
	bool IsPressed(){return mButtonPressed;}
	bool IsReleased(){return mButtonReleased;}
	void Clear();
	void Release();
	void SetActive(bool active, unsigned char state = LAMP_OFF);
	void SetPressed(){mButtonPressed = true;}
	const char* GetName(){return mName;}
	const char* GetLegendName(){return mLlegend;}

//protected:
	bool mActive;
	bool mButtonReleased;
	bool mButtonPressed;
	bool mInstance;
	unsigned int mInstanceNum;
	const char* mName;
	const char* mLlegend;
	const char* mUlegend;
	Object2D *mObject;
	Instance2D *mInstance2D;
};


class Buttons : public NonCopyable
{
private:
	Buttons(void);
	friend class Singleton<Buttons>;
public:
	void Initialise();
	~Buttons(void);
	bool ButtonPressed(const char* buttonName);
	bool OSButtonPressed(const char* buttonName);
	void DisableHWButtons();//used to lock out hw buttons when boxes are displayed etc
	void StandbyButtons();
	void DealStartButtons(float DelayTimer);
	void HoldStartButtons();	
	void LampsOff();
	
	void DisableOSButtons(); // Steven

	void SetButtonActivity(bool active, const char* buttonName, unsigned char state = LAMP_OFF);
	virtual void SetOSButtonActivity(bool active, const char* buttonName, unsigned char state = LAMP_OFF);

	void SetStandbyType(unsigned int Type){mStandbyType = Type;}
	void SetStandbyTimer(unsigned int Timer){mStandbyTimer = Timer;}
	void UpdateButtons();
	unsigned int GetStandbyType(){return mStandbyType;}
	unsigned int GetStandbyTimer(){return mStandbyTimer;}
	
	bool GetButtonProcessStarted() {return mButtonProcessStarted;}
	void SetButtonProcessEnded(){mButtonProcessStarted = false;}

	void SetVisibility(std::string &button);
private:
	void SetLampState(unsigned char Lamp, unsigned char State);
//private:
protected:
	bool mButtonProcessStarted;
	typedef std::map<std::string, HardwareButton*> HardwareButtons;
	typedef std::map<std::string, OSButton*> OnScreenButtons;
	HardwareButtons mButtons;
	OnScreenButtons mOSButtons;
	unsigned int mStandbyType;
	unsigned int mStandbyTimer;
};


class InfoButton : public OSButton
{
public:
	InfoButton(const char *objName,const char* objLlegend,const char * objUlegend) {}
	InfoButton(const char *objName, bool instance = false, int instanceNum = 0); //:
	//mName(objName)//, mInstance(instance), mInstanceNum(instanceNum)
	 

	void SetOSButtonActivity(bool active, const char* buttonName, unsigned char state = LAMP_OFF);

protected:
	void PlayDownSound();
	void PlayUpSound(){}
};

typedef Singleton<Buttons> TheButtons;

#define THE_BUTTONS TheButtons::Instance()

#endif

