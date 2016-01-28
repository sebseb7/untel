#ifndef _DMX_DEVICES_H
#define _DMX_DEVICES_H

#define DMX_NAME_LENGTH 20

enum {
	DMX_DEVICE_GROUP,
	DMX_DEVICE_FOG,
	DMX_DEVICE_LEDPAR,
};

struct dmx_device {
	unsigned int type;
	unsigned int addr;
	unsigned int refcount;
	char name[DMX_NAME_LENGTH];
	void* device;
};

struct dmx_device_ledpar {
	unsigned int addr;
	unsigned int type;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	float dim;
	unsigned int blackout;
};

struct dmx_device_fog {
	unsigned int addr;
	unsigned int ch1;
};


struct dmx_device_ledpar * dmx_device_create_ledpar(unsigned int addr,unsigned int type,char* name);
struct dmx_device_fog*  dmx_device_create_fog(unsigned int addr);
unsigned int dmx_get_device_count(void);
struct dmx_device* dmx_get_device(unsigned int type,char* name);
struct dmx_device* dmx_get_device_byidx(unsigned int index);
void dmx_devices_clear(void);


struct dmx_device_ledpar * dmx_ledpar_lock(unsigned int addr,unsigned int type,char* name);

#endif
