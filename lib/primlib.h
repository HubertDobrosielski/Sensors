#ifndef __PRIMLIB_H__
#define __PRIMLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

enum color { BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, COLOR_MAX };

int gfx_init();

void gfx_pixel(int x, int y, enum color c);
void gfx_line(int x1, int y1, int x2, int y2, enum color c);
void gfx_wline(int x1, int y1, int x2, int y2, enum color c, int width);
void gfx_circle(int x, int y, int r, enum color c);
void gfx_filledTriangle(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, enum color c);
void gfx_filledRect(int x1, int y1, int x2, int y2, enum color c);
void gfx_filledCircle(int x, int y, int r, enum color c);
void gfx_rect(int x1, int y1, int x2, int y2, enum color c);
void gfx_textout(int x, int y, const char *s, enum color c);

int gfx_screenWidth();
int gfx_screenHeight();

void gfx_updateScreen();

int gfx_pollkey();
SDL_Event gfx_getEvent();
int gfx_isKeyDown(int key);

#ifdef __cplusplus
}
#endif

#endif /* __PRIMLIB_H__ */
