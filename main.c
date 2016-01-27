#include <stdio.h>


#include "main.h"
#include "dmx_devices.h"
#include "dmx_image.h"


int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) 
{


	dmx_device_create_ledpar(8,0,"vorn links");
	dmx_device_create_ledpar(16,0,"vorn rechts");
	dmx_device_create_ledpar(24,0,"hinten links");
	dmx_device_create_ledpar(32,0,"hinten rechts");
	dmx_device_create_ledpar(32,0,"hinten rechts");


	struct dmx_image* image1 = dmx_image_add(DMX_DEVICE_LEDPAR,"vorn links");
	dmx_image_add_setI(image1,0,255);
	dmx_image_add_setI(image1,1,255);



	for(unsigned int i = 0;i< dmx_get_device_count();i++)
	{
		printf("%ui %i %i %s\n",i,dmx_get_device_count(),dmx_get_device_byidx(i)->addr,dmx_get_device_byidx(i)->name);
	}



	//render

	unsigned char channels[512];


	while(1)
	{
		for(unsigned int i = 0;i<512;i++)
			channels[i]=0;

		dmx_devices_clear();

		//render devices
		unsigned int image_count = dmx_image_get_count();
		for(unsigned int i = 0;i<image_count;i++)
		{
			struct dmx_image* image = dmx_image_getbyidx(i);

			for(unsigned int j=0;j<image->set_count;j++)
			{

			}
		}


		//render channels
	}

	return 0;
}

