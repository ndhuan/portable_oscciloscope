/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This file was generated automatically by Embedded Wizard. Please do not make 
* any modifications of this file! The modifications are lost when the file is
* generated again by Embedded Wizard!
*
* The template of this heading text can be found in the file 'head.ewt' in the
* directory 'Platforms' of your Embedded Wizard installation directory. If you
* wish to adapt this text, please copy the template file 'head.ewt' into your
* project directory and edit the copy only. Please avoid any modifications of
* the original template file!
*
* Version  : 7.10
* Profile  : STM32Fx
* Platform : STM.STM32Fx.RGBA8888
*
*******************************************************************************/

#ifndef Charts_H
#define Charts_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x0007000A
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x0007000A
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_ChartsCoord.h"
#include "_ChartsCoordList.h"
#include "_ChartsGraph.h"

/* Bitmap resources for a line segment of a chart curve. The image is used as alpha 
   bitmap in order to colorize the line segment dynamically during runtime. The 
   resource can be used by the @LineChart or @Graph class to draw the single line 
   pieces of the diagram. */
EW_DECLARE_BITMAP_RES( ChartsLine7x100 )

/* Bitmap resources for a dot of a chart curve. The image is used as alpha bitmap 
   in order to colorize the dot dynamically during runtime. The resource can be 
   used by the @LineChart or @Graph class to draw the single dots of a diagram. 
   The image resource contains several dots that can be selected with the frame 
   number property. */
EW_DECLARE_BITMAP_RES( ChartsDots8x8 )

#ifdef __cplusplus
  }
#endif

#endif /* Charts_H */

/* Embedded Wizard */
