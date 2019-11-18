#include <klika_os.h>
#include <gfx.h>
#include <string.h>
#include <font_8x8.h>

void gfx_putpixel(context_t* context, int x, int y, uint32_t color) {
	CLIP_XY(x, y);
	CONTEXT_32[FIRST_PIXEL(x, y)] = color;
}

void gfx_hline(context_t* context, int x1, int x2, int y, uint32_t color) {
	CLIP_X(x1);
	CLIP_X(x2);
	CLIP_Y(y);
	for (int x=x1; x<=x2; x++) {
		CONTEXT_32[FIRST_PIXEL(x, y)] = color;
	}
}

void gfx_vline(context_t* context, int x, int y1, int y2, uint32_t color) {
	CLIP_X(x);
	CLIP_Y(y1);
	CLIP_Y(y2);
	for (int y=y1; y<=y2; y++) {
		CONTEXT_32[FIRST_PIXEL(x, y)] = color;
	}
}

void gfx_rect(context_t* context, int x1, int y1, int x2, int y2, uint32_t color) {
	gfx_hline(context, x1, x2, y1, color);
	gfx_hline(context, x1, x2, y2, color);
	gfx_vline(context, x1, y1, y2, color);
	gfx_vline(context, x2, y1, y2, color);
}

void gfx_fillrect(context_t* context, int x1, int y1, int x2, int y2, uint32_t color) {
	for (int i=y1; i<=y2; i++) {
		gfx_hline(context, x1, x2, i, color);
	}
}

void gfx_putchar(context_t* context, int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char c) {
  uint8_t i, j;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
    	if (x+i >= 0 && x+1 < context->width &&
    		  y+j >= 0 && y+1 < context->height) {
  			CONTEXT_32[FIRST_PIXEL(x+i, y+j)] = ((font8x8_basic[c & 0x7F][j] >> i ) & 1) ? fgcolor : bgcolor;
  		}
    }
  }
}

// Optimize + CLIP
void gfx_blit(context_t* context, int x, int y, int width, int height, uint32_t* src) {
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			CONTEXT_32[FIRST_PIXEL(x+j, y+i)] = *(src + (j + i * width));
		}
	}
}

void gfx_puts(context_t* context, int x, int y, uint32_t fgcolor, uint32_t bgcolor, const char *c) {
	while(*c) {
		gfx_putchar(context, x, y, fgcolor, bgcolor, *c++);
		x += 8;
	}
}