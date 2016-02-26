#include <stdlib.h>

#include "menu.h"
#include "menu_setup.h"
//#include "menu_setup_devices.h"
//#include "menu_setup_device_classes.h"
#include "menu_main.h"
//#include "dmxbox_hal.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

//#include "loadsave.h"


static struct menu* menu_setup = NULL;


static void menu_setup_redraw(void)
{

	//		clearDisplay();
	//		clear_buttons();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	draw_filledCircle(17,17,15,40,80,40);
	draw_text_8x6(7,10,"Back",255,100,100);

	uint16_t text_width =  get_text_width_16pt("SETUP");

	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "SETUP", 200,200,255);


	draw_button_icon(11,45,92,1,"Models",155,0,0,0,255,0);
	draw_button_icon(114,45,92,1,"Devices",155,0,0,0,255,0);
	draw_button_icon(217,45,92,1,"Scenes",155,0,0,0,255,0);
	draw_button_icon(11,110,92,1,"Loops",155,0,0,0,255,0);
	draw_button_icon(114,110,92,1,"Playbacks",155,0,0,0,255,0);
	draw_button_icon(217,110,92,1,"Actions",155,0,0,0,255,0);
	draw_button_icon(11,175,92,1,"Load",155,0,0,0,255,0);
	draw_button_icon(114,175,92,1,"Save",155,0,0,0,255,0);
	draw_button_icon(217,175,92,1,"General",155,0,0,0,255,0);
}


static void menu_setup_touch(unsigned int x, unsigned int y)
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
			if(menu_setup->parent != NULL)
				set_current_menu(menu_setup->parent);
		}
	}

	if(field == 1)
	{
		//			set_current_execution(menu_setup_device_classes);
	}
	else if(field == 2)
	{
		//			set_current_execution(menu_setup_devices);
	}
	else if(field == 7)
	{
		//			load_device_classes("defult");
	}
	else if(field == 8)
	{
		//			save_device_classes("defult");
	}

}

struct menu* get_menu_setup()
{
	if(menu_setup == NULL)
	{
		menu_setup = malloc(sizeof(struct menu));
		menu_setup->redraw = menu_setup_redraw;
		menu_setup->touch = menu_setup_touch;
		menu_setup->parent = NULL;
	}

	return menu_setup;
}


