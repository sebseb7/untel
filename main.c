#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "main.h"
#include "dmx_devices.h"
#include "dmx_stack.h"
#include "dmx_image.h"
#include "dmx_channels.h"
#include "dmx_queue.h"
#if LUA_CUE==1
#include "dmx_luaqueue.h"
#endif
#include "dmx_selector.h"
#if DMX_OUT==1
#include "dmx_output.h"
#endif

#include "osc.h"

#if SDL_OUT==1
#include "sdl_util.h"
#endif

#define DMX_FRAMERATE 40

#if SDL_OUT==1
#include "pixel_hal.h"
#include "menu.h"
#endif

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

static struct dmx_img** programmer_images = NULL;
static struct dmx_stack* programmer_stack = NULL;
static unsigned int programmer_loop = 0;

void set_programmer_image_list(struct dmx_img** stash)
{
	programmer_images = stash;
}
void set_programmer_stack(struct dmx_stack* stack)
{
	programmer_stack = stack;
}
void set_programmer_loop(unsigned int loop)
{
	programmer_loop = loop;
}



int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) 
{
//	keyboard_list();
//	exit(0);
//	MidiObj midi_djm;
//	keyboard_init(&midi_djm,"CH345 MIDI 1");

	//render
#if SDL_OUT==1
	unsigned int* pixelbuffer = sdl_init(1024,600,"test",60);
#endif

#if SDL_OUT==1
	menu_init();
#endif

	unsigned int bpm=165;
	unsigned int beat01ms=600000/bpm;

	unsigned int last_beat=getstarttime();
	unsigned int lastsend=getstarttime();

#if DMX_OUT==1
	dmx_output_init();
#endif
//	osc_init();
	
	//printf("plot '-' using (\\$1):(\\$2)\n");
#ifdef CONCH_OUT
	printf("\033[2J");
#endif

//	malloc_info();
//	unsigned int looping=300;
	
//	KeyboardEvent e;
	
//	while(looping-- > 0)

	//dmx_device_load_from_disc();
	dmx_stack_load_from_disc();



	while(1)
	{
#if SDL_OUT==1
		if(0==sdl_handle_events(pixelbuffer)) break;
#endif
//	while(keyboard_poll(&midi_djm,&e)) 
//		{
//			if(e.type != 248)
//				printf("%d %d %d\n", e.x, e.y, e.type);
//		};

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
		
#if LUA_CUE==1
		dmx_luaqueue_process_all(currtime,bpm);
#endif		
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

		if((programmer_images != NULL)&&(programmer_loop==0))
		{
			dmx_img_render(programmer_images[0]);
		}


		//render channels
		
		unsigned int device_count = dmx_get_device_count();
		for(unsigned int i = 0;i<device_count;i++)
		{
			struct dmx_device* device = dmx_get_device_byidx(i);
			
			if(device->type == DMX_DEVICE_LEDPAR6)
			{
				dmx_device_render_ledpar6(device);
#if SDL_OUT==1
				dmx_device_render_ledpar6_sdl(device,pixelbuffer,0,i);
#endif
			}
			else if(device->type == DMX_DEVICE_STROBE)
			{
				dmx_device_render_strobe(device);
#if SDL_OUT==1
				dmx_device_render_strobe_sdl(device,pixelbuffer,0,i);
#endif
			}
		}
			

#ifdef CONCH_OUT
		printf("\033[H");
		dmx_channels_print();
		dmx_queues_print();
#endif

//		printf("%i %i %i %i\nreplot\n",currtime,dmx_channels_get()[8],dmx_channels_get()[9],dmx_channels_get()[10]);
//		fflush(stdout); 

		osc_apply_manual(dmx_channels_get());
#if DMX_OUT==1
		dmx_output_send(dmx_channels_get());
#endif
		osc_update_ui(currtime);
		
#if SDL_OUT==1
		draw_menu(pixelbuffer);
#endif		
		unsigned int currtime2 = getstarttime();
		int sleeptime = (25*1000)-((currtime2-lastsend)/10);
		//printf("%i\n",sleeptime);
		if(sleeptime > 0) usleep(sleeptime);
		lastsend = currtime2;
	}

#if DMX_OUT==1
	dmx_output_deinit();
#endif
//	osc_deinit();


	unsigned int selector_count = dmx_selector_get_count();
	for(unsigned int i = 0;i<selector_count;i++)
	{
		struct dmx_selector* selector = dmx_selector_getbyidx(0);
		dmx_selector_del(selector);
	}
#if LUA_CUE==1
	dmx_luaqueue_del_all();
#endif
unsigned int queue_count = dmx_queue_get_count();
	for(unsigned int i = 0;i<queue_count;i++)
	{
		struct dmx_queue* queue = dmx_queue_getbyidx(0);
		dmx_queue_del(queue);
	}
	dmx_devices_free();
#if SDL_OUT==1
	sdl_deinit();
#endif
	return 0;
}

