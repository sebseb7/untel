#ifndef _MENU_H
#define _MENU_H


struct menu
{
	void (*redraw)(void);
	void (*touch)(unsigned int,unsigned int);
};





void clearDisplay(void);
void draw_menu(void);
void menu_init(void);


unsigned int button_x(unsigned int x);
unsigned int button_y(unsigned int y);

#endif
