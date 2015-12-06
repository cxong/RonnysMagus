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
#include "events.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_timer.h>


EventHandlers gEventHandlers;

void EventInit(EventHandlers *handlers, const bool hideMouse)
{
	memset(handlers, 0, sizeof *handlers);
	KeyInit(&handlers->keyboard);
	MouseInit(&handlers->mouse, hideMouse);
}
void EventTerminate(EventHandlers *handlers)
{
}
void EventReset(EventHandlers *handlers)
{
	KeyInit(&handlers->keyboard);
	MouseInit(&handlers->mouse, handlers->mouse.hideMouse);
}

void EventPoll(EventHandlers *handlers, Uint32 ticks)
{
	SDL_Event e;
	KeyPrePoll(&handlers->keyboard);
	MousePrePoll(&handlers->mouse);
	SDL_free(handlers->DropFile);
	handlers->DropFile = NULL;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
			if (e.key.repeat)
			{
				break;
			}
			KeyOnKeyDown(&handlers->keyboard, e.key.keysym);
			break;
		case SDL_KEYUP:
			KeyOnKeyUp(&handlers->keyboard, e.key.keysym);
			break;
		case SDL_TEXTINPUT:
			strcpy(handlers->keyboard.Typed, e.text.text);
			break;
		case SDL_MOUSEBUTTONDOWN:
			MouseOnButtonDown(&handlers->mouse, e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			MouseOnButtonUp(&handlers->mouse, e.button.button);
			break;
		case SDL_MOUSEWHEEL:
			MouseOnWheel(&handlers->mouse, e.wheel.x, e.wheel.y);
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_LOST:
				// Reset input handlers
				EventReset(handlers);
				break;
			default:
				// do nothing
				break;
			}
			break;
		case SDL_QUIT:
			handlers->HasQuit = true;
			break;
		case SDL_DROPFILE:
			handlers->DropFile = e.drop.file;
			break;
		default:
			break;
		}
	}
	KeyPostPoll(&handlers->keyboard, ticks);
	MousePostPoll(&handlers->mouse, ticks);
}
