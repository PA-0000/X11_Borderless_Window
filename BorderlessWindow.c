/* ---------------------------------------------------------------------------------
 * This program is a simple example of creating a borderless and resizable window
 * in x11. Copyright (c) 2026-2027 Pouya Asadi
 *
 * This software is in the public domain and is provided AS IS, with NO WARRANTY.
 * ---------------------------------------------------------------------------------
 * The borderless window can be resized, moved, and also supports all the XWindow
 * features a regular window has:
 *  - snapping to desktop halves/quadrants
 *  - shake to minimize all other windows
 *  - animations when minimizing, maximizing, restoring, snapping
 *  - native soft shadow around the window
 *
 * Keybinds:
 *  - ESC    Close Window
 *  - SPACE  Close Window
 *  - B      (Enables/Disables) Windowed or Borderless Window
 *  - R      (Enables/Disables) Resizing and Dragging the Borderless Window
 * ------------------------------------------------------------------------------ */

#include "BorderlessWindow.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>


/*
 * function: Creates a graphics context with a white background in client area.
 * input:    display, window, and Reverse video boolean value
 * output:   Graphics Context Struct.
 */
GC Create_Graphics_Context(Display* display, Window win, BOOL reverse_video)
{
  GC gc;                          /* handle of newly created GC.  */
  unsigned long valuemask = 0;    /* which values in 'values' to  */
                                  /* check when creating the GC.  */
  XGCValues values;               /* initial values for the GC.   */
  unsigned int line_width = 2;    /* line width for the GC.       */
  int line_style = LineSolid;     /* style for lines drawing and  */
  int cap_style  = CapButt;       /* style of the line's edje and */
  int join_style = JoinBevel;     /* joined lines.                */
  int screen_num = DefaultScreen(display);

  gc = XCreateGC(display, win, valuemask, &values);
  if (gc < 0) { fprintf(stderr, "XCreateGC: \n"); }

  /* allocate foreground and background colors for this GC. */
  if (reverse_video) {
    XSetForeground(display, gc, WhitePixel(display, screen_num));
    XSetBackground(display, gc, BlackPixel(display, screen_num));
  }
  else {
    XSetForeground(display, gc, BlackPixel(display, screen_num));
    XSetBackground(display, gc, WhitePixel(display, screen_num));
  }

  /* define the style of lines that will be drawn using this GC. */
  XSetLineAttributes(display, gc,
                     line_width, line_style, cap_style, join_style);

  /* define the fill style for the GC. to be 'solid filling'. */
  XSetFillStyle(display, gc, FillSolid);

  return gc;
}


/*
 * function: Set Borderless Window.
 * input:    display, window, and boolean state of windowed or borderless
 * output:   none.
 */
BOOL Borderless_flag = FALSE;  /* Borderless window flag */
void Set_Borderless (Display *display, Window win, BOOL value)
{
    /* Set borderless window property */
    struct {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long input_mode;
        unsigned long status;
    } hints = {0};

    if (value == TRUE) {  /* Borderless Window */
        hints.flags       = 2; // Means       ON
        hints.decorations = 0; // Decorations OFF
        Borderless_flag = TRUE;
    } else {              /* Bordered Window   */
        hints.flags       = 2; // Means       ON
        hints.decorations = 1; // Decorations ON
        Borderless_flag = FALSE;
    }

    Atom MOTIF_WM_HINTS = XInternAtom(display, "_MOTIF_WM_HINTS", False);

    XChangeProperty(display, win, MOTIF_WM_HINTS, MOTIF_WM_HINTS,
                    32, PropModeReplace, (unsigned char*) &hints,
                    sizeof(hints) / sizeof(long));
}


/*
 * function: Set Resizable Window.
 * input:    display, window, and boolean resizable window state
 * output:   none.
 */
BOOL Resizable_flag  = TRUE;  /* Resizable window flag */
void Set_Resizable (Display *display, Window win, BOOL value)
{
    if (value == TRUE) { /* Resizable ON  */
        Resizable_flag = TRUE;
    } else {             /* Resizable OFF */
        Resizable_flag = FALSE;
    }
}


/*
 * function: handle resize. handles a Mouse press event - if the left button
 *           is depressed - and cursor position in padding area resize window
 * input:    display, window, XEvent padding_evt, XButtonEvent event structure,
 *           size of the window (in pixels), and cursor position,
 * output:   none.
 */
#define WINDOW_PADDING_BORDER    10
int SUB_WIN_W_WINDOW_PADDING_BORDER;
int SUB_WIN_H_WINDOW_PADDING_BORDER;
void handle_resize(Display* display, XEvent* Padding_Evt, XButtonEvent* Evt,
                   unsigned int WinW, unsigned int WinH, float CurX, float CurY)
{
  /* Borderless Window (Resize and Drag-Drop) Handeling  */
  SUB_WIN_W_WINDOW_PADDING_BORDER = (WinW - WINDOW_PADDING_BORDER);
  SUB_WIN_H_WINDOW_PADDING_BORDER = (WinH - WINDOW_PADDING_BORDER);

  if(CurY < WINDOW_PADDING_BORDER) { /* Top Area */
    if(CurX < WINDOW_PADDING_BORDER)
    { /* ------------------------- Top Left Corner ------------------------- */
      Padding_Evt->xclient.data.l[0] = Evt->x_root;
      Padding_Evt->xclient.data.l[1] = Evt->y_root;
      Padding_Evt->xclient.data.l[2] = 0; // Top Left Corner

      XSendEvent(display, XDefaultRootWindow(display), 0,
          SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
    }
    else if (CurX > SUB_WIN_W_WINDOW_PADDING_BORDER)
    { /* ------------------------- Top Right Corner ------------------------ */
      Padding_Evt->xclient.data.l[0] = Evt->x_root;
      Padding_Evt->xclient.data.l[1] = Evt->y_root;
      Padding_Evt->xclient.data.l[2] = 2; // Top Right Corner

      XSendEvent(display, XDefaultRootWindow(display), 0,
          SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
    }
    else { /* ------------------------ Top Side ---------------------------- */
      Padding_Evt->xclient.data.l[0] = Evt->x_root;
      Padding_Evt->xclient.data.l[1] = Evt->y_root;
      Padding_Evt->xclient.data.l[2] = 1; // Top Side

      XSendEvent(display, XDefaultRootWindow(display), 0,
          SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
    }
  }

  if (CurY > SUB_WIN_H_WINDOW_PADDING_BORDER) { /* Bottom Area */
    if(CurX < WINDOW_PADDING_BORDER)
    { /* ------------------------ Bottom Left Corner ----------------------- */
      Padding_Evt->xclient.data.l[0] = Evt->x_root;
      Padding_Evt->xclient.data.l[1] = Evt->y_root;
      Padding_Evt->xclient.data.l[2] = 6; // Bottom Left Corner

      XSendEvent(display, XDefaultRootWindow(display), 0,
          SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
    }
    else if (CurX > SUB_WIN_W_WINDOW_PADDING_BORDER)
    { /* ------------------------ Bottom Right Corner ---------------------- */
      Padding_Evt->xclient.data.l[0] = Evt->x_root;
      Padding_Evt->xclient.data.l[1] = Evt->y_root;
      Padding_Evt->xclient.data.l[2] = 4; // Bottom Right Corner

      XSendEvent(display, XDefaultRootWindow(display), 0,
          SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
    }
    else { /* ---------------------- Bottom Side --------------------------- */
      Padding_Evt->xclient.data.l[0] = Evt->x_root;
      Padding_Evt->xclient.data.l[1] = Evt->y_root;
      Padding_Evt->xclient.data.l[2] = 5; // Bottom Side

      XSendEvent(display, XDefaultRootWindow(display), 0,
          SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
    }
  } // -------------------------- Bottom -------------------------

  if(CurX < WINDOW_PADDING_BORDER)
  { /* ----------------------------- Left Side ----------------------------- */
    Padding_Evt->xclient.data.l[0] = Evt->x_root;
    Padding_Evt->xclient.data.l[1] = Evt->y_root;
    Padding_Evt->xclient.data.l[2] = 7; // Left Side

    XSendEvent(display, XDefaultRootWindow(display), 0,
        SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
  }

  if(CurX > SUB_WIN_W_WINDOW_PADDING_BORDER)
  { /* ----------------------------- Right Side ---------------------------- */
    Padding_Evt->xclient.data.l[0] = Evt->x_root;
    Padding_Evt->xclient.data.l[1] = Evt->y_root;
    Padding_Evt->xclient.data.l[2] = 3; // Right Side

    XSendEvent(display, XDefaultRootWindow(display), 0,
        SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
  }

  if (CurX > WINDOW_PADDING_BORDER && CurX < SUB_WIN_W_WINDOW_PADDING_BORDER &&
      CurY > WINDOW_PADDING_BORDER && CurY < SUB_WIN_H_WINDOW_PADDING_BORDER)
  { /* ----------------------------- Drag - Drop --------------------------- */
    Padding_Evt->xclient.data.l[0] = Evt->x_root;
    Padding_Evt->xclient.data.l[1] = Evt->y_root;
    Padding_Evt->xclient.data.l[2] = 8; // Move

    XSendEvent(display, XDefaultRootWindow(display), 0,
        SubstructureRedirectMask | SubstructureNotifyMask, Padding_Evt);
  }
}


int main(int argc, char* argv[])
{
    Display* display;          /* pointer to X Display structure.           */
    int screen_num;            /* number of screen to place the window on.  */
    Window MainWin;            /* pointer to the newly created window.      */
    unsigned int display_width,     /* height and width of the X display.   */
                 display_height;
    unsigned int WinW, WinH;	    /* height and width for the new window. */
    signed   int WinX, WinY;	    /* X and Y position for the new window. */
    signed   int CurX, CurY;        /* Cursor position in the display.      */
    char *display_name = getenv("DISPLAY");    /* address of the X display. */
    GC gc;                  /* GC (graphics context) used for drawing       */
                            /* in our window.                               */

    /* open connection with the X server. */
    display = XOpenDisplay(display_name);
    if (display == NULL) {
        fprintf(stderr, "%s: cannot connect to X server '%s'\n",
            argv[0], display_name);
        exit(1);
    }

    /* get the geometry of the default screen for our display. */
    screen_num     = DefaultScreen(display);
    display_width  = DisplayWidth (display, screen_num);
    display_height = DisplayHeight(display, screen_num);

    /* make the new window occupy 1/9 of the screen's size. */
    WinW = (unsigned int)(display_width  / 3.5);
    WinH = (unsigned int)(display_height / 3.5);
    WinX =   (signed int)(display_width  / 2) - (WinW / 2);
    WinY =   (signed int)(display_height / 2) - (WinH / 2);

    // printf("window width - '%d'; height - '%d'\n", WinW, WinH);

    /* create a simple window, as a direct child of the screen's   */
    /* root window. Use the screen's white color as the background */
    /* color of the window. Place the new window's top-left corner */
    /* at the given 'x,y' coordinates.                             */
    MainWin = XCreateSimpleWindow(display, RootWindow(display, screen_num),
                                    WinX, WinY, WinW, WinH,
                                    2, // Window Border Width,
                                    BlackPixel(display, screen_num),
                                    WhitePixel(display, screen_num));

    /* set window title */
    XStoreName(display, MainWin, APPLICATION_TITLEBAR);

    /* make the window actually appear on the screen. */
    XMapWindow(display, MainWin);

    /* flush all pending requests to the X server. */
    XFlush(display);

    /* allocate two new GCs (graphics contexts) for drawing in the window. */
    /* the first is used for drawing black over white, the second is used  */
    /* for drawing white over black.                                       */
    gc = Create_Graphics_Context(display, MainWin, FALSE);
    XSync(display, False);

    /* subscribe to the given set of event types. */
    XSelectInput(display, MainWin, ExposureMask | KeyPressMask |
                 ButtonPressMask   | Button1MotionMask |
    	         Button2MotionMask | StructureNotifyMask);


    /* Resizable Padding Event */
    XEvent Padding_Evt = {0};
    Padding_Evt.xclient.type         = ClientMessage;
    Padding_Evt.xclient.window       = MainWin;
    Padding_Evt.xclient.message_type = XInternAtom(display, "_NET_WM_MOVERESIZE", False);
    Padding_Evt.xclient.format       = 32;

    Padding_Evt.xclient.data.l[3] =  0; // Defualt Value (0)
    Padding_Evt.xclient.data.l[4] =  1; // source indication (1 = normal application)

    /* perform an events loop */
    {
      int done = 0;
      XEvent Evt;
      while (!done) {
        XNextEvent(display, &Evt);
        switch (Evt.type) {
        case ConfigureNotify:
            /* update the size of our window, for expose events. */
            WinW = Evt.xconfigure.width;
            WinH = Evt.xconfigure.height;
            break;

        case MotionNotify:
            /* Borderless Window (Resize and Drag-Drop) Handeling  */
            CurX = Evt.xbutton.x;  CurY = Evt.xbutton.y;
            if (Borderless_flag == TRUE) {
              if (Resizable_flag == TRUE) {
                handle_resize(display, &Padding_Evt, (XButtonEvent*)&Evt,
                                WinW, WinH, CurX, CurY);
              }
            }
            break;

        case KeyPress:
            /* exit the application by braking out of the events loop. */
            if (Evt.xkey.keycode == 0x09) // Key ESCAPE
              { done = 1; }
            if (Evt.xkey.keycode == 0x41) // Key SPACE
              { done = 1; }
            if (Evt.xkey.keycode == 0x38) { // Key 'B'
              if (Borderless_flag == TRUE) {
                Set_Borderless(display, MainWin, FALSE);
              } else {
                Set_Borderless(display, MainWin, TRUE);
              }
            }
            if (Evt.xkey.keycode == 0x1B) { // Key 'R'
              if (Resizable_flag == TRUE) {
                Set_Resizable(display, MainWin, FALSE);
                if (Borderless_flag == False) {
                  /* allocate a size hints structure. */
                  XSizeHints* size_hints = XAllocSizeHints();
                  if (!size_hints) {
                    fprintf(stderr, "XAllocSizeHints - out of memory\n");
                    exit(1);
                  }
                  /* initialize the structure appropriatly. */
                  /* first, specify which size hints we want to fill in. */
                  /* in our case - setting the minimal size as well as the initial size. */
                  size_hints->flags = PMinSize | PMaxSize;
                  /* next, specify the desired limits.                             */
                  /* in our case - make the window's size at least 300x200 pixels. */
                  /* and make its initial size 400x250.                            */
                  size_hints->min_width  = WinW;
                  size_hints->min_height = WinH;
                  size_hints->max_width  = WinW;
                  size_hints->max_height = WinH;
                  /* pass the size hints to the window manager. */
                  XSetWMNormalHints(display, MainWin, size_hints);
                  /* finally, we can free the size hints structure. */
                  XFree(size_hints);
                }
              } else {
                Set_Resizable(display, MainWin, TRUE);
                if (Borderless_flag == False) {
                  /* allocate a size hints structure. */
                  XSizeHints* size_hints = XAllocSizeHints();
                  if (!size_hints) {
                    fprintf(stderr, "XAllocSizeHints - out of memory\n");
                    exit(1);
                  }
                  /* initialize the structure appropriatly. */
                  /* first, specify which size hints we want to fill in. */
                  /* in our case - setting the minimal size as well as the initial size. */
                  size_hints->flags = PSize;
                  /* next, specify the desired limits.                             */
                  /* in our case - make the window's size at least 300x200 pixels. */
                  /* and make its initial size 400x250.                            */
                  size_hints->base_width  = WinW;
                  size_hints->base_height = WinH;
                  /* pass the size hints to the window manager. */
                  XSetWMNormalHints(display, MainWin, size_hints);
                  /* finally, we can free the size hints structure. */
                  XFree(size_hints);
                }
              }
            }
            break;

        default: /* ignore any other event types. */
            break;
        } /* end switch on event type */
      } /* end while events handling */
    }

  /* free the GCs. */
  XFreeGC(display, gc);

  /* close the connection to the X server. */
  XCloseDisplay(display);
  return 0;
}
