#ifndef _GEN_REEL_CONTROL_H
#define _GEN_REEL_CONTROL_H
#include <string>
#include <map>
#include <vector>

typedef enum{
SYM_O = 0,
SYM_DIAMOND,
SYM_BAR,
SYM_FREESPIN
}SYMBOLS;

typedef enum {
	NOT_MOVING = 0,
	SPINNING,
	BOUNCING
} REEL_STATE;

struct REEL
{	
	float SpinTime;
	std::vector<unsigned int> Symbols;
	int SymsInPlace;
	float Offset;
	REEL_STATE State;
};

struct OFFSETREEL
{	
	float SpinTime;
	int NextSymbol;
	std::vector<unsigned int> Symbols;
	int SymsInPlace;
	float Offset;
	REEL_STATE State;
};

#define SYMBOL_WIDTH	160 
#define SYMBOL_HEIGHT	168 

#define BOUNCE_FRAMES 2
#define BOUNCE_SPEED 15 //20

class GenericReelControl
{
public:
	GenericReelControl(void);
	virtual ~GenericReelControl(void);

	/** Compulsory **/
	virtual bool InitializeReels(unsigned int firstSymbolID, std::string path, unsigned int numReels, unsigned int numRows, unsigned int numSymbols, unsigned int offsetRows);
	void SetSymbolData(int width, int height){mSymbolWidth = width; mSymbolHeight = height;};
	void SetReelSpeeds(float spinTime, float spin, int bounce, int bounceFrames){mSpinTime = spinTime; mSpinSpeed = spin; mBounceSpeed = bounce; mBounceFrames = bounceFrames;}
	/** End **/

	bool IsStoreThreadRunning();
	bool AreReelsSpinning() const;
	bool SymbolMatch(SYMBOLS symbol);
	void ComposeReelLayout(unsigned int Table, unsigned int Row);
	void DrawStaticReels();
	void DrawWinningSymbols(unsigned int NumAnimSymbols);
	virtual void StartReelSpin(unsigned int Win);				//Will Probably need overwriting for a game specific reelspin
	void StartTestReelSpin(unsigned int Win, unsigned int Row);
	void SpinReels();
	unsigned int SelectWinTable(unsigned int Win);
	unsigned int SelectFeatWinTable(unsigned int Win);
	void SetWinTable(unsigned int win);
	void SetFeatureTable(unsigned int feature);

	unsigned int Check4Win(unsigned char r1, unsigned char r2, unsigned char r3, unsigned char r4, unsigned char r5);
	void StoreData();	

	int mWinAmount;
	int *mSymsToAnimate;
	unsigned char mGoldenSpinSymbol;
	unsigned char mGiveFeatureFlag;
	int GetSymbol(unsigned int Reel,unsigned int Row);
	int GetNextSymbol(unsigned int Reel,unsigned int Row);
	int GetOffsetSymbol(unsigned int Reel,unsigned int Row);
	int GetOffsetNextSymbol(unsigned int Reel,unsigned int Row);
	float GetOffset(unsigned int Reel);
	void SetSymbol(unsigned int Reel, unsigned int Row, unsigned int Symbol);
	void RestoreFromDemoMode();

	//NMI Functions
	void RecordAndLogForNMI();
	void TakeNMIAction(unsigned char action, unsigned int value);
private:
	static void _cdecl BeginStoreDataThread(void* Param);
	void StoreDataThread();
	void RestoreData();	

	void DrawCurrentReels();
	void DrawSpinningReels(unsigned int r);
	void DrawBouncingReels(unsigned int r);
	void DrawStoppingReels(int r);
private:
	std::vector<REEL> mReels;
	std::vector<OFFSETREEL> mOffsetReels;

	unsigned char mStoringThreadActive;
	unsigned char mStoringThreadStarted;

	unsigned char *mReelLayout;
	unsigned int *mHexReelLayout;
	unsigned int mWinningSymbol[8192];
	unsigned int mTable;
	unsigned int mRow;

	std::string mPath;
	unsigned int mNumRows;
	unsigned int mNumReels;
	unsigned int mNumSymbols;
	unsigned int mNumVisibleSymbols;
	unsigned int mOffsetRows;
	unsigned char* line;

	float mFrameMove;

	int mSymbolWidth;
	int mSymbolHeight;

	float mSpinSpeed;
	int mBounceSpeed;
	int mBounceFrames;
	float mSpinTime;	
	float mSpinOffset;
	int mSpinOffsetsPerReel;
	unsigned int mFirstSymbolID;

	const unsigned int *SymbolTablePtr3;
	const unsigned int *SymbolTablePtr4;
	const unsigned int *SymbolTablePtr5;
	int *XtraSpace;
};

#endif

