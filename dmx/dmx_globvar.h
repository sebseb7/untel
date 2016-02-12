#ifndef _DMX_GLOBVAR_H
#define _DMX_GLOBVAR_H

struct dmx_globvar
{
	char* name;
	unsigned int type;
	union {
		unsigned int ival;
		float fval;
	};
};

void dmx_globvar_set(const char* name,unsigned int value);
int dmx_globvar_get(const char* name,unsigned int *value);

#endif
