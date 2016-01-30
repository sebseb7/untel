#ifndef _DMX_CHANNELS_H
#define _DMX_CHANNELS_H

#include "dmx_devices.h"

void dmx_channels_clear(void);

void dmx_channel_set(unsigned int ch,unsigned char value);
		
void dmx_channels_print(void);

unsigned char* dmx_channels_get(void);

#endif
