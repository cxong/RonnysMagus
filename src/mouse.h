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
#pragma once

#include <SDL_stdinc.h>

#include "vector.h"

typedef struct
{
	bool previousButtons[8];
	bool currentButtons[8];
	bool pressedButtons[8];
	Vec2i previousPos;
	Vec2i currentPos;
	Vec2i wheel;
	Uint32 ticks;
	Uint32 repeatedTicks;

	bool hideMouse;
	Vec2i mouseMovePos;
} Mouse;

extern Mouse gMouse;

void MouseInit(Mouse *mouse, const bool hideMouse);
void MousePrePoll(Mouse *mouse);
void MouseOnButtonDown(Mouse *mouse, Uint8 button);
void MouseOnButtonUp(Mouse *mouse, Uint8 button);
void MouseOnWheel(Mouse *m, const Sint32 x, const Sint32 y);
void MousePostPoll(Mouse *mouse, Uint32 ticks);
bool MouseHasMoved(const Mouse *m);
int MouseGetPressed(const Mouse *m);
bool MouseIsDown(const Mouse *m, const int button);
bool MouseIsPressed(const Mouse *m, const int button);
// Get wheel movement since last poll
Vec2i MouseWheel(const Mouse *m);
