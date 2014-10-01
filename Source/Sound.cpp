#include "PokerGame.h"
#include <ObjectHandler.h>

int SoundLength[END_WAV] = {213,115,403,482,1839,3546,3909,299,0,0,820,1044,464};

float PokerGame::GetSoundDelay(int SoundIndex, int AdditionalTime)
{
 float DelayLength;

	DelayLength = (float)(SoundLength[SoundIndex]+AdditionalTime)/(float)1000;

 return(DelayLength);

}