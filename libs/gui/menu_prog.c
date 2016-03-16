#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "menu.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"
#include "mcugui/circle.h"

#include "dmx_devices.h"
#include "dmx_programmer.h"

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

#include "touch_binding.h"


static struct touch_binding_list* touchlist = NULL;




static void menu_prog_redraw(void)
{
	clearDisplay();
	if(touchlist != NULL)
		touch_binding_free(touchlist);
	touchlist = touch_binding_new();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	draw_filledCircle(17,17,15,40,80,40);
	draw_text_8x6(7,10,"Back",255,100,100);

	uint16_t text_width =  get_text_width_16pt("Programmer");
	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Programmer", 200,200,255);

	unsigned int buttonx = 0;
	unsigned int buttony = 0;

	touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,1,1,0);
	draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Devices",155,(tab==1)?155:0,0,0,255,0);
	touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,1,2,0);
	draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Attributes",155,(tab==2)?155:0,0,0,255,0);
	

	//void touch_binding_add(struct touch_binding_list* list,unsigned int minx,unsigned int maxx,unsigned int miny,unsigned int maxy,unsigned int attr1,unsigned int attr2,unsigned int attr3);

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

			snprintf(buf2,30,"%s",dmx_device->name);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,3,i,0);
			unsigned int active = dmx_programmer_device_test(dmx_device->name);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,buf2,155,(active)?155:0,0,0,255,0);
			if(buttonx == 8)
			{
				buttony++;
				buttonx=0;
			}
		}
	}


	if(tab==2)
	{
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,2,1,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Color",155,(subtab==1)?155:0,0,0,255,0);
		
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,2,2,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Dimmer",155,(subtab==2)?155:0,0,0,255,0);
		
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,2,3,0);
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
		
		
				unsigned int active = dmx_programmer_color_test(colorname);

				touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,4,i,0);
				draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,buf,155,(active)?155:0,0,0,255,0);

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
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,5,1,0);
			draw_rect(button_x(buttonx),button_y(buttony),92,height,1,155,0,0);
			
			unsigned int runlength = height-54;

			float fvalue = 0.95f;

			unsigned int position = runlength * fvalue;


			draw_filledRect(button_x(buttonx++)+1,button_y(buttony)+position+1,92-2,54-2,155,100,0);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,5,2,0);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Clear",155,(subtab==1)?155:0,0,0,255,0);
			
		}
	}

}


static void menu_prog_touch(unsigned int x, unsigned int y)
{
	if(y < 41)
	{
		if(x < 40)
		{
			if(menu_prog->parent != NULL)
				set_current_menu(menu_prog->parent);
		}
	}
	
	unsigned int attr1 = 0;
	unsigned int attr2 = 0;
	unsigned int attr3 = 0;
	unsigned int relx = 0;
	unsigned int rely = 0;
	if(touch_test(touchlist,x,y,&attr1,&attr2,&attr3,&relx,&rely))
	{
		if(attr1 == 1)
		{
			tab = (tab !=attr2)?attr2:0;
			set_menu_dirty();
		}
		else if(attr1 == 2)
		{
			subtab = (subtab !=attr2)?attr2:0;
			set_menu_dirty();
		}
		else if(attr1 == 3)
		{
			struct dmx_device* dmx_device = dmx_get_device_byidx(attr2);

			if(dmx_programmer_device_test(dmx_device->name))
			{
				dmx_programmer_device_del(dmx_device->name);
			}
			else
			{
				dmx_programmer_device_add(dmx_device->name);
			}
				
			set_menu_dirty();
		}
		else if(attr1 == 4)
		{
			dmx_programmer_color_clear();
			const char * colorname = dmx_attr_colors_get_name(attr2);
			if(dmx_programmer_color_test(colorname) != 1)
			{
				dmx_programmer_color_setbyname(colorname);
			}
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

