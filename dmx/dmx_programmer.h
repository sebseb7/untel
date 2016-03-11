#ifndef _DMX_PROGRAMMER_H
#define _DMX_PROGRAMMER_H



void dmx_programmer_device_clear(void);
unsigned int dmx_progammer_device_count(void);
void dmx_programmer_device_add(char* name);
void dmx_programmer_device_del(char* name);
unsigned int dmx_programmer_device_test(char* name);

void dmx_programmer_color_clear(void);
void dmx_programmer_color_setbyname(char* colorname);
void dmx_programmer_color_setbyrgb(unsigned int r, unsigned int g, unsigned int b);
unsigned int dmx_programmer_color_test(char* colorname);
unsigned int dmx_programmer_color_getrgb(unsigned int *r,unsigned int *g,unsigned int *b);

void dmx_programmer_dim_clear(void);
void dmx_programmer_dim_set(float dim);
unsigned int dmx_programmer_dim_get(float *dim);



#endif
