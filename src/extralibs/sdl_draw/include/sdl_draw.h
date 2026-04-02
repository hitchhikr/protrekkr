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
#include <ptk_types.h>

#define SDL_DRAW_BPP 1

#ifdef __WIN32__
#define STDCALL __stdcall
#else
#define STDCALL
#endif

void STDCALL Draw_Pixel(SDL_Surface *super, INT16 x, INT16 y, UINT32 color);
void STDCALL Draw_HLine(SDL_Surface *super, INT16 x0, INT16 y0, INT16 x1, UINT32 color);
void STDCALL Draw_VLine(SDL_Surface *super, INT16 x0, INT16 y0, INT16 y1, UINT32 color);

#endif /* SDL_DRAW_H */
