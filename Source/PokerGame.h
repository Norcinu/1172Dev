/*
Game.h 2012
Hanldes all aspects of the game. interfaces with Engine class
*/
#ifndef _POKERGAME_H
#define _POKERGAME_H

#include <AudioManager.h>
#include <Engine.h>
#include <GenericGame.h>
#include <ObjectHandler.h>
#include <Input.h>
#include <NonCopyable.h>
#include <Singleton.h>

#include "Defines.h"
#include "GenericButtons.h"
#include "card.h"
#include "HiloCard.h"
#include "Sound.h"



class PokerGame
{
private:	
	PokerGame(void);
	friend class Singleton<PokerGame>;

public:	
	~PokerGame(void);
	void UpdateAwardDigits(void);
	void SetAwardValueLitState(int Index,int State);
	void TurnOnAwardValue(int Index);
	void TurnOffAwardValue(int Index);
	void FlashAwardValue(int Index);
	void AntiFlashAwardValue(int Index);
	void ShowAllAwardValueLitStates(void);

	bool PokerGameInit(void);
	void ShuffleCards(void);

	void DealRoll(void);
	void DrawRoll(void);
	unsigned char RollOffWin(unsigned char *dealt_hand, 
							 unsigned int value,unsigned char hand_number);
	unsigned char RoyalFlushCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char FiveOfKindCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char StraightFlushCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char FourOfKindCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char FullHouseCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char FlushCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char StraightCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char ThreeOfKindCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char TwoPairCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char JacksOrBetterCard(unsigned char *dealt_hand, unsigned char hand_number);
	unsigned char AllowWin(unsigned int value,
						   unsigned char *chance_table,
						   unsigned char hand_number,
						   unsigned char flag);
	void		  InitHandsArrays(void);
	void		  InsertJoker(unsigned char value, unsigned char *dealt_hand);
	unsigned char FindDoubleJokerWins(unsigned char *DealtHand);
	unsigned char CheckForJokerCard(unsigned char *DealtHand);
	void		  PlaceOneOfDoubleJoker(unsigned char *DealtHand);
	unsigned char ConvertWinLevel2RollOffLevel(unsigned char Element);

	unsigned int Chkwin(unsigned char * dealt_hand, unsigned char hand_type);
	unsigned char GetRawCard(unsigned char CardValue);
	unsigned char GetSuit(unsigned CardValue);
	unsigned int get_pay(unsigned char win_type);
	unsigned int HigherWins(unsigned char *CardHand);

	void Shuffle5KCards(unsigned char WinType);
	void ShuffleTstCards(unsigned char *TempCards);
	void ForceWin(unsigned char WinType);
	void CreatePack(void);
	unsigned char NumToSwop(void);
	void SelectCardsToSwop(void);
	void ExecuteSwop(void);
	unsigned char FindHoldPos(unsigned char Ele);
	unsigned char SelectWinType(void);

	void SelectFirstHand(void);
	void DealFirstHand(void);
	void SelectSecondHand(void);
	void DealSecondHand(void);
	unsigned char SelectACard(unsigned char *Hand);
	void ReshuffleCards(void);
	unsigned char FindWinLevel(unsigned int LocalValue);
	
	void PutCard(unsigned char Pos, unsigned char Card);
	void RemoveCard(unsigned char Pos);
	void ExitPokerGame(void);
	void ClearUpAfterGamble(void);
	void RegulateGraphicalLampFlashSpeed(int FlashSpeed);

	void HoldCalc(unsigned char *DealtHand);
	void SetRHS(unsigned char i);
	void TagHeldCards(void);
	 int FindNumHeldCards(void);
	unsigned char WinInvolvingJoker(unsigned char *DealtHand,unsigned char Value);
	unsigned char PokerHoldCondition(void);
	unsigned char PokerHiloCondition(void);

	void UpdatePokerHelds(void);
	void SetHoldStartLamps(void);
	void HoldCardButtons(void);
	bool CardHoldPb(int button);
	bool ReadCardHoldPb(int button);
	void RegisterCardButtons(void);
	void InitializeCardButtonArray(void);
	void SetActiveCardButton(bool State, int Button);
	bool PokerGame::GetActiveCardButton(int Button);

	unsigned int FunctionMaxLevels(void);
	unsigned int FunctionMaxAwards(void);
	unsigned int FunctionLevel(void);

	unsigned int GetStake(void);
	unsigned int Get100pStake(void);
	unsigned int Get200pStake(void);
	unsigned char AllowStakeButton(void);
	void SetGameIndex(unsigned int GameIndexValue);
	unsigned int GetGameIndex(void);
	void ChangeStake(void);
	void DisplayStake(void);

	void InitialHiloRptTab(unsigned char HigherChance);
	unsigned char AllowGambleReflexValue(unsigned char JokerWin);
	unsigned char AllowInsertDoubleJoker(unsigned int Value,unsigned char Hand);
	unsigned char AllowHighWins(void);
	
	void EnterHiloGambleInitialize(unsigned char JokerWin);
	void EnterHiloGambleShowAward(unsigned char JokerWin);
	bool EnterHiloGambleEntry(unsigned char JokerWin);
	void EnterHiloGambleScreen(unsigned char JokerWin);

	void HiloGamble(unsigned char JokerWin);
	unsigned char PlayHiloGambleInitializeGame(unsigned char *HiloStatusArray,unsigned char JokerWin);
	unsigned char PlayHiloGambleSelection(unsigned char *HiloStatusArray,unsigned char JokerWin);
	unsigned char PlayHiloGambleProcessSelection(unsigned char *HiloStatusArray,unsigned char JokerWin);
	unsigned char PlayHiloGambleRepeat(unsigned char *HiloStatusArray,unsigned char JokerWin);
	unsigned char PlayHiloGambleGetPayment(unsigned char *HiloStatusArray,unsigned char JokerWin);	
	void SelectInitialCard(void);
	unsigned char PermitSelection(unsigned char NextCard, unsigned char * HiloStatusArray);
	unsigned char SwopToJoker(unsigned char Card);
	BOOL SpecialSwopCardPb(void);
	void CollectPbLamp(unsigned char state);
	void HiPbLamp(unsigned char state);
	void LoPbLamp(unsigned char state);
	void TS22StartPbLamp(unsigned char state);
	void DealDraw1PndPbLamp(unsigned char state);
	void DealDraw2PndPbLamp(unsigned char state);
	void Swop1PndPbLamp(unsigned char state);
	void Swop2PndPbLamp(unsigned char state);

	bool ReadTS22StartPb(void);
	bool ReadDealDraw2PndPb(void);
	bool ReadDealDraw1PndPb(void);
	bool ReadHiPb(void);
	bool ReadLoPb(void);
	bool ReadCollectPb(void);

	void HiloPutCard(unsigned char Pos,unsigned char Card);
	void HiloRemoveCard(unsigned char Pos);
	void StartHiloGamble(void);
	void ExitHiloGamble(void);
	void DrawHiloCards(void);
	void DrawHiloState(void);
	void DrawRemoveHelds(void);


	unsigned char DecHiWinWin(unsigned char CardValue);
	unsigned char DecLoWinWin(unsigned char CardValue);
	unsigned char DecHiWinLose(unsigned char CardValue);
	unsigned char DecLoWinLose(unsigned char CardValue);
	unsigned char DecLoseHi(unsigned char CardValue);
	unsigned char DecLoseLo(unsigned char CardValue);
	unsigned char DecWinSwap(unsigned char CardValue);
	unsigned char DecLoseSwap(unsigned char CardValue);
	unsigned char DecInitialCardValue(void);
	unsigned char GetChoice(const unsigned char * tab);

	unsigned char GetWinPerc(unsigned char Level);

	void PayOut(void);
	bool PayOutEnterJackpotFanfare(void);
	bool PayOutDoJackpotFanfare(void);
	bool PayOutReflex(void);
	bool PayOutPaytransfer(void);

	void ReflexIn(void);
	void ReflexOut(void);
	void Rin1(unsigned int pcent);
	void Rin2(void);
	void Rinit(void);
	void InitStats(void);
	unsigned char Rlinks(void);
	unsigned int GetPc(void);
	void SetTcStoreTrigger(void);
	void SelectGameModeIn(void);
	void SetPointerAndMode(void);
	unsigned char SetChanceTablePtrs(void);
	void GeneralReflexIn(signed long CoinIn);
	void GeneralReflexOut(signed long CoinOut);
	unsigned char AllowFeatureModeEntry(void);
	unsigned char AllowFeatureModeExit(void);
	void SelectModeGameIn(void);
	void CountHyperModeGames(void);
	void ReallocateModeMoney(void);
	void SetModeTriggerStore(void);
	void InitialModeTriggerStore(void);

	void Cointrol(void);

	void UpdateInfoDigits(bool Visibility);
	void UpdateGoldenDigits(bool Visibility);
	void UpdateErrorDigits(bool Visibility);

	unsigned char GoldenJokerFeature(unsigned int WinValue);
	void GoldenJokerUpdateDrawHand(void);
	bool InsertGoldenJokerButtons(void);
	void ShowCurrentWin(unsigned int WinValue,int State);
	void SetGoldenJokerLamps(void);
	unsigned char AllowGoldenJokerHoldButtons(unsigned int Index);
	void GoldenJokerRemoveHolds(void);
	unsigned char CheckForUnheldCards(unsigned char *DealtHand);
	unsigned char AllowGoldenJokerFeature(void);
	unsigned int FindTheBestPosition(void);
	void GoldenJokerFeatureIntro(void);
	unsigned char GoldenJokerControl(void);
	unsigned char CheckForGoldenJokerBenefit(unsigned char *DealtHand);

	void GoldenJokerUpdateIntroduction(void);
	void GoldenJokerUpdateDeleteHelds(void);
	bool GoldenJokerUpdateButtons(void);
	void GoldenJokerUpdateEnd(void);

	bool YesDemoPb(void);
	bool ReadYesDemoPb(void);
	void RegisterYesDemoButton(void);

	bool CollectBoxPb(int button);
	bool ReadCollectBoxPb(int button);
	void RegisterCollectBoxButtons(void);
	void SetActiveCollectBoxButton(bool State, int Button);
	bool GetActiveCollectBoxButton(int Button);

	float GetSoundDelay(int SoundIndex, int AdditionalTime);

	void SetGameStartTime(void);
	void SetGameEndTime(void);
	float GetGameLengthTime(void);
	float GeneralSetGameDelay(float dValue);

	void TransferBankToCredits(unsigned int MaxTransferValue);
	void SetTransferBankToCredits(bool Status);
	bool GetTransferBankToCredits(void);
	void SetTransferBankToCreditDelay(float DelayTime);
	bool GetTransferBanktoCreditTimeElaspsed(void);

	unsigned int SoakBuildPressButton(bool (PokerGame::*ReadButton)(),unsigned int StakeValue,unsigned int GameIndexValue);
	void SoakStoreThisGame(void);

	
public:

	bool RegisterCardBtnArray[5];
	bool CardButtonActive[5];
	bool RegisterCollectBoxBtnArray[2];
	bool CollectBoxButtonActive[2];

	CARD     PokerHand[5];
	HILOCARD HiloHand[5];

	unsigned int AwardDigitColour[MAX_AWARDS];  //1 
	unsigned int AwardValueLitState[MAX_AWARDS];

	unsigned char Cards[53], TempCards[53];
	unsigned  int CardPtr;

	unsigned char DealHand[5], DrawHand[5];
	unsigned char HandPtr;
	unsigned char CardsUsedArray[5];    		
	unsigned char ValueOfPair;
	unsigned char AllowedHighWin, AllowHighWinFlag;  
      signed char GoldenJokerPositions[5];

	unsigned char PercPtr;
	unsigned char AllowDoubleJokerFlag;
	unsigned char InsertDoubleJokerFlag;

	unsigned char AKind5,AKind4,AKind3,AKind2,NumberPairs;
	unsigned char JokerMem,JokerPrime,RunCtr,StartOfStraight,RunPtr;	
	unsigned char AutoPlaySelected; 
	unsigned char JokerWin;
	unsigned char ActiveCard,i,j;
	unsigned char RC1,RC2,RC3,RC4,RC5;
	unsigned char SC1,SC2,SC3,SC4,SC5;
	unsigned char SuitCardMask[5];
	unsigned char AlikeMask[20];
	unsigned char AKind[6];
	unsigned char FlushCount[5];
	unsigned char RunOf[10][2][2];
	unsigned char Flush,WinPtr,HighestRun,JkrUsed,MaxFlush,FlushSuit,MaxRun;
	unsigned char JokerHand;
	unsigned char RoutCond;
		
	unsigned  int ChkWinPay;
	unsigned  int ValueCardMask[16];
	unsigned char RoyalFlushWinFlag;

	unsigned char SwopPos[3],Nswop,Xcards[53],Tcards[5];
	unsigned char PotentialWin;
	unsigned int  Pay;
	unsigned int  PokerGameWin;

	bool PokerHandProcessComplete;
	unsigned char GameIndex;

	unsigned char HiloRepeatTab[MAX_AWARDS];
	unsigned char AutoPlayFlag,ResetAutoPlayFlag;
	unsigned int  GambleReflexValue;
	unsigned char HiloGambleFlag;
	unsigned char HiloStatusArray[15];
	unsigned char gamble_ctr;
	unsigned char NumGambles;
	unsigned char AwardLevel;
	unsigned char LevelCtr;
	long	      ExtraWin;
	unsigned int  ExtraWinCtr;
	unsigned char Hgamble;
	unsigned char ExternAllowSwap;
	unsigned char NoJokerFlag;
	         char AllowHiPb, AllowLoPb;

	  signed long HiloStore[MaxSelectableGames],HiloStore2[MaxSelectableGames],HighStore[MaxSelectableGames],StandardStore[MaxSelectableGames];
	  signed long ModeStore[MaxSelectableGames],ModeStoreTrigger[MaxSelectableGames],ModeStoreExit[MaxSelectableGames];  
	  signed long GeneralStore[MaxSelectableGames];
	unsigned char ModeGameIn[MaxSelectableGames];

	unsigned char HiloGambleStage;
			float HiloDelayTimer;
	unsigned char ActivateHiloGambleGraphics;
	/**********PlayHiloGamble FrontStart**********/
	unsigned char Lo,Hi,Col,CardValue,NextCard,NewCard;
	unsigned char Tag,AllowSwap,OldCard,CompleteCard;
	unsigned char GamblesLeft, NumCards;
	unsigned char GameType;
	unsigned char PrimeWinSnd;
	unsigned char Perct;
	/**********PlayHiloGamble End**********/

	unsigned char PayOutStage;

	unsigned int  WageredAmount;

	unsigned int  FinalWinValue;

	unsigned char GoldenJokerStage;
	unsigned  int GoldenJokerWins[5];
	unsigned  int PayTemp;
			 bool GoldenJokerHeldProcessEnd;

			 bool YesDemoPbStatus;

			 float mGameStartTimer;
			 float mGameEndTimer;
			 bool  TransferFlag;
			 float TransferDelay;
/****NMI*******/
			 int NmiHiloStrategy;

			 long SoakDoubleJokerFlag;

private:
	unsigned int AwardTable[2][MAX_AWARDS];
	unsigned int AwardValuesX;
	unsigned int AwardValuesY[MAX_AWARDS];
};

typedef Singleton<PokerGame> ThePokerGame;

#endif

