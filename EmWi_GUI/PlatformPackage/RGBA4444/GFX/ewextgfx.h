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
*   This header file contains Graphics Engine configuration parameters and the
*   adaptation for the STM32Fx graphics subsystem.
*
*******************************************************************************/

#ifndef EWEXTGFX_H
#define EWEXTGFX_H


#ifdef __cplusplus
  extern "C" 
  {
#endif


/* 
  EW_USE_DOUBLE_BUFFER - Flag to switch on/off the usage of an additional 
  framebuffer, so that the screen is operated in double-buffering mode.
  Per default, the usage of double-buffering is disabled. To activate the
  double-buffering mode, please set the macro EW_USE_DOUBLE_BUFFER to 1 
  within your makefile. This can be achieved by using the compiler flag 
  -DEW_USE_DOUBLE_BUFFER=1
  If double-buffering is activated, the exchange between front-buffer and
  back-buffer has to be done at V-sync. This has to be implemented in the
  extern function EmWiSetFramebufferAddress().
*/
#ifndef EW_USE_DOUBLE_BUFFER
  #define EW_USE_DOUBLE_BUFFER 0
#endif

#if EW_USE_DOUBLE_BUFFER == 0
  #undef EW_USE_DOUBLE_BUFFER
#endif


/* 
  EW_USE_OFFSCREEN_BUFFER - Flag to switch on/off the usage of an off-screen
  buffer. This is an internal buffer, allocated by the Graphics Engine and 
  used to prepare the graphical content (composition buffer).
  Per default, the usage of the off-screen buffer is enabled. To switch off 
  the usage of this composition buffer, please set the macro 
  EW_USE_OFFSCREEN_BUFFER to 0 within your makefile. This can be achieved by 
  using the compiler flag -DEW_USE_OFFSCREEN_BUFFER=0
  If the usage of an off-screen buffer is deactivated, the resulting display
  update depends on the macro EW_USE_DOUBLE_BUFFER: 
  In case of a double-buffering configuration, the graphics composition is 
  done directly into the back-buffer.
  In case of a single buffered configuration, the graphics composition is done
  directly into the framebuffer. This makes only sence, when you are using 
  an LCD with its own pixelbuffer (e.g. some MIPI-DSI displays). Otherwise,
  the complete drawing composition is visible on the screen (flickering).
*/
#ifndef EW_USE_OFFSCREEN_BUFFER
  #define EW_USE_OFFSCREEN_BUFFER 1
#endif

#if EW_USE_OFFSCREEN_BUFFER == 0
  #undef EW_USE_OFFSCREEN_BUFFER
#endif


/* 
  EW_USE_DMA2D_GRAPHICS_ACCELERATOR - Flag to switch on/off the usage of the 
  graphics accelerator within the STM32Fx target (DMA2D / Chrom-ART).
  Per default, the usage of the DMA2D is enabled. To switch off the usage of 
  the DMA2D, please set the macro EW_USE_DMA2D_GRAPHICS_ACCELERATOR to 0 within 
  your makefile. This can be achieved by using the compiler flag 
  -DEW_USE_DMA2D_GRAPHICS_ACCELERATOR=0
*/
#ifndef EW_USE_DMA2D_GRAPHICS_ACCELERATOR
  #define EW_USE_DMA2D_GRAPHICS_ACCELERATOR 1
#endif

#if EW_USE_DMA2D_GRAPHICS_ACCELERATOR == 0
  #undef EW_USE_DMA2D_GRAPHICS_ACCELERATOR
#endif


/* The following macros override the default color channel allocation to the
   order (bit31) A..R..G..B (bit0). If your graphics hardware supports other 
   color channel order, you can define the following macros in your make file. */
#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_RED
  #define EW_COLOR_CHANNEL_BIT_OFFSET_RED     8
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_GREEN
  #define EW_COLOR_CHANNEL_BIT_OFFSET_GREEN   4
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_BLUE
  #define EW_COLOR_CHANNEL_BIT_OFFSET_BLUE    0
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA
  #define EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA  12
#endif


/* STM32Fx operates with premultiplied colors - but bitmap formats are NOT premultiplied */
#define EW_PREMULTIPLY_COLOR_CHANNELS         0


/*******************************************************************************
* FUNCTION:
*   STM32FxInitGfx
*
* DESCRIPTION:
*   The function STM32FxInitGfx is called from the Graphics Engine during the
*   initialization in order to make target specific configurations of the
*   Graphics Engine
*
* ARGUMENTS:
*   aArgs - Optional argument passed to the Graphics Engine init function.
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int STM32FxInitGfx
(
  void*             aArgs
);


/*******************************************************************************
* FUNCTION:
*   STM32FxInitViewport
*
* DESCRIPTION:
*   The function STM32FxInitViewport is called from the Graphics Engine, 
*   to create a new viewport on the target. The function uses the given  
*   buffers passed in the arguments aDisplay1 and aDisplay2. The argument
*   aDisplay3 contains the color format of the framebuffers (which means the
*   color format of the LCD and not the color format of the Graphics Engine). 
*   The following values are supported: 
*   LTDC_PIXEL_FORMAT_ARGB8888, 
*   LTDC_PIXEL_FORMAT_RGB888, 
*   LTDC_PIXEL_FORMAT_RGB565,
*   LTDC_PIXEL_FORMAT_ARGB4444
*   Please note, that the framebuffer format can be different than the color 
*   format of the Graphics Engine, if the Graphics Engine operates with an 
*   separate off-screen buffer. Otherwise, only LTDC_PIXEL_FORMAT_ARGB4444 is 
*   supported.
*
* ARGUMENTS:
*   aWidth,
*   aHeight       - Size of the viewport in pixel.
*   aExtentX,
*   aExtentY,
*   aExtentWidth,
*   aExtentHeight - not used.
*   aOrient       - not used.
*   aOpacity      - not used.
*   aDisplay1     - Address of the framebuffer / front-buffer.
*   aDisplay2     - Address of the back-buffer in case of double-buffering.
*   aDisplay3     - Color format of the framebuffers (aDisplay1 and aDisplay2). 
*
* RETURN VALUE:
*   Handle of the surface descriptor.
*
*******************************************************************************/
unsigned long STM32FxInitViewport
(
  int               aWidth,
  int               aHeight,
  int               aExtentX,
  int               aExtentY,
  int               aExtentWidth,
  int               aExtentHeight,
  int               aOrient,
  int               aOpacity,
  void*             aDisplay1,
  void*             aDisplay2,
  void*             aDisplay3
);


/*******************************************************************************
* FUNCTION:
*   STM32FxDoneViewport
*
* DESCRIPTION:
*   The function STM32FxDoneViewport is called from the Graphics Engine, to 
*   release a previously created viewport on the STM32Fx target.
*
* ARGUMENTS:
*   aHandle - Handle of the surface descriptor.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void STM32FxDoneViewport
(
  unsigned long     aHandle
);


/*******************************************************************************
* FUNCTION:
*   STM32FxBeginUpdate
*
* DESCRIPTION:
*   The function STM32FxBeginUpdate is called from the Graphics Engine, to 
*   initiate the screen update cycle.
*
* ARGUMENTS:
*   aHandle - Handle to the framebuffer surface.
*
* RETURN VALUE:
*   Handle of the framebuffer surface.
*
*******************************************************************************/
unsigned long STM32FxBeginUpdate
(
  unsigned long     aHandle
);


/*******************************************************************************
* FUNCTION:
*   STM32FxEndUpdate
*
* DESCRIPTION:
*   The function STM32FxEndUpdate is called from the Graphics Engine, to 
*   finalize the screen update cycle.
*
* ARGUMENTS:
*   aHandle - Handle to the framebuffer surface.
*   aX,
*   aY,
*   aWidth,
*   aHeight - Position and size of the area affected from the screen update 
*     (dirty rectangle).
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void STM32FxEndUpdate
(
  unsigned long     aHandle,
  int               aX,
  int               aY,
  int               aWidth,
  int               aHeight
);


/* Redirect the following operations to the functions within this module */
#define EwGfxInit          STM32FxInitGfx
#define EwGfxInitViewport  STM32FxInitViewport
#define EwGfxDoneViewport  STM32FxDoneViewport
#define EwGfxBeginUpdate   STM32FxBeginUpdate
#define EwGfxEndUpdate     STM32FxEndUpdate


/*******************************************************************************
* FUNCTION:
*   STM32FxCreateSurface
*
* DESCRIPTION:
*   The function STM32FxCreateSurface() reserves pixel memory for a new surface 
*   with the given size and color format. The function returns a handle to the 
*   new surface.
*
* ARGUMENTS:
*   aFormat  - Color format of the surface. (See EW_PIXEL_FORMAT_XXX).
*   aWidth,
*   aHeight  - Size of the surface in pixel to create.
*
* RETURN VALUE:
*   The function returns a handle to the created surface. This can be a pointer
*   to a dynamically allocated data structure, an index in a list of surfaces,
*   or a handle returned by the lower level API.
*
*   If the creation is failed, the function should return 0.
*
*******************************************************************************/
unsigned long STM32FxCreateSurface
(
  int               aFormat,
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   STM32FxCreateConstSurface
*
* DESCRIPTION:
*   The function STM32FxCreateConstSurface() creates a surface structure
*   that refers to a constant pixel memory. The function returns a handle to the 
*   new surface.
*
* ARGUMENTS:
*   aFormat - Color format of the surface. (See EW_PIXEL_FORMAT_XXX).
*   aWidth,
*   aHeight - Size of the surface in pixel.
*   aMemory - Pointer to constant pixel memory.
*
* RETURN VALUE:
*   The function returns a handle to the created surface.
*   If the creation is failed, the function should return 0.
*
*******************************************************************************/
unsigned long STM32FxCreateConstSurface
( 
  int               aFormat,
  int               aWidth, 
  int               aHeight, 
  XSurfaceMemory*   aMemory 
);


/*******************************************************************************
* FUNCTION:
*   STM32FxDestroySurface
*
* DESCRIPTION:
*   The function STM32FxDestroySurface() frees the resources of the given surface.
*   This function is a counterpart to STM32FxCreateSurface().
*
* ARGUMENTS:
*   aHandle - Handle to the surface to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void STM32FxDestroySurface
(
  unsigned long     aHandle
);


/*******************************************************************************
* FUNCTION:
*   STM32FxLockSurface
*
* DESCRIPTION:
*   The function LockSurface() provides a direct access to the pixel memory of
*   the given surface. The function returns a lock object containing pointers to
*   memory, where the caller can read/write the surface pixel values. Additional
*   pitch values also returned in the object allow the caller to calculate the
*   desired pixel addresses.
*
*   When finished the access cycle, the function UnlockSurface() should be used
*   in order to release the lock, update the affected surface, flush CPU caches,
*   etc.
*
* ARGUMENTS:
*   aHandle     - Handle to the surface to obtain the direct memory access.
*   aX, aY,
*   aWidth,
*   aHeight     - Area within the surface affected by the access operation.
*     (Relative to the top-left corner of the surface). This is the area, the
*     caller wish to read/write the pixel data.
*   aIndex,
*   Count       - Optional start index and number of entries within the CLUT,
*     the caller wish to read/write. These paramaters are used for surfaces
*     with the index8 color format only.
*   aReadPixel  - Is != 0, if the caller intends to read the pixel information
*     from the surface memory. If == 0, the memory content may remain undefined
*     depending on the underlying graphics sub-system and its video-memory
*     management.
*   aWritePixel - Is != 0, if the caller intends to modify the pixel information
*     within the surface memory. If == 0, any modifications within the memory
*     may remain ignored depending on the underlying graphics sub-system and its
*     video-memory management.
*   aReadClut   - Is != 0, if the caller intends to read the CLUT information.
*     If == 0, the CLUT content may remain undefined.
*   aWritePixel - Is != 0, if the caller intends to modify the CLUT information.
*     If == 0, any modifications within the memory may remain ignored depending
*     on the underlying graphics sub-system and its video-memory management.
*   aMemory     - Pointer to an object, where the desired surface pointers 
*     should be stored.
*
* RETURN VALUE:
*   If successful, the function should return a kind of a lock object. This
*   object can contain additional information needed when the surface is
*   unlocked again. If you don't want to return additional information, return
*   any value != 0.
*
*   If there was not possible to lock the surface, or the desired access mode
*   is just not supported by the underlying graphics sub-system, the function
*   fails and returns zero. (e.g. OpenGL based sub-systems usually allow the
*   write access to surfaces (textures) only. Read access may fail in this
*   case).
*
*******************************************************************************/
unsigned long STM32FxLockSurface
(
  unsigned long     aHandle,
  int               aX,
  int               aY,
  int               aWidth,
  int               aHeight,
  int               aIndex,
  int               aCount,
  int               aReadPixel,
  int               aWritePixel,
  int               aReadClut,
  int               aWriteClut,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   STM32FxUnlockSurface
*
* DESCRIPTION:
*   The function STM32FxUnlockSurface() has the job to unlock the given surface and
*   if necessary free any temporary used resources.
*   This function is a counterpart to STM32FxLockSurface().
**
* ARGUMENTS:
*   aSurfaceHandle - Handle to the surface to release the direct memory access.
*   aLockHandle    - value returned by the corresponding LockSurface() call.
*     If LockSurface() has allocated memory for the lock object, you will need
*     to free it now.
*   aX, aY,
*   aWidth,
*   aHeight     - Area within the surface affected by the access operation.
*     (Relative to the top-left corner of the surface). This is the area, the
*     caller wished to read/write the pixel data.
*   aIndex,
*   Count       - Optional start index and number of entries within the CLUT,
*     the caller wished to read/write. These paramaters are used for surfaces
*     with the index8 color format only.
*   aWritePixel - Is != 0, if the caller has modified the pixel information
*     within the surface memory. If == 0, no modification took place, so no
*     surface updates are needed.
*   aWritePixel - Is != 0, if the caller has modified the CLUT information.
*     If == 0, no modification took place, so no surface updates are needed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void STM32FxUnlockSurface
(
  unsigned long     aSurfaceHandle,
  unsigned long     aLockHandle,
  int               aX,
  int               aY,
  int               aWidth,
  int               aHeight,
  int               aIndex,
  int               aCount,
  int               aWritePixel,
  int               aWriteClut
);


/* Macros to redirect the Graphics Engine operations to the above functions. */
#define EwGfxCreateNativeSurface      STM32FxCreateSurface
#define EwGfxCreateConstNativeSurface STM32FxCreateConstSurface
#define EwGfxDestroyNativeSurface     STM32FxDestroySurface
#define EwGfxLockNativeSurface        STM32FxLockSurface
#define EwGfxUnlockNativeSurface      STM32FxUnlockSurface

#define EwGfxCreateAlpha8Surface      STM32FxCreateSurface
#define EwGfxCreateConstAlpha8Surface STM32FxCreateConstSurface
#define EwGfxDestroyAlpha8Surface     STM32FxDestroySurface
#define EwGfxLockAlpha8Surface        STM32FxLockSurface
#define EwGfxUnlockAlpha8Surface      STM32FxUnlockSurface


/*******************************************************************************
* PROTOTYPE:
*   STM32FxFillDriver
*
* DESCRIPTION:
*   The following function performs the 'fill rectangular area' operation by
*   using solid or gradient color values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the native destination surface object. See the
*      function CreateSurface().
*   aDstX,
*   aDstY       - Origin of the area to fill (relative to the top-left corner
*      of the destination surface).
*   aWidth,
*   aHeight     - Size of the area to fill.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 RGBA8888 color values. The four color values do
*     correspond to the four corners of the area: top-left, top-right, bottom-
*     right and bottom-left.
*     If all colors are equal, the solid variant of the operation is assumed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void STM32FxFillDriver
(
  unsigned long     aDstHandle,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  unsigned long*    aColors
);


/* Macros to redirect the Graphics Engine operations to the above functions. */
#ifdef EW_USE_DMA2D_GRAPHICS_ACCELERATOR
  #define EwGfxFillSolid              STM32FxFillDriver
#endif  


/*******************************************************************************
* PROTOTYPE:
*   STM32FxCopyDriver
*
* DESCRIPTION:
*   The following function performs the 'copy rectangular area' operation from a
*   native or alpha8 surface to a native surface.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the native destination surface object. See the
*      function CreateSurface().
*   aSrcHandle  - Handle to the native/index8/alpha8 source surface object. See
*      the function CreateSurface().
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
*     In case of an alpha8 source surface if all colors are equal, the solid
*     variant of the operation is assumed.
*     In case of native and index8 source surfaces if all colors are equal but
*     their alpha value < 255, the solid variant of the operation is assumed.
*     In case of native and index8 source surfaces if all colors are equal and
*     their alpha value == 255, the variant withouto any modulation is assumed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void STM32FxCopyDriver
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


/* Macros to redirect the Graphics Engine operations to the above function. */
#ifdef EW_USE_DMA2D_GRAPHICS_ACCELERATOR
  #define EwGfxCopyNative                 STM32FxCopyDriver 
#endif  


/*******************************************************************************
* PROTOTYPE:
*   STM32FxBlendDriver
*
* DESCRIPTION:
*   The following function performs the 'blend rectangular area' operation from a
*   native or alpha8 surface to a native surface.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the native destination surface object. See the
*      function CreateSurface().
*   aSrcHandle  - Handle to the native/index8/alpha8 source surface object. See
*      the function CreateSurface().
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
*     In case of an alpha8 source surface if all colors are equal, the solid
*     variant of the operation is assumed.
*     In case of native and index8 source surfaces if all colors are equal but
*     their alpha value < 255, the solid variant of the operation is assumed.
*     In case of native and index8 source surfaces if all colors are equal and
*     their alpha value == 255, the variant withouto any modulation is assumed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void STM32FxBlendDriver
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


/* Macros to redirect the Graphics Engine operations to the above function. */
#ifdef EW_USE_DMA2D_GRAPHICS_ACCELERATOR
  #define EwGfxCopyNativeBlend            STM32FxBlendDriver
  #define EwGfxCopyNativeSolidBlend       STM32FxBlendDriver
  #define EwGfxCopyAlpha8Blend            STM32FxBlendDriver
  #define EwGfxCopyAlpha8SolidBlend       STM32FxBlendDriver
#endif  


#ifdef __cplusplus
  }
#endif

#endif /* EWEXTGFX_H */


/* msy, pba */
