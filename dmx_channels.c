#include <stdio.h>

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

void dmx_channel_set(unsigned int ch,unsigned char value)
{
	if(ch < DMX_CHANNEL_COUNT)
	{
		dmx_channels[ch]=value;
	}
}

void dmx_channels_print(void)
{
	for(unsigned int i = 0;i<48;i++)
	{
		printf("ch:%i val:%i \n",i,dmx_channels[i]);
	}
}

