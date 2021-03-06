#ifndef _PAR56_H
#define _PAR56_H

#define PAR56_COUNT 4
#define PAR56_BASE_ADDR 8
#define PAR56_ADD_OFFSET 8

static void create_par56(void)
{
	dmx_device_create_ledpar6(8,"par-1");
	dmx_device_create_ledpar6(16,"par-2");
	dmx_device_create_ledpar6(24,"par-3");
	dmx_device_create_ledpar6(32,"par-4");
}


#endif
