#include <stdlib.h>


#include "menu.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"
#include "mcugui/circle.h"

static struct menu* menu_prog = NULL;

static void menu_prog_redraw(void)
{
		clearDisplay();

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);
		
		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);

		uint16_t text_width =  get_text_width_16pt("Programmer");
		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Programmer", 200,200,255);


		draw_button_icon(button_x(0),button_y(0),92,1,"Devices",155,0,0,0,255,0);
		draw_button_icon(button_x(0),button_y(0),92,1,"Attributes",155,0,0,0,255,0);
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
	}
	else if(field == 9)
	{
	}
	else if(field == 7)
	{
	}
	else if(field == 8)
	{
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

