
#include "menu.h"
#include "menu_main.h"
//#include "menu_setup.h"
//#include "menu_directdmx.h"
//#include "screen_keyboard.h"
//#include "dmxbox_hal.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"


static uint8_t redraw = 1;

void menu_main()
{


	if(redraw)
	{
		//clearDisplay();
		//clear_buttons();
		redraw=0;

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

		uint16_t text_width =  get_text_width_16pt("DMX BOX");
		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "DMX BOX", 200,200,255);


		draw_button_icon(button_x(0),button_y(0),92,1,"Cue Ctrl",155,0,0,0,255,0);
		draw_button_icon(button_x(1),button_y(0),92,1,"",55,55,55,0,0,0);
		draw_button_icon(button_x(2),button_y(0),92,1,"",55,55,55,0,0,0);
		draw_button_icon(button_x(0),button_y(1),92,1,"",55,55,55,0,0,0);
		draw_button_icon(button_x(1),button_y(1),92,1,"",55,55,55,0,0,0);
		draw_button_icon(button_x(2),button_y(1),92,1,"",55,55,55,0,0,0);
		draw_button_icon(button_x(0),button_y(2),92,1,"Direct DMX",155,0,0,0,255,0);
		draw_button_icon(button_x(1),button_y(2),92,1,"",55,55,55,0,0,0);
		draw_button_icon(button_x(2),button_y(2),92,1,"Setup",155,0,0,0,255,0);
	}


	uint8_t field=0;
	uint16_t x;
	uint16_t y;
	//if(check_button_press(&x,&y)==1)
	if(0)
	{
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
		if(field == 9)
		{
			redraw=1;
//			set_current_execution(menu_setup);
		}
		else if(field == 7)
		{
			redraw=1;
//			set_current_execution(menu_directdmx);
		}
		else if(field == 8)
		{
			redraw=1;
//			invoke_numeric_keyboard("test",0);
		}
	}

}

