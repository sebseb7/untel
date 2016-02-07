#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "main.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_channels.h"
#include "dmx_queue.h"
#include "dmx_luaqueue.h"
#include "dmx_selector.h"
#include "dmx_output.h"

#ifdef DMX_OUT
#include "keyboard.h"
#endif
#include "osc.h"


#define DMX_FRAMERATE 40

static unsigned long long start_time;

static unsigned int getstarttime(void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long long current_time = tv.tv_sec*(unsigned long long)10000+(tv.tv_usec / 100);

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
	unsigned int beat01ms=600000/bpm;

	unsigned int last_beat=getstarttime();
	unsigned int lastsend=getstarttime();

#ifdef DMX_OUT
	dmx_output_init();
#endif
	osc_init();
	
	printf("\033[2J");

	struct dmx_luaqueue* luaqueue = dmx_luaqueue_add("lua1");
	dmx_luaqueue_addstep(luaqueue,"queue_on( \"LED-COL-SLIDE\")\nqueue_off(\"LED-COL-SWITCH\")",30000,0,0,0);
	dmx_luaqueue_addstep(luaqueue,"queue_off(\"LED-COL-SLIDE\")\nqueue_on( \"LED-COL-SWITCH\")",30000,0,0,0);

//	malloc_info();
	while(1)
	{
		osc_process_input();

		unsigned int currtime = getstarttime();
		while(currtime > (last_beat+beat01ms) )
		{
			last_beat=currtime;
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
				if(queue->led_tog==0)
					queue->led_tog = 1;
				else 
					queue->led_tog = 0;

				unsigned int delay = queue->tick(beat01ms-(currtime-last_beat));
				queue->next = currtime+delay;
			}
		}
		
		dmx_luaqueue_process_all(currtime,bpm);
		
		//render devices (images)
		unsigned int image_count = dmx_image_get_count();
		for(unsigned int pri = 0;pri<3;pri++)
		{
			for(unsigned int i = 0;i<image_count;i++)
			{
				struct dmx_image* image = dmx_image_getbyidx(i);

				if(image->priority == pri)
					dmx_image_render(image,currtime);
			}
		}



		//render channels
		
		unsigned int device_count = dmx_get_device_count();
		for(unsigned int i = 0;i<device_count;i++)
		{
			struct dmx_device* device = dmx_get_device_byidx(i);
			
			if(device->type == DMX_DEVICE_LEDPAR6)
			{
				dmx_device_render_ledpar6(device);
			}
		}
			

//		printf("\033[H");
//		dmx_channels_print();
//		dmx_queues_print();
		//dmx_selector_print();

		//printf("%i\n",dmx_image_get_count());

		osc_apply_manual(dmx_channels_get());
#ifdef DMX_OUT
		dmx_output_send(dmx_channels_get());
#endif
		osc_update_ui(currtime);
		
		unsigned int currtime2 = getstarttime();
		int sleeptime = (25*1000)-((currtime2-lastsend)/10);
		//printf("%i\n",sleeptime);
		if(sleeptime > 0) usleep(sleeptime);
		lastsend = currtime2;
	}

#ifdef DMX_OUT
	dmx_output_deinit();
#endif
	osc_deinit();


	unsigned int selector_count = dmx_selector_get_count();
	for(unsigned int i = 0;i<selector_count;i++)
	{
		struct dmx_selector* selector = dmx_selector_getbyidx(0);
		dmx_selector_del(selector);
	}
	unsigned int queue_count = dmx_queue_get_count();
	for(unsigned int i = 0;i<queue_count;i++)
	{
		struct dmx_queue* queue = dmx_queue_getbyidx(0);
		dmx_queue_del(queue);
	}
	dmx_devices_free();
	return 0;
}

