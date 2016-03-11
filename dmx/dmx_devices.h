#ifndef _DMX_DEVICES_H
#define _DMX_DEVICES_H

#include "dmx_defines.h"

enum {
	DMX_DEVICE_GROUP,
	DMX_DEVICE_FOG,
	DMX_DEVICE_LEDPAR6,
	DMX_DEVICE_LEDPAR7,
	DMX_DEVICE_MINISCANHPE,
	DMX_DEVICE_QUADPHASE,
	DMX_DEVICE_STROBE,
};

struct dmx_device {
	unsigned int type;
	unsigned int addr;
	unsigned int refcount;
	char name[DMX_NAME_LENGTH];
	void* device;
};

struct dmx_device_ledpar6 {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	float dim;
	unsigned int blackout;
};
struct dmx_device_strobe {
	float freq;
	float dim;
	unsigned int blackout;
};

struct dmx_device_miniscanhpe {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	float dim;
	float x;
	float y;
	unsigned char gobo;
	float gobo_rot;
	unsigned int blackout;
};

struct dmx_device_fog {
	unsigned int addr;
	unsigned int ch1;
};


struct dmx_device_strobe * dmx_device_create_strobe(unsigned int addr,char* name);
struct dmx_device_ledpar6 * dmx_device_create_ledpar6(unsigned int addr,char* name);
struct dmx_device_miniscanhpe * dmx_device_create_miniscanhpe(unsigned int addr,char* name);
struct dmx_device_fog*  dmx_device_create_fog(unsigned int addr);
unsigned int dmx_get_device_count(void);
struct dmx_device* dmx_get_device(unsigned int type,char* name);
struct dmx_device* dmx_get_device_byidx(unsigned int index);
void dmx_devices_clear(void);
void dmx_devices_free(void);
				
void dmx_device_render_strobe(struct dmx_device* device);
void dmx_device_render_ledpar6(struct dmx_device* device);
void dmx_device_render_miniscanhpe(struct dmx_device* device);
void dmx_device_render_fog(struct dmx_device* device);

void dmx_device_render_ledpar6_sdl(struct dmx_device* device,unsigned int* pixelbuffer,unsigned int row, unsigned int col);
void dmx_device_render_strobe_sdl(struct dmx_device* device,unsigned int* pixelbuffer,unsigned int row, unsigned int col);



#endif
