#include "GenericReelControl.h"
#include "ObjectHandler.h"
#include "AudioManager.h"
#include "Game.h"
#include <sstream>
#include <bo.h>
#include <GenericDefines.h>

#define SPIN_SPEED 45.0f

GenericReelControl::GenericReelControl(void)
{
	mTable = 0;
	mRow = 0;
	mGoldenSpinSymbol = 0;
	mGiveFeatureFlag = 0;
	mSpinTime = 520;
	mSpinSpeed = SPIN_SPEED;
	mBounceSpeed = BOUNCE_SPEED;
	mBounceFrames = BOUNCE_FRAMES;
	mSymbolHeight = SYMBOL_HEIGHT;
	mSymbolWidth = SYMBOL_WIDTH;
	mSpinOffset = 210;
	mSpinOffsetsPerReel = 20;
	mFirstSymbolID = 0;

	mStoringThreadActive = 0;
	mStoringThreadStarted = 0;
}

GenericReelControl::~GenericReelControl(void)
{
	delete[] mHexReelLayout;
	mHexReelLayout = 0;
	delete[] mReelLayout;
	mReelLayout = 0;
	delete[] mSymsToAnimate;
	mSymsToAnimate = 0;
	delete[] line;
	line = 0;

	SymbolTablePtr3 = 0;
	SymbolTablePtr4 = 0;
	SymbolTablePtr5 = 0;
	delete[] XtraSpace;
	XtraSpace= 0;
	
}



bool GenericReelControl::InitializeReels(unsigned int firstSymbolID, std::string path, unsigned int numReels, unsigned int numRows, unsigned int numSymbols, unsigned int offsetRows)
{
	mFirstSymbolID = firstSymbolID;
	mNumReels = numReels;
	mNumRows = numRows;
	mNumSymbols = numSymbols;
	mNumVisibleSymbols = mNumReels* mNumRows;
	mOffsetRows = offsetRows;

	line = new unsigned char[mNumReels];

	mSymsToAnimate = new int[mNumReels];


	mReels.resize(mNumReels);
	mReels.resize(0);
	for(unsigned int i = 0; i < mNumReels;++i)
	{
		REEL reel;
		for(unsigned int j = 0; j < mNumRows;++j)
			reel.Symbols.push_back(0);
		mReels.push_back(reel);
	}

	if(offsetRows>0)
	{
		mOffsetReels.resize(mNumReels);
		mOffsetReels.resize(0);

		for(unsigned int i = 0; i < mNumReels;++i)
		{
			OFFSETREEL oReel;
			for(unsigned int  j = 0; j < mOffsetRows;++j)
				oReel.Symbols.push_back(0);
			mOffsetReels.push_back(oReel);
		}
	}
	mReelLayout = new unsigned char[mNumVisibleSymbols];
	mHexReelLayout = new unsigned int[mNumSymbols];

	mPath = path;
	
	for(unsigned int i = 0; i < mNumVisibleSymbols; ++i)
		mReelLayout[i] = 0;

	for(unsigned int i = 0; i < mNumReels; ++i)
	{
		mSymsToAnimate[i] = -1;
		mReels[i].State = NOT_MOVING;
		mOffsetReels[i].State = NOT_MOVING;

		mReels[i].Offset = 0.0f;
		mOffsetReels[i].Offset = 0.0f;
		mOffsetReels[i].NextSymbol = GetLocalIntRandomNumber(mNumSymbols) + 
			mFirstSymbolID;
		
		for(unsigned int x = 0; x < numRows; ++x)
		{
			mReels[i].Symbols[x] = mFirstSymbolID;
		}

		for(unsigned int x = 0; x < mOffsetRows; ++x)
		{
			mOffsetReels[i].Symbols[x] = mFirstSymbolID+GetLocalIntRandomNumber(mNumSymbols);
		}
	}

	for(int i = 0; i < 8192; ++i)
		mWinningSymbol[i] = mNumSymbols;
	 
#ifdef WILD_USED
	int hex = 0;
#else
	int hex = 1;
#endif
	for(unsigned int i = 0; i < mNumSymbols; ++i)
	{
		mHexReelLayout[i] = hex;

		mWinningSymbol[hex] = i;
		if(!hex)
			++hex;
		else
			hex = hex << 1;
	}	

	RestoreData();
	if(TheGame::Instance()->mGameCounter == 0)
		ComposeReelLayout(0,0);

	XtraSpace = new int[mNumReels];
	XtraSpace[0] = 0;
	XtraSpace[1] = 25;
	XtraSpace[2] = 50;
	XtraSpace[3] = 75;
	XtraSpace[4] = 85;
	return true;
}

void GenericReelControl::RestoreFromDemoMode()
{
	RestoreData();
	DrawStaticReels();
	TheGame::Instance()->SetReelTextures();
}

bool GenericReelControl::SymbolMatch(SYMBOLS symbol)
{
	int symCount = 0;
	for(unsigned int i = 0; i < mNumVisibleSymbols;++i)
	{
		if(mReelLayout[i] == symbol)
		{
			symCount++;
		}
	}
	if(symCount == mNumVisibleSymbols)
	{
		return true;
	}
	return false;
}

unsigned int GenericReelControl::SelectWinTable(unsigned int Win)
{
	unsigned char i = 0;
	

	for(i = 0; i < TheGame::Instance()->GetWinData()->mWinsCount;i++) 
	{
		if(Win == TheGame::Instance()->GetWinData()->mWinValues[i])
			break;
	}

	return(i);
}

unsigned int GenericReelControl::SelectFeatWinTable(unsigned int Win)
{
	unsigned char i = 0;

	for(i = 0; i < TheGame::Instance()->GetFeatureData()->mFeatCount;i++) 
	{
		if(Win == TheGame::Instance()->GetFeatureData()->mFeatValues[i])
			break;
	}
	
	return (TheGame::Instance()->GetWinData()->mWinsCount + i);
}

void GenericReelControl::SetWinTable(unsigned int win)
{
	mTable = SelectWinTable(win);
}

void GenericReelControl::SetFeatureTable(unsigned int feature)
{
	mTable = SelectFeatWinTable(feature);
}

void GenericReelControl::ComposeReelLayout(unsigned int Table, unsigned int Row)
{
	mTable = Table;
	mRow = Row;

	for(unsigned int i = 0; i < mNumVisibleSymbols; ++i)
	{
		mReelLayout[i] = *(TheGame::Instance()->GetWinData()->mWinTables[Table] + (mNumVisibleSymbols*Row)+i);
	}
}

void GenericReelControl::DrawStaticReels()
{
	for(unsigned int i = 0; i < mNumReels; ++i)
	{
		int pos = 0;

		mReels[i].Offset = 0.0f;
		mOffsetReels[i].Offset = 0.0f;
		for(unsigned int x = 0; x < mNumRows; ++x)
		{
			mReels[i].Symbols[x] = mReelLayout[i + pos] + 
				mFirstSymbolID;		
			pos += 5;
		}
	}
}

void GenericReelControl::StartTestReelSpin(unsigned int Win, unsigned int Row)
{	
	mWinAmount = Win;
	mTable = SelectWinTable(Win);
	ComposeReelLayout(mTable,Row);
}

void GenericReelControl::StartReelSpin(unsigned int Win)
{
	for(unsigned int i = 0; i < mNumReels; ++i)
	{
		int pos = 0;
		mOffsetReels[i].NextSymbol = GetLocalIntRandomNumber(mNumSymbols) + 
			mFirstSymbolID;

		for(unsigned int x = 0; x < mNumRows; ++x)
		{
			mReels[i].Symbols[x] = mReelLayout[i + pos] + 
				mFirstSymbolID;
			pos += mNumReels;			
		}

		mReels[i].State = SPINNING;
		mOffsetReels[i].State = SPINNING;
		mReels[i].SymsInPlace = (mNumVisibleSymbols - mNumReels) + i;
		mOffsetReels[i].SymsInPlace = ((mOffsetRows*mNumReels) - mNumReels) + i;

		if(i > 0)
		{
			mReels[i].SpinTime = mReels[i - 1].SpinTime + 0.25f;
			mOffsetReels[i].SpinTime = mOffsetReels[i - 1].SpinTime + 0.25f;
		}
		else
		{
			mOffsetReels[i].SpinTime = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + 0.9f;
			mReels[i].SpinTime = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + 0.9f;
		}
	}
	ComposeReelLayout(mTable, GetLocalIntRandomNumber(TheGame::Instance()->GetWinData()->mOutcomes[mTable]));	

	if(!GetDoorStatus())
	{
		StoreData();
	}
}

void GenericReelControl::SpinReels()
{
	if(mReels[mNumReels - 1].State != NOT_MOVING)
	{
		for(unsigned int i = 0; i < mNumReels; ++i)
		{
			switch(mReels[i].State)
			{
			case SPINNING:
				{
					DrawStoppingReels(i);
					break;
				}
			case BOUNCING:
				{
					DrawBouncingReels(i);
					break;
				}
			}
		}
	}
}

bool GenericReelControl::AreReelsSpinning() const
{
	return !(mReels[mNumReels - 1].State == NOT_MOVING);
}

void GenericReelControl::DrawSpinningReels(unsigned int r)
{
	float FrameMove = mSpinSpeed / (float)mSymbolHeight;

	mReels[r].Offset -= FrameMove;
	mOffsetReels[r].Offset -= FrameMove;
	
	if(mReels[r].Offset < -1.0f && mOffsetReels[r].Offset < -1.0f)
	{
		mReels[r].Offset = 0.0f;//+= 1.0f;
		mOffsetReels[r].Offset  = 0.0f;//+= 1.0f;
		for(int i = mNumRows - 1; i >= 0; --i)
		{		
			if(i)
			{
				if(i == mNumRows - 1)
				{
					mOffsetReels[r].Symbols[1] = mReels[r].Symbols[i];
				}

				mReels[r].Symbols[i] = mReels[r].Symbols[i - 1];
			}
			else
			{
				mReels[r].Symbols[i] = mOffsetReels[r].Symbols[i];	
				mOffsetReels[r].Symbols[i] = mOffsetReels[r].NextSymbol;
				mOffsetReels[r].NextSymbol = GetLocalIntRandomNumber(mNumSymbols) + 
											 mFirstSymbolID;

				 
			}
		}
	}
}

void GenericReelControl::DrawBouncingReels(unsigned int r)
{
	float FrameMove = 0;
	if(mBounceSpeed && mSymbolHeight)
		FrameMove = ((float)mBounceSpeed / (float)mSymbolHeight);
	static int a[5] = {0, 0, 0, 0, 0};
	static int count[5] = {0, 0, 0, 0, 0};
	static float spot[5] = {0.3f, 0.3f, 0.3f, 0.3f, 0.3f};
	static float decrement[10] = {0.20f, 0.03f, 0.04f, 0.01f, 0.01f, 0.01f, 0.005f, 0.0025f, 0.0015f, 0.005f};
	
	FrameMove /= count[r] + 0.25f;

	switch(a[r])
	{
	case 0:
		{
			mReels[r].Offset -= FrameMove;
			mOffsetReels[r].Offset -= FrameMove;

			if(mReels[r].Offset < -spot[r] && mOffsetReels[r].Offset < -spot[r])
			{
				spot[r] -= decrement[count[r]];
				count[r]++;
				a[r]++;
				
				if(count[r] > 4)
				{
					a[r] = 2;
				}
			}
		}
		break;
	case 1:
		{
			mReels[r].Offset += FrameMove;
			mOffsetReels[r].Offset += FrameMove;

			if(mReels[r].Offset > spot[r] && mOffsetReels[r].Offset > spot[r])
			{
				spot[r] -= decrement[count[r]];
				count[r]++;
				a[r]--;
			}
		}
		break;
	case 2:
		{
			if(mReels[r].Offset + FrameMove >= 0.0f)
			{
				mReels[r].Offset = 0.0f;
			}
			else
			{
				mReels[r].Offset += FrameMove;
			}

			if(mOffsetReels[r].Offset + FrameMove >= 0.0f)
			{
				mOffsetReels[r].Offset = 0.0f;
			}
			else
			{
				mOffsetReels[r].Offset += FrameMove;
			}
				
			if(mReels[r].Offset >= 0.0f && mOffsetReels[r].Offset >= 0.0f)
			{
				mReels[r].Offset = 0.0f;
				mReels[r].State = NOT_MOVING;
				mOffsetReels[r].Offset = 0.0f;
				mOffsetReels[r].State = NOT_MOVING;
				a[r] = 0;
				count[r] = 0;
				spot[r] = 0.3f;
			}
		}
		break;
	}
}

void GenericReelControl::DrawStoppingReels(int r)
{
	if(TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mReels[r].SpinTime || TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mOffsetReels[r].SpinTime)
	{
		DrawSpinningReels(r);			//GV: starts spinning 1 reel at the time, if taken out!! 
		return;
	}

	float FrameMove = mSpinSpeed / (float)mSymbolHeight;
	mReels[r].Offset -= FrameMove;
	mOffsetReels[r].Offset -= FrameMove;

	if(mReels[r].Offset < -1.0f && mOffsetReels[r].Offset < -1.0f)
	{
		mReels[r].Offset =0.0f;//+= 0.99f;
		mOffsetReels[r].Offset =0.0f;//+= 0.99f;
		for(int i = mNumRows - 1; i >= 0; --i)
		{			
			if(i)
			{
				if(i == mNumRows - 1)
				{
					mOffsetReels[r].Symbols[1] = mReels[r].Symbols[i];
				}

				mReels[r].Symbols[i] = mReels[r].Symbols[i - 1];
		
			}
			else
			{
				mReels[r].Symbols[i] = mOffsetReels[r].Symbols[i];    
				mOffsetReels[r].Symbols[i] = mOffsetReels[r].NextSymbol;

				if(mReels[r].SymsInPlace >= 0)
				{
					mOffsetReels[r].NextSymbol = mReelLayout[mReels[r].SymsInPlace] + 
						mFirstSymbolID;
				}
				else
				{
					mOffsetReels[r].NextSymbol = GetLocalIntRandomNumber(mNumSymbols) + 
						mFirstSymbolID;

					unsigned int symbolMatch = 0;
					for(unsigned int row = 0; row < mNumRows;row++)
					{
						if(mReels[r].Symbols[row] == mOffsetReels[r].NextSymbol)
						{
							symbolMatch++;
						}
					}

					if(symbolMatch==3)
					{
						unsigned int newSymbol = 0;
						do
						{
							newSymbol = GetLocalIntRandomNumber(mNumSymbols) + 
						mFirstSymbolID;
						}
						while(newSymbol==mOffsetReels[r].NextSymbol);

						mOffsetReels[r].NextSymbol = newSymbol;
					}
				}

				mReels[r].SymsInPlace -= 5;
				mOffsetReels[r].SymsInPlace -= 5;
				if((mReels[r].SymsInPlace < -10 + (r)) && (mOffsetReels[r].SymsInPlace < -10 + (r)))
				{
					TheGame::Instance()->PlayReelAudio(r);
					mReels[r].State = BOUNCING;
					mOffsetReels[r].State = BOUNCING;
					
					float TotalBounce = 0;
					if(mBounceSpeed && mSymbolHeight) //prevent divide by zero
						TotalBounce = (float)mBounceSpeed / (float)mSymbolHeight;
						

					mOffsetReels[r].Offset -= TotalBounce * mBounceFrames;
					mReels[r].Offset -= TotalBounce * mBounceFrames;
				}
			}
		}
	}
}

void GenericReelControl::DrawWinningSymbols(unsigned int NumAnimSymbols)
{
	unsigned int reelNum = 0;
	int rowNum = 0;
	for(unsigned int i = 0; i < mNumVisibleSymbols; ++i)
	{
		mReels[reelNum].Symbols[rowNum] = mReelLayout[i] + 
			mFirstSymbolID;

		++reelNum;
		if(reelNum >= mNumReels)
		{
			reelNum = 0;
			++rowNum;
		}
	}
}

int GenericReelControl::GetNextSymbol(unsigned int Reel,unsigned int Row)
{
	if(Row)
		return mReels[Reel].Symbols[Row - 1];
	
	return mOffsetReels[Reel].Symbols[0];
}

int GenericReelControl::GetOffsetNextSymbol(unsigned int Reel,unsigned int Row)
{
	if(Row)
		return mReels[Reel].Symbols[mNumRows - 1];
	
	return mOffsetReels[Reel].NextSymbol;
}

int GenericReelControl::GetSymbol(unsigned int Reel,unsigned int Row)
{
	return mReels[Reel].Symbols[Row];
}

int GenericReelControl::GetOffsetSymbol(unsigned int Reel,unsigned int Row)
{
	return mOffsetReels[Reel].Symbols[Row];
}

float GenericReelControl::GetOffset(unsigned int Reel)
{
	return mReels[Reel].Offset;
}

void GenericReelControl::SetSymbol(unsigned int Reel, unsigned int Row, unsigned int Symbol)
{
	mReels[Reel].Symbols[Row] = Symbol;
}

bool GenericReelControl::IsStoreThreadRunning()
{
	return mStoringThreadActive?true:false;
}

void GenericReelControl::StoreData()
{
	if(GetDoorStatus())  //Don't store if door Open.
		return; 
	
	if(mStoringThreadStarted)
		return;

	mStoringThreadActive = 1;
	_beginthread(&GenericReelControl::BeginStoreDataThread, 0, static_cast<void*>(this));
}

void GenericReelControl::BeginStoreDataThread(void* Param)
{
	static_cast<GenericReelControl*>(Param)->StoreDataThread();
}

void GenericReelControl::StoreDataThread()
{

	int status,value;
	int file;
	int PrivateKey;



	SetFileAction();

	file = _open(mPath.c_str(),FILE_WRITE, _S_IREAD | _S_IWRITE);
	
	if(file == NULL)
	{
		ClearFileAction();
		mStoringThreadActive = 0;
		return;
	}

	value = GetLocalLongRandomNumber();
	PrivateKey = value;
	value = value ^ PublicKey;
	_write(file,&value,4);
	status  = value;

	value   = mTable;
	value = value ^ PrivateKey;
	_write(file,&value,4);
	status  += value;

	value   = mRow;
	value = value ^ PrivateKey;
	_write(file,&value,4);
	status  += value;

	value   = mWinAmount;
	value = value ^ PrivateKey;
	_write(file,&value,4);
	status  += value;

	for(unsigned int i = 0; i < mNumVisibleSymbols; ++i)
	{
		value   = mReelLayout[i];
		value = value ^ PrivateKey;
		_write(file,&value,4);
		status  += value;
	}

	value   = GetLocalCharRandomNumber(250);
	value = value ^ PrivateKey;
	_write(file,&value,4);
	status  += value;

	value = status;_write(file,&value,4); //Store Check Sum


	_close(file);

	ClearFileAction();
	mStoringThreadActive = 0;
}

void GenericReelControl::RestoreData()
{
	int loopCounter = 0;
	while(mStoringThreadActive && loopCounter < 500)
	{
		Sleep(2);
		loopCounter++;
	}
	if(loopCounter > 499)
	{
		mStoringThreadActive = 0;
	}

	int LiveCsum = 0;
	int StoredCsum = 0;
	int file = 0;
	int value = 0;

	int PrivateKey = 0;

	SetFileAction();

	file = _open(mPath.c_str(),FILE_READ, _S_IREAD | _S_IWRITE);

	if(file == NULL)
	{
		ClearFileAction();
		return;
	}
	if(_read(file, &value, 4) > 0) 
	{
		LiveCsum = value;				//Long Rng (Scramble)
		value = PublicKey ^ value;
		PrivateKey = value;
	}

	if(_read(file, &value, 4) > 0) 
	{
		LiveCsum += value;
		value = PrivateKey ^ value;
		mTable = value;
	}
	

	if(_read(file, &value, 4) > 0)
	{
		LiveCsum += value;
		value = PrivateKey ^ value;
		mRow = value;
	}

	if(_read(file, &value, 4) > 0)
	{
		LiveCsum += value;
		value = PrivateKey ^ value;
		mWinAmount = value;
	}

	for(unsigned int i = 0; i < mNumVisibleSymbols; ++i)
	{
		if(_read(file, &value, 4) > 0)
		{
			LiveCsum += value;
			value = PrivateKey ^ value;
			mReelLayout[i] = value;
		}
	}

	if(_read(file, &value, 4) > 0)
	{
		LiveCsum += value;
		value = PrivateKey ^ value;
	}
	
	if(_read(file, &value, 4) > 0) 
	{
		StoredCsum = value;				//Stored Check Sum
	}
	
	_close(file);
	ClearFileAction();

	if((LiveCsum != StoredCsum)||!StoredCsum)
	{
		mTable = 0;
		mRow = 0;
		mWinAmount = 0;
		StoreData();
		if(LiveCsum != StoredCsum)
		{
			SetCriticalError(ERR_REEL_DATA_RESET);
		}
	}
}