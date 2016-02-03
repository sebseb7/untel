#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "osc.h"
#include "osc_ll.h"
#include "main.h"

#include "dmx_channels.h"



static unsigned int osc_manual_ch_offset_fine = 0;
static unsigned int osc_manual_ch_offset = 0;
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
		printf("EVENT %i %i %i %f\n",oscev.type,oscev.a,oscev.b,oscev.value);
		if(oscev.type == 4)
		{
			//	update_ui=1;
			//	osc_current_page = oscev.a-1;
		}
		else if(oscev.type == 6)
		{
			if(manual[oscev.a+osc_manual_ch_offset]==1)
			{
				manual[oscev.a+osc_manual_ch_offset]=0;
			}
			else
			{
				manual[oscev.a+osc_manual_ch_offset]=1;
			}
		}
		else if(oscev.type == 7)
		{
			update_ui |= (1<<OSC_UI_UPDATE_FADERS);
			override[oscev.a+osc_manual_ch_offset] = 255.0f * oscev.value;
		}
		else if(oscev.type == 10)
		{
			//	custom_master[oscev.a] = oscev.value;
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
			//	osc_current_tab=oscev.a;
		}
	}
}	

void osc_init(void)
{
	osc_connect("192.168.0.140");
	osc_start_server();
}


void osc_send_faders(unsigned char* out)
{
	//if(osc_current_tab == 2)
	//{
	for(int i=0;i<16;i++)
	{
		osc_update_fader(i,i,out[i]);	
		//osc_update_fader(i,i+osc_manual_ch_offset,out[1+i+osc_manual_ch_offset]);	
		//osc_update_manual_state(i,manual[1+i+osc_manual_ch_offset]);	
	}
	//osc_update_xy(out[4],out[5]);	
	osc_send_flush();
	//}
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
