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

GFX gGFX;

void GFXInit(GFX *g)
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
	g->screen = SDL_CreateTexture(
		g->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		640, 480);
	if (g->screen == NULL)
	{
		fprintf(stderr, "cannot create screen texture: %s\n",
			SDL_GetError());
		return;
	}
	g->buf = malloc(640 * 480 * sizeof *g->buf);
}
void GFXQuit(GFX *g)
{
	//SDL_FreeSurface(g->icon);
	SDL_DestroyTexture(g->screen);
	SDL_DestroyRenderer(g->renderer);
	SDL_DestroyWindow(g->window);
	SDL_VideoQuit();
	free(g->buf);
}
