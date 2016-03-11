#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "menu.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"
#include "mcugui/circle.h"

#include "dmx_devices.h"

#include "dmx_attr_colors.h"

static struct menu* menu_prog = NULL;

static unsigned int tab = 0;
static unsigned int subtab = 0;

struct dmx_stack_frame_image* stash[4];

/*
 *
 * one of the 4 stashes is the "current" one
 * stashes can be visible or hidden
 * each stash can be added to a stack or replace a current entry
 *
 */



static void menu_prog_redraw(void)
{
	clearDisplay();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	draw_filledCircle(17,17,15,40,80,40);
	draw_text_8x6(7,10,"Back",255,100,100);

	uint16_t text_width =  get_text_width_16pt("Programmer");
	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Programmer", 200,200,255);

	unsigned int buttonx = 0;
	unsigned int buttony = 0;

	draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Devices",155,(tab==1)?155:0,0,0,255,0);
	draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Attributes",155,(tab==2)?155:0,0,0,255,0);

	buttony++;buttonx=0;

	if(tab==1)
	{
		char buf[30];
		char buf2[30];

		for(unsigned int i = 0; i < dmx_get_device_count();i++)
		{
			struct dmx_device* dmx_device = dmx_get_device_byidx(i);

			snprintf(buf,30,"%i",dmx_device->addr);

			char classname[30];

			if(dmx_device->type == DMX_DEVICE_LEDPAR6) 
			{
				strncpy(classname,"Par56LED",30);
			}
			else if(dmx_device->type == DMX_DEVICE_STROBE)
			{
				strncpy(classname,"Strobe",30);
			}
			else
			{
				strncpy(classname,"<missing>",30);
			}

			//snprintf(buf2,30,"(%i) %s - %s",dmx_device->addr,classname,dmx_device->name);
			snprintf(buf2,30,"%s",dmx_device->name);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,buf2,155,0,0,0,255,0);
			if(buttonx == 8)
			{
				buttony++;
				buttonx=0;
			}
		}
	}


	if(tab==2)
	{
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Color",155,(subtab==1)?155:0,0,0,255,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Dimmer",155,(subtab==2)?155:0,0,0,255,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Freq",155,(subtab==3)?155:0,0,0,255,0);
		buttony++;buttonx=0;
	
		if(subtab==1)
		{
			unsigned int dmx_attr_colors_get_count(void);
			const char* dmx_attr_colors_get_name(unsigned int idx);
			char buf[30];

			for(unsigned int i = 0; i < dmx_attr_colors_get_count();i++)
			{
				const char * colorname = dmx_attr_colors_get_name(i);

				snprintf(buf,30,"%s",colorname);

				draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,buf,155,0,0,0,255,0);

				if(buttonx == 8)
				{
					buttony++;
					buttonx=0;
				}
			}
		}
		if(subtab==2)
		{
			unsigned int height = button_y(buttony+5)-button_y(buttony)-11;
			draw_rect(button_x(buttonx),button_y(buttony),92,height,1,155,0,0);
			
			unsigned int runlength = height-54;

			float fvalue = 0.95f;

			unsigned int position = runlength * fvalue;


			draw_filledRect(button_x(buttonx++)+1,button_y(buttony)+position+1,92-2,54-2,155,100,0);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Clear",155,(subtab==1)?155:0,0,0,255,0);
			
		}
	}

}


static void menu_prog_touch(unsigned int x, unsigned int y)
{
	uint8_t field=0;
	if(y > 41)
	{
		field+=1;

		if(y > 171)
		{
			field+=6;
		}else if(y > 105)
		{
			field+=3;
		}

		if(x > 212)
		{
			field+=2;
		}else if (x > 109)
		{
			field+=1;
		}
	}
	else
	{
		if(x < 40)
		{
			if(menu_prog->parent != NULL)
				set_current_menu(menu_prog->parent);
		}
	}
	if(field == 1)
	{
		tab = (tab !=1)?1:0;
		set_menu_dirty();
	}
	else if(field == 2)
	{
		tab = (tab !=2)?2:0;
		subtab=0;
		set_menu_dirty();
	}
	else if(tab == 2)
	{
		if(field == 4)
		{
			subtab = (subtab !=1)?1:0;
			set_menu_dirty();
		}
		else if(field == 5)
		{
			subtab = (subtab !=2)?2:0;
			set_menu_dirty();
		}
		else if(field == 6)
		{
			subtab = (subtab !=3)?3:0;
			set_menu_dirty();
		}
	}
}

struct menu* get_menu_prog(void)
{
	if(menu_prog == NULL)
	{
		menu_prog = malloc(sizeof(struct menu));
		menu_prog->redraw = menu_prog_redraw;
		menu_prog->touch = menu_prog_touch;
		menu_prog->parent = NULL;
	}
	return menu_prog;
}

