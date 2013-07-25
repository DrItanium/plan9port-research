#include<u.h>
#include<libc.h>
/* Include the ability to draw */
#include<draw.h>

/* 
 * In Plan9, Image is multipurpose
 */
Image* color[256];
Image* bg;
void
main(void) {
   int i;
   int ncolor;
   /*
    * A call to init draw populates extern variables found in draw.h
    * one of them is the display
    */
   initdraw(0,0,"test");
   /* 
    * next part is based off of code from $PLAN9/src/cmd/draw/cmapcube.c 
    */
   ncolor=8;
   for(i = 0; i != ncolor; i++) {
      color[i] = allocimage(display, Rect(0, 0, 1, 1), CMAP8, 1, cmap2rgba(i));
   }

   bg = allocimage(display, Rect(0,0,2,2), screen->chan, 1, DGreygreen);
   draw(bg, Rect(0, 0, 1, 1), color[0], nil, Pt(0,0));

   exits(0);
}
