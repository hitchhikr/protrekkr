/*
  \file SDL_draw.h
  \author Mario Palomo Torrero <mpalomo@ihman.com>
  \author Jose M. de la Huerga Fern�ndez
  \author Pepe Gonz�lez Mora
  \date 05-2002

  Drawing primitives for SDL. Main header file.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA  
*/
#ifndef SDL_DRAW_H
#define SDL_DRAW_H

#include <SDL/SDL.h>

#define SDL_DRAW_BPP 1

#ifdef __WIN32__
#define STDCALL __stdcall
#else
#define STDCALL
#endif

void STDCALL Draw_Pixel(SDL_Surface *super, Sint16 x, Sint16 y, Uint32 color);

void STDCALL Draw_Line(SDL_Surface *super, Sint16 x1, Sint16 y1, Sint16 x2,
Sint16 y2, Uint32 color);

void STDCALL Draw_HLine(SDL_Surface *super, Sint16 x0,Sint16 y0, Sint16 x1,
Uint32 color);

void STDCALL Draw_VLine(SDL_Surface *super, Sint16 x0,Sint16 y0, Sint16 y1,
Uint32 color);

#endif /* SDL_DRAW_H */
