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

#include "ewgfxdriver.h"
#include "ewextgfx.h"
#include "ewgfxdefs.h"
#include "ewextpxl_RGBA8888.h"
#include "ewrte.h"

#if defined STM32F746xx || defined STM32F769xx
  #include "stm32f7xx_hal.h"
#endif

#if defined STM32F429xx || defined STM32F439xx || defined STM32F469xx  
  #include "stm32f4xx_hal.h"
#endif  

#ifdef EW_USE_DMA2D_GRAPHICS_ACCELERATOR

  DMA2D_HandleTypeDef Accelerator;

#endif  

/* Extern function to set the current framebuffer address.
   Please ensure, that the appropriate implementation is available. */
extern void EmWiSetFramebufferAddress( unsigned long aAddress );


/* Error messages */
#define Err01 "Invalid address of framebuffer (front-buffer)!"
#define Err02 "Size of framebuffer device (display size) does not match with given application size!"
#define Err03 "Incompatible color format of the framebuffer device! The following values " \
              "are supported: LTDC_PIXEL_FORMAT_ARGB8888, LTDC_PIXEL_FORMAT_RGB888, " \
              "LTDC_PIXEL_FORMAT_RGB565, LTDC_PIXEL_FORMAT_ARGB4444"
#define Err04 "Invalid address of back-buffer!"
#define Err05 "Requested operation with graphics accelerator DMA2D failed!"
#define Err06 "Could not allocate memory!"
#define Err07 "Incompatible color format of the framebuffer device! The Graphics Engine " \
              "is configured with EW_USE_OFFSCREEN_BUFFER=0 - This means, the Graphics " \
              "Engine does not use a separate off-screen buffer. All graphics operations " \
              "are done directly into the given framebuffer. Please ensure, that the " \
              "color format of the framebuffer is identical to the color format of the " \
              "Graphics Engine!"


/* Descriptor of a STM32Fx surface. This type is used for framebuffers and all 
   internal surfaces (bitmaps). The pixel memory of the surface may be preallocated
   for framebuffers. In all other cases, the pixel memory is allocated and freed
   dynamically. 
   The color format of the surface is equal to the native color format of the
   Graphics Engine. If the drawing composition is done within a separate off-screen
   buffer, the color format of the physical framebuffer can be different. In this
   case, the DMA2D is responsible for the color conversion. */
typedef struct
{
  int   Width;
  int   Height;
  int   Prealloc;
  int   BytesPerPixel;
  int   Format;
  void* Pixel;
} XSTM32FxSurface;


/* Descriptor of an STM32Fx viewport. Due to the different modes that are supported,
   an additional off-screen canvas and/or double-buffering will be used. */
typedef struct
{
  XSTM32FxSurface* FrameBuffer;
  XSTM32FxSurface* DoubleBuffer;
  XSTM32FxSurface* OffScreenBuffer;
  int              LtdcPixelFormat;
} XSTM32FxViewport;


/* Memory usage profiler */
#ifdef EW_PRINT_MEMORY_USAGE
  extern int EwResourcesMemory;
#endif

/* extern variables to control Graphics Engine */
extern int EwPreserveFramebufferContent;
extern int EwMaxSurfaceCacheSize;
extern int EwMaxGlyphSurfaceWidth;
extern int EwMaxGlyphSurfaceHeight;

/* Helper functions */
#if EW_USE_OFFSCREEN_BUFFER
  static void CopyBuffer( XSTM32FxSurface* aDst, XSTM32FxSurface* aSrc, int aLtdcPixelFormat, 
    int aX, int aY, int aWidth, int aHeight );
#endif


/* coordinates to store last update rectangle for accumulated regions */
#if defined EW_USE_OFFSCREEN_BUFFER && defined EW_USE_DOUBLE_BUFFER
  static int LastX1;
  static int LastY1;
  static int LastX2;
  static int LastY2;
#endif


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
int STM32FxInitGfx( void* aArgs )
{
  /* configure bitmap and glyph cache according makefile settings */
  EwMaxSurfaceCacheSize   = EW_MAX_SURFACE_CACHE_SIZE;
  EwMaxGlyphSurfaceWidth  = EW_MAX_GLYPH_SURFACE_WIDTH;
  EwMaxGlyphSurfaceHeight = EW_MAX_GLYPH_SURFACE_HEIGHT;
  
  /* In case of pure double-buffering mode without an off-screen buffer, the
     Mosaic class library has to combine the dirty rectangles of two consecutive
     screen updates. To achieve this, the variable EwPreserveFramebufferContent
     has to be set to 0.
     Normally, the variable EwPreserveFramebufferContent is set to 1, which means 
     that the graphics subsystem retains the content of the framebuffer between 
     two consecutive screen update frames. */
  #if defined EW_USE_DOUBLE_BUFFER && !defined EW_USE_OFFSCREEN_BUFFER
    EwPreserveFramebufferContent = 0;
  #endif

  return 1;
}


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
*   separate off-screen buffer. Otherwise, only LTDC_PIXEL_FORMAT_ARGB8888 is 
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
unsigned long STM32FxInitViewport( int aWidth, int aHeight, int aExtentX,
  int aExtentY, int aExtentWidth, int aExtentHeight, int aOrient, int aOpacity, 
  void* aDisplay1, void* aDisplay2, void* aDisplay3 )
{
  int bytesPerPixel = 0;
  XSTM32FxViewport* viewport;
  /* verify that the given framebuffer address is valid */
  if ( !aDisplay1 )
  {
    EW_ERROR( Err01 );
    return 0;
  }
  
  /* compare metrics of display with metrics of application */
  if (( aWidth != aExtentWidth ) || ( aHeight != aExtentHeight ))
  {
    EW_ERROR( Err02 );
    return 0;
  }
  
  /* determine bytes per pixel of the framebuffer depending on given color format */
  #if EW_USE_OFFSCREEN_BUFFER
  
    if ((int)aDisplay3 == LTDC_PIXEL_FORMAT_ARGB8888 )
      bytesPerPixel = 4;
    else if ((int)aDisplay3 == LTDC_PIXEL_FORMAT_RGB888 )
      bytesPerPixel = 3;
    else if (((int)aDisplay3 == LTDC_PIXEL_FORMAT_RGB565 ) || ((int)aDisplay3 == LTDC_PIXEL_FORMAT_ARGB4444 ))
      bytesPerPixel = 2;
    else  
    {
      EW_ERROR( Err03 );
      return 0;
    }
  
  #else
  
    if ((int)aDisplay3 == LTDC_PIXEL_FORMAT_ARGB8888 )
      bytesPerPixel = 4;
    else  
    {
      EW_ERROR( Err07 );
      return 0;
    }
  
  #endif  
  
  /* verify that the given back-buffer address matchs the choosen configuration */
  #if EW_USE_DOUBLE_BUFFER

    if ( !aDisplay2 )
    {
      EW_ERROR( Err04 );
      return 0;
    }

  #endif

  /* allocate memory for the descriptor structure */
  viewport = EwAlloc( sizeof( XSTM32FxViewport ));
  if ( !viewport )
  {
    EW_ERROR( Err06 );
    return 0;
  }
  viewport->FrameBuffer     = 0;
  viewport->DoubleBuffer    = 0;
  viewport->OffScreenBuffer = 0;
  viewport->LtdcPixelFormat = (int)aDisplay3;

  /* allocate memory for the framebuffer descriptor */
  viewport->FrameBuffer = EwAlloc( sizeof( XSTM32FxSurface ));
  if ( !viewport->FrameBuffer )
  {
    EW_ERROR( Err06 );
    return 0;
  }

  /* initialize the framebuffer descriptor */
  viewport->FrameBuffer->Width         = aWidth;
  viewport->FrameBuffer->Height        = aHeight;
  viewport->FrameBuffer->Prealloc      = 1;
  viewport->FrameBuffer->BytesPerPixel = bytesPerPixel;
  viewport->FrameBuffer->Pixel         = aDisplay1;

  #if EW_USE_DOUBLE_BUFFER

    /* allocate memory for the double-buffer descriptor */
    viewport->DoubleBuffer = EwAlloc( sizeof( XSTM32FxSurface ));
    if ( !viewport->DoubleBuffer )
    {
      EW_ERROR( Err06 );
      return 0;
    }
  
    /* initialize the double-buffer descriptor */
    viewport->DoubleBuffer->Width         = aWidth;
    viewport->DoubleBuffer->Height        = aHeight;
    viewport->DoubleBuffer->Prealloc      = 1;
    viewport->DoubleBuffer->BytesPerPixel = bytesPerPixel;
    viewport->DoubleBuffer->Pixel         = aDisplay2;

  #endif

  #if EW_USE_OFFSCREEN_BUFFER

    /* create the surface for the offscreen-buffer in the memory */
    viewport->OffScreenBuffer = (XSTM32FxSurface*)STM32FxCreateSurface( EW_PIXEL_FORMAT_NATIVE, aWidth, aHeight );
    if ( !viewport->OffScreenBuffer )
    {
      EW_ERROR( Err06 );
      return 0;
    }
    
  #endif  
  
  #ifdef EW_USE_DMA2D_GRAPHICS_ACCELERATOR

    /* prepare configuration of the DMA2D graphics accelerator */
    Accelerator.Instance = DMA2D;
    Accelerator.Init.Mode = DMA2D_M2M; 
    Accelerator.Init.ColorMode = DMA2D_ARGB8888;
    Accelerator.Init.OutputOffset = 0;
    Accelerator.XferCpltCallback = 0;
    Accelerator.XferErrorCallback = 0;
    Accelerator.LayerCfg[0].InputOffset = 0;
    Accelerator.LayerCfg[0].InputColorMode = CM_ARGB8888;
    Accelerator.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    Accelerator.LayerCfg[0].InputAlpha = 0;
    Accelerator.LayerCfg[1].InputOffset = 0;
    Accelerator.LayerCfg[1].InputColorMode = CM_ARGB8888;
    Accelerator.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    Accelerator.LayerCfg[1].InputAlpha = 0;
    Accelerator.Lock  = HAL_UNLOCKED;
    Accelerator.State = HAL_DMA2D_STATE_RESET;
    Accelerator.ErrorCode = 0;
    
    /* initialize the DMA2D graphics accelerator */
    if ( HAL_DMA2D_Init( &Accelerator ) != HAL_OK )
    { 	
      EW_ERROR( Err05 );
      return 0;
    }
  
  #endif  
  
  #ifdef EW_PRINT_MEMORY_USAGE
  
    EwResourcesMemory += sizeof( XSTM32FxViewport );
    EwResourcesMemory += sizeof( XSTM32FxSurface );
  
    #if EW_USE_DOUBLE_BUFFER
      EwResourcesMemory += sizeof( XSTM32FxSurface );
    #endif  
  
  #endif

  /* ensure that current framebuffer is shown */
  EmWiSetFramebufferAddress( (unsigned long)viewport->FrameBuffer->Pixel );
  
  return (unsigned long)viewport;
}


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
void STM32FxDoneViewport( unsigned long aHandle )
{
  XSTM32FxViewport* viewport = (XSTM32FxViewport*)aHandle;

  /* if there was an off-screen buffer in the viewport, destroy it... */
  if ( viewport->OffScreenBuffer )
    STM32FxDestroySurface((unsigned long) viewport->OffScreenBuffer );
 
  /* destroy the double-buffer descriptor */
  if ( viewport->DoubleBuffer )
    EwFree( viewport->DoubleBuffer );
 
  /* destroy the framebuffer descriptor */
  if ( viewport->FrameBuffer )
    EwFree( viewport->FrameBuffer );
 
  /* destroy the viewport */
  EwFree( viewport );
  
  #ifdef EW_USE_DMA2D_GRAPHICS_ACCELERATOR
    /* deinitialize the DMA2D graphics accelerator */
    HAL_DMA2D_DeInit( &Accelerator );
  #endif  

  #ifdef EW_PRINT_MEMORY_USAGE

    EwResourcesMemory -= sizeof( XSTM32FxViewport );
    EwResourcesMemory -= sizeof( XSTM32FxSurface );
  
    #if EW_USE_DOUBLE_BUFFER
      EwResourcesMemory -= sizeof( XSTM32FxSurface );
    #endif  
  
  #endif
}


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
*   Handle of the destination surface, used for all drawing operations.
*
*******************************************************************************/
unsigned long STM32FxBeginUpdate( unsigned long aHandle )
{
  XSTM32FxViewport* viewport = (XSTM32FxViewport*)aHandle;

  #if EW_USE_OFFSCREEN_BUFFER

    return (unsigned long)viewport->OffScreenBuffer;
  
  #endif  

  #if EW_USE_DOUBLE_BUFFER
  
    return (unsigned long)viewport->DoubleBuffer;
  
  #endif  

  return (unsigned long)viewport->FrameBuffer;
}


/*******************************************************************************
* FUNCTION:
*   STM32FxEndUpdate
*
* DESCRIPTION:
*   The function STM32FxBeginUpdate is called from the Graphics Engine, to 
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
void STM32FxEndUpdate( unsigned long aHandle, int aX, int aY, int aWidth,
  int aHeight )
{
  #if defined EW_USE_OFFSCREEN_BUFFER || defined EW_USE_DOUBLE_BUFFER
  
    XSTM32FxViewport* viewport = (XSTM32FxViewport*)aHandle;
  
  #endif  

  /* nothing to do */
  if ( ( aWidth <= 0 ) || ( aHeight <= 0 ) )
    return;

  #if EW_USE_OFFSCREEN_BUFFER
  
    #if EW_USE_DOUBLE_BUFFER
      
      /* in case if double-buffering with off-screen buffer, we have to update the
         current region and the previous region => intersect both rects */
      if ( aX < LastX1 ) LastX1 = aX;
      if ( aY < LastY1 ) LastY1 = aY;
      if ( aX + aWidth > LastX2 ) LastX2 = aX + aWidth;
      if ( aY + aHeight > LastY2 ) LastY2 = aY + aHeight;
         
      /* copy content from off-screen buffer into back-buffer */
      CopyBuffer( viewport->DoubleBuffer, viewport->OffScreenBuffer, viewport->LtdcPixelFormat, 
        LastX1, LastY1, LastX2 - LastX1, LastY2 - LastY1 );
      
      /* save give region for next update */
      LastX1 = aX;
      LastY1 = aY; 
      LastX2 = aX + aWidth;
      LastY2 = aY + aHeight;
      
    #else      
      
      /* copy content from off-screen buffer directly into frame-buffer */
      CopyBuffer( viewport->FrameBuffer, viewport->OffScreenBuffer, viewport->LtdcPixelFormat, 
        aX, aY, aWidth, aHeight );

    #endif  

  #endif  
  
  #if EW_USE_DOUBLE_BUFFER
  {
  
    /* exchange front- and back-buffer objects */
    XSTM32FxSurface* tmp = viewport->DoubleBuffer;
    viewport->DoubleBuffer = viewport->FrameBuffer;
    viewport->FrameBuffer = tmp;
  
    /* make the new front-buffer visible */
    EmWiSetFramebufferAddress( (unsigned long)viewport->FrameBuffer->Pixel );
    
  }
  #endif  
}


#if defined EW_USE_OFFSCREEN_BUFFER

static void CopyBuffer( XSTM32FxSurface* aDst, XSTM32FxSurface* aSrc, int aLtdcPixelFormat, 
  int aX, int aY, int aWidth, int aHeight )
{
  XSurfaceMemory          dstMem;
  XSurfaceMemory          srcMem;
  register unsigned char* dst;
  register unsigned char* src;
  int                     dstOfs;
  int                     srcOfs;

  /* obtain direct access to the destination buffer */
  if ( !STM32FxLockSurface( (unsigned long)aDst, aX, aY, aWidth, aHeight, 0, 0, 0, 0, 0, 0, &dstMem ) )        
      return;
    
  /* obtain direct access to the source buffer */
  if ( !STM32FxLockSurface( (unsigned long)aSrc, aX, aY, aWidth, aHeight, 0, 0, 0, 0, 0, 0, &srcMem ) )        
      return;
    
  /* get the start address in the source and destination and their offsets */
  dst    = (unsigned char*)( dstMem.Pixel1 );
  src    = (unsigned char*)( srcMem.Pixel1 );
  dstOfs = ( dstMem.Pitch1Y / dstMem.Pitch1X ) - aWidth;
  srcOfs = ( srcMem.Pitch1Y / srcMem.Pitch1X ) - aWidth;

  #ifndef EW_USE_DMA2D_GRAPHICS_ACCELERATOR

    /* transfer the modified area from canvas to the framebuffer - row by row and
       convert each pixel from native 32 bit color format into 32, 24 or 16 bit layer format */
    if ( aLtdcPixelFormat == LTDC_PIXEL_FORMAT_ARGB8888 )
    {     
      for ( ; aHeight > 0; aHeight--, src += srcOfs * 4, dst += dstOfs * 4 )
      {
        register int i;
        for ( i = 0; i < aWidth; i++, src += 4, dst += 4 )
        {
          /* copy ARGB8888 to ARGB8888 */
          *((unsigned long*)dst) = *((unsigned long*)src);
        }  
      }    
	  }
    else if ( aLtdcPixelFormat == LTDC_PIXEL_FORMAT_RGB888 )
    {     
      for ( ; aHeight > 0; aHeight--, src += srcOfs * 4, dst += dstOfs * 3 )
      {
        register int i;
        for ( i = 0; i < aWidth; i++, src++ )
        {
          /* copy ARGB8888 to RGB888 */
          *dst++ = *src++;
          *dst++ = *src++;
          *dst++ = *src++;
        }  
      }    
	  }
    else if ( aLtdcPixelFormat == LTDC_PIXEL_FORMAT_RGB565 )
    {     
      for ( ; aHeight > 0; aHeight--, src += srcOfs * 4, dst += dstOfs * 2 )
      {
        register int i;
        for ( i = 0; i < aWidth; i++, src += 4, dst += 2 )
        {
          /* copy ARGB8888 to RGB565 */
          register unsigned long c = *((unsigned long*)src); 
          *((unsigned short*)dst) = (unsigned short)((( c & 0x000000F8 ) >> 3 ) | (( c & 0x0000FC00 ) >> 5 ) | (( c & 0x00F80000 ) >> 8 ));
        }  
      }    
	  }
    else if ( aLtdcPixelFormat == LTDC_PIXEL_FORMAT_ARGB4444 )
    {     
      for ( ; aHeight > 0; aHeight--, src += srcOfs * 4, dst += dstOfs * 2 )
      {
        register int i;
        for ( i = 0; i < aWidth; i++, src += 4, dst += 2 )
        {
          /* copy ARGB8888 to ARGB4444 */
          register unsigned long c = *((unsigned long*)src); 
          *((unsigned short*)dst) = (unsigned short)((( c & 0x000000F0 ) >> 4 ) | (( c & 0x0000F000 ) >> 8 ) | (( c & 0x00F00000 ) >> 12 ) | (( c & 0xF0000000 ) >> 16 ));
        }  
      }    
	  }

  #else  
  
    /* prepare configuration of the DMA2D graphics accelerator */
    Accelerator.Init.Mode = DMA2D_M2M_PFC; 
    if ( aLtdcPixelFormat == LTDC_PIXEL_FORMAT_ARGB8888 )
      Accelerator.Init.ColorMode = DMA2D_ARGB8888;
    else if ( aLtdcPixelFormat == LTDC_PIXEL_FORMAT_RGB888 )
      Accelerator.Init.ColorMode = DMA2D_RGB888;
    else if ( aLtdcPixelFormat == LTDC_PIXEL_FORMAT_RGB565 )
      Accelerator.Init.ColorMode = DMA2D_RGB565;
    else if ( aLtdcPixelFormat == LTDC_PIXEL_FORMAT_ARGB4444 )
      Accelerator.Init.ColorMode = DMA2D_ARGB4444;
    Accelerator.Init.OutputOffset = dstOfs;
    Accelerator.LayerCfg[1].InputOffset = srcOfs;
    Accelerator.LayerCfg[1].InputColorMode = CM_ARGB8888;
    Accelerator.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    Accelerator.LayerCfg[1].InputAlpha = 0;
  
    /* deinitialize the DMA2D graphics accelerator */
    HAL_DMA2D_DeInit( &Accelerator );
  
    /* initialize the DMA2D graphics accelerator */
    HAL_DMA2D_Init( &Accelerator );
    
    /* set the layer configurtation (foreground layer) */
    HAL_DMA2D_ConfigLayer( &Accelerator, 1 );
  
    /* start the transfer */
    HAL_DMA2D_Start( &Accelerator, (uint32_t)src, (uint32_t)dst, aWidth, aHeight );  
    
    /* wait until transfer is done */	  
    if ( HAL_DMA2D_PollForTransfer( &Accelerator, 1000 ) != HAL_OK )
      EW_ERROR( Err05 );
    
  #endif    
}

#endif


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
unsigned long STM32FxCreateSurface( int aFormat, int aWidth, int aHeight )
{
  XSTM32FxSurface* surface = 0;
  int              size;
  
  if ( aFormat == EW_PIXEL_FORMAT_NATIVE )
  {  
    size = aWidth * aHeight * sizeof( long );
    surface = EwAllocVideo( sizeof( XSTM32FxSurface ) + size );
  
    if ( surface )
    {
      surface->Width         = aWidth;
      surface->Height        = aHeight;
      surface->Prealloc      = 0;
      surface->BytesPerPixel = sizeof( long );
      surface->Format        = aFormat;
      surface->Pixel         = (void*)( surface + 1 );
  
      #ifdef EW_PRINT_MEMORY_USAGE
        EwResourcesMemory += sizeof( XSTM32FxSurface ) + size;
      #endif
    }
  }
  else if ( aFormat == EW_PIXEL_FORMAT_ALPHA8 )
  {  
    size = aWidth * aHeight;
    surface = EwAllocVideo( sizeof( XSTM32FxSurface ) + size );
  
    if ( surface )
    {
      surface->Width         = aWidth;
      surface->Height        = aHeight;
      surface->Prealloc      = 0;
      surface->BytesPerPixel = sizeof( char );
      surface->Format        = aFormat;
      surface->Pixel         = (void*)( surface + 1 );
  
      #ifdef EW_PRINT_MEMORY_USAGE
        EwResourcesMemory += sizeof( XSTM32FxSurface ) + size;
      #endif
    }
  }
  return (unsigned long)surface;  
}


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
unsigned long STM32FxCreateConstSurface( int aFormat, int aWidth, int aHeight, 
  XSurfaceMemory* aMemory )
{
  XSTM32FxSurface* surface = 0;
  int              bytesPerPixel = 0;
  
  /* determine expected size of pixel */
  if ( aFormat == EW_PIXEL_FORMAT_NATIVE )
    bytesPerPixel = sizeof( long );
  if ( aFormat == EW_PIXEL_FORMAT_ALPHA8 )
    bytesPerPixel = sizeof( char );
    
  /* verify superficially the passed pixel memory */
  if ( !aMemory || !aMemory->Pixel1 || ( aMemory->Pitch1X != bytesPerPixel ) ||
     ( aMemory->Pitch1Y < (int)( aWidth * bytesPerPixel )))
    return 0;

  if ( bytesPerPixel > 0 )
  {  
    /* allocate memory only for the administration structure */
    surface = EwAllocVideo( sizeof( XSTM32FxSurface ));
    
    if ( surface )
    {
      surface->Width         = aWidth;
      surface->Height        = aHeight;
      surface->Prealloc      = 1;
      surface->BytesPerPixel = bytesPerPixel;
      surface->Format        = aFormat;
      surface->Pixel         = (void*)( aMemory->Pixel1 );
  
      #ifdef EW_PRINT_MEMORY_USAGE
        EwResourcesMemory += sizeof( XSTM32FxSurface );
      #endif
    }
  }
    
  return (unsigned long)surface;  
}


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
void STM32FxDestroySurface( unsigned long aHandle )
{
  #ifdef EW_PRINT_MEMORY_USAGE
    XSTM32FxSurface* surface = (XSTM32FxSurface*)aHandle;
    EwResourcesMemory -= sizeof( XSTM32FxSurface ) + ( surface->Prealloc ? 
      0 : ( surface->Width * surface->Height * surface->BytesPerPixel ));
  #endif

  EwFreeVideo((void*)aHandle );
}


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
unsigned long STM32FxLockSurface( unsigned long aHandle, int aX, int aY,
  int aWidth, int aHeight, int aIndex, int aCount, int aReadPixel, int aWritePixel,
  int aReadClut, int aWriteClut, XSurfaceMemory* aMemory )
{
  XSTM32FxSurface* surface = (XSTM32FxSurface*)aHandle;

  EW_UNUSED_ARG( aIndex );

  aMemory->Pixel1  = (unsigned char *)surface->Pixel + (( aY * surface->Width ) + aX ) * surface->BytesPerPixel;
  aMemory->Pitch1Y = surface->Width * surface->BytesPerPixel;
  aMemory->Pitch1X = surface->BytesPerPixel;
  aMemory->Pixel2  = 0;
  aMemory->Pitch2Y = 0;
  aMemory->Pitch2X = 0;
  aMemory->Clut    = 0;

  return 1;
} 


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
void STM32FxUnlockSurface( unsigned long aSurfaceHandle, unsigned long aLockHandle,
  int aX, int aY, int aWidth, int aHeight, int aIndex, int aCount, int aWritePixel,
  int aWriteClut )
{
}
	

#ifdef EW_USE_DMA2D_GRAPHICS_ACCELERATOR

/*******************************************************************************
* FUNCTION:
*   STM32FxFillDriver
*
* DESCRIPTION:
*   The function STM32FxFillDriver is called from the Graphics Engine, when a 
*   rectangular area should be filled by using the DMA2D functionality.
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
void STM32FxFillDriver( unsigned long aDstHandle, int aDstX, int aDstY, 
  int aWidth, int aHeight, int aBlend, unsigned long* aColors )
{
  XSurfaceMemory dstMem;	
  unsigned long* dst;
  int            dstOfs;
  unsigned long  color = ( EW_ALPHA( aColors[0]) << 24 ) 
                       | ( EW_RED  ( aColors[0]) << 16 ) 
                       | ( EW_GREEN( aColors[0]) <<  8 ) 
                       |   EW_BLUE ( aColors[0]);

  if ( !STM32FxLockSurface( aDstHandle, aDstX, aDstY, aWidth, aHeight, 0, 0, 0, 0, 0, 0, &dstMem ) )        
    return;

  dst    = (unsigned long*)( dstMem.Pixel1 );
  dstOfs = ( dstMem.Pitch1Y / sizeof( long )) - aWidth;

  /* prepare configuration of the DMA2D graphics accelerator */
  Accelerator.Init.Mode = DMA2D_R2M; 
  Accelerator.Init.ColorMode = DMA2D_ARGB8888;
  Accelerator.Init.OutputOffset = dstOfs;

  /* deinitialize the DMA2D graphics accelerator */
  HAL_DMA2D_DeInit( &Accelerator );

  /* initialize the DMA2D graphics accelerator */
  HAL_DMA2D_Init( &Accelerator );
  
  /* start the transfer */
  HAL_DMA2D_Start( &Accelerator, (uint32_t)color, (uint32_t)dst, aWidth, aHeight );  
  
  /* wait until transfer is done */	  
  if ( HAL_DMA2D_PollForTransfer( &Accelerator, 1000 ) != HAL_OK )
    EW_ERROR( Err05 );
}


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
void STM32FxCopyDriver( unsigned long aDstHandle, unsigned long aSrcHandle,
  int aDstX, int aDstY, int aSrcX, int aSrcY, int aWidth, int aHeight,
  int aBlend, unsigned long* aColors )
{
  XSurfaceMemory dstMem;	
  unsigned long* dst;
  int            dstOfs;
  XSurfaceMemory srcMem;	
  unsigned long* src;
  int            srcOfs;

  if ( !STM32FxLockSurface( aDstHandle, aDstX, aDstY, aWidth, aHeight, 0, 0, 0, 0, 0, 0, &dstMem ) )        
		return;
  dst    = (unsigned long*)( dstMem.Pixel1 );
  dstOfs = ( dstMem.Pitch1Y / sizeof( long )) - aWidth;

  if ( !STM32FxLockSurface( aSrcHandle, aSrcX, aSrcY, aWidth, aHeight, 0, 0, 0, 0, 0, 0, &srcMem ) )        
		return;
  src    = (unsigned long*)( srcMem.Pixel1 );
  srcOfs = ( srcMem.Pitch1Y / sizeof( long )) - aWidth;

  /* prepare configuration of the DMA2D graphics accelerator */
  Accelerator.Init.Mode = DMA2D_M2M; 
  Accelerator.Init.ColorMode = DMA2D_ARGB8888;
  Accelerator.Init.OutputOffset = dstOfs;
  Accelerator.LayerCfg[1].InputOffset = srcOfs;
  Accelerator.LayerCfg[1].InputColorMode = CM_ARGB8888;
  Accelerator.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  Accelerator.LayerCfg[1].InputAlpha = 0;

  /* deinitialize the DMA2D graphics accelerator */
  HAL_DMA2D_DeInit( &Accelerator );

  /* initialize the DMA2D graphics accelerator */
  HAL_DMA2D_Init( &Accelerator );
  
  /* set the layer configurtation (foreground layer) */
  HAL_DMA2D_ConfigLayer( &Accelerator, 1 );

  /* start the transfer */
  HAL_DMA2D_Start( &Accelerator, (uint32_t)src, (uint32_t)dst, aWidth, aHeight );

  /* wait until transfer is done */	  
  if ( HAL_DMA2D_PollForTransfer( &Accelerator, 1000 ) != HAL_OK )
    EW_ERROR( Err05 );
}  


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
void STM32FxBlendDriver( unsigned long aDstHandle, unsigned long aSrcHandle,
  int aDstX, int aDstY, int aSrcX, int aSrcY, int aWidth, int aHeight,
  int aBlend, unsigned long* aColors )
{
  XSurfaceMemory dstMem;	
  unsigned long* dst;
  int            dstOfs;
  XSurfaceMemory srcMem;	
  void*          src;
  int            srcOfs;
  unsigned char  alpha = EW_ALPHA( aColors[0]);
  int            format = ((XSTM32FxSurface*)aSrcHandle)->Format;

  if ( !STM32FxLockSurface( aDstHandle, aDstX, aDstY, aWidth, aHeight, 0, 0, 0, 0, 0, 0, &dstMem ) )        
		return;
  dst    = (unsigned long*)( dstMem.Pixel1 );
  dstOfs = ( dstMem.Pitch1Y / sizeof( long )) - aWidth;

  if ( !STM32FxLockSurface( aSrcHandle, aSrcX, aSrcY, aWidth, aHeight, 0, 0, 0, 0, 0, 0, &srcMem ) )        
		return;
  src    = (void*)( srcMem.Pixel1 );

  if ( format == EW_PIXEL_FORMAT_NATIVE )
  {  
    srcOfs = ( srcMem.Pitch1Y / sizeof( long )) - aWidth;

    /* prepare configuration of the DMA2D graphics accelerator */
    Accelerator.Init.Mode = DMA2D_M2M_BLEND; 
    Accelerator.Init.ColorMode = DMA2D_ARGB8888;
    Accelerator.Init.OutputOffset = dstOfs;
    Accelerator.LayerCfg[0].InputOffset = dstOfs;
    Accelerator.LayerCfg[0].InputColorMode = CM_ARGB8888;
    Accelerator.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    Accelerator.LayerCfg[0].InputAlpha = 0;
    Accelerator.LayerCfg[1].InputOffset = srcOfs;
    Accelerator.LayerCfg[1].InputColorMode = CM_ARGB8888;
    Accelerator.LayerCfg[1].AlphaMode = (alpha == 0xFF) ? DMA2D_NO_MODIF_ALPHA : DMA2D_COMBINE_ALPHA;
    Accelerator.LayerCfg[1].InputAlpha = alpha;
  }  

  else if ( format == EW_PIXEL_FORMAT_ALPHA8 )
  {  
    unsigned long  color = ( EW_ALPHA( aColors[0]) << 24 ) 
                         | ( EW_RED  ( aColors[0]) << 16 ) 
                         | ( EW_GREEN( aColors[0]) <<  8 ) 
                         |   EW_BLUE ( aColors[0]);

    srcOfs = ( srcMem.Pitch1Y / sizeof( char )) - aWidth;

    /* prepare configuration of the DMA2D graphics accelerator */
    Accelerator.Init.Mode = DMA2D_M2M_BLEND; 
    Accelerator.Init.ColorMode = DMA2D_ARGB8888;
    Accelerator.Init.OutputOffset = dstOfs;
    Accelerator.LayerCfg[0].InputOffset = dstOfs;
    Accelerator.LayerCfg[0].InputColorMode = CM_ARGB8888;
    Accelerator.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    Accelerator.LayerCfg[0].InputAlpha = 0;
    Accelerator.LayerCfg[1].InputOffset = srcOfs;
    Accelerator.LayerCfg[1].InputColorMode = CM_A8;
    Accelerator.LayerCfg[1].AlphaMode = DMA2D_COMBINE_ALPHA;
    Accelerator.LayerCfg[1].InputAlpha = color;
  }  

  /* deinitialize the DMA2D graphics accelerator */
  HAL_DMA2D_DeInit( &Accelerator );

  /* initialize the DMA2D graphics accelerator */
  HAL_DMA2D_Init( &Accelerator );
  
  /* set the layer configurtation (foreground layer) */
  HAL_DMA2D_ConfigLayer( &Accelerator, 1 );

  /* set the layer configurtation (background layer) */
  HAL_DMA2D_ConfigLayer( &Accelerator, 0 );
    
  /* start the transfer */
  HAL_DMA2D_BlendingStart( &Accelerator, (uint32_t)src, (uint32_t)dst, (uint32_t)dst, aWidth, aHeight );

  /* wait until transfer is done */	  
  if ( HAL_DMA2D_PollForTransfer( &Accelerator, 1000 ) != HAL_OK )
    EW_ERROR( Err05 );
}  

#endif /* EW_USE_DMA2D_GRAPHICS_ACCELERATOR */

/* msy, pba */
