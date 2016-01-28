
#include "dmx_channels.h"

#define DMX_CHANNEL_COUNT 512

static unsigned char dmx_channels[DMX_CHANNEL_COUNT];

void dmx_channels_clear(void)
{
	for(unsigned int i = 0;i<DMX_CHANNEL_COUNT;i++)
	{
		dmx_channels[i]=0;
	}
}

