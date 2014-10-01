#include "WinData.h"
#include "OutcomesFS_100.h"
#include "OutcomesFS_200.h"
#include "Outcomes100.h"
#include "Outcomes200.h"


int NumberOfOutcomes[ALL_1PND_WINS+FEATURE_WINS]={
	1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, //10
	1000, 1000, 1000, 1000, 1000, 1000, 1000,  610,  285,  790, //20
	6, 6, 														//21 wins = +1 lose = 22
	1000,  215,   25,  950,  355,   60,  250,  145,  100,   80,   
	  60,   45,   35,   20,   25,   25,   10,    5,    5,    5,   
	  10,    5,    5,    5,    5 };	




int NumberOfOutcomes_2pnd[ALL_2PND_WINS+FEATURE_WINS]={
	1000, 1000,	1000, 1000,	1000, 1000,	1000, 1000,	1000, 1000,	//10
	 170,    6,													//11 wins  +1 lose = 12 wins

	1000,  230,	 30,   930,	 300,   65,	 290,  170,	 100,   70,	  
	  60,	40,	 25,	30,	  30,	20,	  10,	 5,   10,    5,	   
	   5,	10,	  5,	 5,    5 };	





/********************************************************************/
unsigned char *WinTables[(ALL_1PND_WINS+FEATURE_WINS)] ={	
								(unsigned char *)outcome1_00000,
								(unsigned char *)outcome1_00500,
								(unsigned char *)outcome1_01000,
								(unsigned char *)outcome1_01500,
								(unsigned char *)outcome1_02000,
								(unsigned char *)outcome1_02500,
								(unsigned char *)outcome1_03000,
								(unsigned char *)outcome1_03500,
								(unsigned char *)outcome1_04000,
								(unsigned char *)outcome1_04500,
								(unsigned char *)outcome1_05000,
								(unsigned char *)outcome1_05500,
								(unsigned char *)outcome1_06000,
								(unsigned char *)outcome1_06500,
								(unsigned char *)outcome1_07000,
								(unsigned char *)outcome1_07500,
								(unsigned char *)outcome1_10000,
								(unsigned char *)outcome1_12500,
								(unsigned char *)outcome1_15000,
								(unsigned char *)outcome1_20000,
								(unsigned char *)outcome1_25000,
								(unsigned char *)outcome1_50000,

							   (unsigned char *)outcome1_f03,
							   (unsigned char *)outcome1_f04,
							   (unsigned char *)outcome1_f05,
							   (unsigned char *)outcome1_f06,
							   (unsigned char *)outcome1_f07,
							   (unsigned char *)outcome1_f08,
							   (unsigned char *)outcome1_f09,
							   (unsigned char *)outcome1_f10,
							   (unsigned char *)outcome1_f11,
							   (unsigned char *)outcome1_f12,
							   (unsigned char *)outcome1_f13,
							   (unsigned char *)outcome1_f14,
							   (unsigned char *)outcome1_f15,
							   (unsigned char *)outcome1_f16,
							   (unsigned char *)outcome1_f17,
							   (unsigned char *)outcome1_f18,
							   (unsigned char *)outcome1_f19,
							   (unsigned char *)outcome1_f20,
							   (unsigned char *)outcome1_f21,
							   (unsigned char *)outcome1_f22,
							   (unsigned char *)outcome1_f23,
							   (unsigned char *)outcome1_f25,
							   (unsigned char *)outcome1_f30,
							   (unsigned char *)outcome1_f40,
							   (unsigned char *)outcome1_f50 };


//GV: use table with no feature symbols in wins
unsigned char *TwoPnd_WinTables[(ALL_2PND_WINS+FEATURE_WINS)] ={
								(unsigned char *)outcome2_00000, //00
							   (unsigned char *)outcome2_01000, //01
							   (unsigned char *)outcome2_02000, //02
							   (unsigned char *)outcome2_03000, //03
							   (unsigned char *)outcome2_04000, //04
							   (unsigned char *)outcome2_05000, //05
							   (unsigned char *)outcome2_06000, //06
							   (unsigned char *)outcome2_07000, //07
							   (unsigned char *)outcome2_10000, //08
							   (unsigned char *)outcome2_15000, //09
							   (unsigned char *)outcome2_25000, //10
							   (unsigned char *)outcome2_50000, //11	// 11 wins +1 lose = 12 wins

							   (unsigned char *)outcome2_f03,
							   (unsigned char *)outcome2_f04,
							   (unsigned char *)outcome2_f05,
							   (unsigned char *)outcome2_f06,
							   (unsigned char *)outcome2_f07,
							   (unsigned char *)outcome2_f08,
							   (unsigned char *)outcome2_f09,
							   (unsigned char *)outcome2_f10,
							   (unsigned char *)outcome2_f11,
							   (unsigned char *)outcome2_f12,
							   (unsigned char *)outcome2_f13,
							   (unsigned char *)outcome2_f14,
							   (unsigned char *)outcome2_f15,
							   (unsigned char *)outcome2_f16,
							   (unsigned char *)outcome2_f17,
							   (unsigned char *)outcome2_f18,
							   (unsigned char *)outcome2_f19,
							   (unsigned char *)outcome2_f20,
							   (unsigned char *)outcome2_f21,
							   (unsigned char *)outcome2_f22,
							   (unsigned char *)outcome2_f23,
							   (unsigned char *)outcome2_f25,
							   (unsigned char *)outcome2_f30,
							   (unsigned char *)outcome2_f40,
							   (unsigned char *)outcome2_f50 };
/********************************************************************/