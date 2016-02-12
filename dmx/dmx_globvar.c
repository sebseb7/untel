#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_globvar.h"
#include "dmx_devices.h"

/*
struct dmx_globvar
{
	char* name;
	unsigned int type;
	union {
		unsigned int ival;
		float fval;
	};
};

*/




#define DMX_GLOBVAR_ALLOCATE_INITIAL 100
#define DMX_GLOBVAR_ALLOCATE_STEP 10

static struct dmx_globvar** dmx_globvar_list;

static unsigned int globvar_allocated=0;
static unsigned int globvar_inuse=0;

void dmx_globvar_set(const char* name,unsigned int value)
{
	if(0==globvar_allocated)
	{
		dmx_globvar_list = malloc(sizeof(struct dmx_globvar*)*DMX_GLOBVAR_ALLOCATE_INITIAL);
		globvar_allocated=DMX_GLOBVAR_ALLOCATE_INITIAL;
	}

	if(globvar_inuse>0)
	{
		for(unsigned int i=0;i<globvar_inuse;i++)
		{
			if(
				(0==strncmp(dmx_globvar_list[i]->name,name,DMX_NAME_LENGTH))
			)
			{
					dmx_globvar_list[i]->ival=value;
					return;
			}
		}
	}
	
	if(globvar_inuse == globvar_allocated)
	{
		//implement realloc
		printf("fgv\n");
		exit(EXIT_FAILURE);
	}
	
	struct dmx_globvar* globvar = malloc(sizeof(struct dmx_globvar));
	
	dmx_globvar_list[globvar_inuse]=globvar;
	globvar_inuse++;
	
	globvar->name = strndup(name,DMX_NAME_LENGTH);
	globvar->type = 0;
	globvar->ival = value;
}

int dmx_globvar_get(const char* name,unsigned int *value)
{
	if(globvar_inuse>0)
	{
		for(unsigned int i=0;i<globvar_inuse;i++)
		{
			if(
				(0==strncmp(dmx_globvar_list[i]->name,name,DMX_NAME_LENGTH))
			)
			{
					*value = dmx_globvar_list[i]->ival;
					return 1;
			}
		}
	}
	return 0;
}

