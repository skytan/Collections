#ifndef SFONT_H
#define SFONT_H

#include "sconfig.h"

const unsigned char* find_font(wchar_t unicode);
int draw_font(int bx, int by, wchar_t unicode, class sFramebuffer *pfb);
int draw_font_reverse(int bx, int by, wchar_t unicode, class sFramebuffer *pfb);


bool font_enable_draw(wchar_t unicode);

int get_font_weight(wchar_t unicode = 0xff);
int get_font_height();

int get_text_weight(wchar_t *str);
int get_text_weight(wchar_t *str, int len);

#endif
