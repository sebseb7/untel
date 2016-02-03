#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "main.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_channels.h"
#include "dmx_queue.h"
#include "dmx_selector.h"
#include "dmx_output.h"

#include "keyboard.h"
#include "osc.h"


#define DMX_FRAMERATE 40

static unsigned long long start_time;

static unsigned int getstarttime(void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long long current_time = tv.tv_sec*(unsigned long long)1000+(tv.tv_usec / 1000);

	if(start_time == 0)
		start_time = current_time;
	
	return current_time-start_time;
}




int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) 
{
//	keyboard_list();
//	exit(0);

	//render

	unsigned int bpm=165;
	unsigned int beatpulse=bpm*24;
	unsigned int beatms=60000/bpm;
	unsigned int beatpulsems=60000/beatpulse;

	unsigned int beats=0;
	unsigned int beatpulses=0;
	unsigned int last_beat=getstarttime();
	unsigned int last_beatpulse=getstarttime();

	dmx_output_init();
	osc_init();
	
	printf("\033[2J");

	while(1)
	{
		unsigned int currtime = getstarttime();
		while(currtime > (last_beat+beatms) )
		{
			beats++;
			last_beat=currtime;
		}
		while(currtime > (last_beatpulse+beatpulsems) )
		{
			beatpulses++;
			last_beatpulse=currtime;
		}
	
		dmx_channels_clear();
		dmx_devices_clear();


		// process queues

		unsigned int queue_count = dmx_queue_get_count();
		for(unsigned int i = 0;i<queue_count;i++)
		{
			struct dmx_queue* queue = dmx_queue_getbyidx(i);
			if((queue->active!=0)&&(queue->next < currtime))
			{
				unsigned int delay = queue->tick(beats);
				queue->next = currtime+((beatms/24)*delay);
			}
		}





		
		//render devices (images)
		unsigned int image_count = dmx_image_get_count();
		for(unsigned int i = 0;i<image_count;i++)
		{
			struct dmx_image* image = dmx_image_getbyidx(i);

			dmx_image_render(image,beatpulses);
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
		dmx_queues_print();
		//dmx_selector_print();

		osc_apply_manual(dmx_channels_get());
		dmx_output_send(dmx_channels_get());
		osc_send_faders(dmx_channels_get());

		usleep(25*1000);
	}

	return 0;
}

