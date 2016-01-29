#include <stdio.h>
#include <unistd.h>

#include "main.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_channels.h"
#include "dmx_queue.h"
#include "dmx_selector.h"


#define DMX_FRAMERATE 40

int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) 
{

	unsigned int queue_count = dmx_queue_get_count();
	for(unsigned int i = 0;i<queue_count;i++)
	{
		struct dmx_queue* queue = dmx_queue_getbyidx(i);
		queue->init();
	}

	printf("\033[2J");

	//render

	while(1)
	{

		dmx_channels_clear();
		dmx_devices_clear();


		// process queues

		unsigned int queue_count = dmx_queue_get_count();
		for(unsigned int i = 0;i<queue_count;i++)
		{
			struct dmx_queue* queue = dmx_queue_getbyidx(i);

			queue->tick(0);
		}





		
		//render devices (images)
		unsigned int image_count = dmx_image_get_count();
		for(unsigned int i = 0;i<image_count;i++)
		{
			struct dmx_image* image = dmx_image_getbyidx(i);

			dmx_image_render(image);
		}



		//render channels
		
		unsigned int device_count = dmx_get_device_count();
		for(unsigned int i = 0;i<device_count;i++)
		{
			struct dmx_device* device = dmx_get_device_byidx(i);
			
			if(device->type == DMX_DEVICE_LEDPAR)
			{
				dmx_device_render_ledpar(device);
			}
		}

		printf("\033[H");
		dmx_channels_print();

		usleep(25*1000);
	}

	return 0;
}

