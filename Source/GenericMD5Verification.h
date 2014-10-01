#ifndef MD5VERIFICATION_H
#define MD5VERIFICATION_H

#include <string>

static const int NUMBER_OF_CHECKSUMS = 7;
static const int SIZE_OF_DIGEST = 33;
static const int PATH_LENGTH = 260;

static const char* FILE_NAMES[NUMBER_OF_CHECKSUMS] =
{
	"AudioSamples.pdt",
	"Cameras.pdt",
	"Lights.pdt",
	"Objects.pdt",
	"RenderSequences.pdt",
	"Shaders.pdt",
	"Textures.pdt"
};

static const char* MDAT_CHECKSUMS[NUMBER_OF_CHECKSUMS] =
{
	"E2872FA075DCE2C8CD3B924470BD1A4B",
	"E2A197B3AE1D513D176074A83D1D705B",
	"08A4415E9D594FF960030B921D42B91E",
	"AE144ED2E2B8017B67680C5D402E9040",
	"08A4415E9D594FF960030B921D42B91E",
	"08A4415E9D594FF960030B921D42B91E",
	"A1444483EDD3819D634882F1FC5C436A"
};

bool CompareMD5Hash(const char* checkSum, const char* name)
{
	unsigned int i = 0;
	for(i = 0; i < NUMBER_OF_CHECKSUMS; i++)
	{
		if(strcmp(name, FILE_NAMES[i]) == 0)
		{
			break;
		}
	}

	if(strcmp(MDAT_CHECKSUMS[i], checkSum) == 0)
	{
		return true;
	}
	return false;
}

#endif MD5VERIFICATION_H