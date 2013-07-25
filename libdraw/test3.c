#include<u.h>
#include<libc.h>
/* Include the ability to draw to the screen */
#include<draw.h>
/* Include the ability to query the mouse */
#include<event.h>

void
eresized(int new) {
   //do nothing
}
/* 
 * In Plan9, Image is multipurpose
 */
Image* color[256];
Image* bg;
void
main(void) {
   int i;
   int ncolor;
   /* The mouse struct stores mouse information */
   Mouse m;
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
   /* 
    * The problem with the previous tests was that the program quits
    * instantly due to the call to exits(0). I want to actually see the
    * window this time. 
    *
    * Initialize the mouse.
    * Requires a resized handler according to the linker
    */

   einit(Emouse);
   
   for(;;) {
      /* query the mouse for information */
      m = emouse();
      /* 
       * now we need to do something with the buttons 
       * not sure what this does so lets print out things to the 
       * console
       *
       * all of the usage examples use fprint(2,...) which 
       * probably means it prints out to stderr.
       *
       * Mouse combinations
       * m.buttons & 1 => left-click
       * m.buttons & 2 => middle-click 
       * m.buttons & 4 => right-click 
       *
       */
      if(m.buttons & 1) {
         fprint(2, "m.buttons & 1 pressed\n");
      } else if(m.buttons & 2) {
         fprint(2, "m.buttons & 2 pressed\n");
      } else if(m.buttons & 3) {
         fprint(2, "m.buttons & 3 pressed\n");
      } else if(m.buttons & 4) {
         fprint(2, "m.buttons & 4 pressed\n");
      } 
      /* exiting on else was a bad idea. The program will immediately
       * close since no buttons are being pressed */
   }
   exits(0);

}
