#ifndef _SDL_UTIL_H
#define _SDL_UTIL_H

unsigned int* sdl_init(int h, int v,const char* title, int fps);
void sdl_deinit(void);
int sdl_handle_events(const void* pixels);
void release_upped_keys(void);
int getkey(int key);

#endif
