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
*   This is an internal header of the Embedded Wizard Graphics Engine EWGFX.
*
*   The module 'ewgfxdriver' implements the platform independent layer to the
*   software pixel driver. Its functionality provides a kind of reserve in case
*   the underlying graphics subsystem can not handle the necessary operations.
*   For example, if there is no support for index8 surfaces, 'ewgfxdriver' will
*   recompense it by a software emulated index8 surface including all necessary
*   drawing operations.
*
*   This header file provides a generic and platform independent interface to
*   the software pixel driver functionality. The interface consists of two
*   categories of functions:
*
*   1. 'per row' worker functions. These functions build the low-level software
*      pixel driver. They are intended to perform drawing operations optimized
*      for the particular pixel format and the drawing mode. The declarations,
*      any way, are platform independent. 
*
*   2. 'per area' drawing functions. These functions provide the top-level view
*      to the software pixel driver. They are intended to drive the 'per row'
*      worker functions for the affected fill/copy/warp operation. When called,
*      the top-level function will receive a pointer to one of the low-level
*      worker functions. The passed worker function can be used thereupon to
*      perform row-wise the operation without any dependency to the underlying
*      pixel format.
*
*   Additionally, the header file provides generic declarations to simplify 
*   the integration of external graphics subsystem drivers into the Embedded
*   Wizard Graphics Engine.
*
*******************************************************************************/

#ifndef EWGFXDRIVER_H
#define EWGFXDRIVER_H


#ifdef __cplusplus
  extern "C"
  {
#endif


/*******************************************************************************
* MACRO:
*   EW_RED
*   EW_GREEN
*   EW_BLUE
*   EW_ALPHA
*
* DESCRIPTION:
*   The following macros provide a generic way to extract the RGBA color 
*   channels from the RGBA8888 aColor value as it is passed to all driver
*   functions.
*
*   Note, the RGBA8888 color format is the internal, platform independent way
*   to store color values.
*
*******************************************************************************/
#define EW_RED( aColor )   ((( aColor ) >> 0  ) & 0xFF )
#define EW_GREEN( aColor ) ((( aColor ) >> 8  ) & 0xFF )
#define EW_BLUE( aColor )  ((( aColor ) >> 16 ) & 0xFF )
#define EW_ALPHA( aColor ) ((( aColor ) >> 24 ) & 0xFF )


/*******************************************************************************
* MACRO:
*   EW_PIXEL_FORMAT_XXX
*
* DESCRIPTION:
*   The following enumeration defines the 3 fundamental surface formats. They
*   are needed amongst other things to specify the desired pixel format, when
*   creating new surfaces or when accessing surface memory directly.
*
* ELEMENTS:
*   EW_PIXEL_FORMAT_NATIVE - Represents the main pixel format valid within the
*     particular graphics subsystem without any assumptions about the respective
*     color space, color channels, etc. This is the pixel format, the subsystem
*     is working with internally. In this manner the Graphics Engine remains
*     platform and color independent.
*   EW_PIXEL_FORMAT_INDEX8 - Universal 8 bit per pixel CLUT format. This format
*     expects an additional color look-up table for translation between an 8 bit
*     index and the native color value.
*   EW_PIXEL_FORMAT_ALPHA8 - Universal 8 bit per pixel alpha only format. Useful
*     as storage for glyphs, masks, etc.
*
*******************************************************************************/
#define EW_PIXEL_FORMAT_NATIVE  0
#define EW_PIXEL_FORMAT_INDEX8  1
#define EW_PIXEL_FORMAT_ALPHA8  2


/*******************************************************************************
* MACRO:
*   EW_DRIVER_VARIANT_XXX
*
* DESCRIPTION:
*   Following macros specify the basic color formats supported by the Graphics
*   Engine. These are used for verification purpose and plausibility tests.
*
*******************************************************************************/
#define EW_DRIVER_VARIANT_RGBA8888  1
#define EW_DRIVER_VARIANT_RGBA4444  2
#define EW_DRIVER_VARIANT_RGB565A8  3
#define EW_DRIVER_VARIANT_RGB555A8  4
#define EW_DRIVER_VARIANT_YUVA8888  5
#define EW_DRIVER_VARIANT_LUMA44    6
#define EW_DRIVER_VARIANT_INDEX8    11
#define EW_DRIVER_VARIANT_ALPHA8    12


/*******************************************************************************
* VARIABLE:
*   EwPixelDriverVariant
*
* DESCRIPTION:
*   This global variable stores the target basic color format supported by this
*   pixel driver. It is used at the runtime to verify, whether correct pixel
*   driver are linked together with the application.
*
*   This variable can take one of EW_DRIVER_VARIANT_XXX values.
*
*******************************************************************************/
extern const int EwPixelDriverVariant;


/*******************************************************************************
* TYPE:
*   XSurfaceMemory
*
* DESCRIPTION:
*   The XSurfaceMemory structure provides a lightweight storage for adresses to
*   color planes and (if any) to the color table of a previously locked surface.
*   It is used by the software pixel driver to read and modify surface content.
*
* ELEMENTS:
*   Pixel1  - Pointer to the first surface plane.
*   Pitch1X - Distance between two pixel columns of the first surface plane.
*   Pitch1Y - Distance between two pixel rows of the first surface plane.
*   Pixel2  - Pointer to the second (optional) surface plane.
*   Pitch2X - Distance between two pixel columns of the second surface plane.
*   Pitch2Y - Distance between two pixel rows of the second surface plane.
*   Clut    - Pointer to the (optional) color table of the surface. In order
*     to cover all color formats, a single entry of the clut is always 32 bit
*     wide - even when the target color format is 16 or 8 bit wide.
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
  unsigned long*    Clut;
} XSurfaceMemory;


/*******************************************************************************
* TYPE:
*   XGradient
*
* DESCRIPTION:
*   The XGradient structure provides a storage for the components of a 2D color
*   or 2D opacity gradient. It defines a start value and slopes. For precision
*   purpose, the values are codes as 12.20 fixed point numbers.
*
* ELEMENTS:
*   R0, R1, R2, R3 - Startvalues and slopes for the red channel.
*   G0, G1, G2, G3 - Startvalues and slopes for the green channel.
*   B0, B1, B2, B3 - Startvalues and slopes for the blue channel.
*   A0, A1, A2, A3 - Startvalues and slopes for the alpha channel.
*   Width, Height  - Size of the gradient area.
*   InvWidth,
*   InvHeight      - Reciprocal values of the gradient area width and height.
*   IsVertical     - != 0 if the gradient has a vertical trend.
*   IsHorizontal   - != 0 if the gradient has a horizontal trend.
*
*******************************************************************************/
typedef struct
{
  long              R0, R1, R2, R3;
  long              G0, G1, G2, G3;
  long              B0, B1, B2, B3;
  long              A0, A1, A2, A3;
  long              Width;
  long              Height;
  long              InvWidth;
  long              InvHeight;
  int               IsVertical;
  int               IsHorizontal;
} XGradient;


/*******************************************************************************
* PROTOTYPE:
*   XLineWorker
*
* DESCRIPTION:
*   The following type declares a prototype for a worker function, which should
*   perform the low level software 'draw line pixel' operation.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aX, aY    - Position of the pixel to draw.
*   aGradient - Color information to draw the pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XLineWorker)
(
  XSurfaceMemory*   aDst,
  int               aX,
  int               aY,
  XGradient*        aGradient
);


/*******************************************************************************
* PROTOTYPE:
*   XFillWorker
*
* DESCRIPTION:
*   The following type declares a prototype for a worker function, which should
*   perform the low level software 'fill native pixel row' operation.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the affected row.
*   aWidth    - Length of the row in pixel.
*   aGradient - Color information to fill the row.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XFillWorker)
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* PROTOTYPE:
*   XCopyWorker
*
* DESCRIPTION:
*   The following type declares a prototype for a worker function, which should
*   perform the low level software 'copy pixel row' operation.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Length of the row in pixel.
*   aGradient - Color or opacity information to modulate the copied pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XCopyWorker)
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* PROTOTYPE:
*   XWarpWorker
*
* DESCRIPTION:
*   The following type declares a prototype for a worker function, which should
*   perform the low level software 'warp pixel row' operation.
*
* ARGUMENTS:
*   aDst          - Pointer to the first pixel of the destination row.
*   aSrc          - Pointer to the first pixel of the source surface area to
*     project on the destination row.
*   aWidth        - Length of the row in pixel.
*   aS, aT        - Source surface coordinates corresponding to the first pixel
*     of the destination row. 
*   aSS, aTS      - Factors to interpolate the s and t coefficients for next
*     following pixel of the destination row.
*   aSrcWidth,
*   aSrcHeight    - Size of the source area in pixel to limit the operation.
*   aGradient     - Color or opacity information to modulate the copied pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XWarpWorker)
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* PROTOTYPE:
*   XLineDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which should perform the 'draw line' operation by using solid or
*   gradient color values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the native destination surface.
*   aDstX1,
*   aDstY1      - Start position to draw the line (relative to the top-left
*      corner of the destination surface).
*   aDstX2,
*   aDstY2      - End position to draw the line (relative to the top-left
*      corner of the destination surface). The pixel at the end position does
*      not belong to the line - it is invisible. In this manner polylines can
*      be drawn.
*   aClipX,
*   aClipY,
*   aClipWidth,
*   aClipHeight - Optional clipping area relative to the top-left corner of the
*     destination surface. No pixel outside this area may be drawn.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 2 RGBA8888 color values. The both color values do
*     correspond to the start and to the end pixel of the drawn line.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XLineDriver)
(
  unsigned long     aDstHandle,
  int               aDstX1,
  int               aDstY1,
  int               aDstX2,
  int               aDstY2,
  int               aClipX,
  int               aClipY,
  int               aClipWidth,
  int               aClipHeight,
  int               aBlend,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XFillDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which should perform the 'fill rectangular area' operation by
*   using solid or gradient color values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the native destination surface.
*   aDstX,
*   aDstY       - Origin of the area to fill (relative to the top-left corner
*      of the destination surface).
*   aWidth,
*   aHeight     - Size of the area to fill.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 RGBA8888 color values. The four color values do
*     correspond to the four corners of the area: top-left, top-right, bottom-
*     right and bottom-left.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XFillDriver)
(
  unsigned long     aDstHandle,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XCopyDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which should perform the 'copy rectangular area' operation from a
*   native, index8 or alpha8 surface to a native surface. The copied pixel can
*   optionally be modulated by solid or gradient color/opacity values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the native destination surface.
*   aSrcHandle  - Handle to the native/index8/alpha8 source surface.
*   aDstX,
*   aDstY       - Origin of the area to fill with the copied source surface
*     pixel (relative to the top-left corner of the destination surface).
*   aWidth,
*   aHeight     - Size of the area to fill with the copied source surface pixel.
*   aSrcX,
*   aSrcY       - Origin of the area to copy from the source surface.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 color values. These four values do correspond
*     to the four corners of the area: top-left, top-right, bottom-right and 
*     bottom-left.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XCopyDriver)
(
  unsigned long     aDstHandle,
  unsigned long     aSrcHandle,
  int               aDstX,
  int               aDstY,
  int               aSrcX,
  int               aSrcY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XTileDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which will perform the 'multiple copy rectangular area' operation
*   from a native, index8 or alpha8 surface to a native surface. The copied 
*   pixel can be modulated by solid or gradient color/opacity values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the native destination surface.
*   aSrcHandle  - Handle to the native/index8/alpha8 source surface.
*   aDstX,
*   aDstY       - Origin of the area to fill with the copied source surface
*     pixel (relative to the top-left corner of the destination surface).
*   aDstWidth,
*   aDstHeight  - Size of the area to fill with the copied source surface pixel.
*   aSrcX,
*   aSrcY       - Origin of the area to copy from the source surface.
*   aSrcWidth,
*   aSrcHeight  - Size of the source area to copy.
*   aOfsX,
*   aOfsY       - Offset to the first visible source pixel at the origin of the
*     destination area.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 color values. These four values do correspond
*     to the four corners of the area: top-left, top-right, bottom-right and 
*     bottom-left.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XTileDriver)
(
  unsigned long     aDstHandle,
  unsigned long     aSrcHandle,
  int               aDstX,
  int               aDstY,
  int               aDstWidth,
  int               aDstHeight,
  int               aSrcX,
  int               aSrcY,
  int               aSrcWidth,
  int               aSrcHeight,
  int               aOfsX,
  int               aOfsY,
  int               aBlend,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XWarpDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which can perform the 'copy and warp rectangular area' operation
*   from native, index8 or alpha8 surface to a native surface. The copied pixel
*   can optionally be modulated by solid or gradient color/opacity values.
*
*   The warp operation abstracts the scale, rotate and perspective projection
*   operations. The operation is limited to convex polygons only. In case of a
*   non convex polygon, the operation may fail.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the native destination surface.
*   aSrcHandle  - Handle to the native/index8/alpha8 source surface.
*   DstX1, 
*   DstY1,
*   DstW1,
*   ...
*   DstX4,
*   DstY4,
*   DstW4       - Coordinates of the polygon to fill with the source bitmap as
*     floating point values for sub-pixel precision (Relative to the top-left
*     corner of the destination bitmap). The 'W' coefficients do control the
*     perspective distortion as it is realized in 3D graphics hardware (see
*     OpenGL homogeneous coordinates for more information).
*   aSrcX,
*   aSrcY       - Origin of the area to copy from the source surface.
*   aSrcWidth,
*   aSrcHeight  - Size of the source area to copy.
*   aClipX,
*   aClipY,
*   aClipWidth,
*   aClipHeight - Optional clipping area relative to the top-left corner of the
*     destination surface. No pixel outside this area may be drawn.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aFilter     - != 0 if the bi-linear filter should be applied to the copied
*      pixel. This argument is optional and controls the output quality only.
*   aColors     - Array with 4 color values. These four values do correspond to
*     the respective corners of the polygon.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XWarpDriver)
(
  unsigned long     aDstHandle,
  unsigned long     aSrcHandle,
  float             aDstX1,
  float             aDstY1,
  float             aDstW1,
  float             aDstX2,
  float             aDstY2,
  float             aDstW2,
  float             aDstX3,
  float             aDstY3,
  float             aDstW3,
  float             aDstX4,
  float             aDstY4,
  float             aDstW4,
  int               aSrcX,
  int               aSrcY,
  int               aSrcWidth,
  int               aSrcHeight,
  int               aClipX,
  int               aClipY,
  int               aClipWidth,
  int               aClipHeight,
  int               aBlend,
  int               aFilter,
  unsigned long*    aColors
);


/*******************************************************************************
* FUNCTION:
*   EwCreateNativeSurface
*
* DESCRIPTION:
*   The function EwCreateNativeSurface() has the job to create and return a new
*   native surface with the given size. If no surface could be created due to
*   memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function creates a pure software surface
*   within the CPU address space only.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateNativeSurface
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateConstNativeSurface
*
* DESCRIPTION:
*   The function EwCreateConstNativeSurface() has the job to create and return
*   a new native surface with the given size and pixel data content. Important
*   here is the fact that the function doesn't reserve any memory to store the
*   pixel information. Instead it, the surface associates the pixel data passed
*   in the parameter aMemory.
*
*   This function is thus intended to create surfaces from the pixel data stored
*   directly within a ROM area. Accordingly it is essential that the ROM content
*   does exactly correspond to the native surface pixel format. It includes the
*   order of color channels, premultiplication with alpha value, etc..
*
*   If no surface could be created due to memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality.
*
*   Please note, the surfaces created with this function are signed internally
*   as constant. Trying to obtrain write-access to such surface will result in
*   a runtime error.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*   aMemory - Structure to pass the ROM address information.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateConstNativeSurface
(
  int               aWidth,
  int               aHeight,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyNativeSurface
*
* DESCRIPTION:
*   The function EwDestroyNativeSurface() has the job to release resources of
*   a previously created native surface.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function destroys pure software surfaces
*   previously created by the function EwCreateNativeSurface() only.
*
* ARGUMENTS:
*   aSurface - Handle to the surface to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyNativeSurface
(
  unsigned long     aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwGetNativeSurfaceMemory
*
* DESCRIPTION:
*   The function EwGetNativeSurfaceMemory() has the job to obtain pointers to
*   the memory planes and if existing to the color table of the given surface.
*
*   The returned pointers are calculated for the given pixel position aX, aY and
*   in case of an existing color table, to the entry at the position aIndex.
*
*   Please note, if the affected surface has been created with constant pixel
*   information by using EwCreateConstNativeSurface(), you can't obtain the
*   write access to such surface. In such case the function will fail with the
*   return value 0.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function can handle with the pure software
*   surfaces previously created by the function EwCreateNativeSurface() and
*   EwCreateConstNativeSurface() only.
*
* ARGUMENTS:
*   aSurface     - Handle to the surface to obtain the addresses.
*   aX, aY       - Position within the surface to calculate the pixel address.
*   aIndex       - Index within the color table to calculate the CLUT address.
*   aWriteAccess - If != 0 the caller has the intention to modify the returned
*     memory content.
*   aMemory      - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwGetNativeSurfaceMemory
(
  unsigned long     aSurface,
  int               aX,
  int               aY,
  int               aIndex,
  int               aWriteAccess,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwGetNativeSurfaceMemSize
*
* DESCRIPTION:
*   The function EwGetNativeSurfaceMemSize() has the job to determine the number
*   of memory bytes occupied by a native surface�with the given size.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface in pixel.
*
* RETURN VALUE:
*   If successful, the function return the surface size in bytes.
*
*******************************************************************************/
int EwGetNativeSurfaceMemSize
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateConstIndex8Surface
*
* DESCRIPTION:
*   The function EwCreateConstIndex8Surface() has the job to create and return
*   a new index8 surface with the given size and pixel data content. Important
*   here is the fact that the function doesn't reserve any memory to store the
*   pixel information. Instead it, the surface associates the pixel data passed
*   in the parameter aMemory.
*
*   This function is thus intended to create surfaces from the pixel data stored
*   directly within a ROM area. Accordingly it is essential that the ROM content
*   does exactly correspond to the index8 surface pixel format. It includes the
*   order of CLUT color channels, premultiplication with alpha value, etc..
*
*   If no surface could be created due to memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality.
*
*   Please note, the surfaces created with this function are signed internally
*   as constant. Trying to obtrain write-access to such surface will result in
*   a runtime error.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*   aMemory - Structure to pass the ROM address information.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateConstIndex8Surface
(
  int               aWidth,
  int               aHeight,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwCreateIndex8Surface
*
* DESCRIPTION:
*   The function EwCreateIndex8Surface() has the job to create and return a new
*   index8 surface with the given size. If no surface could be created due to
*   memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function creates a pure software surface
*   within the CPU address space only.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateIndex8Surface
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyIndex8Surface
*
* DESCRIPTION:
*   The function EwDestroyIndex8Surface() has the job to release resources of
*   a previously created index8 surface.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function destroys pure software surfaces
*   previously created by the function EwCreateIndex8Surface() only.
*
* ARGUMENTS:
*   aSurface - Handle to the surface to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyIndex8Surface
(
  unsigned long     aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwGetIndex8SurfaceMemory
*
* DESCRIPTION:
*   The function EwGetIndex8SurfaceMemory() has the job to obtain the pointer to
*   the index8 memory plane and to the color table of the given surface.
*
*   The returned pointer is calculated for the given pixel position aX, aY and
*   in case of the color table, to the entry at the position aIndex.
*
*   Please note, if the affected surface has been created with constant pixel
*   information by using EwCreateConstIndex8Surface(), you can't obtain the
*   write access to such surface. In such case the function will fail with the
*   return value 0.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function can handle with the pure software
*   surfaces previously created by the function EwCreateIndex8Surface() and
*   EwCreateConstIndex8Surface() only.
*
* ARGUMENTS:
*   aSurface     - Handle to the surface to obtain the addresses.
*   aX, aY       - Position within the surface to calculate the pixel address.
*   aIndex       - Index within the color table to calculate the CLUT address.
*   aWriteAccess - If != 0 the caller has the intention to modify the returned
*     memory content.
*   aMemory      - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwGetIndex8SurfaceMemory
(
  unsigned long     aSurface,
  int               aX,
  int               aY,
  int               aIndex,
  int               aWriteAccess,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwGetIndex8SurfaceMemSize
*
* DESCRIPTION:
*   The function EwGetIndex8SurfaceMemSize() has the job to determine the number
*   of memory bytes occupied by an index8 surface�with the given size.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface in pixel.
*
* RETURN VALUE:
*   If successful, the function return the surface size in bytes.
*
*******************************************************************************/
int EwGetIndex8SurfaceMemSize
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateAlpha8Surface
*
* DESCRIPTION:
*   The function EwCreateAlpha8Surface() has the job to create and return a new
*   alpha8 surface with the given size. If no surface could be created due to
*   memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function creates a pure software surface
*   within the CPU address space only.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateAlpha8Surface
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateConstAlpha8Surface
*
* DESCRIPTION:
*   The function EwCreateConstAlpha8Surface() has the job to create and return
*   a new alpha8 surface with the given size and pixel data content. Important
*   here is the fact that the function doesn't reserve any memory to store the
*   pixel information. Instead it, the surface associates the pixel data passed
*   in the parameter aMemory.
*
*   This function is thus intended to create surfaces from the pixel data stored
*   directly within a ROM area. Accordingly it is essential that the ROM content
*   does exactly correspond to the alpha8 surface pixel format.
*
*   If no surface could be created due to memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality.
*
*   Please note, the surfaces created with this function are signed internally
*   as constant. Trying to obtrain write-access to such surface will result in
*   a runtime error.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*   aMemory - Structure to pass the ROM address information.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateConstAlpha8Surface
(
  int               aWidth,
  int               aHeight,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyAlpha8Surface
*
* DESCRIPTION:
*   The function EwDestroyAlphaSurface() has the job to release resources of
*   a previously created alpha8 surface.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function destroys pure software surfaces
*   previously created by the function EwCreateAlpha8Surface() only.
*
* ARGUMENTS:
*   aSurface - Handle to the surface to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyAlpha8Surface
(
  unsigned long     aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwGetAlpha8SurfaceMemory
*
* DESCRIPTION:
*   The function EwGetAlpha8SurfaceMemory() has the job to obtain pointers to
*   the memory planes of the given surface.
*
*   The returned pointer is calculated for the given pixel position aX, aY.
*
*   Please note, if the affected surface has been created with constant pixel
*   information by using EwCreateConstAlpha8Surface(), you can't obtain the
*   write access to such surface. In such case the function will fail with the
*   return value 0.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function can handle with the pure software
*   surfaces previously created by the function EwCreateIAlpha8Surface() and
*   EwCreateConstAlpha8Surface() only.
*
* ARGUMENTS:
*   aSurface     - Handle to the surface to obtain the addresses.
*   aX, aY       - Position within the surface to calculate the pixel address.
*   aIndex       - Index within the color table to calculate the CLUT address.
*   aWriteAccess - If != 0 the caller has the intention to modify the returned
*     memory content.
*   aMemory      - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwGetAlpha8SurfaceMemory
(
  unsigned long     aSurface,
  int               aX,
  int               aY,
  int               aIndex,
  int               aWriteAccess,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwGetAlpha8SurfaceMemSize
*
* DESCRIPTION:
*   The function EwGetAlpha8SurfaceMemSize() has the job to determine the number
*   of memory bytes occupied by an alpha8 surface�with the given size.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface in pixel.
*
* RETURN VALUE:
*   If successful, the function return the surface size in bytes.
*
*******************************************************************************/
int EwGetAlpha8SurfaceMemSize
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwInitColorGradient
*
* DESCRIPTION:
*   The function EwInitColorGradient() has the job to initialize a rectangular,
*   linear gradient from 4 color values. The color values do correspond to the
*   four corners of the rectangular gradient: top-left, top-right, bottom-right
*   and bottom-left.
*
* ARGUMENTS:
*   aWidth,
*   aHeight   - Size of the gradient area in pixel.
*   aColors   - Array of 4 color values in the universal RGBA8888 color format.
*   aGradient - Destination gradient structure to initialize.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwInitColorGradient
(
  int               aWidth,
  int               aHeight,
  unsigned long*    aColors,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwInitOpacityGradient
*
* DESCRIPTION:
*   The function EwInitOpacityGradient() has the job to initialize a new linear,
*   rectangular gradient from 4 color values. The color values do correspond to
*   the four corners of the gradient area: top-left, top-right, bottom-right and
*   bottom-left.
*
* ARGUMENTS:
*   aWidth,
*   aHeight   - Size of the gradient area in pixel.
*   aColors   - Array of 4 color values in the universal RGBA8888 color format.
*   aGradient - Destination gradient structure to initialize.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwInitOpacityGradient
(
  int               aWidth,
  int               aHeight,
  unsigned long*    aColors,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwGetColorFromGradient
*
* DESCRIPTION:
*   The function EwGetColorFromGradient() has the job to interpolate the color
*   value for the given position within a color gradient.
*
* ARGUMENTS:
*   aGradient  - Color gradient to get the color.
*   aX, aY     - Position relative to the upper-left corner of the gradient to 
*     get the color value.    
*
* RETURN VALUE:
*   Interpolated color value in the universal RGBA8888 color format.
*
*******************************************************************************/
unsigned long EwGetColorFromGradient
(
  XGradient*        aGradient,
  int               aX,
  int               aY
);


/*******************************************************************************
* FUNCTION:
*   EwGetOpacityFromGradient
*
* DESCRIPTION:
*   The function EwGetOpacityFromGradient() has the job to interpolate the 
*   opacity value for the given position within a color gradient.
*
* ARGUMENTS:
*   aGradient  - Color gradient to get the value.
*   aX, aY     - Position relative to the upper-left corner of the gradient to 
*     get the opacity value.    
*
* RETURN VALUE:
*   Interpolated color value in the universal RGBA8888 color format. Only the
*   alpha channel is valid.
*
*******************************************************************************/
unsigned long EwGetOpacityFromGradient
(
  XGradient*        aGradient,
  int               aX,
  int               aY
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateLine
*
* DESCRIPTION:
*   The function EwEmulateLine() drives a draw line operation by using solid or
*   gradient color values.
*
*   The function provides the top-level interface to the software pixel driver.
*   The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst        - Pointer to the first pixel of the destination surface.
*   aDstX1,
*   aDstY1      - Start position to draw the line (relative to the top-left
*     corner of the destination surface).
*   aDstX2,
*   aDstY2      - End position to draw the line (relative to the top-left
*      corner of the destination surface). The pixel at the end position does
*      not belong to the line - it is invisible. In this manner polylines can
*      be drawn.
*   aClipX1,
*   aClipY1,
*   aClipX2,
*   aClipY2     - Optional clipping area relative to the top-left corner of the
*     destination surface. No pixel outside this area may be drawn.
*   aGradient   - Color information to modulate the drawn pixel.
*   aWorker     - Low-level worker function to perform the operation.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateLine
(
  XSurfaceMemory*   aDst,
  int               aDstX1,
  int               aDstY1,
  int               aDstX2,
  int               aDstY2,
  int               aClipX1,
  int               aClipY1,
  int               aClipX2,
  int               aClipY2,
  XGradient*        aGradient,
  XLineWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateFill
*
* DESCRIPTION:
*   The function EwEmulateFill() drives a fill operation for a rectangular area
*   by using solid or gradient color values.
*
*   The function provides the top-level interface to the software pixel driver.
*   The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aDstX,
*   aDstY     - Origin of the area to fill (relative to the top-left corner of
*     the destination surface).
*   aWidth,
*   aHeight   - Size of the area to fill.
*   aGradient - Color information to modulate the filled pixel.
*   aGrdX,
*   aGrdY     - Origin of the affected area in the gradient coordinate space.
*   aWorker   - Low-level worker function to perform the operation row-wise.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateFill
(
  XSurfaceMemory*   aDst,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  XGradient*        aGradient,
  int               aGrdX,
  int               aGrdY,
  XFillWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateCopy
*
* DESCRIPTION:
*   The function EwEmulateCopy() drives a copy operation for a rectangular area
*   from a native, index8 or alpha8 surface to a native surface. The function
*   modulates the copied pixel by solid or gradient opacity values.
*
*   The function provides the top-level interface to the software pixel driver.
*   The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aSrc      - Pointer to the first pixel of the source surface.
*   aDstX,
*   aDstY     - Origin of the area to fill (relative to the top-left corner of
*     the destination surface).
*   aWidth,
*   aHeight   - Size of the area to fill.
*   aSrcX,
*   aSrcY     - Origin of the source area to copy (relative to the top-left
*     corner of the source surface).
*   aGradient - Color information to modulate the copied pixel.
*   aGrdX,
*   aGrdY     - Origin of the affected area in the gradient coordinate space.
*   aWorker   - Low-level worker function to perform the operation row-wise.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateCopy
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aSrcX,
  int               aSrcY,
  XGradient*        aGradient,
  int               aGrdX,
  int               aGrdY,
  XCopyWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateWarp
*
* DESCRIPTION:
*   The function EwEmulateWarp() drives a warp operation for a rectangular area
*   of a native, index8 or alpha8 surface to a polygon within a native surface.
*   The function modulates the copied pixel by solid or gradient opacity values.
*
*   The function provides the top-level interface to the software pixel driver.
*   The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst        - Pointer to the first pixel of the destination surface.
*   aSrc        - Pointer to the first pixel of the source surface.
*   DstX1, 
*   DstY1,
*   DstW1,
*   ...
*   DstX4,
*   DstY4,
*   DstW4       - Coordinates of the polygon to fill with the source bitmap
*     (Relative to the top-left corner of the destination bitmap). The 'X', 'Y'
*     and 'W' coefficients are specified in 16.16 fixed point format due to the
*     sub-pixel precision.
*   aSrcX,
*   aSrcY       - Origin of the source area to copy (relative to the top-left
*     corner of the source surface).
*   aSrcWidth,
*   aSrcHeight  - Size of the source area to copy and warp.
*   aClipX1,
*   aClipY1,
*   aClipX2,
*   aClipY2     - Optional clipping area relative to the top-left corner of the
*     destination surface. No pixel outside this area may be drawn.
*   aGradient   - Color information to modulate the copied pixel.
*   aWorker     - Low-level worker function to perform the operation row-wise.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateWarp
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aDstX1,
  int               aDstY1,
  int               aDstW1,
  int               aDstX2,
  int               aDstY2,
  int               aDstW2,
  int               aDstX3,
  int               aDstY3,
  int               aDstW3,
  int               aDstX4,
  int               aDstY4,
  int               aDstW4,
  int               aSrcX,
  int               aSrcY,
  int               aSrcWidth,
  int               aSrcHeight,
  int               aClipX1,
  int               aClipY1,
  int               aClipX2,
  int               aClipY2,
  XGradient*        aGradient,
  XWarpWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwPackColor
*
* DESCRIPTION:
*   The following EwPackColor() function has the job to convert the given RGBA
*   color channels into a generic packed 32 bit format as it is used internally
*   by the Graphics Engine.
*
*   The generic packed 32 bit color format provides an abstraction of a 32 bit
*   color value consisting of three color channels and one alpha channel. The
*   content of the three color chanels is a subject of the particular target
*   system - Graphics Engine doesn't need to know anything about the meaning of
*   these channels - they are abstract.
*
*   The generic packed 32 bit color format stores the channels in the following
*   order:
*
*              31           24           16             8            0
*               +------------+---------------------------------------+
*               |   alpha    |  channel 3 |  channel 2  |  channel 1 |
*               +------------+---------------------------------------+
*
*   Depending on the target color format, the function can modify and convert
*   the values of the channels. For example, the RGB -> YUV conversion can be
*   executed or the color channels can be pre-multiplied by the alpha value.
*
*   The resulting 32 bit value is particular for the respective target color
*   format. It is predestined to be used more effectively durring all drawing
*   operations.
*
* ARGUMENTS:
*   aRed,
*   aGreen,
*   aBlue,
*   aAlpha  - Non-premultiplied color channel values in the range 0 .. 255. 
*
* RETURN VALUE:
*   Packed 32 bit color value. Particular for the target color format.
*
*******************************************************************************/
unsigned long EwPackColor
(
  long              aRed,
  long              aGreen,
  long              aBlue,
  long              aAlpha
);


/*******************************************************************************
* FUNCTION:
*   EwPackClutEntry
*
* DESCRIPTION:
*   The following EwPackClutEntry() function has the job to convert the given
*   RGBA color channels into a 32 bit wide clut entry as it is used by Index8
*   surfaces internally.
*
*   Like the EwPackColor() function, the resulting value depends on the used
*   native color format. Therefore, the order of color channels, color spaces,
*   etc. are not predetermined here. The result is just a 32 bit value without
*   any assumptions about its structure.
*
*   The resulting 32 bit value is particular for the respective target color
*   format. It is predestined to be used more effectively durring all drawing
*   Index8 bitmaps.
*
* ARGUMENTS:
*   aRed,
*   aGreen,
*   aBlue,
*   aAlpha  - Non-premultiplied color channel values in the range 0 .. 255. 
*
* RETURN VALUE:
*   32 bit clut entry. Particular for the target color format.
*
*******************************************************************************/
unsigned long EwPackClutEntry
(
  long              aRed,
  long              aGreen,
  long              aBlue,
  long              aAlpha
);


/*******************************************************************************
* FUNCTION:
*   EwSetPixelSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to draw a single pixel with a solid color. The new pixel overwrites the
*   existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aX, aY    - Position to draw the pixel.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetPixelSolid
(
  XSurfaceMemory*   aDst,
  int               aX,
  int               aY,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwSetPixelSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to draw a single pixel with a solid color. The new pixel will be alpha 
*   blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aX, aY    - Position to draw the pixel.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetPixelSolidBlend
(
  XSurfaceMemory*   aDst,
  int               aX,
  int               aY,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwFillRowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a solid color. The new pixel overwrites
*   the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aWidth    - Width of the row.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRowSolid
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwFillRowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a solid color. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aWidth    - Width of the row.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRowSolidBlend
(
  XSurfaceMemory*   aDst,
  int               aWidth, 
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwFillRowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a color gradient. The new pixel will
*   overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aWidth    - Width of the row.
*   aGradient - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRowGradient
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwFillRowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a color gradient. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aWidth    - Width of the row.
*   aGradient - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRowGradientBlend
(
  XSurfaceMemory*   aDst,
  int               aWidth, 
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRow
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed without any additional opacity values. The new pixel overwrites
*   the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRow
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed without any additional opacity values. The new pixel will be
*   alpha blended withe the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed with an additional solid opacity value. The new pixel will 
*   overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed with an additional solid opacity value. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed with an additional opacity gradient. The new pixel overwrites
*   the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed with an additional opacity gradient. The new pixel will be alpha
*   blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8Row
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The 
*   operation is executed without any additional opacity values. The new pixel
*   overwrites the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The 
*   operation is executed without any additional opacity values. The new pixel
*   will be alpha blended withe the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The 
*   operation is executed with an additional solid opacity value. The new pixel
*   will overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The 
*   operation is executed with an additional solid opacity value. The new pixel
*   will be alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The 
*   operation is executed with an additional opacity gradient. The new pixel 
*   overwrites the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The 
*   operation is executed with an additional opacity gradient. The new pixel
*   will be alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyAlpha8RowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a solid color additionally modulated by
*   the opacity values from the source alpha8 surface. The new pixel overwrite
*   the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyAlpha8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyAlpha8RowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a solid color additionally modulated by
*   the opacity values from the source alpha8 surface. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyAlpha8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyAlpha8RowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a color gradient additionally modulated
*   by the opacity values from the source alpha8 surface. The new pixel will
*   overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyAlpha8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyAlpha8RowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a color gradient additionally modulated
*   by the opacity values from the source alpha8 surface. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyAlpha8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRow
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will overwrite the existing pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRow
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowFilter
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The resulting pixel will overwrite the existing pixel in
*   the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowFilter
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowFilterBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The new pixel will be alpha-blended with the existing
*   pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowFilterBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The new
*   pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowFilterGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter. The
*   new pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The
*   resulting pixel will be alpha-blended with the pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowFilterGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The resulting pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8Row
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will overwrite the existing pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowFilter
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The resulting pixel will overwrite the existing pixel in
*   the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowFilter
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowFilterBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The new pixel will be alpha-blended with the existing
*   pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowFilterBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The new
*   pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowFilterGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The new pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The
*   resulting pixel will be alpha-blended with the pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowFilterGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The resulting pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpAlpha8RowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The new
*   pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpAlpha8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpAlpha8RowFilterGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter. The
*   new pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpAlpha8RowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpAlpha8RowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The
*   resulting pixel will be alpha-blended with the pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpAlpha8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpAlpha8RowFilterGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The resulting pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpAlpha8RowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc, 
  int               aWidth,
  long              aS,
  long              aT,
  long              aSS,
  long              aTS,
  long              aSrcWidth,
  long              aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwAllocVideo
*
* DESCRIPTION:
*   The function EwAllocVideo() has the job to reserve a block of memory which
*   is exclusively intended for pure software surfaces.
*
*   The large size and the persistent character of surfaces may expect special
*   heap management and algorithms. The intension of this function is to handle
*   surface memory allocations in a separate way of any other allocations.
*
* ARGUMENTS:
*   aSize - Desired size of the new memory block in bytes.
*
* RETURN VALUE:
*   If successful, returns the pointer to the reserved memory block, otherwise
*   null is returned.
*
*******************************************************************************/
void* EwAllocVideo
(
  int               aSize
);


/*******************************************************************************
* FUNCTION:
*   EwFreeVideo
*
* DESCRIPTION:
*   The function EwFreeVideo() is the counterpart of EwAllocVideo() function.
*   Its job is to release the given memory block.
*
*   The large size and the persistent character of surfaces may expect special
*   heap management and algorithms. The intension of this function is to handle
*   surface memory allocations in a separate way of any other allocations.
*
* ARGUMENTS:
*   aMemory - Address of the memory block to release.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeVideo
(
  void*             aMemory
);


#ifdef __cplusplus
  }
#endif

#endif /* EWGFXDRIVER_H */

/* pba */
