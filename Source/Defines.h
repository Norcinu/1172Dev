/**************************************************************************       
** FILE NAME: Defines.h
** DESCRIP:	  Configuration Defines   
***************************************************************************/		             
#ifndef DEFINES_H
#define DEFINES_H

#define WIN32_LEAN_AND_MEAN  
	
#include <windows.h>   // include important windows stuff
#include <mmsystem.h>
#include <stdio.h> 
#define WINDOW_TITLE  "Project 1170 Game"
#define MODEL_NUMBER   1170

#define RELEASE_NUMBER   1

#ifdef _AUTOPLAY
	#include <crtdbg.h> //needed to trap memory leaks
	#define SOAK_BUILD
	#define FAST_PLAY
	#define WORKING_CAPITAL_LOG
	#define FIXED_POP 100
	#define RECORD_NMI_LOG
#endif

#define NUMBER_OF_SCREENS 2
	
#define B3_25P	25
#define B3_50P	50
#define B3_1PND	100
#define B3_2PND	200

#define MAXIMUM_BET 		200 	//£2
#define MINIMUM_BET			100		//£1
#define TOTAL_WINNING_LINES  10
#define TOTAL_STAKES          2

/* LAMP STATES */
#define MLAMP_ON				1
#define MLAMP_FLASH				2
#define MLAMP_AFLASH			3
#define MLAMP_OFF				0

#define MAX_LEVEL				12
#define MAX_AWARDS				13
#define LEVEL					13

/* REFERENCES TO SPECIFIC FIVE OF A KIND WINS */
#define J_AKIND5			    1
#define Q_AKIND5			    2
#define K_AKIND5				3	

/* POKER WIN TYPES */
#define ONE_PAIR		(unsigned char)1
#define TWO_PAIR		(unsigned char)2
#define AKIND3			(unsigned char)3
#define STRAIGHT		(unsigned char)4
#define FLUSH			(unsigned char)5
#define FULL_HOUSE		(unsigned char)6
#define AKIND4			(unsigned char)7
#define STRAIGHT_FLUSH	(unsigned char)8
#define AKIND5			(unsigned char)9
#define ALLJACKS		(unsigned char)10
#define ALLQUEENS		(unsigned char)11
#define ALLKINGS		(unsigned char)12
#define ROYAL_FLUSH		(unsigned char)13

#define MAX_TRIES					   30
#define MAX_ELEMENT						5

#define JOKER						    4
#define JOKER_CARD					   53
#define GOLDEN_JOKER_CARD			   54

#define STANDARD02_MODE	  				1
#define STANDARD01_MODE	  				0

#define MaxSelectableGames				2
#define Game100p						0
#define Game200p						1

#define MINIMUM_BET_100p			  100
#define MINIMUM_BET_200p			  200

#define HIGH_STORE_MIN_VALUE	   500000

#define	FEATURE_WIN					 0x01
#define	FEATURE_LOSE				 0x02
#define COLLECT_WIN					 0x03

#define LO_SELECTED					 0x80
#define HI_SELECTED					 0x40

#define EOT							 0xff 

#define MAX_TRIES2					 20

#define NO_LEGEND					 (LAMP_ANTI<<1)

#define	NUM_INFO_SCREEN					2

#define	CARD_HELD					0x40

#define COLLECT_COINS_PB			   0  
#define COLLECT_TICKET_PB			   1  

#define MIN_GAME_TIME				 2.5f

#define SensibleStrategy				0
#define AggressiveStrategy				1
#define CollectStrategy					2

static const int AllStakes[(TOTAL_STAKES+2)] = 
{
	B3_25P,
	B3_50P,
	B3_1PND,
	B3_2PND
};

static const char* StakeButtons[TOTAL_STAKES] = 
{
	"Pop1Pnd",
	"Pop2Pnd"
};

#define PublicKey 976458532

#endif DEFINES_H