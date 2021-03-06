#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "osc.h"
#include "osc_ll.h"
#include "main.h"

#include "dmx_channels.h"
#include "dmx_queue.h"



static unsigned int osc_manual_ch_offset_fine = 0;
static unsigned int osc_manual_ch_offset = 0;
static unsigned int osc_current_tab = 0;
static unsigned char override[DMX_CHANNEL_COUNT];
static uint8_t manual[DMX_CHANNEL_COUNT];

enum {
	OSC_UI_UPDATE_FADERS,

};



static unsigned int update_ui=0;


void osc_process_input(void)
{
	struct osc_event oscev;

	while(poll_osc_event(&oscev))
	{
		//printf("EVENT %i %i %i %f\n",oscev.type,oscev.a,oscev.b,oscev.value);
		if(oscev.type == 4)
		{
			//	update_ui=1;
			//	osc_current_page = oscev.a-1;
		}
		else if(oscev.type == 6)
		{
			update_ui |= (1<<OSC_UI_UPDATE_FADERS);
			if(manual[(oscev.a-1)+osc_manual_ch_offset]==1)
			{
				manual[(oscev.a-1)+osc_manual_ch_offset]=0;
			}
			else
			{
				manual[(oscev.a-1)+osc_manual_ch_offset]=1;
			}
		}
		else if(oscev.type == 7)
		{
			update_ui |= (1<<OSC_UI_UPDATE_FADERS);
			override[(oscev.a-1)+osc_manual_ch_offset] = 255.0f * oscev.value;
		}
		else if(oscev.type == 10)
		{
			//	custom_master[oscev.a] = oscev.value;
		}
		else if(oscev.type == 11)
		{
			if(oscev.a < dmx_queue_get_count())
			{
				struct dmx_queue* queue = dmx_queue_getbyidx(oscev.a);
				if(queue->button==1)
				{
					queue->button=0;
					dmx_queue_deactivate(queue);
				}
				else
				{
					queue->button=1;
					dmx_queue_activate(queue);
				}
			}
		}
		else if(oscev.type == 8)
		{
			update_ui |= (1<<OSC_UI_UPDATE_FADERS);
			if((oscev.a == 1)&&(osc_manual_ch_offset < 239))
			{
				osc_manual_ch_offset_fine++;
			}
			else if((oscev.a == 0)&&(osc_manual_ch_offset > 0))
			{
				osc_manual_ch_offset_fine--;
			}
			osc_manual_ch_offset = osc_manual_ch_offset_fine>>3;
		}
		else if(oscev.type == 9)
		{
			update_ui |= (1<<OSC_UI_UPDATE_FADERS);
			if((oscev.a == 1)&&(osc_manual_ch_offset < 238))
			{
				osc_manual_ch_offset_fine+=16;
			}
			else if((oscev.a == 0)&&(osc_manual_ch_offset > 1))
			{
				osc_manual_ch_offset_fine-=16;
			}
			osc_manual_ch_offset = osc_manual_ch_offset_fine>>3;
		}
		else if(oscev.type == 5)
		{
			osc_current_tab=oscev.a;
		}
	}
}	

void osc_init(void)
{
	osc_connect("192.168.0.140");
	osc_start_server();
	update_ui |= (1<<OSC_UI_UPDATE_FADERS);
}
void osc_deinit(void)
{
	osc_disconnect();
	osc_stop_server();
}


void osc_update_ui(unsigned int time)
{
	if((osc_current_tab == 2)||(osc_current_tab == 0))
	{
		if( update_ui & (1<<OSC_UI_UPDATE_FADERS))
		{
			update_ui &= ~(1<<OSC_UI_UPDATE_FADERS);
		
			for(int i=0;i<16;i++)
			{
				osc_update_manual_state(i,manual[i+osc_manual_ch_offset]);	
			}
		
		}
			
		unsigned char* out = dmx_channels_get();
		for(int i=0;i<16;i++)
		{
			osc_update_fader(i,i+osc_manual_ch_offset,out[i+osc_manual_ch_offset]);	
		}
	}

	if((osc_current_tab == 5)||(osc_current_tab == 0))
	{
		unsigned int queue_count = dmx_queue_get_count();
		for(unsigned int i = 0;i<queue_count;i++)
		{
			if((i < 18) &&(i <  dmx_queue_get_count()))
			{
				struct dmx_queue* queue = dmx_queue_getbyidx(i);

				osc_update_seq_label(i,queue->name);
				char sublabel[200];
				if(queue->active)
				{
					sprintf(sublabel, "%i",((queue->next)-time)/1000);
				}
				else
				{
					sprintf(sublabel, "  ");
				}
				osc_update_seq_sublabel(i,sublabel);
				osc_update_seq_led(i,2,queue->active);
				osc_update_seq_led(i,3, queue->active ? queue->led_tog : 0 );
				osc_update_seq_led(i,1,queue->button);
			}
		}
	}

	osc_send_flush();
}

void osc_apply_manual(unsigned char* out)
{
	for(int i=0;i<DMX_CHANNEL_COUNT;i++)
	{
		if(manual[i])
		{
			out[i]=override[i];
		}
	}
}
