#include<u.h>
#include<libc.h>
/* Include the ability to draw to the screen */
#include<draw.h>
/* Include the ability to query the mouse */
#include<event.h>

/* tweak these macros to change how the color cycler works */
#define COLOR_COUNT 256 
#define PLANE_COUNT 8 
#define PLANE_FACTOR 64 
#define NEST_OFFSET 0 


Image* color[COLOR_COUNT];
/* use this variable to shift through the entire pallete on a limited number of 
 * planes */
int offset;
Rectangle crect[PLANE_COUNT];
Rectangle center;

void
redraw(Image *screen) {
    int i, j;
    if(offset < COLOR_COUNT) {
        offset++;
    } else {
        offset = 0;
    }
    for(i = 0, j = 1; j < PLANE_COUNT; i++, j++) {
        draw(screen, crect[i], color[(offset + j) % COLOR_COUNT], nil, ZP); 
    }
    draw(screen, crect[i], color[(offset + j) % COLOR_COUNT], nil, ZP);
    draw(screen, center, display->black, nil, ZP);
    flushimage(display, 1);
}
void 
setup_rectangles() {
    int i;
    for(i = 0; i < PLANE_COUNT; i++) {
        crect[i] = insetrect(screen->r, PLANE_FACTOR * i + NEST_OFFSET);
    }
    center = insetrect(screen->r, PLANE_FACTOR * i + NEST_OFFSET);
}
void
eresized(int new) {
    int i;

    /* If its a new window but we can't get it then fail out */
    if(new && getwindow(display, Refnone) < 0) {
        fprint(2, "can't reattach to window: %r\n");
        exits("resized");
    }
    /* refresh the screen ? */
    draw(screen, screen->r, display->black, nil, ZP);

    for(i = 0; i < PLANE_COUNT; i++) {
        draw(screen, crect[i], color[i], nil, ZP);
    }
    offset = i;
    flushimage(display, 1);
}
/* 
 * In Plan9, Image is multipurpose
 */
void
main(void) {
    int i, key, t;
    int timer;
    Event e;
    Mouse m;
    offset = -1;
    /* The mouse struct stores mouse information */
    /*
     * A call to init draw populates extern variables found in draw.h
     * one of them is the display
     */
    if(initdraw(0,0,"test") < 0) {
        sysfatal("initdraw failed: %r");
    }
    /* 
     * next part is based off of code from $PLAN9/src/cmd/draw/cmapcube.c 
     */

    setup_rectangles();
    /* 
     * The problem with the previous tests was that the program quits
     * instantly due to the call to exits(0). I want to actually see the
     * window this time. 
     *
     * Initialize the mouse.
     * Requires a resized handler according to the linker
     */
    for(i=0; i < COLOR_COUNT; i++) {
        color[i] = allocimage(display, Rect(0,0,1,1), screen->chan, 1, (cmap2rgb(i)<<8)+0xFF);
        if(color == nil) {
            sysfatal("can't allocate image: %r");
        }
    }
    eresized(0);
    einit(Emouse);
    /* 72 hz I think */
    t = (72);
    /* setup a timer so we can fire events off at constant intervals */
    timer = etimer(0, t);
    for(;;) {
        /* query the mouse for information */
        key = event(&e);
        if(key == Emouse) {
            m = e.mouse;
            /* quit on right click */
            if(m.buttons & 4){
                exits(0);
            }
        } else if(key == timer) {
            redraw(screen);
        }
    }
}

