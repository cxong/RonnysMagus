/*
    Copyright (c) 2013-2014, Cong Xu
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
#include "vector.h"

#include <math.h>

#include "defs.h"


Vec2i Vec2iNew(int x, int y)
{
	Vec2i v;
	v.x = x;
	v.y = y;
	return v;
}

Vec2i Vec2iZero(void)
{
	return Vec2iNew(0, 0);
}
Vec2i Vec2iUnit(void)
{
	return Vec2iNew(1, 1);
}

Vec2i Vec2iAdd(Vec2i a, Vec2i b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}
Vec2i Vec2iMinus(Vec2i a, Vec2i b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

Vec2i Vec2iScale(Vec2i v, int scalar)
{
	v.x *= scalar;
	v.y *= scalar;
	return v;
}

Vec2i Vec2iScaleDiv(Vec2i v, int scaleDiv)
{
	v.x /= scaleDiv;
	v.y /= scaleDiv;
	return v;
}

Vec2i Vec2iNorm(Vec2i v)
{
	double magnitude;
	if (Vec2iIsZero(v))
	{
		return v;
	}
	magnitude = sqrt(v.x*v.x + v.y*v.y);
	v.x = (int)floor(v.x / magnitude + 0.5);
	v.y = (int)floor(v.y / magnitude + 0.5);
	return v;
}

bool Vec2iEqual(const Vec2i a, const Vec2i b)
{
	return a.x == b.x && a.y == b.y;
}
bool Vec2iIsZero(const Vec2i v)
{
	return Vec2iEqual(v, Vec2iZero());
}

Vec2i Vec2iMin(Vec2i a, Vec2i b)
{
	return Vec2iNew(MIN(a.x, b.x), MIN(a.y, b.y));
}
Vec2i Vec2iMax(Vec2i a, Vec2i b)
{
	return Vec2iNew(MAX(a.x, b.x), MAX(a.y, b.y));
}
Vec2i Vec2iClamp(Vec2i v, Vec2i lo, Vec2i hi)
{
	v.x = CLAMP(v.x, lo.x, hi.x);
	v.y = CLAMP(v.y, lo.y, hi.y);
	return v;
}

Vec2i Vec2iFull2Real(Vec2i v)
{
	return Vec2iScaleDiv(v, 256);
}
Vec2i Vec2iReal2Full(Vec2i v)
{
	return Vec2iScale(v, 256);
}
Vec2i Vec2iReal2FullCentered(const Vec2i v)
{
	return Vec2iAdd(
		Vec2iScale(v, 256), Vec2iNew(128 * SIGN(v.x), 128 * SIGN(v.y)));
}

Vec2i Vec2iToTile(Vec2i v)
{
	return Vec2iNew(v.x / ICONWIDTH, v.y / ICONHEIGHT);
}
Vec2i Vec2iCenterOfTile(Vec2i v)
{
	return Vec2iNew(
		v.x * ICONWIDTH + ICONWIDTH / 2, v.y * ICONHEIGHT + ICONHEIGHT / 2);
}

int DistanceSquared(Vec2i a, Vec2i b)
{
	int dx = a.x - b.x;
	int dy = a.y - b.y;
	return dx*dx + dy*dy;
}
