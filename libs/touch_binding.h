#ifndef _TOUCH_BINDING_H
#define _TOUCH_BINDING_H

struct touch_binding
{
	unsigned int minx;
	unsigned int maxx;
	unsigned int miny;
	unsigned int maxy;
	unsigned int attr1;
	unsigned int attr2;
	unsigned int attr3;
	struct touch_binding* next;
};

struct touch_binding_list
{
	unsigned int length;
	struct touch_binding* first;
	struct touch_binding* last;
};

struct touch_binding_list* touch_binding_new(void);
void touch_binding_add(struct touch_binding_list* list,unsigned int minx,unsigned int w,unsigned int miny,unsigned int h,unsigned int attr1,unsigned int attr2,unsigned int attr3);
void touch_binding_free(struct touch_binding_list* list);

unsigned int touch_test(struct touch_binding_list* list,unsigned int x,unsigned int y,unsigned int* attr1,unsigned int* attr2,unsigned int* attr3);


#endif
