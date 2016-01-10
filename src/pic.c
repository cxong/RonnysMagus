/*
    Copyright (c) 2013-2015, Cong Xu
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
#include "pic.h"

#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "magus.h"
#include "utils.h"

Pic picNone = { { 0, 0 }, { 0, 0 }, NULL };


SDL_Color PixelToColor(
	const SDL_PixelFormat *f, const Uint8 aShift, const Uint32 pixel)
{
	SDL_Color c;
	SDL_GetRGBA(pixel, f, &c.r, &c.g, &c.b, &c.a);
	// Manually apply the alpha as SDL seems to always set it to 0
	c.a = (Uint8)((pixel & ~(f->Rmask | f->Gmask | f->Bmask)) >> aShift);
	return c;
}
Uint32 ColorToPixel(
	const SDL_PixelFormat *f, const Uint8 aShift, const SDL_Color color)
{
	const Uint32 pixel = SDL_MapRGBA(f, color.r, color.g, color.b, color.a);
	// Manually apply the alpha as SDL seems to always set it to 0
	return (pixel & (f->Rmask | f->Gmask | f->Bmask)) | (color.a << aShift);
}


void PicLoad(
	Pic *p, const Vec2i size, const Vec2i offset, const SDL_Surface *image)
{
	p->size = size;
	p->offset = Vec2iZero();
	p->Data = malloc(size.x * size.y * sizeof *((Pic *)0)->Data);
	// Manually copy the pixels and replace the alpha component,
	// since our gfx device format has no alpha
	int srcI = offset.y*image->w + offset.x;
	for (int i = 0; i < size.x * size.y; i++, srcI++)
	{
		const Uint32 pixel = ((Uint32 *)image->pixels)[srcI];
		SDL_Color c;
		SDL_GetRGBA(pixel, image->format, &c.r, &c.g, &c.b, &c.a);
		// If completely transparent, replace rgb with black (0) too
		// This is because transparency blitting checks entire pixel
		if (c.a == 0)
		{
			p->Data[i] = 0;
		}
		else
		{
			p->Data[i] = WHITE;
		}
		if ((i + 1) % size.x == 0)
		{
			srcI += image->w - size.x;
		}
	}
}

Pic PicCopy(const Pic *src)
{
	Pic p = *src;
	const size_t size = p.size.x * p.size.y * sizeof *p.Data;
	p.Data = malloc(size);
	memcpy(p.Data, src->Data, size);
	return p;
}

void PicFree(Pic *pic)
{
	free(pic->Data);
}

int PicIsNotNone(Pic *pic)
{
	return pic->size.x > 0 && pic->size.y > 0 && pic->Data != NULL;
}

void PicTrim(Pic *pic, const bool xTrim, const bool yTrim)
{
	// Scan all pixels looking for the min/max of x and y
	Vec2i min = pic->size;
	Vec2i max = Vec2iZero();
	for (Vec2i pos = Vec2iZero(); pos.y < pic->size.y; pos.y++)
	{
		for (pos.x = 0; pos.x < pic->size.x; pos.x++)
		{
			const Uint32 pixel = *(pic->Data + pos.x + pos.y * pic->size.x);
			if (pixel > 0)
			{
				min.x = MIN(min.x, pos.x);
				min.y = MIN(min.y, pos.y);
				max.x = MAX(max.x, pos.x);
				max.y = MAX(max.y, pos.y);
			}
		}
	}
	// If no opaque pixels found, don't trim
	Vec2i newSize = pic->size;
	Vec2i offset = Vec2iZero();
	if (min.x < max.x && min.y < max.y)
	{
		if (xTrim)
		{
			newSize.x = max.x - min.x + 1;
			offset.x = min.x;
		}
		if (yTrim)
		{
			newSize.y = max.y - min.y + 1;
			offset.y = min.y;
		}
	}
	// Trim by copying pixels
	Uint32 *newData = malloc(newSize.x * newSize.y * sizeof *newData);
	for (Vec2i pos = Vec2iZero(); pos.y < newSize.y; pos.y++)
	{
		for (pos.x = 0; pos.x < newSize.x; pos.x++)
		{
			Uint32 *target = newData + pos.x + pos.y * newSize.x;
			const int srcIdx =
				pos.x + offset.x + (pos.y + offset.y) * pic->size.x;
			*target = *(pic->Data + srcIdx);
		}
	}
	// Replace the old data
	free(pic->Data);
	pic->Data = newData;
	pic->size = newSize;
	pic->offset = Vec2iZero();
}
