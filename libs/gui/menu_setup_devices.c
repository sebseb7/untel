#include <stdlib.h>

#include "menu.h"
#include "menu_list.h"
#include "menu_setup.h"
#include "menu_setup_devices.h"
#include "screen_keyboard.h"
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
static unsigned int mode = 0;

static unsigned int active_type=0;
static unsigned int active_addr=0;
static char* active_name=NULL;

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

	touch_binding_add(touchlist,button_x(0),92,button_y(0),54,1,0,0);//add

	if((list1 != NULL)&&(list1->length>0))
	{
		touch_binding_add(touchlist,button_x(1),92,button_y(0),54,2,0,0);//edit
		touch_binding_add(touchlist,button_x(2),92,button_y(0),54,3,0,0);//del
	}
	else if(mode==2) mode = 0;

	draw_button_icon(button_x(0),button_y(0),92,1,"Add",155,(mode==1)?155:0,0,55,255,0);
	draw_button_icon(button_x(1),button_y(0),92,1,"Edit",((list1 != NULL)&&(list1->length>0))?155:55,(mode==2)?155:0,0,55,((list1 != NULL)&&(list1->length>0))?255:55,0);
	draw_button_icon(button_x(2),button_y(0),92,1,"Delete",((list1 != NULL)&&(list1->length>0))?155:55,0,0,55,((list1 != NULL)&&(list1->length>0))?255:55,0);


	if(list1 != NULL)
	{
		touch_binding_add(touchlist,button_x(5),297,button_y(0),listsize*18-4,7,0,0);
		menu_list_draw(list1,button_x(5),button_y(0),listsize);
	}

	if(mode!=0)
	{
		touch_binding_add(touchlist,button_x(0),92,button_y(1),54,4,DMX_DEVICE_LEDPAR6,0);
		touch_binding_add(touchlist,button_x(1),92,button_y(1),54,4,DMX_DEVICE_FOG,0);
		touch_binding_add(touchlist,button_x(2),92,button_y(1),54,4,DMX_DEVICE_STROBE,0);
		draw_button_icon(button_x(0),button_y(1),92,1,"Par56",155,(active_type==DMX_DEVICE_LEDPAR6)?155:0,0,55,255,0);
		draw_button_icon(button_x(1),button_y(1),92,1,"Fog",155,(active_type==DMX_DEVICE_FOG)?155:0,0,55,255,0);
		draw_button_icon(button_x(2),button_y(1),92,1,"Strobe",155,(active_type==DMX_DEVICE_STROBE)?155:0,0,55,255,0);
		touch_binding_add(touchlist,button_x(0),92,button_y(2),54,5,0,0);
		touch_binding_add(touchlist,button_x(0),92,button_y(3),54,6,0,0);
		draw_button_icon(button_x(0),button_y(2),92,1,"Change Name",155,0,0,55,255,0);
		draw_button_icon(button_x(0),button_y(3),92,1,"Change Addr",155,0,0,55,255,0);

		draw_text_8x6(button_x(1),button_y(2)+20,"Name:",255,255,255);
		draw_text_8x6(button_x(1)+(6*6),button_y(2)+20,active_name,255,((mode==1)&&(-1 !=  dmx_get_device_idx(active_name)))?0:255,((mode==1)&&(-1 !=  dmx_get_device_idx(active_name)))?0:255);

		draw_text_8x6(button_x(1),button_y(3)+20,"Addr:",255,255,255);
		draw_number_8x6(button_x(1)+(6*6),button_y(3)+20,active_addr,3,' ',255,255,255);

		
		unsigned int valid = 1;

		if((active_name == NULL)||(strncmp(active_name,"",DMX_NAME_LENGTH)==0)) valid=0;
		if(active_type == 0) valid=0;
		if((mode==1)&&(-1 !=  dmx_get_device_idx(active_name))) valid=0;
		
		
		if(valid) 
			touch_binding_add(touchlist,button_x(0),92,button_y(4),54,8,0,0);//save
		draw_button_icon(button_x(0),button_y(4),92,1,"Save",(valid)?155:55,(mode==2)?155:0,0,55,(valid)?255:55,0);

	}

}
static void return_from_namechange(char * name)
{
	free(active_name);
	active_name=strndup(name,DMX_NAME_LENGTH);
}

static void return_from_addrchange(uint32_t newaddr)
{
	active_addr = newaddr;
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
		if(attr1 == 1)
		{
			if(mode!=1)
				mode=1;
			else
				mode=0;

	
			free(active_name);
			active_name=strdup("");
			active_type=0;
			active_addr=0;

			set_menu_dirty();
		}	
		else if(attr1 == 2)
		{
			if(mode!=2)
				mode=2;
			else
				mode=0;

			set_menu_dirty();
		}	
		else if(attr1 == 3)
		{
			if((list1 != NULL)&&(list1->length>0))
			{
				signed int selected = menu_list_get_selected(list1);
				if(selected >= 0)
				{
					dmx_devices_delete((unsigned int)selected);
					set_menu_dirty();
				}
			}

		}
		else if(attr1 == 4)
		{
			if(active_type != attr2)
				active_type = attr2;
			else
				active_type=0;

			set_menu_dirty();
		}
		else if(attr1 == 5)
		{
			invoke_keyboard("Change Name",active_name,return_from_namechange);
		}
		else if(attr1 == 6)
		{
			invoke_numeric_keyboard("Change Address",active_addr,return_from_addrchange);
		}
		else if(attr1 == 7)
		{
			signed int selected = menu_list_touch(list1,relx,rely);
			if(selected >= 0)
			{
				set_menu_dirty();
			}
		}
		else if(attr1 == 8)//save
		{
			if(active_type==DMX_DEVICE_LEDPAR6)
			{
				if(-1 ==  dmx_get_device_idx(active_name))
				{
					dmx_device_create_ledpar6(active_addr,active_name);
					dmx_device_store_to_disc();
				}
			}
			set_menu_dirty();
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
	if(active_name == NULL) active_name=strdup("");

	return menu_setup_devices;
}

