#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"

#include "dmx_devices.h"
#include "dmx_channels.h"
#include <led_gamma.h>

#define DMX_DEVICE_ALLOCATE_INITIAL 100
#define DMX_DEVICE_ALLOCATE_STEP 10

static struct dmx_device* dmx_device_list;

static unsigned int devices_allocated=0;
static unsigned int devices_inuse=0;

static void dmx_device_add(void* device,unsigned int addr,unsigned int type,char* name)
{
	if(0==devices_allocated)
	{
		dmx_device_list = malloc(sizeof(*dmx_device_list)*DMX_DEVICE_ALLOCATE_INITIAL);
		devices_allocated=DMX_DEVICE_ALLOCATE_INITIAL;
	}

	if(devices_inuse == devices_allocated)
	{
		//implement realloc
		printf("f1\n");
		exit(EXIT_FAILURE);
	}
	strncpy(dmx_device_list[devices_inuse].name,name,DMX_NAME_LENGTH);
	dmx_device_list[devices_inuse].type=type;
	dmx_device_list[devices_inuse].addr=addr;
	dmx_device_list[devices_inuse].device=device;
	dmx_device_list[devices_inuse].refcount=1;

	devices_inuse++;
}

unsigned int dmx_get_device_count(void)
{
	return devices_inuse;
}
int dmx_get_device_idx(char* name)
{
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		if(0==strncmp(dmx_device_list[i].name,name,DMX_NAME_LENGTH))
		{
			return i;
		}
	}
	return -1;
}

void dmx_devices_free(void)
{
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		if(dmx_device_list[i].type == DMX_DEVICE_LEDPAR6)
		{
			struct dmx_device_ledpar6* ledpar = dmx_device_list[i].device;
			free(ledpar);
		}
		else if(dmx_device_list[i].type == DMX_DEVICE_STROBE)
		{
			struct dmx_device_strobe* strobe = dmx_device_list[i].device;
			free(strobe);
		}
	}
	devices_inuse=0;
	devices_allocated=0;
	free(dmx_device_list);
}
void dmx_devices_clear(void)
{
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		if(dmx_device_list[i].type == DMX_DEVICE_LEDPAR6)
		{
			struct dmx_device_ledpar6* ledpar = dmx_device_list[i].device;
			ledpar->red = 0;
			ledpar->green = 0;
			ledpar->blue = 0;
			ledpar->dim = 0.0f;
			ledpar->blackout = 0;
		}
		else if(dmx_device_list[i].type == DMX_DEVICE_STROBE)
		{
			struct dmx_device_strobe* strobe = dmx_device_list[i].device;
			strobe->dim = 0.0f;
			strobe->freq = 0.0f;
		}
	}
}
void dmx_devices_delete(unsigned int index)
{
	if(index >= devices_inuse)
		return;

	if(dmx_device_list[index].type == DMX_DEVICE_LEDPAR6)
	{
		free(dmx_device_list[index].device);
	}
	else if(dmx_device_list[index].type == DMX_DEVICE_STROBE)
	{
		free(dmx_device_list[index].device);
	}
	else
	{
		printf("error deleting unknown device\n");
	}

	for(unsigned int x = index;x < devices_inuse;x++)
	{
		dmx_device_list[x].type=     dmx_device_list[x+1].type;
		dmx_device_list[x].addr=     dmx_device_list[x+1].addr;
		dmx_device_list[x].refcount= dmx_device_list[x+1].refcount;
		for(unsigned int y=0;y<DMX_NAME_LENGTH;y++)
			dmx_device_list[x].name[y]=     dmx_device_list[x+1].name[y];
		dmx_device_list[x].device=   dmx_device_list[x+1].device;
	}
	devices_inuse--;
}
struct dmx_device* dmx_get_device(unsigned int type,char* name)
{
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		if((dmx_device_list[i].type==type)&&(0==strncmp(dmx_device_list[i].name,name,DMX_NAME_LENGTH)))
		{
			return &dmx_device_list[i];
		}
	}
	return NULL;
}
struct dmx_device* dmx_get_device_byidx(unsigned int index)
{
	if(index < devices_inuse)
	{
		return &dmx_device_list[index];
	}
	return NULL;
}


struct dmx_device_ledpar6* dmx_device_create_ledpar6(unsigned int addr,char* name)
{
	if(devices_inuse>0)
	{
		for(unsigned int i=0;i<devices_inuse;i++)
		{
			if(
					(dmx_device_list[i].type==DMX_DEVICE_LEDPAR6)&&
					(dmx_device_list[i].addr==addr)&&
					(0==strncmp(dmx_device_list[i].name,name,DMX_NAME_LENGTH))
			  )
			{
				dmx_device_list[i].refcount++;
				return dmx_device_list[i].device;
			}
		}
	}

	struct dmx_device_ledpar6* ledpar = malloc(sizeof(*ledpar));

	ledpar->red = 0;
	ledpar->green = 0;
	ledpar->blue = 0;
	ledpar->dim = 0.0f;
	ledpar->blackout = 0;

	dmx_device_add(ledpar,addr,DMX_DEVICE_LEDPAR6,name);

	return ledpar;
}

struct dmx_device_strobe* dmx_device_create_strobe(unsigned int addr,char* name)
{
	if(devices_inuse>0)
	{
		for(unsigned int i=0;i<devices_inuse;i++)
		{
			if(
					(dmx_device_list[i].type==DMX_DEVICE_STROBE)&&
					(dmx_device_list[i].addr==addr)&&
					(0==strncmp(dmx_device_list[i].name,name,DMX_NAME_LENGTH))
			  )
			{
				dmx_device_list[i].refcount++;
				return dmx_device_list[i].device;
			}
		}
	}

	struct dmx_device_strobe* strobe = malloc(sizeof(*strobe));

	strobe->freq = 0.0f;
	strobe->dim  = 0.0f;

	dmx_device_add(strobe,addr,DMX_DEVICE_STROBE,name);

	return strobe;
}
//void dmx_device_create_fog(unsigned int addr);
//void dmx_device_create_miniscanhpe(unsigned int addr);


void dmx_device_render_ledpar6(struct dmx_device* device)
{
	struct dmx_device_ledpar6* ledpar = device->device;

	unsigned char red=0;
	unsigned char green=0;
	unsigned char blue=0;

	red = ledpar->red*ledpar->dim;
	green = ledpar->green*ledpar->dim;
	blue = ledpar->blue*ledpar->dim;

	red = led_gamma(red);
	green = led_gamma(green);
	blue = led_gamma(blue);

	dmx_channel_set(device->addr,red);
	dmx_channel_set(device->addr+1,green);
	dmx_channel_set(device->addr+2,blue);

	dmx_channel_set(device->addr+3,0);
	dmx_channel_set(device->addr+4,0);
	dmx_channel_set(device->addr+5,0);


}

void dmx_device_render_strobe(struct dmx_device* device)
{
	struct dmx_device_strobe* strobe = device->device;

	unsigned char freq=0;
	unsigned char dim=0;

	//dim = led_gamma(255*strobe->dim);
	dim = 255*strobe->dim;
	freq = 255*strobe->freq;

	dmx_channel_set(device->addr,freq);
	dmx_channel_set(device->addr+1,dim);


}

#define ZOOM 50

void dmx_device_render_ledpar6_sdl(struct dmx_device* device,unsigned int* pixelbuffer,unsigned int row, unsigned int col)
{

	struct dmx_device_ledpar6* ledpar = device->device;

	unsigned char red=0;
	unsigned char green=0;
	unsigned char blue=0;

	red = ledpar->red*ledpar->dim;
	green = ledpar->green*ledpar->dim;
	blue = ledpar->blue*ledpar->dim;

	unsigned int color = (red<<16)+(green<<8)+blue;

	if(pixelbuffer[(1024*550)+((row*ZOOM)*1024)+(col*ZOOM)] != color)
	{
		for(unsigned int i=0;i<ZOOM;i++)
		{
			for(unsigned int j=0;j<ZOOM;j++) 
			{
				pixelbuffer[(1024*550)+(((row*ZOOM)+j)*1024)+(col*ZOOM)+i] = color;
			}
		}
	}
}
void dmx_device_render_strobe_sdl(struct dmx_device* device,unsigned int* pixelbuffer,unsigned int row, unsigned int col)
{

	struct dmx_device_strobe* strobe = device->device;

	unsigned int red = 255*strobe->dim;
	unsigned int green = 255*strobe->dim;
	unsigned int blue = 255*strobe->freq;

	unsigned int color = (red<<16)+(green<<8)+blue;

	if(pixelbuffer[(1024*550)+((row*ZOOM)*1024)+(col*ZOOM)] != color)
	{
		for(unsigned int i=0;i<ZOOM;i++)
		{
			for(unsigned int j=0;j<ZOOM;j++) 
			{
				pixelbuffer[(1024*550)+(((row*ZOOM)+j)*1024)+(col*ZOOM)+i] = color;
			}
		}
	}

}

void dmx_device_store_to_disc(void)
{
	char *file_name = "device";
	char *base_path = SDL_GetPrefPath("net.exse", "untel");
	char *file_path = calloc((strlen(base_path)+strlen(file_name)+1),sizeof(char));
	strcat(file_path,base_path);
	strcat(file_path,file_name);
	free(base_path);	
	SDL_RWops *file = SDL_RWFromFile(file_path, "w");
	free(file_path);

	SDL_WriteBE32(file,devices_inuse);
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		SDL_WriteBE32(file,dmx_device_list[i].type);
		SDL_WriteBE32(file,dmx_device_list[i].addr);
		SDL_WriteU8(file,strnlen(dmx_device_list[i].name,DMX_NAME_LENGTH));
		SDL_RWwrite(file,dmx_device_list[i].name,1,1+strnlen(dmx_device_list[i].name,DMX_NAME_LENGTH));
	}

	SDL_RWclose(file);
}

void dmx_device_load_from_disc(void)
{
	char *file_name = "device";
	char *base_path = SDL_GetPrefPath("net.exse", "untel");
	char *file_path = calloc((strlen(base_path)+strlen(file_name)+1),sizeof(char));
	strcat(file_path,base_path);
	strcat(file_path,file_name);
	free(base_path);	
	SDL_RWops *file = SDL_RWFromFile(file_path, "r");
	free(file_path);

	if(file == NULL)
		return;


	unsigned int devices = SDL_ReadBE32(file);

	for(unsigned int x = 0; x < devices;x++)
	{
		unsigned int type =SDL_ReadBE32(file);
		unsigned int addr =SDL_ReadBE32(file);
		unsigned int namelen=SDL_ReadU8(file);

		char name[DMX_NAME_LENGTH];
		if(namelen > DMX_NAME_LENGTH) namelen=DMX_NAME_LENGTH;
		SDL_RWread(file, name, 1, namelen+1);

		if(type == DMX_DEVICE_LEDPAR6)
		{
			dmx_device_create_ledpar6(addr,name);
		}
	}

	SDL_RWclose(file);
}


