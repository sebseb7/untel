#ifndef _MENU_H
#define _MENU_H


struct menu
{
	void (*redraw)(void);
	void (*update)(void);
	void (*touch)(unsigned int,unsigned int);
	struct menu* parent;
};





void clearDisplay(void);
void set_current_menu(struct menu* new_menu);
void draw_menu(unsigned int* pixelbuffer);
void menu_init(void);
void menu_autoupdate(void);


unsigned int button_x(unsigned int x);
unsigned int button_y(unsigned int y);

#endif
