/*!
  \file Draw_VLine.c
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

#define SDL_DRAW_PUTPIXEL  \
  i = y1-y0+1;                             \
  switch( i % 4 ) {                        \
    do{                                    \
      case 0: *p = color; p+=super->pitch; \
      case 3: *p = color; p+=super->pitch; \
      case 2: *p = color; p+=super->pitch; \
      case 1: *p = color; p+=super->pitch; \
    }while( (i-=4) > 0 );                  \
  }

void STDCALL Draw_VLine(SDL_Surface *super,
                      Sint16 x0,Sint16 y0, Sint16 y1,
                      Uint32 color)
{
    Uint8 *p;
    Sint16 i;

    if(y0 > y1)
    {
        i = y1;
        y1 = y0; 
        y0 = i;
    }
  
    p = (Uint8 *) super->pixels + y0 * super->pitch + x0 * SDL_DRAW_BPP;

    /* Lock surface */
    if(SDL_MUSTLOCK(super))
    {
        if(SDL_LockSurface(super) < 0)
        { 
            return; 
        }
    }
  
    SDL_DRAW_PUTPIXEL

    /* Unlock surface */
    if(SDL_MUSTLOCK(super))
    { 
        SDL_UnlockSurface(super); 
    }
  
}/*Draw_VLine*/

#undef SDL_DRAW_PUTPIXEL
#undef SDL_DRAW_PUTPIXEL_BPP_3_AUX

