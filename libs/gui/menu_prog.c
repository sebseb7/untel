#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"


#include "menu.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"
#include "mcugui/circle.h"

#include "dmx_devices.h"
#include "dmx_frame.h"
#include "dmx_stack.h"

#include "dmx_attr_colors.h"

static struct menu* menu_prog = NULL;

static unsigned int tab = 0;
static unsigned int subtab = 0;

static struct dmx_img* stash[4];

static struct dmx_stack* prog_stack;

/*
 *
 * one of the 4 stashes is the "current" one
 * stashes can be visible or hidden
 * each stash can be added to a stack or replace a current entry
 *
 */

#include "touch_binding.h"


static struct touch_binding_list* touchlist = NULL;


static unsigned int act = 0;


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
			unsigned int active = dmx_img_device_test(stash[act],dmx_device->name);
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
			
		unsigned int height = button_y(5)-button_y(0)-11;
		unsigned int runlength = height-54;
	
		if(subtab==1)
		{
			unsigned int r=0;
			unsigned int g=0;
			unsigned int b=0;

			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,5,1,0);
			draw_rect(button_x(buttonx++),button_y(buttony),92,height,1,155,0,0);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,5,2,0);
			draw_rect(button_x(buttonx++),button_y(buttony),92,height,1,155,0,0);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,5,3,0);
			draw_rect(button_x(buttonx++),button_y(buttony),92,height,1,155,0,0);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,5,4,0);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Clear",155,0,0,0,255,0);
			
			if(dmx_img_color_getrgb(stash[act],&r,&g,&b))
			{	
				unsigned int position_r = runlength * (1-(r/255.0f));
				draw_filledRect(button_x(buttonx-4)+1,button_y(buttony)+position_r+1,92-2,54-2,155,50,50);
				unsigned int position_g = runlength * (1-(g/255.0f));
				draw_filledRect(button_x(buttonx-3)+1,button_y(buttony)+position_g+1,92-2,54-2,50,155,50);
				unsigned int position_b = runlength * (1-(b/255.0f));
				draw_filledRect(button_x(buttonx-2)+1,button_y(buttony)+position_b+1,92-2,54-2,50,50,155);
			}

			unsigned int dmx_attr_colors_get_count(void);
			const char* dmx_attr_colors_get_name(unsigned int idx);
			char buf[30];

			unsigned int initial_buttonx = buttonx;

			for(unsigned int i = 0; i < dmx_attr_colors_get_count();i++)
			{
				const char * colorname = dmx_attr_colors_get_name(i);

				snprintf(buf,30,"%s",colorname);
		
		
				unsigned int active = dmx_img_color_test(stash[act],colorname);

				touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,4,i,0);
				draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,buf,155,(active)?155:0,0,0,255,0);

				if(buttonx == 8)
				{
					buttony++;
					buttonx=initial_buttonx;
				}
			}
		}
		if(subtab==2)
		{
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,6,1,0);
			draw_rect(button_x(buttonx),button_y(buttony),92,height,1,155,0,0);

			float fvalue = 0;

			if(dmx_img_dim_get(stash[act],&fvalue))
			{
				unsigned int position = runlength * (1-fvalue);
				draw_filledRect(button_x(buttonx)+1,button_y(buttony)+position+1,92-2,54-2,155,100,0);
			}
			buttonx++;

			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,6,2,0);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Clear",155,0,0,0,255,0);
			
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

			if(dmx_img_device_test(stash[act],dmx_device->name))
			{
				dmx_img_device_del(stash[act],dmx_device->name);
			}
			else
			{
				dmx_img_device_add(stash[act],dmx_device->name);
			}
				
			set_menu_dirty();
		}
		else if(attr1 == 4)
		{
			const char * colorname = dmx_attr_colors_get_name(attr2);
			if(dmx_img_color_test(stash[act],colorname) != 1)
			{
				dmx_img_color_clear(stash[act]);
				dmx_img_color_setname(stash[act],colorname);
			}
			else
			{
				dmx_img_color_clear(stash[act]);
			}
			set_menu_dirty();
		}
		else if(attr1 == 5)
		{
			unsigned int r=0;
			unsigned int g=0;
			unsigned int b=0;
			dmx_img_color_getrgb(stash[act],&r,&g,&b);
				
				if(rely > (54/2))
				{
					rely -= (54/2);
				}
				else
				{
					rely=0;
				}
				unsigned int height = button_y(5)-button_y(0)-11;
				unsigned int runlength = height-54;
				
			if(attr2 == 1)
			{
				r = 255-(rely/(float)runlength*255.0f);
				dmx_img_color_setrgb(stash[act],r,g,b);
			}	
			else if(attr2 == 2)
			{
				g = 255-(rely/(float)runlength*255.0f);
				dmx_img_color_setrgb(stash[act],r,g,b);
			}	
			else if(attr2 == 3)
			{
				b = 255-(rely/(float)runlength*255.0f);
				dmx_img_color_setrgb(stash[act],r,g,b);
			}	
			else if(attr2 == 4)
			{
				dmx_img_color_clear(stash[act]);
			}
			set_menu_dirty();
		}
		else if(attr1 == 6)
		{
			if(attr2 == 1)
			{
				if(rely > (54/2))
				{
					rely -= (54/2);
				}
				else
				{
					rely=0;
				}
				unsigned int height = button_y(5)-button_y(0)-11;
				unsigned int runlength = height-54;

				dmx_img_dim_set(stash[act],1-(rely/(float)runlength));
			}
			else if(attr2 == 2)
			{
				dmx_img_dim_clear(stash[act]);
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

		stash[0] = dmx_img_new();
		stash[1] = dmx_img_new();
		stash[2] = dmx_img_new();
		stash[3] = dmx_img_new();

		prog_stack= dmx_stack_new();

		set_programmer_image_list(stash[0]);
		set_programmer_stack(prog_stack);

	}
	return menu_prog;
}

