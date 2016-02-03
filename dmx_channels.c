#include <stdio.h>

#include "dmx_channels.h"


static unsigned char dmx_channels[DMX_CHANNEL_COUNT];

unsigned char* dmx_channels_get(void)
{
	return dmx_channels;
}

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
	for(unsigned int i = 0;i<24;i++)
	{
		printf("ch:%03i val:%03i ",i,dmx_channels[i]);

		for(unsigned int j=0;j<64;j++)
		{
			if(j*4 > dmx_channels[i])
			{
				printf(".");
			}
			else
			{
				printf("#");
			}
		}

		printf("\n");
	}
	printf("\n");
}

