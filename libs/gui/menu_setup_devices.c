#include <stdlib.h>

#include "menu.h"
#include "menu_list.h"
#include "menu_setup.h"
#include "menu_setup_devices.h"
//#include "menu_setup_devices_add.h"
//#include "menu_setup_devices_edit.h"
//#include "dmxbox_hal.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

#include "dmx_devices.h"

#include <stdio.h>
#include <string.h>

static struct menu* menu_setup_devices = NULL;
static struct touch_binding_list* touchlist = NULL;
static struct menu_list* list1 = NULL;

static const unsigned int listsize = 15;


static unsigned int update_list(void)
{
	unsigned int selected = 0;
	if(list1 != NULL)
	{
		selected = list1->selected;
		menu_list_free(list1);
	}
	list1 = menu_list_new(0);

	for(unsigned int i = 0; i < dmx_get_device_count();i++)
	{
		struct dmx_device* dmx_device = dmx_get_device_byidx(i);

		char* label = malloc(sizeof(char)*DMX_NAME_LENGTH);

		char addr[30];
		char classname[30];

		snprintf(addr,30,"%i",dmx_device->addr);


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

		snprintf(label,30,"%s : %s - %s",addr,classname,dmx_device->name);
		menu_list_add_entry(list1, menu_list_entry_new(MENU_LIST_ENTRY_LABEL,label,0,0),-1);
		free(label);
	}

	if(selected >= dmx_get_device_count())
	{
		selected = dmx_get_device_count()-1;
	}

	unsigned int offset = 0;
	if(selected > (listsize-1))
	{
		offset = selected-(listsize-1);
		selected = selected - offset;
	}
	list1->selected=selected;
	list1->offset=offset;

	return (offset+selected);
}

static void menu_setup_devices_redraw(void)
{
	clearDisplay();

	if(touchlist != NULL)
		touch_binding_free(touchlist);
	touchlist = touch_binding_new();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	draw_filledCircle(17,17,15,40,80,40);
	draw_text_8x6(7,10,"Back",255,100,100);

	uint16_t text_width =  get_text_width_16pt("Setup Devices");
	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Setup Devices", 200,200,255);



	update_list();

	if(list1 != NULL)
	{
		touch_binding_add(touchlist,button_x(5),297,button_y(3),listsize*18-4,7,0,0);
		menu_list_draw(list1,button_x(5),button_y(3),listsize);
	}

}

static void menu_setup_devices_touch(unsigned int x, unsigned int y)
{
	if(y < 41)
	{
		if(x < 40)
		{
			if(menu_setup_devices->parent != NULL)
			{
				set_current_menu(menu_setup_devices->parent);
			}
		}
	}

	unsigned int attr1 = 0;
	unsigned int attr2 = 0;
	unsigned int attr3 = 0;
	unsigned int relx = 0;
	unsigned int rely = 0;
	if(touch_test(touchlist,x,y,&attr1,&attr2,&attr3,&relx,&rely))
	{
		if(attr1 == 7)
		{
			signed int selected = menu_list_touch(list1,relx,rely);
			if(selected >= 0)
			{
				set_menu_dirty();
			}
		}
	}

}

struct menu* get_menu_setup_devices()
{
	if(menu_setup_devices == NULL)
	{
		menu_setup_devices = malloc(sizeof(struct menu));
		menu_setup_devices->redraw = menu_setup_devices_redraw;
		menu_setup_devices->touch = menu_setup_devices_touch;
		menu_setup_devices->parent = NULL;
		menu_setup_devices->update = NULL;
	}

	return menu_setup_devices;
}

