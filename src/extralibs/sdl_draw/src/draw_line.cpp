/*!
  \file Draw_Line.c
  \author Mario Palomo <mpalomo@ihman.com>
  \author Jose M. de la Huerga Fern�ndez
  \author Pepe Gonz�lez Mora
  \date 05-2002

  Based in Kenny Hoff sourcer.
*/
/* 
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

#define SDL_DRAW_PUTPIXEL_BPP(A, B, C, D) *(A(B(Uint8*)D))=C;
  
#if SDL_DRAW_BPP == 1
#define SDL_DRAW_PUTPIXEL(D) SDL_DRAW_PUTPIXEL_BPP(0+,0+,color,D)

#elif SDL_DRAW_BPP == 4
#define SDL_DRAW_PUTPIXEL(D) SDL_DRAW_PUTPIXEL_BPP((Uint32*),0+,color,D)

#endif /*SDL_DRAW_BPP*/


void STDCALL Draw_Line(SDL_Surface *super,
                      Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,
                      Uint32 color)
{
  
  Sint16 dx;
  Sint16 dy;
  
  Sint16 fbXincr, fbYincr, fbXYincr;
  Sint16 dPr, dPru, P;

  Sint16 pixx = super->format->BytesPerPixel;
  Sint16 pixy = super->pitch;

  Uint8 *AfbAddr, *BfbAddr;

  /* Clip line and test if we have to draw only if we need to do it */
/* It is in next version.
#ifdef SDL_DRAW_CLIP
  if (!(clipLine(super, &x1, &y1, &x2, &y2))) { return; }
#endif
*/
  /* Store the fremebuffer Endpoint-Addresses (A and B) */ 
  AfbAddr = ((Uint8*)super->pixels) + pixx * (int)x1 + pixy * (int)y1;
  BfbAddr = ((Uint8*)super->pixels) + pixx * (int)x2 + pixy * (int)y2;

  
    
  /* Lock surface */
  if (SDL_MUSTLOCK(super)) {
      if (SDL_LockSurface(super) < 0)  { return; }
  }
  
  fbXincr=pixx;
  if ( (dx=x2-x1) >= 0 ) goto AFTERNEGX;
    dx = -dx;
    fbXincr = -pixx;
  AFTERNEGX:

  fbYincr=pixy; //debug
  if ( (dy=y2-y1) >= 0) goto AFTERNEGY;
    fbYincr = -pixy;
    dy = -dy;
  AFTERNEGY:

  fbXYincr = fbXincr+fbYincr;

  if (dy > dx) goto YisIndependent;	/*Check if X or Y is independent vaiable */

  /*XisIndependent;*/
    dPr = dy+dy;
    P = -dx;
    dPru = P+P;
    dy = dx>>1;
    XLOOP:
      SDL_DRAW_PUTPIXEL(AfbAddr);	/*Plot the pixel from end of pointer one*/
      SDL_DRAW_PUTPIXEL(BfbAddr);	/*Plot the pixel from end of pointer two*/
      if ((P+=dPr) > 0) goto RightAndUp;
      /*Up:*/
        AfbAddr+=fbXincr;
	BfbAddr-=fbXincr;
        if ((dy=dy-1) > 0) goto XLOOP;
        SDL_DRAW_PUTPIXEL(AfbAddr)  	/*(Fix midpoint problem) Plot last PT from end pointer one*/
        if (( dx & 1) == 0) goto END_P;
        SDL_DRAW_PUTPIXEL(BfbAddr);	/*Plot last PT from end of pointer two if independent is odd*/
        goto END_P;
      RightAndUp:
	AfbAddr+=fbXYincr;		 /*Advance to next point from end of pointer one*/
	BfbAddr-=fbXYincr;		 /*Advance to next point from end of pointer two*/
	P+=dPru;
	if ((dy=dy-1) > 0) goto XLOOP;
	SDL_DRAW_PUTPIXEL(AfbAddr);	 /*(Fix midpoint problem) Plot last PT from end of pointer one*/
	if ((dx & 1) == 0) goto END_P;
	SDL_DRAW_PUTPIXEL(BfbAddr);	 /*Plot last PT from end of pointer two if indepent is odd*/
	goto END_P;
	
    YisIndependent:
	dPr = dx+dx;
	P = -dy;
	dPru = P+P;
	dx = dy >>1;
     YLOOP:                                 /* PROCESS EACH POINT IN THE LINE ONE AT A TIME (use dX as loop counter) */
        SDL_DRAW_PUTPIXEL(AfbAddr);         /* PLOT THE PIXEL FROM END A */
        SDL_DRAW_PUTPIXEL(BfbAddr);         /* PLOT THE PIXEL FROM END B */
        if ((P+=dPr) > 0) goto RightAndUp2; /* INCREMENT DECISION, CHECK IF THE PIXEL IS GOING RIGHT AND UP */
        /*Up:*/
            AfbAddr+=fbYincr;               /* ADVANCE TO NEXT POINT FROM END A */
            BfbAddr-=fbYincr;               /* ADVANCE TO NEXT POINT FROM END B */
            if ((dx=dx-1) > 0) goto YLOOP;  /* DECREMENT LOOP VARIABLE AND LOOP */
            SDL_DRAW_PUTPIXEL(AfbAddr);     /* (FIX MIDPOINT PROBLEM) PLOT THE LAST POINT FROM END A */
            if ((dy & 1) == 0) goto END_P;      /* FINISHED IF INDEPENDENT IS EVEN (ODD # STEPS) */
            SDL_DRAW_PUTPIXEL(BfbAddr);     /* PLOT LAST PT FROM END B IF INDEPENDENT IS ODD (EVEN # STEPS) */
            goto END_P;
        RightAndUp2:
             AfbAddr+=fbXYincr;             /* ADVANCE TO NEXT POINT FROM END A */
             BfbAddr-=fbXYincr;             /* ADVANCE TO NEXT POINT FROM END B */
            P+=dPru;                        /* INCREMENT DECISION (for up) */
            if ((dx=dx-1) > 0) goto YLOOP;  /* DECREMENT LOOP VARIABLE AND LOOP */
            SDL_DRAW_PUTPIXEL(AfbAddr);     /* (FIX MIDPOINT PROBLEM) PLOT THE LAST POINT FROM END A */
            if ((dy & 1) == 0) goto END_P;      /* FINISHED IF INDEPENDENT IS EVEN (ODD # STEPS) */
            SDL_DRAW_PUTPIXEL(BfbAddr);     /* PLOT LAST PT FROM END B IF INDEPENDENT IS ODD (EVEN # STEPS) */
     
END_P: 
  /* Unlock surface */
  if (SDL_MUSTLOCK(super))  { SDL_UnlockSurface(super); }
  
}/*Draw_Line*/


#undef SDL_DRAW_PUTPIXEL
#undef SDL_DRAW_PUTPIXEL_BPP

