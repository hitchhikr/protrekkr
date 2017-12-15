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

#define SDL_DRAW_PUTPIXEL_BPP(A, B, C)  \
*(A(B(Uint8*)super->pixels + y*super->pitch + x*SDL_DRAW_BPP))=C;

#if SDL_DRAW_BPP == 1
#define SDL_DRAW_PUTPIXEL SDL_DRAW_PUTPIXEL_BPP(0+,0+,(Uint8)color)

#elif SDL_DRAW_BPP == 4
#define SDL_DRAW_PUTPIXEL SDL_DRAW_PUTPIXEL_BPP((Uint32*),0+,color)

#endif /*SDL_DRAW_BPP*/

void STDCALL Draw_Pixel(SDL_Surface *super,
                      Sint16 x, Sint16 y, Uint32 color)
{
  /* Lock surface */
  if (SDL_MUSTLOCK(super)) {
      if (SDL_LockSurface(super) < 0)  { return; }
  }
  
  SDL_DRAW_PUTPIXEL

  /* Unlock surface */
  if (SDL_MUSTLOCK(super))  { SDL_UnlockSurface(super); }
  
}/*Draw_Pixel*/


#undef SDL_DRAW_PUTPIXEL
#undef SDL_DRAW_PUTPIXEL_BPP

