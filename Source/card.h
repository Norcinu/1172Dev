

/**********************************************************/
//
//	Card definitions
//
/**********************************************************/
#ifndef	__CARD
#define __CARD

#include <math.h>
#include <windows.h>   // include important windows stuff

#define NUM_CARDS_IN_HAND	5


typedef struct _CARD{

	int		id;
	int		hold;
	int		xpos; 
	int		ypos;
	int		height; 
	int		width;

}CARD;

#endif






