#include <stdlib.h>

#include "touch_binding.h"

struct touch_binding_list* touch_binding_new(void)
{
	struct touch_binding_list* list = malloc(sizeof(struct touch_binding_list));
	list->length=0;
	list->first=NULL;
	list->last=NULL;
	return list;
}

void touch_binding_add(struct touch_binding_list* list,unsigned int minx,unsigned int w,unsigned int miny,unsigned int h,unsigned int attr1,unsigned int attr2,unsigned int attr3)
{
	if(list == NULL)
		return;

	struct touch_binding* binding = malloc(sizeof(struct touch_binding));

	binding->minx=minx;
	binding->maxx=minx+w;
	binding->miny=miny;
	binding->maxy=miny+h;
	binding->attr1=attr1;
	binding->attr2=attr2;
	binding->attr3=attr3;
	binding->next=NULL;
	if(list->last != NULL)
	{
		list->last->next = binding;
	}
	list->last = binding;
	
	if(list->first == NULL)
	{
		list->first = binding;
	}
	list->length++;
}

void touch_binding_free(struct touch_binding_list* list)
{
	if(list == NULL)
		return;
	
	struct touch_binding* binding = list->first;

	while(binding != NULL)
	{
		struct touch_binding* next = binding->next;
		free(binding);
		binding = next;
	}
	free(list);
}

unsigned int touch_test(struct touch_binding_list* list,unsigned int x,unsigned int y,unsigned int* attr1,unsigned int* attr2,unsigned int* attr3)
{
	if(list == NULL)
		return 0;

	struct touch_binding* binding = list->first;

	while(binding != NULL)
	{
		if(
			(binding->minx < x)&&
			(binding->maxx > x)&&
			(binding->miny < y)&&
			(binding->maxy > y)
		)
		{
			*attr1 = binding->attr1;
			*attr2 = binding->attr2;
			*attr3 = binding->attr3;
			return 1;
		}
		binding = binding->next;
	}

	return 0;
}

