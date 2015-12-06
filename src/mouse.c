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
#include "mouse.h"

#include <string.h>

#include <SDL_events.h>
#include <SDL_mouse.h>


#define MOUSE_REPEAT_TICKS 150
#define MOUSE_MOVE_DEAD_ZONE 12
#define TRAIL_NUM_DOTS 4

Mouse gMouse;


void MouseInit(Mouse *mouse, const bool hideMouse)
{
	memset(mouse, 0, sizeof *mouse);
	mouse->ticks = 0;
	mouse->repeatedTicks = 0;
	mouse->hideMouse = hideMouse;
	if (hideMouse)
	{
		SDL_ShowCursor(SDL_DISABLE);
	}
}

void MousePrePoll(Mouse *mouse)
{
	memset(mouse->pressedButtons, 0, sizeof mouse->pressedButtons);
	memcpy(
		mouse->previousButtons,
		mouse->currentButtons,
		sizeof mouse->previousButtons);
	mouse->wheel = Vec2iZero();
	mouse->previousPos = mouse->currentPos;
	SDL_GetMouseState(&mouse->currentPos.x, &mouse->currentPos.y);
}


void MouseOnButtonDown(Mouse *mouse, Uint8 button)
{
	mouse->currentButtons[button] = 1;
}
void MouseOnButtonUp(Mouse *mouse, Uint8 button)
{
	// Certain mouse buttons can be pressed very quickly (e.g. mousewheel)
	// Detect these mouse presses before PostPoll
	if (mouse->currentButtons[button] && !mouse->previousButtons[button])
	{
		mouse->pressedButtons[button] = 1;
	}
	mouse->currentButtons[button] = 0;
}
void MouseOnWheel(Mouse *m, const Sint32 x, const Sint32 y)
{
	m->wheel = Vec2iNew(x, y);
}

void MousePostPoll(Mouse *mouse, Uint32 ticks)
{
	int areSameButtonsPressed = 1;
	int i;
	for (i = 0; i < 8; i++)
	{
		if (mouse->previousButtons[i] ^ mouse->currentButtons[i])
		{
			areSameButtonsPressed = 0;
			break;
		}
	}
	// If same buttons have been pressed, remember how long they have been pressed
	if (areSameButtonsPressed)
	{
		Uint32 ticksElapsed = ticks - mouse->ticks;
		mouse->repeatedTicks += ticksElapsed;
	}
	else
	{
		mouse->repeatedTicks = 0;
	}
	// If more time has elapsed, forget about previous buttons for repeating
	if (mouse->repeatedTicks > MOUSE_REPEAT_TICKS)
	{
		mouse->repeatedTicks -= MOUSE_REPEAT_TICKS;
		for (i = 0; i < 8; i++)
		{
			mouse->pressedButtons[i] |= mouse->currentButtons[i];
		}
		memcpy(
			mouse->pressedButtons,
			mouse->currentButtons,
			sizeof mouse->pressedButtons);
	}
	else
	{
		for (i = 0; i < 8; i++)
		{
			mouse->pressedButtons[i] |=
				mouse->currentButtons[i] && !mouse->previousButtons[i];
		}
	}
	mouse->ticks = ticks;
}

bool MouseHasMoved(const Mouse *m)
{
	return !Vec2iEqual(m->previousPos, m->currentPos);
}

int MouseGetPressed(const Mouse *m)
{
	for (int i = 0; i < 8; i++)
	{
		if (m->pressedButtons[i])
		{
			return i;
		}
	}
	return 0;
}

bool MouseIsPressed(const Mouse *m, const int button)
{
	return m->pressedButtons[button];
}

bool MouseIsDown(const Mouse *m, const int button)
{
	return m->currentButtons[button];
}

Vec2i MouseWheel(const Mouse *m)
{
	return m->wheel;
}
