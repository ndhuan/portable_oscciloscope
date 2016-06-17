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

#ifndef _ChartsCoordList_H
#define _ChartsCoordList_H

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

/* Forward declaration of the class Charts::Coord */
#ifndef _ChartsCoord_
  EW_DECLARE_CLASS( ChartsCoord )
#define _ChartsCoord_
#endif

/* Forward declaration of the class Charts::CoordList */
#ifndef _ChartsCoordList_
  EW_DECLARE_CLASS( ChartsCoordList )
#define _ChartsCoordList_
#endif


/* The class 'CoordList' implements a list of X/Y coordinates used to describe a 
   complete graph. */
EW_DEFINE_FIELDS( ChartsCoordList, XObject )
  EW_VARIABLE( firstCoord,      ChartsCoord )
EW_END_OF_FIELDS( ChartsCoordList )

/* Virtual Method Table (VMT) for the class : 'Charts::CoordList' */
EW_DEFINE_METHODS( ChartsCoordList, XObject )
EW_END_OF_METHODS( ChartsCoordList )

#ifdef __cplusplus
  }
#endif

#endif /* _ChartsCoordList_H */

/* Embedded Wizard */
