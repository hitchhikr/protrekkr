/*
  \file Draw_HLine.c
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

#if SDL_DRAW_BPP == 1
#define SDL_DRAW_PUTPIXEL \
  memset(p, color, x1-x0+1);

#elif SDL_DRAW_BPP == 4

#ifdef __LINUX__
#define SDL_DRAW_WMEMSET_START \
if (sizeof(wchar_t) == sizeof(Uint32)) { \
  wmemset((wchar_t*)p, color, x1-x0+1);  \
} else {
#define SDL_DRAW_WMEMSET_END }
#else
#define SDL_DRAW_WMEMSET_START
#define SDL_DRAW_WMEMSET_END
#endif

#define SDL_DRAW_PUTPIXEL \
SDL_DRAW_WMEMSET_START                   \
  i = x1-x0+1;                           \
  switch( i % 4 ) {                      \
    do{                                  \
      case 0: *(Uint32*)p = color; p+=4; \
      case 3: *(Uint32*)p = color; p+=4; \
      case 2: *(Uint32*)p = color; p+=4; \
      case 1: *(Uint32*)p = color; p+=4; \
    }while( (i-=4) > 0 );                \
  }                                      \
SDL_DRAW_WMEMSET_END

#endif /*SDL_DRAW_BPP*/


void STDCALL Draw_HLine(SDL_Surface *super,
                      Sint16 x0,Sint16 y0, Sint16 x1,
                      Uint32 color)
{

  register Uint8 *p;
  register Sint16 i;

  if (x0 > x1) { i=x1; x1=x0; x0=i; }
  p = (Uint8*) super->pixels + y0 * super->pitch + x0 * SDL_DRAW_BPP;

  /* Lock surface */
  if(SDL_MUSTLOCK(super)) {
      if(SDL_LockSurface(super) < 0)  { return; }
  }
  
  SDL_DRAW_PUTPIXEL

  /* Unlock surface */
  if (SDL_MUSTLOCK(super))  { SDL_UnlockSurface(super); }
  
}/*Draw_HLine*/


#undef SDL_DRAW_PUTPIXEL
#undef SDL_DRAW_PUTPIXEL_BPP_3_AUX

#undef SDL_DRAW_WMEMSET_START
#undef SDL_DRAW_WMEMSET_END

