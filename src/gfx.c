/*
Copyright (c) 2015, Cong Xu
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#include "gfx.h"

#include <stdio.h>
#include <string.h>

GFX gGFX;

static SDL_Surface *CreateScreen(SDL_Renderer *renderer, const uint8_t *pal);
void GFXInit(GFX *g, const uint8_t *pal)
{
	// TODO: icon
	int flags = SDL_WINDOW_RESIZABLE;
	// TODO: fullscreen option?
	SDL_CreateWindowAndRenderer(640, 480, flags, &g->window, &g->renderer);
	if (g->window == NULL || g->renderer == NULL)
	{
		fprintf(stderr, "cannot create window or renderer: %s\n",
			SDL_GetError());
		return;
	}
	SDL_SetWindowTitle(g->window, "Ronny's Magus");
	//SDL_SetWindowIcon(g->window, g->icon);
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"))
	{
		fprintf(stderr, "cannot set render quality hint: %s\n",
			SDL_GetError());
	}

	if (SDL_RenderSetLogicalSize(g->renderer, 640, 480) != 0)
	{
		fprintf(stderr, "cannot set renderer logical size: %s\n",
			SDL_GetError());
		return;
	}
	g->screen = CreateScreen(g->renderer, pal);
	if (g->screen == NULL)
	{
		fprintf(stderr, "cannot create screen surface: %s\n", SDL_GetError());
		return;
	}
	g->buf = malloc(640 * 480 * sizeof *g->buf);
}
void GFXQuit(GFX *g)
{
	//SDL_FreeSurface(g->icon);
	SDL_FreeSurface(g->screen);
	SDL_DestroyRenderer(g->renderer);
	SDL_DestroyWindow(g->window);
	SDL_VideoQuit();
	free(g->buf);
}

static int SurfaceSetPalette(SDL_Surface *s, const uint8_t *pal);
static SDL_Surface *CreateScreen(SDL_Renderer *renderer, const uint8_t *pal)
{
	SDL_Surface *s = SDL_CreateRGBSurface(0, 640, 480, 8, 0, 0, 0, 0);
	if (s == NULL)
	{
		fprintf(stderr, "cannot create surface: %s\n", SDL_GetError());
		goto bail;
	}
	if (SurfaceSetPalette(s, pal) == -1)
	{
		fprintf(stderr, "cannot set palette: %s\n", SDL_GetError());
		goto bail;
	}

bail:
	return s;
}
static int SurfaceSetPalette(SDL_Surface *s, const uint8_t *pal)
{
	SDL_Color palette[16];
	memset(palette, 0, sizeof palette);
	for (int i = 0; i < 16; i++)
	{
		// RGB stored in two bytes :
		// Lower byte is R
		// Upper byte contains GB (4 bits each)
		const uint8_t b1 = pal[i * 2];
		const uint8_t b2 = pal[i * 2 + 1];
		const int r = b1;
		const int g = b2 >> 4;
		const int b = b2 & 0x0f;
		// Colours are actually 3-bits, giving a range of 0-7
		// Max brightness is actually based on 6-bit depth (i.e. max 63); this
		// was done by multiplying by 8
		// Now we convert to 0-255 range, which is an additional multiply by 4
		// Therefore we multiply by 32
		const int gamma = 32;
		palette[i].r = r * gamma;
		palette[i].g = g * gamma;
		palette[i].b = b * gamma;
		palette[i].a = 255;
	}
	return SDL_SetPaletteColors(s->format->palette, palette, 0, 16);
}

void GFXClear(GFX *g)
{
	memset(g->buf, 0, 640 * 480 * sizeof *g->buf);
}

void GFXRect(
	GFX *g, const int x1, const int y1, const int x2, const int y2,
	const int color, const bool fill)
{
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			if (fill || x == x1 || x == x2 || y == y1 || y == y2)
			{
				g->buf[x + y * 640] = color;
			}
		}
	}
}

void GFXFlip(GFX *g)
{
	if (SDL_LockSurface(g->screen) == -1)
	{
		fprintf(stderr, "cannot lock screen: %s\n", SDL_GetError());
		return;
	}
	memcpy(g->screen->pixels, g->buf, 640 * 480 * sizeof *g->buf);
	SDL_UnlockSurface(g->screen);
	SDL_Texture *t = SDL_CreateTextureFromSurface(g->renderer, g->screen);
	if (g->screen == NULL)
	{
		fprintf(stderr, "cannot create screen texture: %s\n",
			SDL_GetError());
		return;
	}
	if (SDL_RenderCopy(g->renderer, t, NULL, NULL) == -1)
	{
		fprintf(stderr, "cannot render texture: %s\n", SDL_GetError());
		return;
	}
	SDL_DestroyTexture(t);
	SDL_RenderPresent(g->renderer);
}
