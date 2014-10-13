#include "PokerGame.h"
#include "Game.h"
#include "VariableSoundProcess.h"
#include <ObjectHandler.h>

extern unsigned char global_quit;
extern   signed long HiloStoreMinValue[MaxSelectableGames];

static bool FirstTimeEntry = true;

void PokerGame::EnterHiloGambleInitialize(unsigned char JokerWin)
{
	if (AutoPlayFlag)
	{
		ResetAutoPlayFlag = 1;
		HiloEnteredOnAP = true;
	}
	AutoPlayFlag = 0;

	THE_GAME->SetAutoplay(false);
	THE_BUTTONS->SetButtonActivity(false, "AutoPlay");
	THE_BUTTONS->SetOSButtonActivity(false, "AutoplayButton");

	if(AllowGambleReflexValue(JokerWin))
	{
		GambleReflexValue = Pay;
	}

	InitialHiloRptTab(JokerWin|ModeGameIn[GameIndex]);
	SelectInitialCard();
}

void PokerGame::EnterHiloGambleShowAward(unsigned char JokerWin)
{
	SelectInitialCard();

	AwardLevel = FindWinLevel(Pay);

	SetAwardValueLitState(AwardLevel,MLAMP_ON);

	ENGINE->GetProcessManager()->AddProcessToQueue(new VariableSoundProcess("WINTOTAL",WINTOTAL,2));

	/* find number of gambles need to reach jackpot */
	NumGambles = FunctionMaxLevels() - AwardLevel;
}

bool PokerGame::EnterHiloGambleEntry(unsigned char JokerWin)
{
static int StartOnce=0;
bool EntryFlag=false;

//	if (GetGameIndex() == Game200p)
//		DealDraw2PndPbLamp(MLAMP_ON); 
//	else
		DealDraw1PndPbLamp(MLAMP_ON);

	if (!StartOnce)
	{
		TheAudioManager::Instance()->GetAudioSample("HILOSND")->Play(true);
		StartOnce = 1;
	}

	if (GetGameIndex() == Game200p)
	{
#ifdef SOAK_BUILD
		if(SoakBuildPressButton(&PokerGame::ReadDealDraw2PndPb,Get200pStake(),Game200p))
#else
		if (ReadTS22StartPb() || ReadDealDraw2PndPb())
#endif
			EntryFlag = true;
	}
	else
	{
#ifdef SOAK_BUILD
		if(SoakBuildPressButton(&PokerGame::ReadDealDraw1PndPb,Get100pStake(),Game100p))
#else
		if (ReadTS22StartPb() || ReadDealDraw1PndPb())		
#endif
			EntryFlag = true;
	}

	if (EntryFlag)
		StartOnce = 0;

	return(EntryFlag);
}

void PokerGame::TurnOffButtons()
{
	if (this->ActivateHiloGambleGraphics)
	{
		THE_BUTTONS->SetButtonActivity(false, "FrontStart");
		THE_BUTTONS->SetButtonActivity(false, "Stake");
		THE_BUTTONS->SetButtonActivity(false, "TopStart");
		THE_BUTTONS->SetButtonActivity(false, "Collect");

		THE_BUTTONS->SetOSButtonActivity(false, "Hold1Button");
		THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
		THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
		THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
		THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
		THE_BUTTONS->SetOSButtonActivity(false, "Hold5Button");
		THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
		THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

		OBJECT_HANDLER->GetObject2D("GraphicalButton08")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("Legend1DealDrawLit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("Legend1DealDrawNlit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("Legend2DealDrawLit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("Legend2DealDrawNlit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("LegendAutoPlayLit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("LegendAutoPlayNlit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("LegendHoldInfoLit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("LegendHoldInfoNlit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("LegendCollectNlit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("LegendCollectLit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("LegendHold2Nlit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("LegendHold2Lit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("LegendHold3Nlit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("LegendHold3Lit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("LegendHold4Nlit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("LegendHold4Lit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("LegendHoldTransferNlit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("LegendHoldTransferLit")->SetVisible(false);
	
		OBJECT_HANDLER->GetObject2D("LegendHiNlit")->SetVisible(true);
		OBJECT_HANDLER->GetObject2D("LegendLoNlit")->SetVisible(true);
	
		OBJECT_HANDLER->GetObject2D("LegendHoldTransferNlit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("Legend1SwopNlit")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("Legend1SwopLit")->SetVisible(false);

		OBJECT_HANDLER->GetObject2D("GraphicalButton01")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("GraphicalButton02")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("GraphicalButton04")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("GraphicalButton06")->SetVisible(false);
		OBJECT_HANDLER->GetObject2D("GraphicalButton09")->SetVisible(false);
	}
}


void PokerGame::EnterHiloGambleScreen(unsigned char JokerWin)
{
	//DealDraw2PndPbLamp(MLAMP_OFF); 
	//DealDraw1PndPbLamp(MLAMP_OFF);


	TurnOffButtons();
	/*if (GetGameIndex() == Game200p)
	{		
		THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton",NO_LEGEND);
		Swop2PndPbLamp(MLAMP_OFF);
	}
	else
	{		
		THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton",NO_LEGEND);
		Swop1PndPbLamp(MLAMP_OFF);
	}*/

	Swop1PndPbLamp(MLAMP_OFF);
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button",NO_LEGEND);
		
	for (i=0; i<NUM_CARDS_IN_HAND;i++)
		RemoveCard(i);
				
	/* Max cards displayable on screen is five		*/
	/* so display max cards if gambles to jackpot	*/
	/* are greater than 5 otherwise display less	*/		
	if (NumGambles < 4)
	{
		for (i=0; i < NumGambles+1; i++)			
			HiloPutCard(i,HiloStatusArray[i]);		
	}
	else
	{
		for(i=0; i<5; i++)					
			HiloPutCard(i,HiloStatusArray[i]);		
	}

	HiloGambleFlag = 1;
	LevelCtr = 0;
	Hgamble = 0;	
}

void PokerGame::HiloGamble(unsigned char JokerWin)
{

	if (ENGINE->GetSystemTimer().GetRunningTime() < HiloDelayTimer)
		return;

	HiloDelayTimer = 0;
	switch (HiloGambleStage)
	{
		case 1: EnterHiloGambleInitialize(JokerWin);	
				HiloGambleStage = 2;
				break;
		case 2: EnterHiloGambleShowAward(JokerWin);	
				HiloGambleStage = 3;
				break;				
		case 3: if (EnterHiloGambleEntry(JokerWin))	
				{
					ActivateHiloGambleGraphics = 1;
					HiloGambleStage = 4;
				}
				break;
		case 4: EnterHiloGambleScreen(JokerWin);	
				HiloGambleStage = 5;
				break;
		case 5:	PlayHiloGambleInitializeGame(HiloStatusArray,JokerWin|ModeGameIn[GameIndex]);
				HiloGambleStage = 6;
				break;
		case 6:	if (PlayHiloGambleSelection(HiloStatusArray,JokerWin|ModeGameIn[GameIndex]))				
					HiloGambleStage = 7;
				break;
		case 7:	PlayHiloGambleProcessSelection(HiloStatusArray,JokerWin|ModeGameIn[GameIndex]);
				HiloGambleStage = 8;
				break;
		case 8:	if (PlayHiloGambleRepeat(HiloStatusArray,JokerWin|ModeGameIn[GameIndex]))
					HiloGambleStage = 5;
			    else
					HiloGambleStage = 9;
				break;
		case 9:	PlayHiloGambleGetPayment(HiloStatusArray,JokerWin|ModeGameIn[GameIndex]);
			    HiloGambleStage = 10;
				FirstTimeEntry = true;
				break;
		default:break;
	}
}

unsigned char PokerGame::PlayHiloGambleInitializeGame(unsigned char *HiloStatusArray,unsigned char JokerWin)
{
	i = 0;
	Hi = Lo = Col = 0;
	// find element of hilo_status_array
	// into which the next card is to be placed
	for(i=0;HiloStatusArray[i];i++);
	
	SetAwardValueLitState(AwardLevel+LevelCtr,MLAMP_ON);
	SetAwardValueLitState(AwardLevel+LevelCtr+1,MLAMP_FLASH);
	
	CollectPbLamp(MLAMP_FLASH);

	/* draw additional cards for gamble involving more than five*/
	/* gambles */
	if (i==5)
	{
		/* erase all displayed cards */
		for (j=0;j<NUM_CARDS_IN_HAND;j++)
			HiloRemoveCard(j);
			
		HiloStatusArray[5] = HiloStatusArray[4];

		GamblesLeft = NumGambles - 4;

		if (GamblesLeft > 4)
			NumCards = 5;
		else
			NumCards = GamblesLeft+1;

		/* draw back of remaining cards */
		for (j=0;j<NumCards;j++)
			HiloPutCard(j,HiloStatusArray[5+j]);
		i=6;
	}
	else if (i==10)
	{
		/* erase all displayed cards */
		for (j=0;j<NUM_CARDS_IN_HAND;j++)
			HiloRemoveCard(j);

		HiloStatusArray[10] = HiloStatusArray[9];
		
		// Remaining Gambles (after two HiLo screens [8 Cards])
		GamblesLeft = NumGambles - 8;

		if (GamblesLeft > 4)
			NumCards = 5;
		else
			NumCards = GamblesLeft+1;
		
		// Draws remaining HiLo cards on the screen
		for (j=0;j<NumCards;j++)
			HiloPutCard(j,HiloStatusArray[10+j]);
		i=11;
	}


	CardValue = (HiloStatusArray[i-1]&0x3f);	// ditch suite

	if (CardValue == JOKER_CARD)
		NoJokerFlag = 1;
	else
		NoJokerFlag = 0;

	if (CardValue != JOKER_CARD)
	{
		do{
			if(CardValue > 13)
				CardValue -=13;
		}while(CardValue > 13);
	}
	
	if(CardValue > 5 && CardValue < 10)	
	{
		if (!LevelCtr)
			Perct = 20;
		else
			Perct = 90;
		

		if (GetLocalCharRandomNumber(100) < Perct 
			&& (HiloRepeatTab[LevelCtr]||GetLocalCharRandomNumber(100)<5) 
			&& JokerWin 
			&& HiloStore[GameIndex] > HiloStoreMinValue[GameIndex])
		{
			FirstTimeEntry = false;
			AllowSwap = ExternAllowSwap = 1;
			
			Swop1PndPbLamp(MLAMP_FLASH);
				
			TheAudioManager::Instance()->GetAudioSample("WOLF_SND")->Play();
			OBJECT_HANDLER->GetObject2D("GraphicalButton10")->SetVisible(true);
			THE_BUTTONS->SetButtonActivity(true, "SwopButton");
		}
		else
			AllowSwap = ExternAllowSwap = 0;
	}
	else
		AllowSwap = ExternAllowSwap = 0;


	AllowHiPb = 0;
	AllowLoPb = 0;

	return(0);
}


unsigned char PokerGame::PlayHiloGambleSelection(unsigned char *HiloStatusArray,unsigned char JokerWin)
{
	if (AllowSwap)
	{
	//	if (GetGameIndex() == Game200p)				
	//		Swop2PndPbLamp(MLAMP_FLASH);
	//	else
			Swop1PndPbLamp(MLAMP_FLASH);
	}

	CollectPbLamp(MLAMP_FLASH); //Added 24/9/13

	if (CardValue==1)
	{
		AllowHiPb = 1;

		HiPbLamp(MLAMP_FLASH);
		LoPbLamp(MLAMP_OFF);

#ifdef SOAK_BUILD
		if (1) //Modification for InnoCore.			
		{
			if (NmiHiloStrategy == SensibleStrategy || NmiHiloStrategy == AggressiveStrategy)
				Hi = 1; 
			else
				Col = 1;			
		} 		                
#else
        if (Hi = ReadHiPb()) 
		{			
			HiPbLamp(MLAMP_ON);
		}
#endif
	}
	else if (CardValue == 13)
	{			
		AllowLoPb = 1;

		HiPbLamp(MLAMP_OFF);
		LoPbLamp(MLAMP_FLASH);			

#ifdef SOAK_BUILD
		if (1) //Modification for InnoCore.			
		{
			if (NmiHiloStrategy == SensibleStrategy || NmiHiloStrategy == AggressiveStrategy)
				Lo = 1;
			else
				Col = 1;		
		}
#else
        if ((Lo = ReadLoPb())) 
		{	
			LoPbLamp(MLAMP_ON);			
		}
#endif
	}
	else
	{
		AllowHiPb = 1;
		AllowLoPb = 1;

		HiPbLamp(MLAMP_FLASH);
		LoPbLamp(MLAMP_AFLASH);			

#ifdef SOAK_BUILD
		if (!AllowSwap) //Modification for Innocore.			
		{
			if (NmiHiloStrategy == SensibleStrategy)
			{
				if (ModeGameIn[GameIndex])
				{
					if(CardValue <= 6)
					{
						Hi = 1;						
					}
					else 
					{
						Lo = 1;						
					}
				}
				else
				{
					if(CardValue <=4)
					{
						Hi = 1;						
					}
					else if(CardValue >= 9)
					{
						Lo = 1;
					}
					else
					{
						Col = 1;
					}
				}
			}
			else if (NmiHiloStrategy == AggressiveStrategy)
			{
				if(CardValue <= 6)
				{
					Hi = 1;
				}
				else 
				{
					Lo = 1;
				}
			}
			else if (NmiHiloStrategy == CollectStrategy)
			{
				Col = 1;
			}
		}		
#else
        if ((Hi = ReadHiPb()) || (Lo = ReadLoPb())) 
		{
			//LightsOff();
			if (Hi) HiPbLamp(MLAMP_ON);
			if (Lo) LoPbLamp(MLAMP_ON);			
		}
#endif
	}

	if(AllowSwap)
	{	
#ifdef SOAK_BUILD
		if (1)
#else
		if(SpecialSwopCardPb() || ReadTS22StartPb())
#endif

		{
			/*if (GetGameIndex() == Game200p)
			{
				Swop2PndPbLamp(MLAMP_ON);
				Swop2PndPbLamp(MLAMP_OFF);
			}
			else
			{*/
				Swop1PndPbLamp(MLAMP_ON);
				Swop1PndPbLamp(MLAMP_OFF);
			//}
				
			OldCard = (HiloStatusArray[i-1]&0x3f);
				
			if (OldCard != JOKER_CARD)
			{
				do{
					if(OldCard > 13)
					OldCard -=13;
				}while(OldCard > 13);
			}
				

			do{
				if (HiloRepeatTab[LevelCtr])
					NewCard = DecWinSwap(OldCard);						
				else
					NewCard = DecLoseSwap(OldCard);
			}while(!(NewCard=PermitSelection(NewCard,HiloStatusArray)));

			NewCard = SwopToJoker(NewCard);
					
			HiloStatusArray[i-1] = NewCard; //0xc0; Tag swap
			TheAudioManager::Instance()->GetAudioSample("CFLIP")->Play();

			HiloPutCard(i-1,HiloStatusArray[i-1]);
			AllowSwap = 0;
			NewCard = (HiloStatusArray[i-1]&0x3f);
				
			if (NewCard != JOKER_CARD)
			{
				do{
					if(NewCard > 13)
						NewCard -=13;
				}while(NewCard > 13);
			}

			AllowSwap = ExternAllowSwap = 0;
					
			CardValue = NewCard; //I have moved this could this have caused a % problem
				
			if(NewCard == 1) //Ace
			{
				LoPbLamp(MLAMP_OFF);
			}
			if(NewCard == 13) //King
			{
				HiPbLamp(MLAMP_OFF);
			}			
#ifdef SOAK_BUILD
			if (1) //Modification for InnoCore.				
			{
				if (NmiHiloStrategy == SensibleStrategy)
				{
					if (ModeGameIn[GameIndex])
					{
						if(CardValue <= 6)
						{
							Hi = 1;		
						}
						else 
						{
							Lo = 1;
						}
					}
					else
					{
						if(CardValue <=4)
						{
							Hi = 1;
						}
						else if(CardValue >= 9)
						{
							Lo = 1;
						}
						else
						{
							Col = 1;
						}
					}
				}
				else if (NmiHiloStrategy == AggressiveStrategy)
				{
					if(CardValue <= 6)
					{
						Hi = 1;						
					}
					else 
					{
						Lo = 1;
					}
				}
				else if (NmiHiloStrategy == CollectStrategy)
				{
					Col = 1;			
				}
			}
#endif
		}
	}

	if(ReadCollectPb())
		Col = 1;	
       
	if (Hi || Lo || Col)
		return(1);
	else
		return(0);
}


unsigned char PokerGame::PlayHiloGambleProcessSelection(unsigned char *HiloStatusArray,unsigned char JokerWin)
{
	CardValue = (HiloStatusArray[i-1]&0x3f);	// ditch suite

	if (CardValue != JOKER_CARD)
	{
		do{
			if(CardValue > 13)
				CardValue -=13;
		}while(CardValue > 13);
	}

	/*if (GetGameIndex() == Game200p)
		Swop2PndPbLamp(MLAMP_OFF);
	else*/
//	Swop1PndPbLamp(MLAMP_OFF);
//	THE_BUTTONS->SetOSButtonActivity(false, "SwopButton");
	
	CollectPbLamp(MLAMP_OFF);
	if(Lo)
	{
		Tag = LO_SELECTED;
		HiPbLamp(MLAMP_OFF);		
		LoPbLamp(MLAMP_ON);		
	}
	else if (Hi)
	{
		Tag = HI_SELECTED;	
		HiPbLamp(MLAMP_ON);		
		LoPbLamp(MLAMP_OFF);		
	}
	else if(Col)
	{	
		HiPbLamp(MLAMP_OFF);
		LoPbLamp(MLAMP_OFF);		
		CollectPbLamp(MLAMP_ON);
		SetAwardValueLitState(AwardLevel+LevelCtr+1,MLAMP_OFF);
		GameType = COLLECT_WIN;
		CollectPbLamp(MLAMP_OFF);
	}

	if (!Col)
	{
		do
		{
			if(!HiloRepeatTab[LevelCtr]) /* losing gamble */
			{
				PrimeWinSnd = 0;
				if(Lo)
				{
					NextCard = DecLoseLo(CardValue);
					if ((NextCard < CardValue) || (CardValue == JOKER_CARD))
					{
						PrimeWinSnd = 1;
						ExtraWin += AwardTable[GameIndex][AwardLevel + LevelCtr];
					}
				}
				else // player gone Hi
				{
					NextCard = DecLoseHi(CardValue);
					if ((NextCard > CardValue) || (CardValue == JOKER_CARD)) 
					{
						PrimeWinSnd = 1;
						ExtraWin += AwardTable[GameIndex][AwardLevel + LevelCtr];
					}
				}
			}
			else // player must win
			{
				PrimeWinSnd = 1;
				if(Lo)
				{
					if(HiloRepeatTab[LevelCtr+1]) /* next repeat is win */
					{
					       NextCard = DecLoWinWin(CardValue);						  
					}
					else
					{
					       NextCard = DecLoWinLose(CardValue);
					}

					if ((NextCard > CardValue) && (NextCard != JOKER_CARD) && (CardValue != JOKER_CARD))
						PrimeWinSnd = 0;
				}
				else // player gone Hi
				{
					if(HiloRepeatTab[LevelCtr+1]) /* next repeat is win */
					{
					       NextCard = DecHiWinWin(CardValue);						   
					}
					else
					{
					       NextCard = DecHiWinLose(CardValue);
					}

					if ((NextCard < CardValue) && (CardValue != JOKER_CARD))
						PrimeWinSnd = 0;
				}

			}

		}while(!(CompleteCard=PermitSelection(NextCard,HiloStatusArray)));
		HiloStatusArray[i]= (CompleteCard|Tag);//store this games card

		SetAwardValueLitState(AwardLevel+LevelCtr,MLAMP_OFF);

		TheAudioManager::Instance()->GetAudioSample("CFLIP")->Play();
		HiloPutCard(i,HiloStatusArray[i]);

		if(PrimeWinSnd)
		{
			SetAwardValueLitState(AwardLevel+LevelCtr+1,MLAMP_ON);			
			if (AwardLevel+LevelCtr+1 < (unsigned char)FunctionMaxLevels())
			{
				TheAudioManager::Instance()->GetAudioSample("WOLF_SND")->Play();			
			}
			GameType = FEATURE_WIN;
		}
		else
		{		
			SetAwardValueLitState(AwardLevel+LevelCtr+1,MLAMP_OFF);
			GameType = FEATURE_LOSE;
		}

		if (++Hgamble > 3)			
			Hgamble = 0;

		HiloDelayTimer = ENGINE->GetSystemTimer().GetRunningTime();
		HiloDelayTimer +=ThePokerGame::Instance()->GetSoundDelay(CFLIP,0);
		if (GameType == FEATURE_WIN)										
			HiloDelayTimer +=ThePokerGame::Instance()->GetSoundDelay(WOLF_SND,0);
#ifdef SOAK_BUILD
		HiloDelayTimer = 0;
#endif

	}
	return(0);
}

unsigned char PokerGame::PlayHiloGambleRepeat(unsigned char *HiloStatusArray,unsigned char JokerWin)
{
	if (GameType == FEATURE_WIN && !HiloRepeatTab[LevelCtr] && !JokerWin )
		ExtraWinCtr++;

	if (GameType == FEATURE_WIN)
		LevelCtr++;
	

	if((GameType == FEATURE_WIN && LevelCtr < NumGambles) && !global_quit)
		return(1);
	else
		return(0);
}

unsigned char PokerGame::PlayHiloGambleGetPayment(unsigned char *HiloStatusArray,unsigned char JokerWin)
{
	TheAudioManager::Instance()->GetAudioSample("HILOSND")->Stop();

	if (GameType == FEATURE_WIN || GameType == COLLECT_WIN)
	{
		Pay = AwardTable[GameIndex][AwardLevel + LevelCtr];		
	}
	else
		Pay = 0;

	FirstTimeEntry = true;
	return(0);
}


void PokerGame::SelectInitialCard(void)
{
unsigned char FirstCard;
unsigned char i;

	for(i=0;i<15;i++)
	{
		HiloStatusArray[i]=0;
	}


	FirstCard = DecInitialCardValue();

	i = GetLocalCharRandomNumber(4);
	if(i==1)
		FirstCard += 13;
	else if(i==2)
		FirstCard += 26;
	else if(i==3)
		FirstCard += 39;
			
	HiloStatusArray[0] = FirstCard;

}

unsigned char PokerGame::PermitSelection(unsigned char NextCard, unsigned char * HiloStatusArray)
{
unsigned char ReturnValue;
unsigned char i;
	
	if (NextCard == JOKER_CARD)
		return(NextCard);
	else
	{
		i = GetLocalCharRandomNumber(4);
		if(i==1)
			NextCard += 13;
		else if(i==2)
			NextCard += 26;
		else if(i==3)
			NextCard += 39;

		ReturnValue = NextCard;

		for(i=0;i<= FunctionMaxLevels()-FindWinLevel(Pay);i++)
		{
			if((HiloStatusArray[i]&0x3f)==NextCard)
				ReturnValue = 0;
		}
		return(ReturnValue);
	}
}

unsigned char PokerGame::SwopToJoker(unsigned char Card)
{
unsigned char InitialCard;

	InitialCard = Card;

	if (Card != JOKER_CARD)
	{
		do{
			if(Card > 13)
				Card -=13;
		}while(Card > 13);
	}


	if ((Card == 1 || Card == 13) && (GetLocalCharRandomNumber(100) < 15))
		Card = JOKER_CARD;
	else
		Card = InitialCard;

	return(Card);
}

BOOL PokerGame::SpecialSwopCardPb(void)
{
	if(THE_BUTTONS->ButtonPressed("TopStart") || THE_BUTTONS->OSButtonPressed("SwopButton"))
		return(true);
	else
		return(false);
}

void PokerGame::CollectPbLamp(unsigned char state)
{
	if(state==MLAMP_ON)
	{
		//THE_BUTTONS->SetOSButtonActivity(true, "Hold1Button",LAMP_ON);
		THE_BUTTONS->SetButtonActivity(true, "Collect",LAMP_ON);
	}			
	else if(state==MLAMP_FLASH)
	{
		//THE_BUTTONS->SetOSButtonActivity(true, "Hold1Button",LAMP_FLASH);
		THE_BUTTONS->SetButtonActivity(true, "Collect",LAMP_FLASH);
	}
	else if(state==MLAMP_AFLASH)
	{
		//THE_BUTTONS->SetOSButtonActivity(true, "Hold1Button",LAMP_ANTI);
		THE_BUTTONS->SetButtonActivity(true, "Collect",LAMP_ANTI);
	}
	else	
	{
		//THE_BUTTONS->SetOSButtonActivity(false, "Hold1Button",LAMP_OFF);
		THE_BUTTONS->SetButtonActivity(false, "Collect",LAMP_OFF);
	}
}

void PokerGame::HiPbLamp(unsigned char state)
{
	if(state==MLAMP_ON)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "HiButton",LAMP_ON);		
	}			
	else if(state==MLAMP_FLASH)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "HiButton",LAMP_FLASH);		
	}
	else if(state==MLAMP_AFLASH)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "HiButton",LAMP_ANTI);
	}
	else	
	{
		THE_BUTTONS->SetOSButtonActivity(false, "HiButton",LAMP_OFF);		
	}

}

void PokerGame::LoPbLamp(unsigned char state)
{
	if(state==MLAMP_ON)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "LoButton",LAMP_ON);		
	}			
	else if(state==MLAMP_FLASH)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "LoButton",LAMP_FLASH);		
	}
	else if(state==MLAMP_AFLASH)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "LoButton",LAMP_ANTI);
	}
	else	
	{
		THE_BUTTONS->SetOSButtonActivity(false, "LoButton",LAMP_OFF);		
	}

}

void PokerGame::TS22StartPbLamp(unsigned char state)
{
	if(state==MLAMP_ON)
	{		
		THE_BUTTONS->SetButtonActivity(true, "FrontStart",LAMP_ON);
	}			
	else if(state==MLAMP_FLASH)
	{		
		THE_BUTTONS->SetButtonActivity(true, "FrontStart",LAMP_FLASH);
	}
	else if(state==MLAMP_AFLASH)
	{		
		THE_BUTTONS->SetButtonActivity(true, "FrontStart",LAMP_ANTI);
	}
	else	
	{		
		THE_BUTTONS->SetButtonActivity(false, "FrontStart",LAMP_OFF);
	}

}

void PokerGame::DealDraw1PndPbLamp(unsigned char state)
{
	TS22StartPbLamp(state);

	if(state==MLAMP_ON)
	{
		//THE_BUTTONS->SetOSButtonActivity(true, "DealStart1PndButton",LAMP_ON);
		THE_BUTTONS->SetButtonActivity(true, "TopStart", LAMP_ON);
		//GET_BUTTONS->SetButtonActivity(true, "Stake",LAMP_ON);
	}			
	else if(state==MLAMP_FLASH)
	{
		//THE_BUTTONS->SetOSButtonActivity(true, "DealStart1PndButton",LAMP_FLASH);
		THE_BUTTONS->SetButtonActivity(true, "TopStart",LAMP_FLASH); // Was Stake
	}
	else if(state==MLAMP_AFLASH)
	{
		//THE_BUTTONS->SetOSButtonActivity(true, "DealStart1PndButton",LAMP_ANTI);
		THE_BUTTONS->SetButtonActivity(true, "TopStart",LAMP_ANTI); //was Stake
	}
	else	
	{
		//THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton",LAMP_OFF);
		//GET_BUTTONS->SetButtonActivity(false, "Stake",LAMP_OFF);
		THE_BUTTONS->SetButtonActivity(false, "TopStart",LAMP_OFF);
	}
}

void PokerGame::DealDraw2PndPbLamp(unsigned char state)
{
	TS22StartPbLamp(state);

	/*if(state==MLAMP_ON)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "DealStart2PndButton",LAMP_ON);
		THE_BUTTONS->SetButtonActivity(true, "TopStart",LAMP_ON);
	}			
	else if(state==MLAMP_FLASH)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "DealStart2PndButton",LAMP_FLASH);
		THE_BUTTONS->SetButtonActivity(true, "TopStart",LAMP_FLASH);
	}
	else if(state==MLAMP_AFLASH)
	{
		THE_BUTTONS->SetOSButtonActivity(true, "DealStart2PndButton",LAMP_ANTI);
		THE_BUTTONS->SetButtonActivity(true, "TopStart",LAMP_ANTI);
	}
	else	
	{
		THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton",LAMP_OFF);
		THE_BUTTONS->SetButtonActivity(false, "TopStart",LAMP_OFF);
	}*/
}

void PokerGame::Swop1PndPbLamp(unsigned char state)
{
	TS22StartPbLamp(state);

	if(state==MLAMP_ON)
	{
		if (!FirstTimeEntry)
			THE_BUTTONS->SetOSButtonActivity(true, "SwopButton",LAMP_ON);
		THE_BUTTONS->SetButtonActivity(true, "Stake",LAMP_ON);
	}			
	else if(state==MLAMP_FLASH)
	{
		if (!FirstTimeEntry)
			THE_BUTTONS->SetOSButtonActivity(true, "SwopButton",LAMP_FLASH);
		THE_BUTTONS->SetButtonActivity(true, "Stake",LAMP_FLASH);
	}
	else if(state==MLAMP_AFLASH)
	{
		if (!FirstTimeEntry)
			THE_BUTTONS->SetOSButtonActivity(true, "SwopButton",LAMP_ANTI);
		THE_BUTTONS->SetButtonActivity(true, "Stake",LAMP_ANTI);
	}
	else	
	{
		if (!FirstTimeEntry)
		{
			THE_BUTTONS->SetOSButtonActivity(false, "SwopButton",LAMP_OFF);
			OBJECT_HANDLER->GetObject2D("GraphicalButton10")->SetVisible(false);
			//THE_BUTTONS->SetOSButtonActivity(false, "SwopButton");
			OBJECT_HANDLER->GetObject2D("Legend1SwopNlit")->SetVisible(false);
			OBJECT_HANDLER->GetObject2D("Legend1SwopLit")->SetVisible(false);
		}
		THE_BUTTONS->SetButtonActivity(false, "Stake",LAMP_OFF);
	}
}

void PokerGame::Swop2PndPbLamp(unsigned char state)
{
	TS22StartPbLamp(state);

	if(state==MLAMP_ON)
	{
		//GET_BUTTONS->SetOSButtonActivity(true, "Swop2PndButton",LAMP_ON);
		THE_BUTTONS->SetButtonActivity(true, "TopStart",LAMP_ON);
	}			
	else if(state==MLAMP_FLASH)
	{
		//GET_BUTTONS->SetOSButtonActivity(true, "Swop2PndButton",LAMP_FLASH);
		THE_BUTTONS->SetButtonActivity(true, "TopStart",LAMP_FLASH);
	}
	else if(state==MLAMP_AFLASH)
	{
		//GET_BUTTONS->SetOSButtonActivity(true, "Swop2PndButton",LAMP_ANTI);
		THE_BUTTONS->SetButtonActivity(true, "TopStart",LAMP_ANTI);
	}
	else	
	{
		//GET_BUTTONS->SetOSButtonActivity(false, "Swop2PndButton",LAMP_OFF);
		THE_BUTTONS->SetButtonActivity(false, "TopStart",LAMP_OFF);
	}
}

bool PokerGame::ReadTS22StartPb(void)
{
	if (THE_BUTTONS->ButtonPressed("FrontStart"))
		return(true);
	else
		return(false);		
}

bool PokerGame::ReadDealDraw2PndPb(void)
{
	if (THE_BUTTONS->ButtonPressed("TopStart") || THE_BUTTONS->ButtonPressed("FrontStart"))
		return true;
		/* || */
		//THE_BUTTONS->OSButtonPressed("DealStart2PndButton"))
		//return(true);
	//else
		return(false);	
}

bool PokerGame::ReadDealDraw1PndPb(void)
{	
	if (THE_BUTTONS->ButtonPressed("TopStart") || THE_BUTTONS->ButtonPressed("FrontStart"))
		return true;
	else
		return false;
		/* ||
		//THE_BUTTONS->OSButtonPressed("DealStart1PndButton"))
		//return(true);
	//else
		return(false);*/
}

bool PokerGame::ReadHiPb(void)
{
	if (THE_BUTTONS->OSButtonPressed("HiButton"))
		return(true);
	else
		return(false);
}

bool PokerGame::ReadLoPb(void)
{
	if (THE_BUTTONS->OSButtonPressed("LoButton"))
		return(true);
	else
		return(false);
}

bool PokerGame::ReadCollectPb(void)
{
	if (THE_BUTTONS->ButtonPressed("Collect"))// || THE_BUTTONS->OSButtonPressed("Hold1Button"))
		return(true);
	else
		return(false);	
}
