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

#define X11_BORDERLESS_WINDOW
#ifdef  X11_BORDERLESS_WINDOW

/* ---------------------------------------------------------------------------------
 *                      X11 Borderless Window API tokens
 * ------------------------------------------------------------------------------ */

/*! @brief The major version number of the X11 Borderless Window header.
 *
 *  The major version number of the X11 Borderless Window header.
 *  This is incremented when the API is changed in non-compatible ways.
 *  @ingroup init
 */
#define APPLICATION_VERSION_MAJOR                  1
/*! @brief The minor version number of the X11 Borderless Window header.
 *
 *  The minor version number of the X11 Borderless Window header.
 *  This is incremented when features are added to the API but it remains
 *  backward-compatible.
 *  @ingroup init
 */
#define APPLICATION_VERSION_MINOR                  0
/*! @brief The revision number of the X11 Borderless Window header.
 *
 *  The revision number of the X11 Borderless Window header.
 *  This is incremented when a bug fix release is made that does not contain
 *  any API changes.
 *  @ingroup init
 */
#define APPLICATION_VERSION_REVISION               0
/*! @} */

/*! @name X11 Borderless Window version macros (Numerical)
 *  @{ */
/*! @brief The version number (Numerical) of the X11 Borderless Window header.
 *
 *  The version number of the X11 Borderless Window header.
 *  @ingroup init
 */
#define APPLICATION_VERSION_NUM    (APPLICATION_VERSION_MAJOR * 100) + \
                                   (APPLICATION_VERSION_MINOR * 10)  + \
                                   (APPLICATION_VERSION_REVISION)

/*! @name X11 Borderless Window version macros
 *  @{ */
/*! @brief The version number of the X11 Borderless Window header.
 *
 *  The version number of the X11 Borderless Window header.
 *  @ingroup init
 */
#define APPLICATION_VERSION                   "1.0.0"


/*! @name X11 Borderless Window TitleBar macros
 *  @{ */
/*! @brief The titlebar value of the X11 Borderless Window.
 *
 *  X11 Bordered (Windowed) Title Bar
 *  @ingroup init
 */
#define APPLICATION_TITLEBAR     "X11 Borderless Window" \
                               "  v(" APPLICATION_VERSION ")"


/*! @brief BOOL.
 *
 *  This is only unsigned 8bit (byte) intger.  You can instead use `1` or
 *  `true` in True Mode and instead `0` or`false` for False Mode
 *
 *  @ingroup init
 */
typedef unsigned char           BOOL;

/*! @brief One.
 *
 *  This is only semantic sugar for the number 1.  You can instead use `1` or
 *  `true` or `_True` or `GL_TRUE` or `VK_TRUE` or anything else that is equal
 *  to one.
 *
 *  @ingroup init
 */
#define TRUE                        1
/*! @brief Zero.
 *
 *  This is only semantic sugar for the number 0.  You can instead use `0` or
 *  `false` or `_False` or `GL_FALSE` or `VK_FALSE` or anything else that is
 *  equal to zero.
 *
 *  @ingroup init
 */
#define FALSE                       0

#endif
    /* X11_BORDERLESS_WINDOW */
