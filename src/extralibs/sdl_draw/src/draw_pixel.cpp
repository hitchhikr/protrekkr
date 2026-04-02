/*!
  \file Draw_Pixel.c
  \author Mario Palomo <mpalomo@ihman.com>
  \author Jose M. de la Huerga Fern�ndez
  \author Pepe Gonz�lez Mora
  \date 05-2002

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

#if defined(__GCC__)
#include "../include/sdl_draw.h"
#else
#include "../../include/sdl_draw.h"
#endif

#define SDL_DRAW_PUTPIXEL_BPP(A, B, C) *(A(B(UINT8 *) super->pixels + y * super->pitch + x * SDL_DRAW_BPP)) = C;

#define SDL_DRAW_PUTPIXEL SDL_DRAW_PUTPIXEL_BPP(0+, 0+, (UINT8) color)

void STDCALL Draw_Pixel(SDL_Surface *super, INT16 x, INT16 y, UINT32 color)
{
    SDL_DRAW_PUTPIXEL

}/*Draw_Pixel*/

#undef SDL_DRAW_PUTPIXEL
#undef SDL_DRAW_PUTPIXEL_BPP
