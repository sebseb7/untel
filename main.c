#include <stdio.h>


#include "main.h"
#include "dmx_devices.h"


int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) 
{


	dmx_device_create_ledpar(8,0,"vorn links");
	dmx_device_create_ledpar(16,0,"vorn rechts");
	dmx_device_create_ledpar(24,0,"hinten links");
	dmx_device_create_ledpar(32,0,"hinten rechts");
	dmx_device_create_ledpar(32,0,"hinten rechts2");





	for(unsigned int i = 0;i< dmx_get_device_count();i++)
	{
		printf("%ui %i %i %s\n",i,dmx_get_device_count(),dmx_get_device_byidx(i)->addr,dmx_get_device_byidx(i)->name);
	}


	return 0;
}

