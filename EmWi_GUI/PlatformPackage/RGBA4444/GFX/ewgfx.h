/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software and related documentation are intellectual property owned by 
* TARA Systems and are copyright of TARA Systems.
* Any copying, reproduction or redistribution of the software in whole or in 
* part by any means not in accordance with the End-User License Agreement for
* Embedded Wizard software is expressly prohibited.
* 
********************************************************************************
*
* DESCRIPTION:
*   Embedded Wizard Graphics Engine provides a platform independent environment
*   for the execution of graphical operations as it is required by applications
*   developped with Embedded Wizard. Following features are available:
*
*   > 3 bitmap formats: Index8, Alpha8 and Native. The Graphics Engine provides
*     a rich set of drawing operations with bitmaps as a destination and source.
*     Bitmaps can be copied, alpha-blended or even perspective correct projected
*     as it is necessary for 3D effects.
*
*     The denomination 'Native' refers to the bitmap format supported natively
*     by the particular target system (e.g. 32 bit RGBA8888). Native bitmaps can
*     serve as the destination and/or as the source for a drawing operation.
*
*     Index8 bitmaps provide a generic format of an 8 bit / pixel bitmap with a
*     color look-up table CLUT. When drawing an Index8 bitmap, its pixel values
*     will be translates through the CLUT in the particular native color format.
*     An advantage of Index8 bitmaps is the posibility to change its colors by
*     modifying the color entries within its CLUT.
*
*     Alpha8 bitmaps provide a generic format of an 8 bit / pixel bitmap with a
*     single alpha (A) channel - no color (RGB) information is stored there.
*     Alpha8 bitmaps are suitable for drawing patterns modulated by externally
*     specified colors. For example: text output.
*
*     In contrast to the Native bitmap format, Index8 and Alpha8 can serve as
*     source only. It is not possible to draw to an Index8 or Alpha8 bitmap.
*
*     An additional feature are 'frames'. Each bitmap can consist of several
*     contents, so called frames. When working with the bitmap, the desired
*     frame needs to be selected. Within a bitmap all frames always have the
*     same size.
*
*   > Rich set of drawing operations: lines, filled rectangles, bitmap copying,
*     3D perspective correct projection and text output. All operations can be
*     performed with or without the alpha-blending. Alpha-blending allows fine
*     fading effects when several graphical objects are drawn one upon an other.
*
*     Additionally all drawn pixel can be modulated by color or opacity values
*     based on linear gradient calculation. In this manner, copied bitmap areas
*     can, for example, be faded out on its edges. A gradient can be vertical,
*     horizontal or both. In the last case for each corner of the gradient a
*     different color value can be set, so 4 colors (or opacity values) can be
*     mixed in the gradient area.
*
*     With 'Warp' operations, Graphics Engine provides 3D perspective correct
*     projection of bitmaps. Beside the 3D effect, this functionality allows 2D
*     scaling and rotating of bitmap areas. For best possible quality, all warp
*     operations are performed with increased sub-pixel precision. Additionally
*     bi-linear filters reduce artefacts, when bitmaps are warped.
*
*   > Resource management: bitmap and fonts can be loaded at the runtime into
*     the Graphics Engine with the objective to display images and output text.
*     The loaded resources remain in the Graphics Engine as long as they are
*     needed. Unused resources also remain in the Graphics Engine as long as
*     there is sufficient memory available. This kind of cache functionality
*     reduces the effort for repeated reload of frequently used resources.
*
*   > Optimization: before executing drawing operations, Graphics Engine can
*     analyse them in order to optimize the stream of drawing instructions. In
*     this manner superfluous drawing operations are detected and eliminated.
*     If possible, the order of operations is changed in order to reduce the
*     synchronization overhead between the hardware accelerator and software
*     drawing routines.
*
*   > Platform independency: The entire Graphics Engine is platform independent.
*     It handles the complex drawing operations and generates from them a stream
*     of primitive instructions destined for the particular graphics subsystem.
*
*     Graphics functionality, which is not supported by the underlying graphics
*     subsystem will be emulated by Graphics Engine's own software driver. In
*     this manner the entire range of drawing operations is always guaranteed
*     for all Embedded Wizard generated GUI applications.
*
*     The integration with the underlying (native) graphics subsystem will take
*     place through a well defined interface of external functions. Except few
*     of them, the most of these functions are optional - if a function is not
*     implemented, the corresponding functionality will be emulated as described
*     above.
*
*     Due to the wide range of possible graphical operations and the modes, the
*     interface to the underlying graphics subsystem is scalable. For example,
*     if the graphics hardware supports only one of the different copy modes,
*     this special mode can be redirected to the hardware while all other modes
*     continue using the software emulation.
*
*     Note the possible performance limits, when software emulation is used.
*
*   This is the public header file of the Embedded Wizard Graphics Engine 
*   EWGFX. It declares the top level API of the engine:
*
*   > Bitmap and font management. Creation and destroying of bitmaps, loading
*     of bitmap and font resources.
*
*   > Drawing operations. Functions to perform all kinds of drawing operations
*     like drawing lines, attributed text, bitmaps, etc.
*
*   > Text, flow text and attributed text processing and size calculation.
*     The attributed text provides a powerful way to layout and output strings
*     with different fonts, embedded images, etc. Multiline text can be drawn
*     with automatic line wraps, soft-hyphen, etc.
*
*******************************************************************************/

#ifndef EWGFX_H
#define EWGFX_H


/* Include platform dependent bitmap and font resource provider declarations */
#include "ewextbmp.h"
#include "ewextfnt.h"
#include "ewgfxdriver.h"
#include "ewgfxcore.h"


#ifdef __cplusplus
  extern "C" 
  {
#endif

/* The current version of the Runtime Environment. */
#define EW_GFX_VERSION 0x0007000A


/******************************************************************************
* TYPE: 
*   EwFullScreenUpdate
*   EwPreserveFramebufferContent
*
* DESCRIPTION:
*   The following variables reflect the current screen update mode as it is
*   predetermined by the used graphics subsystem. The variables are used
*   internally by the Mosaic 2.0 framework only. You never should evaluate
*   or modify these variables directly.
*
******************************************************************************/
extern int EwFullScreenUpdate;
extern int EwPreserveFramebufferContent;


/******************************************************************************
* TYPE: 
*   XBitmapFrame
*
* DESCRIPTION:
*   The structure XBitmapFrame describes the location and an optional opaque
*   area of a single bitmap frame.
*
* ELEMENTS:
*   Surface    - Reference to the low-level surface, where the frame pixel are
*     stored.
*   Origin     - Frame origin relative to the top-left corner of the surface.
*   OpaqueRect - Optional opaque area relative to the top-left corner of the
*     frame. Empty rectangle, if no opaque area is available in the bitmap
*     frame.
*
******************************************************************************/
typedef struct
{ 
  struct XSurface*  Surface;
  XPoint            Origin;
  XRect             OpaqueRect;
} XBitmapFrame;


/******************************************************************************
* TYPE: 
*   XBitmap
*
* DESCRIPTION:
*   The structure XBitmap describes the attributes of a bitmap. XBitmap serves
*   as the public and abstract representation of a surface.
*
* ELEMENTS:
*   Format       - Pixel format of the bitmap. (See EW_PIXEL_FORMAT_XXX).
*   FrameSize    - Size of a single frame within the bitmap.
*   FrameDelay   - Delay in milliseconds for animated bitmaps. If no animation
*     is specified for the bitmap, the value == 0.
*   NoOfFrames   - Number of frames, this bitmap resources consists of.
*   Frames       - Pointer to an array containing the attributes of all frames.
*     At least one frame is available.
*   NoOfSurfaces - Number of low-level surfaces to store the pixel data of the
*     bitmap frames.
*   Surfaces     - Pointer to an array containing references to low-level
*     surfaces with the bitmap pixel data.
*
******************************************************************************/
typedef struct
{ 
  XInt32              Format;
  XPoint              FrameSize;
  XInt32              FrameDelay;
  XInt32              NoOfFrames;
  XBitmapFrame*       Frames;
  XInt32              NoOfSurfaces;
  struct XSurface**   Surfaces;
} XBitmap;


/******************************************************************************
* TYPE: 
*   XFont
*
* DESCRIPTION:
*   The structure XFont describes the attributes of a loaded font resource.
*   XFont serves as the public and abstract representation of a font.
*
* ELEMENTS:
*   Ascent   - Ascent of the font (means the area above the baseline).
*   Descent  - Descent of the font (means the area below the baseline).
*   Leading  - Leading of the font (additional distance between two rows).
*   DefChar  - Default character code.
*   Tag      - Unique identification of the font. Used to find glyphs within
*     the glyph cache.
*   Handle   - Reference to the opened font resource. Used to find and load
*     glyphs.
*
******************************************************************************/
typedef struct
{
  XInt32            Ascent;
  XInt32            Descent;
  XInt32            Leading;
  XChar             DefChar;
  unsigned long     Tag;
  unsigned long     Handle;
} XFont;


/******************************************************************************
* TYPE: 
*   XGlyphMetrics
*
* DESCRIPTION:
*   The structure XGlyphMetrics covers the metrics of a single glyph.
*
* ELEMENTS:
*   Size    - Size of the glyph in pixel (means the size of the ink-box).
*   Origin  - Upper left corner of the glyph, relative to the current printing
*     position on the baseline.
*   Advance - Advance in horizontal direction added to the printing position.
*
*******************************************************************************/
typedef struct
{
  XPoint            Size;
  XPoint            Origin;
  XInt32            Advance;
} XGlyphMetrics;


/*******************************************************************************
* TYPE:
*   XBitmapLock
*
* DESCRIPTION:
*   The XBitmapLock structure provides adresses to color planes of a previously
*   locked bitmap. To lock a bitmap, use the function EwLockBitmap(). See the
*   description of this function for more details about locking and the access
*   to the bitmap memory.
*
*   Note, the pixel storage depends on the pixel format. For more details see
*   the 'ewextpxl_XXX.h' file (XXX stands for your target pixel format. e.g.
*   'ewextpxl_RGBA8888.h').
*
* ELEMENTS:
*   Pixel1  - First color plane pointer. The pointer refers to the first pixel
*     of the locked bitmap area.
*   Pitch1X - Distance between two pixel columns of the first color plane in
*     bytes.
*   Pitch1Y - Distance between two pixel rows of the first color plane in
*     bytes.
*   Pixel2  - Pointer to the second (optional/if any) color plane or zero.
*     If not zero, the pointer also refers to the first pixel of the locked
*     bitmap area.
*   Pitch2X - Distance between two pixel columns of the second color plane
*     in bytes or zero if not applicable.
*   Pitch2Y - Distance between two pixel rows of the second color plane in
*     bytes or zero if not applicable.
*
*******************************************************************************/
typedef struct
{
  void*             Pixel1;
  int               Pitch1X;
  int               Pitch1Y;
  void*             Pixel2;
  int               Pitch2X;
  int               Pitch2Y;
} XBitmapLock;



/*******************************************************************************
* TYPE:
*   XGlyphLock
*
* DESCRIPTION:
*   The XGlyphLock structure provides adress to the pixel information of a
*   previously locked glyph. To lock a glyph, use the function EwLockGlyph().
*   See the description of this function for more details about locking and the
*   access to the glyph memory.
*
*   Note, glyphs are stored in the Alpha8 pixel format -> one byte per pixel.
*   The pixel value 0 determines transparent pixel. The 255 full opaque pixel.
*   Values 1 .. 254 determine the semitransparent pixel.
*
* ELEMENTS:
*   Pixel   - Pointer to the glyph pixel memory. The pointer refers to the first
*     pixel of the locked glyph.
*   PitchX  - Distance between two pixel columns. Is always 1 byte.
*   PitchY  - Distance between two pixel rows in bytes.
*
*******************************************************************************/
typedef struct
{
  unsigned char*    Pixel;
  int               PitchX;
  int               PitchY;
} XGlyphLock;


/* Forward declaration */
typedef struct XViewport XViewport;


/*******************************************************************************
* TYPE:
*   XViewportProc
*
* DESCRIPTION:
*  The following type declares a prototype of a user defined function, which
*  can be passed as callback during the initialization of a viewport.
*
*  This callback function will then be called each time the viewport update
*  cycle terminates. In this manner some kind of viewport specific completion
*  can be performed.
*
* ARGUMENTS:
*   aViewport - Viewport which is performing the screen update.
*   aHandle   - Reference to the real viewport, the framebuffer, display, etc.
*    This value depends on the underlying graphics subsystem.
*   aDisplay1,
*   aDisplay2,
*   aDisplay3 - Platform dependent values, where already existing framebuffer
*     or drawing context, etc. are stored. These values are passed during the
*     initialization of the viewport (see EwInitViewport()).
*   aArea     - Area modified by the update relative to the top-left corner of
*     the viewport.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XViewportProc)
(
  XViewport*        aViewport,
  unsigned long     aHandle,
  void*             aDisplay1,
  void*             aDisplay2,
  void*             aDisplay3,
  XRect             aArea
);


/*******************************************************************************
* TYPE:
*   XViewport
*
* DESCRIPTION:
*   The structure XViewport stores the attributes of a viewport. A viewport
*   provides a kind of abstraction of the physical framebuffer or display 
*   where graphical outputs are drawn. 
*
* ELEMENTS:
*   Size       - Size of the viewport in pixel. This is the area where graphics
*     outputs will occur.
*   Bitmap     - Internal bitmap object to cover the access to the framebuffer
*     or to the display where the graphical outputs are drawn.
*   Frames     - The single entry of the bitmap frames array.
*   Surfaces   - The single entry of the bitmap surfaces array.
*   Surface    - Internal surface object to cover the access to the framebuffer
*     or to the display where the graphical outputs are drawn.
*   Valid      - Flag indicating whether the screen update cycle is active. The
*     screen update begins with EwBeginUpdate() and ends with EwEndUpdate(). As
*     long as Valid != 0, the internal bitmap and surface objects are connected
*     to the framebuffer and thay may be used as the destination in all drawing
*     operations.
*   Proc       - Optional callback function to call when viewport update has
*     been done. The callback function can then perform some particular tasks
*     to complete the screen update, etc. If 0, no callback is called.
*   Display1,
*   Display2,
*   Display3   - Platform dependent values, where already existing framebuffer
*     or drawing context, etc. are stored. These values are passed during the
*     initialization of the viewport (see EwInitViewport()).
*   Handle     - Reference to the real viewport, the framebuffer, display, etc.
*    This value depends on the underlying graphics subsystem.
*
*******************************************************************************/
struct XViewport
{
  XPoint            Size;
  XBitmap           Bitmap;
  XBitmapFrame      Frames;
  XSurface*         Surfaces;
  XSurface          Surface;
  XBool             Valid;
  XViewportProc     Proc;
  void*             Display1;
  void*             Display2;
  void*             Display3;
  unsigned long     Handle;
};


/*******************************************************************************
* TYPE:
*   XAttrSet
*
* DESCRIPTION:
*   The structure XAttrSet stores a set of fonts, bitmaps and colors used while
*   parsing and drawing of attributed strings. The set is a container with a
*   fixed, predefined number of resource and color entries. The desired size of
*   the set should be specified in the function call EwCreateAttrSet().
*
* ELEMENTS:
*   NoOfFonts   - Max. number of fonts.
*   NoOfBitmaps - Max. number of bitmaps.
*   NoOfColors  - Max. number of colors.
*   Fonts       - Pointer to the list of fonts.
*   Bitmaps     - Pointer to the list of bitmaps.
*   Colors      - Pointer to the list of colors.
*
*******************************************************************************/
typedef struct
{
  XInt32            NoOfFonts;
  XInt32            NoOfBitmaps;
  XInt32            NoOfColors;
  XFont**           Fonts;
  XBitmap**         Bitmaps;
  XColor*           Colors;
} XAttrSet;


/*******************************************************************************
* TYPE:
*   XAttrLink
*
* DESCRIPTION:
*   The structure XAttrLink describes a single link stored within an already
*   parsed, preprocessed attributed string. 
*
* ELEMENTS:
*   Data        - Pointer to the link statement within the attributed string.
*   NoOfRegions - Number of rectangular areas, the link consists of.
*   X, Y        - Offset to the left/top origin of the group statement 
*     containing the link.
*
*******************************************************************************/
typedef struct
{
  unsigned short*   Data;
  XInt32            NoOfRegions;
  XInt32            X;
  XInt32            Y;
} XAttrLink;


/*******************************************************************************
* TYPE:
*   XAttrString
*
* DESCRIPTION:
*   The structure XAttrString is used to store the content of an already parsed,
*   preprocessed attributed string. It consists of a lot of drawing statements,
*   which describe how the attributed string should be drawn on the screen.
*   These drawing statements are for internal use only. They are binary coded
*   and optimized for fast drawing operations.
*
*   Before an attributed string can be drawn on the screen, it has to be parsed
*   (converted) into this internal attributed string format. This is done by the
*   function EwParseAttrString().
*
* ELEMENTS:
*   Size      - Length of the parsed, preprocessed attributed string in words.
*   Data      - Pointer to the first statement within the attributed string.
*   NoOfLinks - Number of links defined within the attributed string.
*   Links     - Pointer to a table with the links.
*
*******************************************************************************/
typedef struct
{
  XInt32            Size;
  unsigned short*   Data;
  XInt32            NoOfLinks;
  XAttrLink*        Links;
} XAttrString;


/*******************************************************************************
* FUNCTION:
*   EwInitGraphicsEngine
*
* DESCRIPTION:
*   The function EwInitGraphicsEngine() initializes the Graphics Engine.
*
* ARGUMENTS:
*   aArgs - Optional argument to pass to the init function of the underlying 
*     platform specific adaptation layer.
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int EwInitGraphicsEngine
(
  void*             aArgs
);


/*******************************************************************************
* FUNCTION:
*   EwDoneGraphicsEngine
*
* DESCRIPTION:
*   The function EwDoneGraphicsEngine() deinitializes the Graphics Engine. If
*   necessary, still existing resources will be freed.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDoneGraphicsEngine
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwCreateBitmap
*
* DESCRIPTION:
*   The function EwCreateBitmap() creates a new bitmap with the given size and
*   color format. If no more used, the bitmap should be freed by calling the
*   function EwFreeBitmap().
*
* ARGUMENTS:
*   aFormat     - Color format of the bitmap. (See EW_PIXEL_FORMAT_XXX).
*   aFrameSize  - Size of a single bitmap frame.
*   aFrameDelay - Delay in milliseconds for animated bitmaps. If no animation
*     is specified for the bitmap, the value == 0.
*   aNoOfFrames - Number of frames to embedd within the bitmap. At least one
*     frame is created.
*
* RETURN VALUE:
*   The function returns the pointer to the XBitmap structure, if the bitmap
*   was created successfully, otherwise 0.
*
*******************************************************************************/
XBitmap* EwCreateBitmap
(
  int               aFormat,
  XPoint            aFrameSize,
  XInt32            aFrameDelay,
  XInt32            aNoOfFrames
);


/*******************************************************************************
* FUNCTION:
*   EwLoadBitmap
*
* DESCRIPTION:
*   The function EwLoadBitmap() creates a new bitmap and loads it with the
*   content of the passed bitmap resource.
*
* ARGUMENTS:
*   aResource - Descriptor of the bitmap resource to load. The content of this
*     descriptor depends on the particular platform system.
*
* RETURN VALUE:
*   If sucessful, the function returns a new bitmap initialized with the content
*   of the resource. If failed, the function returns null.
*
*******************************************************************************/
XBitmap* EwLoadBitmap
(
  const struct XBmpRes* aResource
);


/*******************************************************************************
* FUNCTION:
*   EwFreeBitmap
*
* DESCRIPTION:
*   The function EwFreeBitmap() frees the given bitmap. Bitmaps may be created
*   by using EwCreateBitmap() or they may be loaded from a bitmap resource by
*   calling the function EwLoadBitmap().
*
* ARGUMENTS:
*   aBitmap - Bitmap to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeBitmap
(
  XBitmap*          aBitmap
);


/*******************************************************************************
* FUNCTION:
*   EwLockBitmap
*
* DESCRIPTION:
*   The function EwLockBitmap() provides a direct access to the pixel memory of
*   the given bitmap. The function returns a lock object containing pointers to
*   memory, where the caller can read/write the bitmap pixel values. Additional
*   pitch values also returned in the lock object allow the caller to calculate
*   the desired pixel addresses.
*
*   When finished the access cycle, the function EwUnlockBitmap() should be used
*   in order to release the lock, update the affected bitmap, flush CPU caches,
*   etc.
*
*   The memory returned by the function is not guaranteed to be the real video
*   memory of the bitmap. If necessary, the function will handle the access by
*   using a shadow memory area. Therefore to limit the effort of memory copy
*   operations, the desired bitmap area and the access mode should be specified.
*   Note the three parameters aArea, aRead and aWrite. Depending on the graphics
*   sub-system these three parameters may affect significantly the performance.
*
*   If there was not possible to lock the surface, or the desired access mode
*   is just not supported by the underlying graphics sub-system, the function
*   fails and returns zero. (e.g. OpenGL based sub-systems usually allow the
*   write access to bitmaps (textures) only. Read access may fail in this case)
*
*   Note, the pixel storage depends on the pixel format. For more details see
*   the 'ewextpxl_XXX.h' file (XXX stands for your target pixel format. e.g.
*   'ewextpxl_RGBA8888.h').
*
* ARGUMENTS:
*   aBitmap  - Bitmap to obtain the direct memory access.
*   aFrameNo - Frame within the destination bitmap affected by the access
*     operation.
*   aArea    - Area within the frame affected by the access operation (Relative
*     to the top-left corner of the bitmap frame). This is the area, the caller
*     intends to read/write the pixel data.
*   aRead    - Is != 0, if the caller intends to read the pixel information
*     from the bitmap memory. If == 0, the memory content may remain undefined
*     depending on the underlying graphics sub-system and its video-memory
*     management.
*   aWrite   - Is != 0, if the caller intends to overwrite the pixel information
*     within the bitmap memory. If == 0, any modifications within the memory may
*     remain ignored depending on the underlying graphics sub-system and its
*     video-memory management.
*
* RETURN VALUE:
*   If successful, the function returns a temporary bitmap lock object. If the
*   intended access is not possible or not supported by the underlying graphics
*   sub-system, 0 is returned.
*
*******************************************************************************/
XBitmapLock* EwLockBitmap
(
  XBitmap*          aBitmap,
  XInt32            aFrameNo,
  XRect             aArea,
  XBool             aRead,
  XBool             aWrite
);


/*******************************************************************************
* FUNCTION:
*   EwUnlockBitmap
*
* DESCRIPTION:
*   The function EwUnlockBitmap() provides a counterpart to EwLockBitmap(). When
*   called, the function releases the given lock and if necessary, transfers the
*   memory modifications back to the video memory of the bitmap and flushes the
*   CPU caches.
*
*   After calling this function, the aLock structure will become invalid.
*
* ARGUMENTS:
*   aLock - Lock object returned by the previous EwLockBitmap() call.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwUnlockBitmap
(
  XBitmapLock*      aLock
);


/*******************************************************************************
* FUNCTION:
*   EwModifyBitmapPalette
*
* DESCRIPTION:
*   The function EwModifyBitmapPalette() changes color values within the palette
*   of the given Index8 bitmap. If applied on a non Index8 bitmap, the function
*   will ignore the operation.
*
*   In case of a multi-frame bitmap, the modification affects all frames within
*   this bitmap.
*
* ARGUMENTS:
*   aBitmap - Index8 bitmap to modify its palette.
*   aIndex  - Number of the first palette entry affected by the modification.
*   aCount  - Number of entries to modify.
*   aColors - Array with aCount colors to store within the palette starting
*     with palette entry aIndex.
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwModifyBitmapPalette
(
  XBitmap*          aBitmap,
  XInt32            aIndex,
  XInt32            aCount,
  XColor*           aColors
);


/*******************************************************************************
* FUNCTION:
*   EwFlushBitmap
*
* DESCRIPTION:
*   The function EwFlushBitmap() has the job to flush any outstanding drawing
*   Graphics Engine operations for this bitmap as destination.
*
* ARGUMENTS:
*   aBitmap - Bitmap to flush drawing operations.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFlushBitmap
(
  XBitmap*          aBitmap
);


/*******************************************************************************
* FUNCTION:
*   EwLoadFont
*
* DESCRIPTION:
*   The function EwLoadFont() creates a new font and loads it with the content
*   of the passed font resource.
*
* ARGUMENTS:
*   aResource - Descriptor of the font resource to load. The content of this
*     descriptor depends on the particular platform system.
*
* RETURN VALUE:
*   If sucessful, the function returns a new font initialized with the content
*   of the resource. If failed, the function returns null.
*
*******************************************************************************/
XFont* EwLoadFont
(
  const struct XFntRes* aResource
);


/*******************************************************************************
* FUNCTION:
*   EwFreeFont
*
* DESCRIPTION:
*   The function EwFreeFont() frees the given font. Fonts can be loaded from a
*   font resource by calling the function EwLoadFont().
*
* ARGUMENTS:
*   aFont - Font to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeFont
(
  XFont*            aFont
);


/*******************************************************************************
* FUNCTION:
*   EwInitViewport
*
* DESCRIPTION:
*   The function EwInitViewport() prepares a new viewport. Viewports provide a
*   kind of abstraction of the physical framebuffer or display where graphical
*   outputs are drawn. Depending on the particular target system there are two
*   mayor approaches how Graphics Engine can deal with a framebuffer:
*
*   1. The creation and the initialization of the framebuffer are perfomed by
*      the Graphics Engine in the particular platform adaptation layer. In this
*      case Graphics Engine maintains the full control and the ownership over
*      the framebuffer. The Graphics Engine can show, hide and reconfigure the
*      framebuffer every time. Therefore the viewport API provides functions
*      which allow the main software or other external software parts to change
*      the framebuffer configuration - in a platform independent way.
*
*      In this use case, the arguments passed to EwInitViewport() are used to
*      create and configure a new framebuffer and finally to arrange it on the
*      display. These configuration can be changed later by EwConfigViewport().
*
*   2. The framebuffer creation and its initialization are tasks of the main
*      software, external frameworks, the operating system, etc. This means,
*      the framebuffer control lies completely beyond the Embedded Wizard. The
*      Graphics Engine is limited to draw into this framebuffer. The ownership
*      and the framebuffer configuration are aspects of the main software or
*      the external framework only.
*
*      In this use case, EwInitViewport() receives a reference to an existing
*      framebuffer or other kind of graphical context where drawing operation
*      will take place and stores it internally. Optionally, EwInitViewport()
*      can create a private, internal off-screen surface which is used as the
*      framebuffer. When screen update is performed, Graphics Engine copies
*      the affected area from the off-screen surface into the framebuffer or
*      display passed to the EwInitViewport() function.
*
*   After a viewport has been initialized, screen updates can be performed. To
*   do this the functions EwBeginUpdate() and EwEndUpdate() are intended. These
*   functions cover the internal aspects of the framebuffer access, the double
*   buffering, V-Sync, etc.
*
*   If unused anymore, viewports should be freed by EwDoneViewport().
*
* ARGUMENTS:
*   aSize     - Size of the viewport in pixel. This is the area where graphics
*     outputs will occur. Depending on the use case, this parameter determines
*     the size of the framebuffer to create or the size of an already existing
*     framebuffer to use.
*   aExtent   - Position and size, where the created framebuffer should be shown
*     on the screen. If the size of aExtent differs from aSize, the framebuffer
*     content should be scaled in order to fit in the aExtent area.
*     Please note: The area is already expressed in coordinates valid within the
*     target display by taking in account all particular configuration aspects
*     like the default rotation of the surface contents.
*   aOrient   - Orientation hint. The meaning of this parameter depends on the
*     target system. For example, OpenGL target uses aOrient to determine the
*     screen rotation in degrees.
*   aOpacity  - Opacity value for the created framebuffer in the range 0 .. 255.
*     0 -> fully transparent. 255 -> fully opaque. 
*   aDisplay1,
*   aDisplay2,
*   aDisplay3 - Platform dependent parameter, where already existing framebuffer
*     or drawing context, etc. are passed.
*   aProc     - Optional callback function to call when viewport update has
*     been done. The callback function can then perform some particular tasks
*     to complete the screen update, etc. If 0, no callback is called.
*
* RETURN VALUE:
*   If successful, the function returns a new viewport otherwise 0 is returned.
*
*******************************************************************************/
XViewport* EwInitViewport
(
  XPoint            aSize,
  XRect             aExtent,
  XInt32            aOrient,
  XInt32            aOpacity,
  void*             aDisplay1,
  void*             aDisplay2,
  void*             aDisplay3,
  XViewportProc     aProc
);


/*******************************************************************************
* FUNCTION:
*   EwDoneViewport
*
* DESCRIPTION:
*   The function EwDoneViewport() deinitializes the given viewport. If necessary
*   the previously created framebuffers are hidden and video memory is released.
*
* ARGUMENTS:
*   aViewport - Viewport to deinitialize.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDoneViewport
(
  XViewport*        aViewport
);


/*******************************************************************************
* FUNCTION:
*   EwConfigViewport
*
* DESCRIPTION:
*   The function EwConfigViewport() changes the configuration of the viewport.
*   Generally, the function can modify the settings of framebuffers belonging
*   to the viewport.
*
* ARGUMENTS:
*   aViewport - Viewport to reconfigure its framebuffer.
*   aPos      - Position, where the framebuffer should be shown on the screen.
*     Please note: The value is already expressed in coordinates valid within
*     the target display by taking in account all particular configuration 
*     aspects like the default rotation of the surface contents.
*   aOpacity  - Opacity value for the framebuffer in the range 0 .. 255.
*     0 -> fully transparent. 255 -> fully opaque. 
*
* RETURN VALUE:
*   If successful, the function returns != 0. If the used framebuffer doesn't
*   belong to the viewport, or if it doesn't support the parameter modification
*   0 is returned.
*
*******************************************************************************/
int EwConfigViewport
(
  XViewport*        aViewport,
  XPoint            aPos,
  XInt32            aOpacity
);


/*******************************************************************************
* FUNCTION:
*   EwBeginUpdate
*
* DESCRIPTION:
*   The function EwBeginUpdate() initiates the screen update cycle for the given
*   viewport and provides access to its framebuffer via the returned temporary
*   bitmap.
*
*   The returned bitmap covers the framebuffer and can serve as the destination
*   in all drawing operations until the function EwEndUpdate() is called. Due to
*   the platform specific framebuffer limitations the returned bitmap may not be
*   used as source in the drawing operations.
*
*   The bitmap remains valid until EwEndUpdate() is called. Afterwards it may
*   not be used.
*
* ARGUMENTS:
*   aViewport - Viewport to begin the drawing operations.
*
* RETURN VALUE:
*   If successful, the function returns a temporary bitmap providing the access
*   to the framebuffer of the viewport.
*
*******************************************************************************/
XBitmap* EwBeginUpdate
(
  XViewport*        aViewport
);


/*******************************************************************************
* FUNCTION:
*   EwEndUpdate
*
* DESCRIPTION:
*   The function EwEndUpdate() finalizes the screen update cycle for the given
*   viewport. This function flushes all outstanding drawing operations, updates
*   the framebuffer state and frees the temporary bitmap object provided by the
*   previous EwBeginUpdate() function call.
*
*   Depending on the particular platform double buffering is used or the update
*   is synchronized with the V-Sync.
*
*   If there was a callback function specified during the initialization of the
*   viewport, the function is called at the fin of the update cycle.
*
* ARGUMENTS:
*   aViewport - Viewport to finalize the drawing operations.
*   aArea     - Area modified by the update relative to the top-left corner of
*     the viewport.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEndUpdate
(
  XViewport*        aViewport,
  XRect             aArea
);


/*******************************************************************************
* FUNCTION:
*   EwFillRectangle
*
* DESCRIPTION:
*   The function EwFillRectangle() fills the rectangular area aDstRect of the
*   bitmap aDst with a color gradient specified by the four color parameters
*   aColorTL .. aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the color gradient (Relative to the top-
*     left corner of the destination bitmap frame).
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRectangle
(
  XBitmap*          aDst,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwCopyBitmap
*
* DESCRIPTION:
*   The function EwCopyBitmap() copies a rectangular area from the bitmap aSrc
*   to the bitmap aDst. The areas affected by this operation are determined by
*   the both arguments aDstRect and aSrcPos. Optionally the copied pixel can be
*   modulated by a color gradient specified by the four parameters aColorTL ..
*   aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aSrc        - Source bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aSrcFrameNo - Source bitmap frame affected by the operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the copied pixel (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcPos     - Origin of the area to copy from the source bitmap (Relative
*     to the top-left corner of the source bitmap frame). The size of the 
*     source area corresponds to the size of the destination area as it is 
*     specified in aDstRect.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyBitmap
(
  XBitmap*          aDst,
  XBitmap*          aSrc,
  XInt32            aDstFrameNo,
  XInt32            aSrcFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XPoint            aSrcPos,
  XColor            aColorTL,
  XColor            aColorTR, 
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwTileBitmap
*
* DESCRIPTION:
*   The function EwTileBitmap() fills the rectangular area aDstRect of the aDst
*   bitmap with multiple copies of an area aSrcRect from the bitmap aSrc. The
*   copied pixel can optionally be modulated by a color gradient specified by
*   the four color parameters aColorTL .. aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aSrc        - Source bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aSrcFrameNo - Source bitmap frame affected by the operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the copied pixel (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcRect    - Area to copy from the source bitmap (Relative to the top-left
*     corner of the source bitmap frame).
*   aSrcPos     - Position within the source area to start the operation. This
*     is an offset, which allows the scrolling of the drawn content within the
*     destination area. The source position has to lie within the aSrcRect.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwTileBitmap
(
  XBitmap*          aDst,
  XBitmap*          aSrc,
  XInt32            aDstFrameNo,
  XInt32            aSrcFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XRect             aSrcRect,
  XPoint            aSrcPos,
  XColor            aColorTL,
  XColor            aColorTR, 
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwWarpBitmap
*
* DESCRIPTION:
*   The function EwWarpBitmap() performs the projection of a rectangular source
*   bitmap area on a 4 corner polygon within the destination bitmap. The kind
*   of transformation is predetermined by the given polygon coordinates. That
*   way scaling, rotating, mirroring and even 3D perspective warp effects can
*   be performed. In case of a perspective correct projection, the coefficients
*   aW1 .. aW4 controls the perspective warp.
*
*   Please note that only convex polygons can be drawn with this technique.
*   Drawing non convex polygons can produce unexpected outputs depending on the
*   underlying graphics subsystem or the 3D hardware engine.
*
*   When performing the projection, the function assumes that the 4 corners of
*   of the source bitmap area are mapped to the four corners of the destination
*   polygon. The top-left source corner corresponds to the first polygon corner.
*   The top-right to the second, bottom-right to the third and the bottom-left
*   to the fourth.
*
*   The copied pixel can optionally be modulated by a color gradient specified
*   by the four color parameters aColor1 .. aColor4.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aSrc        - Source bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aSrcFrameNo - Source bitmap frame affected by the operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstX1, 
*   aDstY1,
*   aDstW1,
*   ...
*   aDstX4,
*   aDstY4,
*   aDstW4      - Coordinates of the polygon to fill with the source bitmap as
*     floating point values for sub-pixel precision (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcRect    - Area to copy from the source bitmap (Relative to the top-left
*     corner of the source bitmap frame).
*   aColor1,
*   aColor2,
*   aColor3,
*   aColor4     - Color values corresponding to the four corners of the polygon.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*   aFilter     - If != 0, the source bitmap pixel will be bi-linear filtered
*     in order to get better output.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpBitmap
(
  XBitmap*          aDst,
  XBitmap*          aSrc,
  XInt32            aDstFrameNo,
  XInt32            aSrcFrameNo,
  XRect             aClipRect,
  XFloat            aDstX1,
  XFloat            aDstY1,
  XFloat            aDstW1,
  XFloat            aDstX2,
  XFloat            aDstY2,
  XFloat            aDstW2,
  XFloat            aDstX3,
  XFloat            aDstY3,
  XFloat            aDstW3,
  XFloat            aDstX4,
  XFloat            aDstY4,
  XFloat            aDstW4,
  XRect             aSrcRect,
  XColor            aColor1,
  XColor            aColor2, 
  XColor            aColor3,
  XColor            aColor4,
  XBool             aBlend,
  XBool             aFilter
);


/*******************************************************************************
* FUNCTION:
*   EwDrawLine
*
* DESCRIPTION:
*   The function EwDrawLine() draws a line between the coordinates aDstPos1 and
*   aDstPos2 within the destination bitmap aDst. The line will be drawn with a
*   color gradient specified by the both color values aColor1 and aColor2.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstPos1,
*   aDstPos2    - Start and end coordinates of the line (Relative to the top-
*     left corner of the destination bitmap frame).
*   aColor1,
*   aColor2     - Color values corresponding to the start and end of the line.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawLine
(
  XBitmap*          aDst,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XPoint            aDstPos1,
  XPoint            aDstPos2,
  XColor            aColor1,
  XColor            aColor2,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwDrawText
*
* DESCRIPTION:
*   The function EwDrawText() prints a single text row within the destination
*   bitmap aDst. The font and the characters to draw are specified in aFont and
*   aString. The area, where the text will be drawn is defined by aDstRect and
*   by aSrcPos. The copied pixel are modulated by a color gradient specified by
*   the four color parameters aColorTL .. aColorBL.
*
*   The parameter aMinWidth controls optionally the minimum width of the text to
*   draw. If applied, the function extends white space characters in order to
*   stretch the text. By using this parameter justified text output is possible.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aFont       - Font to draw the glyphs.
*   aString     - Text to process.
*   aCount      - Maximum number of text characters to draw or -1 if the entire
*     text should be drawn.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the text glyphs (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcPos     - Base line origin (relative to the top-left corner of the text
*     area).
*   aMinWidth   - Minimum width of the text to draw. If text width is less than
*     aMinWidth, white space characters will be stretched automatically.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawText
(
  XBitmap*          aDst,
  XFont*            aFont,
  XChar*            aString,
  XInt32            aCount,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XPoint            aSrcPos,
  XInt32            aMinWidth,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwGetTextExtent
*
* DESCRIPTION:
*   The function EwGetTextExtent() calculates an area required to draw the text
*   aString with the font aFont. The size of the resulting rectangle determines
*   the space where character glyphs will be drawn. The origin of the rectangle
*   specifies the distance between a print position and the first drawn glyph.
*
*   The origin of the calculated area can be passed as aSrcPos to the function
*   EwDrawText(), if seamless text output relative to the top-left corner of
*   the destination area is desired.
*
* ARGUMENTS:
*   aFont   - Font to use for the text extent calculation.
*   aString - Text to process.
*   aCount  - Maximum number of text characters to evaluate or -1 if the entire
*     text should be evaluated.
*
* RETURN VALUE:
*   Returns the rectangular area needed to display the string.
*
*******************************************************************************/
XRect EwGetTextExtent
(
  XFont*            aFont,
  XChar*            aString,
  XInt32            aCount
);


/*******************************************************************************
* FUNCTION:
*   EwGetTextAdvance
*
* DESCRIPTION:
*   The function EwGetTextAdvance() calculates the advance of the entire text
*   aString with the font aFont. The value is calculated by the sum of advance
*   values of glyphs to display the text.
*
* ARGUMENTS:
*   aFont   - Font to use for the text calculation.
*   aString - Text to process.
*   aCount  - Maximum number of text characters to evaluate or -1 if the entire
*     text should be evaluated.
*
* RETURN VALUE:
*   Returns the advance width of the text to display.
*
*******************************************************************************/
XInt32 EwGetTextAdvance
(
  XFont*           aFont,
  XChar*           aString,
  XInt32           aCount
);


/*******************************************************************************
* FUNCTION:
*   EwGetGlyphMetrics
*
* DESCRIPTION:
*   The function EwGetGlyphMetrics() determines the glyph metrics of the given
*   font glyph. The function is useful when special application dependent text
*   flow calculations are necessary. The caller can determine the size and the
*   position of each glyph within the text.
*
* ARGUMENTS:
*   aFont     - Font to get the glyph metrics.
*   aCharCode - Character code of the glyph.
*   aMetrics  - Pointer to the structure where the metrics should be returned.
*
* RETURN VALUE:
*   If successful, the function returns != 0. When the desired glyph doesn't
*   exist in the font and there is no default glyph defined, zero is returned.
*
*******************************************************************************/
int EwGetGlyphMetrics
(
  XFont*            aFont,
  XChar             aCharCode,
  XGlyphMetrics*    aMetrics
);


/*******************************************************************************
* FUNCTION:
*   EwLockGlyph
*
* DESCRIPTION:
*   The function EwLockGlyph() provides a direct access to the pixel memory of
*   the given glyph. The function returns a lock object containing pointers to
*   memory, where the caller can read the glyph pixel values. Additional pitch
*   values also returned in the lock object allow the caller to calculate the
*   desired pixel addresses.
*
*   When finished the access cycle, the function EwUnlockGlyph() should be used
*   in order to release the lock.
*
*   Note, glyphs are stored in the Alpha8 pixel format -> one byte per pixel.
*   The pixel value 0 determines transparent pixel. The 255 full opaque pixel.
*   Values 1 .. 254 determine the semitransparent pixel.
*
* ARGUMENTS:
*   aFont     - Font to lock the glyph pixel memory.
*   aCharCode - Character code of the glyph.
*
* RETURN VALUE:
*   If successful, the function returns a temporary glyph lock object. If the
*   intended glyph doesn't exist, 0 is returned.
*
*******************************************************************************/
XGlyphLock* EwLockGlyph
(
  XFont*            aFont,
  XChar             aCharCode
);


/*******************************************************************************
* FUNCTION:
*   EwUnlockGlyph
*
* DESCRIPTION:
*   The function EwUnlockGlyph() provides a counterpart to EwLockGlyph(). When
*   called, the function releases the given lock.
*
*   After calling this function, the aLock structure will become invalid.
*
* ARGUMENTS:
*   aLock - Lock object returned by the previous EwLockGlyph() call.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwUnlockGlyph
(
  XGlyphLock*       aLock
);


/*******************************************************************************
* FUNCTION:
*   EwDrawBitmapFrame
*
* DESCRIPTION:
*   The function EwDrawBitmapFrame() draws a free scalable frame by composing it
*   from up to nine bitmap segments. These segments do correspond to the 4 frame
*   corners, 4 edges and to the frame interior areas. By filling these areas with
*   repeated copies of the bitmap segments, flexible scalable frame is drawn.
*
*   The segments come from the source bitmap aSrc, which for this purpose has to
*   be organized as 3 x 3 matrix of equal sized tiles --> the size of the source
*   area aSrcRect has to a multiple of 3.
*
*   The resulting size of the drawn frame is specified by aDstRect parameter.
*   The aDrawXXX parameters control which edges are drawn. Optionally the copied
*   pixel can be modulated by a color gradient specified by the four parameters
*   aColorTL .. aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aSrc        - Source bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aSrcFrameNo - Source bitmap frame affected by the operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the segments (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcRect    - Area of the source bitmap to use as source for the drawing
*     operation. This area is assumed as composed of 3x3 equal sized tiles.
*     The area lies relative to the top-left corner of the source bitmap frame.
*   aDrawLeftEdge,
*   aDrawTopEdge,
*   aDrawRightEdge,
*   aDrawBottomEdge,
*   aDrawInterior - Flags to control, whether the corresponding area of the
*     frame is drawn or not. Not drawn edges are filled by the interior area.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawBitmapFrame
(
  XBitmap*          aDst,
  XBitmap*          aSrc,
  XInt32            aDstFrameNo,
  XInt32            aSrcFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XRect             aSrcRect,
  XBool             aDrawLeftEdge,
  XBool             aDrawTopEdge,
  XBool             aDrawRightEdge,
  XBool             aDrawBottomEdge,
  XBool             aDrawInterior,
  XColor            aColorTL,
  XColor            aColorTR, 
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwParseFlowString
*
* DESCRIPTION:
*   The function EwParseFlowString() converts the given source string aString
*   into a new string consisting of an array of wrapped text lines. These lines
*   can then be drawn on the screen by using EwDrawText() function, so multiline
*   flow text output is possible.
*
*   The function parses the passed string and tries to wrap it in separate text
*   lines as soon as the text width exceeds the size limits specified in aSize.
*   The last line can lie partially out of this area in vertical direction.
*
*   The automatic line wrap is performed primarily between words. Additionally
*   the 'blind wrap' and 'silent wrap' are possible. These are controlled by
*   the special characters '^' (for the blind wrap) and '~' (for silent wrap).
*
*   The both special characters '^' and '~' are usually ignored and they are not
*   displayed until the line wrap took place at its position. Then the '~' sign
*   is converted into the hyphen '-'. The blind wrap in contrast breaks a line
*   only and remains invisible. These both special characters provide a flexibel
*   way to output flow text. The silent wrap is also possible at the soft-hyphen
*   character \x00AD unicode.
*
*   Beside the automatic text wrap, an explicit linefeed is possible when the
*   '\n' sign has been found in the string.
*
*   In order to be able to output the special characters as regular signs, the
*   character '%' can be applied in front of the affected sign to convert it
*   to a regular sign.
*
*   The returned string contains an array of text lines. These are stored in
*   following structure:
*
*   +-------------+
*   | No of lines |
*   +-------------+-------------+------+     +------+
*   |    Offset   | No of char  | Char | ... | Char | 
*   +-------------+-------------+------+     +------+
*   |    Offset   | No of char  | Char | ... | Char | 
*   +-------------+-------------+------+     +------+
*      ...
*   +-------------+-------------+------+     +------+
*   |    Offset   | No of char  | Char | ... | Char | 
*   +-------------+-------------+------+     +------+
*   |      0      |
*   +-------------+
*
*   The first character contains the number of lines inside the string. Each
*   line contains the offset (in character) to the next line and the number of
*   characters processed from the origin source string, followed by the signs
*   belonging to the line. The lines are not terminated. The entire string is
*   terminated with 0.
*
* ARGUMENTS:
*   aFont     - Pointer to the used font.
*   aString   - String to wrap.
*   aSize     - Area to limit the text.
*
* RETURN VALUE:
*   The function returns a string containing an array of text lines.
*
*   Please note, the returned string is controlled by the Embedded Wizard
*   Runtime Environment EWRTE. If not used anymore, the string is released
*   automatically. If you plan to store the string in a 'C' variable, use the
*   function EwRetainString() to lock the string (see ewrte.h).
*
*******************************************************************************/
XString EwParseFlowString
(
  XFont*            aFont,
  XChar*            aString,
  XPoint            aSize
);


/*******************************************************************************
* FUNCTION:
*   EwCreateAttrSet
*
* DESCRIPTION:
*   The function EwCreateAttrSet() creates and initializes a new XAttrSet. The
*   set is used as a simple container for storing of fonts, bitmaps and colors.
*   These resources are necessary for parsing and drawing of attributed strings. 
*
*   The size of the set (the max. number of entries, it can store) is passed in
*   the arguments aNoOfFonts, aNoOfBitmaps and aNoOfColors. After creation, the
*   size of the set is fixed and can not be changed any more.
*
*   All entries of a newly created set are always initialized with 0. To start
*   working with attributed strings, the set should be loaded with necessary 
*   fonts, bitmaps and colors. The functions, defined below allow an access to
*   these entries.
*
* ARGUMENTS:
*   aNoOfFonts   - Max. number of fonts.
*   aNoOfBitmaps - Max. number of bitmaps.
*   aNoOfColors  - Max. number of colors.
*
* RETURN VALUE:
*   If successful, the function returns a pointer to the newly created set.
*   Otherwise the function returns 0.
*
*******************************************************************************/
XAttrSet* EwCreateAttrSet
(
  XInt32            aNoOfFonts,
  XInt32            aNoOfBitmaps,
  XInt32            aNoOfColors
);


/*******************************************************************************
* FUNCTION:
*   EwFreeAttrSet
*
* DESCRIPTION:
*   The function EwFreeAttrSet() frees the memory occuped by the set. The
*   function frees the memory only - the corresponding fonts, bitmaps are not
*   affected!
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeAttrSet
(
  XAttrSet*         aAttrSet
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrFonts
*
* DESCRIPTION:
*   The function EwGetNoOfAttrFonts() determinate how many fonts can be stored
*   within the given set.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*
* RETURN VALUE:
*   If successful, the function returns the max. number of fonts. Otherwise the
*   function returns 0.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrFonts
(
  XAttrSet*         aAttrSet
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrBitmaps
*
* DESCRIPTION:
*   The function EwGetNoOfAttrBitmaps() determinate how many bitmaps can be 
*   stored within the given set.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*
* RETURN VALUE:
*   If successful, the function returns the max. number of bitmaps. Otherwise
*   the function returns 0.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrBitmaps
(
  XAttrSet*         aAttrSet
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrColors
*
* DESCRIPTION:
*   The function EwGetNoOfAttrColors() determinate how many colors can be stored
*   within the given set.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*
* RETURN VALUE:
*   If successful, the function returns the max. number of colors. Otherwise
*   the function returns 0.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrColors
(
  XAttrSet*         aAttrSet
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrFont
*
* DESCRIPTION:
*   The function EwGetAttrFont() returns the font entry aFontNo from the given
*   set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*   aFontNo  - Number of the affected font entry within the set. The first
*     entry has the number 0, the second 1, and so far.
*
* RETURN VALUE:
*   If successful, the function returns the pointer to the corresponding font
*   object. Otherwise the function returns 0.
*
*******************************************************************************/
XFont* EwGetAttrFont
(
  XAttrSet*         aAttrSet,
  XInt32            aFontNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrBitmap
*
* DESCRIPTION:
*   The function EwGetAttrBitmap() returns the bitmap entry aBitmapNo from the
*   given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet  - Pointer to the set.
*   aBitmapNo - Number of the affected bitmap entry within the set. The first
*     entry has the number 0, the second 1, and so far.
*
* RETURN VALUE:
*   If successful, the function returns the pointer to the corresponding bitmap
*   object. Otherwise the function returns 0.
*
*******************************************************************************/
XBitmap* EwGetAttrBitmap
(
  XAttrSet*         aAttrSet,
  XInt32            aBitmapNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrColor
*
* DESCRIPTION:
*   The function EwGetAttrColor() returns the color entry aColorNo from the
*   given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*   aColorNo - Number of the affected color entry within the set. The first
*     entry has the number 0, the second 1, and so far.
*
* RETURN VALUE:
*   If successful, the function returns the affected color value. Otherwise the
*   function returns 0.
*
*******************************************************************************/
XColor EwGetAttrColor
(
  XAttrSet*         aAttrSet,
  XInt32            aColorNo
);


/*******************************************************************************
* FUNCTION:
*   EwSetAttrFont
*
* DESCRIPTION:
*   The function EwSetAttrFont() assigns new font aFont to the entry aFontNo
*   within the given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*   aFontNo  - Number of the affected font entry. The first entry has the number
*     0, the second 1, and so far.
*   aFont    - Pointer to the font object to be stored within the set. If aFont
*     is 0, the affected font entry is set to zero.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetAttrFont
(
  XAttrSet*         aAttrSet,
  XInt32            aFontNo,
  XFont*            aFont
);


/*******************************************************************************
* FUNCTION:
*   EwSetAttrBitmap
*
* DESCRIPTION:
*   The function EwSetAttrBitmap() assigns new bitmap aBitmap to the entry 
*   aBitmapNo within the given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet  - Pointer to the set.
*   aBitmapNo - Number of the affected bitmap entry. The first entry has the
*     number 0, the second 1, and so far.
*   aBitmap   - Pointer to the bitmap object to be stored within the set. If
*     aBitmap is 0, the affected bitmap entry is set to zero.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetAttrBitmap
(
  XAttrSet*         aAttrSet,
  XInt32            aBitmapNo,
  XBitmap*          aBitmap
);


/*******************************************************************************
* FUNCTION:
*   EwSetAttrColor
*
* DESCRIPTION:
*   The function EwSetAttrColor() assigns new color value aColor to the entry 
*   aColorNo within the given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*   aColorNo - Number of the affected color entry. The first entry has the
*     number 0, the second 1, and so far.
*   aColor   - The color value to store within the set.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetAttrColor
(
  XAttrSet*         aAttrSet,
  XInt32            aColorNo,
  XColor            aColor
);


/*******************************************************************************
* FUNCTION:
*   EwParseAttrString
*
* DESCRIPTION:
*   The function EwParseAttrString() converts the given source string aString
*   into a list of drawing statements. Unlike simple text drawing operations,
*   attributed strings may appear with multiple fonts, colors and images - like
*   HTML.
*
*   The appearance of the string is determinated by the used attributes and by 
*   the passed aAttrSet container. The attributes describe what to do with the
*   text, which font/color should be used or what images should be displayed
*   together with the text. The attributes are always enclosed in '{ .... }'
*   braces. For example, the attribute for color selection looks like this:
*
*     "The text with other {clr1}text color{clr0}."
*
*   The attribute {clr1} forces the Graphics Engine to draw the following text
*   with the color number 1. The value of the desired color is stored in the
*   aAttrSet container. In the same manner a font selection can be applied or
*   an image can be displayed together with the text. The attributed strings 
*   use a small but very powerfull set of attributes. They control the entire
*   layout and the appearance of the displayed text.
*
*   The function EwParseAttrString() returns a memory block containing the
*   preprocessed drawing statements. These statements can then be executed
*   very fast by the EwDrawAttrText() function. In this manner the parsing of
*   the string is done only once - the text can be displayed several times.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set containing fonts, bitmaps and colors for the
*     attributed string.
*   aString  - Source string containing the text and the attributes.
*   aWidth   - Width of the rectangular area used for the text formatting and
*     line wrapping.
*
* RETURN VALUE:
*   If successful, the function returns a pointer to a memory area containing
*   drawing statements. If the parsing is failed, or the memory is exhausted
*   the function returns 0.
*
*******************************************************************************/
XAttrString* EwParseAttrString
(
  XAttrSet*         aAttrSet,
  XChar*            aString,
  XInt32            aWidth
);


/*******************************************************************************
* FUNCTION:
*   EwFreeAttrString
*
* DESCRIPTION:
*   The function EwFreeAttrString() releases the memory area reserved by the
*   function EwParseAttrString() for drawing statements.
*
* ARGUMENTS:
*   aAttrString - Pointer to the attributed string to free the memory.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeAttrString
(
  XAttrString*      aAttrString
);


/*******************************************************************************
* FUNCTION:
*   EwDrawAttrText
*
* DESCRIPTION:
*   The function EwDrawAttrText() executes the drawing statements from the
*   attributed string aAttrString and draws the text and images into the aDst
*   bitmap at the origin (aDstX, aDstY). The drawing area is clipped by the 
*   aClipping rectangle. The necessary fonts, bitmaps and colors are passed in
*   the aAttrSet container.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aAttrSet    - Pointer to the set of fonts, bitmaps and colors.
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString() 
*     function.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the text (Relative to the top-left corner
*     of the destination bitmap frame).
*   aSrcPos     - Text output origin (relative to the top-left corner of the 
*     text area).
*   aOpacityTL,
*   aOpacityTR,
*   aOpacityBR,
*   aOpacityBL  - Opacity values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawAttrText
(
  XBitmap*          aDst, 
  XAttrSet*         aAttrSet,
  XAttrString*      aAttrString,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XPoint            aSrcPos,
  XInt32            aOpacityTL,
  XInt32            aOpacityTR,
  XInt32            aOpacityBR,
  XInt32            aOpacityBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrTextSize
*
* DESCRIPTION:
*   The function EwGetAttrTextSize() calculates the rectangular area necessary
*   to draw the given attributed string. The rectangular area is calculated by 
*   building an union of all drawing statements of the attributed string.
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString() 
*     function.
*
* RETURN VALUE:
*   Returns the size of the rectangular area needed to display the string.
*
*******************************************************************************/
XPoint EwGetAttrTextSize
(
  XAttrString*      aAttrString
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrLinks
*
* DESCRIPTION:
*   The function EwGetNoOfAttrLinks() returns the total number of links stored
*   within the given attributed string aAttrString.
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString() 
*     function.
*
* RETURN VALUE:
*   The function returns the number of links or zero if no links are defined in
*   the given attributed string.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrLinks
(
  XAttrString*      aAttrString
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrLinkName
*
* DESCRIPTION:
*   The function EwGetAttrLinkName() returns the name of a link, which is stored
*   within the given attributed string aAttrString. The number of the desired
*   link is passed in the argument aLinkNo. The first link has the number 0,
*   the second 1, ...
*
*   The total number of available links can be determinated by the call to the
*   function EwGetNoOfAttrLinks().
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString() 
*     function.
*   aLinkNo     - The number of the affected link. The first link has the 
*     number 0, the second 1, ...
*
* RETURN VALUE:
*   The function returns a pointer to the zero terminated string containing the
*   link's name. If the link has no name or the desired link does not exist,
*   null pointer is returned.
*
*   Please note, the returned string is controlled by the Embedded Wizard
*   Runtime Environment EWRTE. If not used anymore, the string is released
*   automatically. If you plan to store the string in a 'C' variable, use the
*   function EwRetainString() to lock the string (see ewrte.h).
*
*******************************************************************************/
XString EwGetAttrLinkName
(
  XAttrString*      aAttrString,
  XInt32            aLinkNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrLinkRegions
*
* DESCRIPTION:
*   The function EwGetNoOfAttrLinkRegions() returns the number of rectangular 
*   text areas enclosed by the given link aLinkNo within the attributed string
*   aAttrString. 
*
*   Due to the line wrap, the text enclosed by a single link can be wrapped in 
*   several text lines, so the link area may become a very complex polygon. To
*   describe this polygon, it is divided in several rectangular areas, one for
*   each text line enclosed by the link. To get the origin and the size of an
*   area, the function EwGetAttrLinkRect() should be used.
*
*   The regions are very useful, if a selection frame or an other kind of 
*   decoration should be drawn together with the link. 
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString() 
*     function.
*   aLinkNo     - Number of the affected link. The first link has the number 0,
*     the second 1, ...
*
* RETURN VALUE:
*   The function returns the number of rectangular areas or zero if the desired
*   link is not defined within the attributed string, or the link does not
*   enclose any text.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrLinkRegions
(
  XAttrString*      aAttrString,
  XInt32            aLinkNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrLinkRect
*
* DESCRIPTION:
*   The function EwGetAttrLinkRect() returns the origin and the size of an area
*   occuped by the link aLinkNo within the attributed string aAttrString.
*
*   Due to the line wrap, the text enclosed by a single link can be wrapped in 
*   several text lines, so the link area may become a very complex polygon. To
*   describe this polygon, it is divided in several rectangular areas, one for
*   each text line enclosed by the link. The number of the desired rectangular
*   area should be passed in the argument aRegionNo. The first region has the 
*   number 0, the second 1, ...
*
*   The total number of available regions can be determinated by the call to the
*   function EwGetNoOfAttrLinkRegions().
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString() 
*     function.
*   aLinkNo     - Number of the affected link. The first link has the number 0,
*     the second 1, ...
*   aRegionNo   - Number of the affected region. The first region has the number
*     0, the second 1, ...
*
* RETURN VALUE:
*   Returns the origin and the size of the area relative to the origin of the
*   drawn attributed text. If the desired link or region do not exist, an empty
*   rectangle is returned.
*
*******************************************************************************/
XRect EwGetAttrLinkRect
(
  XAttrString*      aAttrString,
  XInt32            aLinkNo,
  XInt32            aRegionNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrLinkBaseline
*
* DESCRIPTION:
*   The function EwGetAttrLinkBaseline() returns the vertical offset to the base
*   line of the text enclosed by the link aLinkNo within the attributed string 
*   aAttrString. The base line is used for vertical text alignment.
*
*   Due to the line wrap, the text enclosed by a single link can be wrapped in 
*   several text lines. For each text line a different base line may be used.
*   This function provides you with an access to this base line offset for each
*   text region enclosed by the link. The number of the desired region should be
*   passed in the argument aRegionNo. The first region has the number 0, the 
*   second 1, ...
*
*   The total number of available regions can be determinated by the call to the
*   function EwGetNoOfAttrLinkRegions().
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString() 
*     function.
*   aLinkNo     - Number of the affected link. The first link has the number 0,
*     the second 1, ...
*   aRegionNo   - Number of the affected region. The first region has the number
*     0, the second 1, ...
*
* RETURN VALUE:
*   The function returns the vertical offset to the base line of the affected
*   region relative to the origin of the drawn attributed text. If the desired
*   link or region do not exist, the function returns 0.
*
*******************************************************************************/
XInt32 EwGetAttrLinkBaseline
(
  XAttrString*      aAttrString,
  XInt32            aLinkNo,
  XInt32            aRegionNo
);


/*******************************************************************************
* FUNCTION:
*   EwIndexToColor
*
* DESCRIPTION:
*   The function EwIndexToColor() returns the color entry from the global CLUT
*   (palette).
*
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aIndex - Index of the desired color entry within the global CLUT/palette.
*
* RETURN VALUE:
*   The color value or a transparent color if there is no global CLUT available
*   or the entry index is invalid.
*
*******************************************************************************/
XColor EwIndexToColor
( 
  XInt32            aIndex
);


/*******************************************************************************
* FUNCTION:
*   EwColorToIndex
*
* DESCRIPTION:
*   The function EwColorToIndex() determines an entry within the global CLUT/
*   palette, which corresponds to the given color (nearest color matching).
*
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aColor - Color to be found in the global CLUT/palette.
*
* RETURN VALUE:
*   The determined global CLUT/palette entry in range 0 .. 255. If there is no
*   global CLUT/palette functionality available, the returned value is 0.
*
*******************************************************************************/
XUInt8 EwColorToIndex
( 
  XColor            aColor
);


/*******************************************************************************
* FUNCTION:
*   EwAllocUserColor
*
* DESCRIPTION:
*   The function EwAllocUserColor() tries to store the given user defined color 
*   within an empty area in the global CLUT/palette. User defined colors exist
*   for external applications (native applications) only. In this manner the
*   external application can manage its own/private set of color values.
*   
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aColor - Color to be stored in the global CLUT/palette.
*
* RETURN VALUE:
*   The global CLUT/palette entry, where the color could be stored. If there
*   are no entries available for new colors, 0 is returned.
*
*******************************************************************************/
XUInt8 EwAllocUserColor
( 
  XColor           aColor
);


/*******************************************************************************
* FUNCTION:
*   EwFreeUserColor
*
* DESCRIPTION:
*   The function EwFreeUserColor() removes the user defined color entry from
*   the global CLUT/palette. After this the affected entry remains empty. User
*   defined colors exist for external applications (native applications) only.
*   In this manner the external application can manage its own/private set of
*   color values.
*   
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aIndex - Index of the user color entry within the global CLUT/palette.
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwFreeUserColor
( 
  XInt32            aIndex
);


/*******************************************************************************
* FUNCTION:
*   EwSetUserColor
*
* DESCRIPTION:
*   The function EwSetUserColor() changes the color value of an user defined 
*   color within the global CLUT/palette. User defined colors exist for external
*   applications (native applications) only. In this manner the application can
*   manage its own/private set of color values.
*   
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aIndex - Index of the user color entry within the global CLUT/palette.
*   aColor - New color to be replace the old color value.
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwSetUserColor
( 
  XInt32            aIndex,
  XColor            aColor
);


/*******************************************************************************
* FUNCTION:
*   EwFindUserColor
*
* DESCRIPTION:
*   The function EwFindUserColor() searches the global CLUT/palette for an entry
*   containing exactly the given user defined color. If no corresponding entry
*   could be found, the function returns 0. In this case, the best fit color can
*   be found by using the function EwColorToIndex().
*
*   User defined colors exist for external applications (native applications)
*   only. In this manner the application can manage its own/private set of color
*   values.
*   
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aColor - Color to be found within the user defined color area of the global
*     CLUT/palette.
*
* RETURN VALUE:
*   The global CLUT/palette entry, where the desired color is stored. If no
*   corresponding entry could be found, the function returns 0.
*
*******************************************************************************/
XUInt8 EwFindUserColor
( 
  XColor            aColor
);


/*******************************************************************************
* FUNCTION:
*   EwGetOverlayArea
*
* DESCRIPTION:
*  The function EwGetOverlayArea() is used internaly by the Mosaic framework
*  to obtain the additional area occupied by the overlay image. The Mosaic
*  framework uses the returned area to optimize the screen redrawing. Without
*  this information, the update process would result in an incomplete screen
*  showing only parts of the overlay.
*
*  Please note, this function can't supress the overlay from being shown.
*  It exists only for optimization purpose.
*
* ARGUMENTS:
*   aVisible - If != 0, the function returns the currently visible area of
*     the overlay. If the overlay is hidden, the function will return an
*     empty area.
*   aUpdate  - If != 0, the function returns the current update area of the
*     overlay. The update area results when overlay changes between the
*     visible and hidden state. If no state transition is recorded, an empty
*     area is returned.
*
* RETURN VALUE:
*   The currently visible area or the update area of the overlay. If no area
*   is available, the function returns an empty rectangle.
*
*******************************************************************************/
XRect EwGetOverlayArea
(
  XBool             aVisible,
  XBool             aUpdate
);


#ifdef __cplusplus
  }
#endif

#endif /* EWGFX_H */


/* pba */
