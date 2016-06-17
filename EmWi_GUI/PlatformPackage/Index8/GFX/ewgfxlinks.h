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
*   This is an internal header of the Embedded Wizard Graphics Engine EWGFX. It
*   resolves the dependencies to the external graphics subsystem functionality
*   and replaces the missing functionality by Embedded Wizard own software pixel
*   driver.
*
*   The product of this header file is a set of arrays containing pointers to
*   external drawing functions or to the functions of the software pixel driver.
*
*   This header file also performs a rudimentary integrity test of the external
*   drawing and surface adaptation. If there are incomplete functionality, the
*   'C' preprocessor will report error messages.
*
*******************************************************************************/


/* Support of a platform specific native surface? */
#if defined EwGfxCreateNativeSurface ||                                        \
    defined EwGfxCreateConstNativeSurface ||                                   \
    defined EwGfxDestroyNativeSurface ||                                       \
    defined EwGfxLockNativeSurface ||                                          \
    defined EwGfxUnlockNativeSurface
  #define EwProvideNativeSurface
#endif


/* Support of a platform specific index8 surface? */
#if defined EwGfxCreateIndex8Surface ||                                        \
    defined EwGfxCreateConstIndex8Surface ||                                   \
    defined EwGfxDestroyIndex8Surface ||                                       \
    defined EwGfxLockIndex8Surface ||                                          \
    defined EwGfxUnlockIndex8Surface
  #define EwProvideIndex8Surface
#endif


/* Support of a platform specific alpha8 surface? */
#if defined EwGfxCreateAlpha8Surface ||                                        \
    defined EwGfxCreateConstAlpha8Surface ||                                   \
    defined EwGfxDestroyAlpha8Surface ||                                       \
    defined EwGfxLockAlpha8Surface ||                                          \
    defined EwGfxUnlockAlpha8Surface
  #define EwProvideAlpha8Surface
#endif


/* Verify the integrity of the native surface adaptation */
#if defined EwProvideNativeSurface &&                                          \
    (                                                                          \
      !defined EwGfxCreateNativeSurface ||                                     \
      !defined EwGfxDestroyNativeSurface ||                                    \
      !defined EwGfxLockNativeSurface ||                                       \
      !defined EwGfxUnlockNativeSurface                                        \
    )
  #error "Incomplete native surface support. Some functions are missing."
#endif


/* Verify the integrity of the index8 surface adaptation */
#if defined EwProvideIndex8Surface &&                                          \
    (                                                                          \
      !defined EwGfxCreateIndex8Surface ||                                     \
      !defined EwGfxDestroyIndex8Surface ||                                    \
      !defined EwGfxLockIndex8Surface ||                                       \
      !defined EwGfxUnlockIndex8Surface                                        \
    )
  #error "Incomplete index8 surface support. Some functions are missing."
#endif


/* Verify the integrity of the native surface adaptation */
#if defined EwProvideAlpha8Surface &&                                          \
    (                                                                          \
      !defined EwGfxCreateAlpha8Surface ||                                     \
      !defined EwGfxDestroyAlpha8Surface ||                                    \
      !defined EwGfxLockAlpha8Surface ||                                       \
      !defined EwGfxUnlockAlpha8Surface                                        \
    )
  #error "Incomplete alpha8 surface support. Some functions are missing."
#endif


/* Platform specific drawing operations involving native surface as source and
   destination? */
#if defined EwGfxDrawLineGradient ||                                           \
    defined EwGfxDrawLineGradientBlend ||                                      \
    defined EwGfxFillGradient ||                                               \
    defined EwGfxFillGradientBlend ||                                          \
    defined EwGfxCopyNative ||                                                 \
    defined EwGfxCopyNativeBlend ||                                            \
    defined EwGfxCopyNativeGradient ||                                         \
    defined EwGfxCopyNativeGradientBlend ||                                    \
    defined EwGfxTileNative ||                                                 \
    defined EwGfxTileNativeBlend ||                                            \
    defined EwGfxTileNativeSolid ||                                            \
    defined EwGfxTileNativeSolidBlend ||                                       \
    defined EwGfxTileNativeGradient ||                                         \
    defined EwGfxTileNativeGradientBlend ||                                    \
    defined EwGfxWarpPerspNative ||                                            \
    defined EwGfxWarpPerspNativeBlend ||                                       \
    defined EwGfxWarpPerspNativeGradient ||                                    \
    defined EwGfxWarpPerspNativeGradientBlend ||                               \
    defined EwGfxWarpPerspNativeFilter ||                                      \
    defined EwGfxWarpPerspNativeFilterBlend ||                                 \
    defined EwGfxWarpPerspNativeFilterGradient ||                              \
    defined EwGfxWarpPerspNativeFilterGradientBlend ||                         \
    defined EwGfxWarpAffineNative ||                                           \
    defined EwGfxWarpAffineNativeBlend ||                                      \
    defined EwGfxWarpAffineNativeGradient ||                                   \
    defined EwGfxWarpAffineNativeGradientBlend ||                              \
    defined EwGfxWarpAffineNativeFilter ||                                     \
    defined EwGfxWarpAffineNativeFilterBlend ||                                \
    defined EwGfxWarpAffineNativeFilterGradient ||                             \
    defined EwGfxWarpAffineNativeFilterGradientBlend ||                        \
    defined EwGfxScaleNative ||                                                \
    defined EwGfxScaleNativeBlend ||                                           \
    defined EwGfxScaleNativeGradient ||                                        \
    defined EwGfxScaleNativeGradientBlend ||                                   \
    defined EwGfxScaleNativeFilter ||                                          \
    defined EwGfxScaleNativeFilterBlend ||                                     \
    defined EwGfxScaleNativeFilterGradient ||                                  \
    defined EwGfxScaleNativeFilterGradientBlend
  #define EwNeedNativeSurface
#endif


/* Platform specific drawing operations involving index8 surface as source and
   the native surface as destination? */
#if defined EwGfxCopyIndex8 ||                                                 \
    defined EwGfxCopyIndex8Blend ||                                            \
    defined EwGfxCopyIndex8Gradient ||                                         \
    defined EwGfxCopyIndex8GradientBlend ||                                    \
    defined EwGfxTileIndex8 ||                                                 \
    defined EwGfxTileIndex8Blend ||                                            \
    defined EwGfxTileIndex8Solid ||                                            \
    defined EwGfxTileIndex8SolidBlend ||                                       \
    defined EwGfxTileIndex8Gradient ||                                         \
    defined EwGfxTileIndex8GradientBlend ||                                    \
    defined EwGfxWarpPerspIndex8 ||                                            \
    defined EwGfxWarpPerspIndex8Blend ||                                       \
    defined EwGfxWarpPerspIndex8Gradient ||                                    \
    defined EwGfxWarpPerspIndex8GradientBlend ||                               \
    defined EwGfxWarpPerspIndex8Filter ||                                      \
    defined EwGfxWarpPerspIndex8FilterBlend ||                                 \
    defined EwGfxWarpPerspIndex8FilterGradient ||                              \
    defined EwGfxWarpPerspIndex8FilterGradientBlend ||                         \
    defined EwGfxWarpAffineIndex8 ||                                           \
    defined EwGfxWarpAffineIndex8Blend ||                                      \
    defined EwGfxWarpAffineIndex8Gradient ||                                   \
    defined EwGfxWarpAffineIndex8GradientBlend ||                              \
    defined EwGfxWarpAffineIndex8Filter ||                                     \
    defined EwGfxWarpAffineIndex8FilterBlend ||                                \
    defined EwGfxWarpAffineIndex8FilterGradient ||                             \
    defined EwGfxWarpAffineIndex8FilterGradientBlend ||                        \
    defined EwGfxScaleIndex8 ||                                                \
    defined EwGfxScaleIndex8Blend ||                                           \
    defined EwGfxScaleIndex8Gradient ||                                        \
    defined EwGfxScaleIndex8GradientBlend ||                                   \
    defined EwGfxScaleIndex8Filter ||                                          \
    defined EwGfxScaleIndex8FilterBlend ||                                     \
    defined EwGfxScaleIndex8FilterGradient ||                                  \
    defined EwGfxScaleIndex8FilterGradientBlend
  #define EwNeedNativeSurface
  #define EwNeedIndex8Surface
#endif


/* Platform specific drawing operations involving alpha8 surface as source and
   the native surface as destination? */
#if defined EwGfxCopyAlpha8Gradient ||                                         \
    defined EwGfxCopyAlpha8GradientBlend ||                                    \
    defined EwGfxTileAlpha8Solid ||                                            \
    defined EwGfxTileAlpha8SolidBlend ||                                       \
    defined EwGfxTileAlpha8Gradient ||                                         \
    defined EwGfxTileAlpha8GradientBlend ||                                    \
    defined EwGfxWarpPerspAlpha8Gradient ||                                    \
    defined EwGfxWarpPerspAlpha8GradientBlend ||                               \
    defined EwGfxWarpPerspAlpha8FilterGradient ||                              \
    defined EwGfxWarpPerspAlpha8FilterGradientBlend ||                         \
    defined EwGfxWarpAffineAlpha8Gradient ||                                   \
    defined EwGfxWarpAffineAlpha8GradientBlend ||                              \
    defined EwGfxWarpAffineAlpha8FilterGradient ||                             \
    defined EwGfxWarpAffineAlpha8FilterGradientBlend ||                        \
    defined EwGfxScaleAlpha8Gradient ||                                        \
    defined EwGfxScaleAlpha8GradientBlend ||                                   \
    defined EwGfxScaleAlpha8FilterGradient ||                                  \
    defined EwGfxScaleAlpha8FilterGradientBlend
  #define EwNeedNativeSurface
  #define EwNeedAlpha8Surface
#endif


/* Verify the integrity of the adaptation of native drawing operations */
#if defined EwNeedNativeSurface && !defined EwProvideNativeSurface
  #error "Native surface drawing operations without native surface support."
#endif


/* Verify the integrity of the adaptation of index8 drawing operations */
#if defined EwNeedIndex8Surface && !defined EwProvideIndex8Surface
  #error "Index8 surface drawing operations without index8 surface support."
#endif


/* Verify the integrity of the adaptation of alpha8 drawing operations */
#if defined EwNeedAlpha8Surface && !defined EwProvideAlpha8Surface
  #error "Alpha8 drawing operations found but no alpha8 surface support."
#endif


/* Verify the integrity of the framebuffer/display adaptation */
#if !defined EwGfxInitViewport || !defined EwGfxDoneViewport
  #error "Incomplete viewport adaptation."
#endif

#if !defined EwGfxBeginUpdate || !defined EwGfxEndUpdate
  #error "Incomplete viewport adaptation."
#endif


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'draw a line' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxDrawLineGradient
  #define EwSetPixelSolid            0
#else
  #define EwGfxDrawLineGradient      0
#endif

#ifdef EwGfxDrawLineGradientBlend
  #define EwSetPixelSolidBlend       0
#else
  #define EwGfxDrawLineGradientBlend 0
#endif

#ifndef EwGfxDrawLineSolid
  #define EwGfxDrawLineSolid         EwGfxDrawLineGradient
#endif

#ifndef EwGfxDrawLineSolidBlend
  #define EwGfxDrawLineSolidBlend    EwGfxDrawLineGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XLineDriver LineDrivers[] =
{
  0, 0,
  EwGfxDrawLineSolid,                EwGfxDrawLineSolidBlend, 
  EwGfxDrawLineGradient,             EwGfxDrawLineGradientBlend
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XLineWorker LineWorkers[] =
{
  0, 0,
  EwSetPixelSolid,                  EwSetPixelSolidBlend, 
  EwSetPixelSolid,                  EwSetPixelSolidBlend
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'fill rectangle' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxFillGradient
  #define EwFillRowGradient         0
  #define EwFillRowSolid            0
#else
  #define EwGfxFillGradient         0
#endif

#ifdef EwGfxFillGradientBlend
  #define EwFillRowGradientBlend    0
  #define EwFillRowSolidBlend       0
#else
  #define EwGfxFillGradientBlend    0
#endif

#ifndef EwGfxFillSolid
  #define EwGfxFillSolid            EwGfxFillGradient
#endif

#ifndef EwGfxFillSolidBlend
  #define EwGfxFillSolidBlend       EwGfxFillGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XFillDriver FillDrivers[] =
{
  0, 0,
  EwGfxFillSolid,                   EwGfxFillSolidBlend, 
  EwGfxFillGradient,                EwGfxFillGradientBlend
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XFillWorker FillWorkers[] =
{
  0, 0,
  EwFillRowSolid,                   EwFillRowSolidBlend, 
  EwFillRowGradient,                EwFillRowGradientBlend
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'copy surface' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxCopyNative
  #define EwCopyNativeRow                 0
#else
  #define EwGfxCopyNative                 0
#endif

#ifdef EwGfxCopyNativeBlend
  #define EwCopyNativeRowBlend            0
#else
  #define EwGfxCopyNativeBlend            0
#endif

#ifdef EwGfxCopyNativeGradient
  #define EwCopyNativeRowGradient         0
  #define EwCopyNativeRowSolid            0
#else
  #define EwGfxCopyNativeGradient         0
#endif

#ifdef EwGfxCopyNativeGradientBlend
  #define EwCopyNativeRowGradientBlend    0
  #define EwCopyNativeRowSolidBlend       0
#else
  #define EwGfxCopyNativeGradientBlend    0
#endif

#ifndef EwGfxCopyNativeSolid
  #define EwGfxCopyNativeSolid            EwGfxCopyNativeGradient
#endif

#ifndef EwGfxCopyNativeSolidBlend
  #define EwGfxCopyNativeSolidBlend       EwGfxCopyNativeGradientBlend
#endif

#ifdef EwGfxCopyIndex8
  #define EwCopyIndex8Row                 0
#else
  #define EwGfxCopyIndex8                 0
#endif

#ifdef EwGfxCopyIndex8Blend
  #define EwCopyIndex8RowBlend            0
#else
  #define EwGfxCopyIndex8Blend            0
#endif

#ifdef EwGfxCopyIndex8Gradient
  #define EwCopyIndex8RowGradient         0
  #define EwCopyIndex8RowSolid            0
#else
  #define EwGfxCopyIndex8Gradient         0
#endif

#ifdef EwGfxCopyIndex8GradientBlend
  #define EwCopyIndex8RowGradientBlend    0
  #define EwCopyIndex8RowSolidBlend       0
#else
  #define EwGfxCopyIndex8GradientBlend    0
#endif

#ifndef EwGfxCopyIndex8Solid
  #define EwGfxCopyIndex8Solid            EwGfxCopyIndex8Gradient
#endif

#ifndef EwGfxCopyIndex8SolidBlend
  #define EwGfxCopyIndex8SolidBlend       EwGfxCopyIndex8GradientBlend
#endif

#ifdef EwGfxCopyAlpha8Gradient
  #define EwCopyAlpha8RowGradient         0
  #define EwCopyAlpha8RowSolid            0
#else
  #define EwGfxCopyAlpha8Gradient         0
#endif

#ifdef EwGfxCopyAlpha8GradientBlend
  #define EwCopyAlpha8RowGradientBlend    0
  #define EwCopyAlpha8RowSolidBlend       0
#else
  #define EwGfxCopyAlpha8GradientBlend    0
#endif

#ifndef EwGfxCopyAlpha8Solid
  #define EwGfxCopyAlpha8Solid            EwGfxCopyAlpha8Gradient
#endif

#ifndef EwGfxCopyAlpha8SolidBlend
  #define EwGfxCopyAlpha8SolidBlend       EwGfxCopyAlpha8GradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XCopyDriver CopyDrivers[] =
{
  EwGfxCopyNative,                  EwGfxCopyNativeBlend,
  EwGfxCopyNativeSolid,             EwGfxCopyNativeSolidBlend,
  EwGfxCopyNativeGradient,          EwGfxCopyNativeGradientBlend,
  0,                                0,
  EwGfxCopyIndex8,                  EwGfxCopyIndex8Blend,
  EwGfxCopyIndex8Solid,             EwGfxCopyIndex8SolidBlend,
  EwGfxCopyIndex8Gradient,          EwGfxCopyIndex8GradientBlend,
  0,                                0,
  0,                                0,
  EwGfxCopyAlpha8Solid,             EwGfxCopyAlpha8SolidBlend,
  EwGfxCopyAlpha8Gradient,          EwGfxCopyAlpha8GradientBlend
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XCopyWorker CopyWorkers[] =
{
  EwCopyNativeRow,                  EwCopyNativeRowBlend,
  EwCopyNativeRowSolid,             EwCopyNativeRowSolidBlend,
  EwCopyNativeRowGradient,          EwCopyNativeRowGradientBlend,
  0,                                0,
  EwCopyIndex8Row,                  EwCopyIndex8RowBlend,
  EwCopyIndex8RowSolid,             EwCopyIndex8RowSolidBlend,
  EwCopyIndex8RowGradient,          EwCopyIndex8RowGradientBlend,
  0,                                0,
  0,                                0,
  EwCopyAlpha8RowSolid,             EwCopyAlpha8RowSolidBlend,
  EwCopyAlpha8RowGradient,          EwCopyAlpha8RowGradientBlend
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'tile surface' operation. For missing tile
   functionality multiple copy operations will be used. */
#ifndef EwGfxTileNative
  #define EwGfxTileNative              0
#endif

#ifndef EwGfxTileNativeBlend
  #define EwGfxTileNativeBlend         0
#endif

#ifndef EwGfxTileNativeSolid
  #define EwGfxTileNativeSolid         0
#endif

#ifndef EwGfxTileNativeSolidBlend
  #define EwGfxTileNativeSolidBlend    0
#endif

#ifndef EwGfxTileNativeGradient
  #define EwGfxTileNativeGradient      0
#endif

#ifndef EwGfxTileNativeGradientBlend
  #define EwGfxTileNativeGradientBlend 0
#endif

#ifndef EwGfxTileIndex8
  #define EwGfxTileIndex8              0
#endif

#ifndef EwGfxTileIndex8Blend
  #define EwGfxTileIndex8Blend         0
#endif

#ifndef EwGfxTileIndex8Solid
  #define EwGfxTileIndex8Solid         0
#endif

#ifndef EwGfxTileIndex8SolidBlend
  #define EwGfxTileIndex8SolidBlend    0
#endif

#ifndef EwGfxTileIndex8Gradient
  #define EwGfxTileIndex8Gradient      0
#endif

#ifndef EwGfxTileIndex8GradientBlend
  #define EwGfxTileIndex8GradientBlend 0
#endif

#ifndef EwGfxTileAlpha8Solid
  #define EwGfxTileAlpha8Solid         0
#endif

#ifndef EwGfxTileAlpha8SolidBlend
  #define EwGfxTileAlpha8SolidBlend    0
#endif

#ifndef EwGfxTileAlpha8Gradient
  #define EwGfxTileAlpha8Gradient      0
#endif

#ifndef EwGfxTileAlpha8GradientBlend
  #define EwGfxTileAlpha8GradientBlend 0
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XTileDriver TileDrivers[] =
{
  EwGfxTileNative,                  EwGfxTileNativeBlend,
  EwGfxTileNativeSolid,             EwGfxTileNativeSolidBlend,
  EwGfxTileNativeGradient,          EwGfxTileNativeGradientBlend,
  0,                                0,
  EwGfxTileIndex8,                  EwGfxTileIndex8Blend,
  EwGfxTileIndex8Solid,             EwGfxTileIndex8SolidBlend,
  EwGfxTileIndex8Gradient,          EwGfxTileIndex8GradientBlend,
  0,                                0,
  0,                                0,
  EwGfxTileAlpha8Solid,             EwGfxTileAlpha8SolidBlend,
  EwGfxTileAlpha8Gradient,          EwGfxTileAlpha8GradientBlend
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'warp perspective surface' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxWarpPerspNative
  #define EwWarpPerspNativeRow                       0
#else
  #define EwGfxWarpPerspNative                       0
  #define EwWarpPerspNativeRow                       EwWarpNativeRow
#endif

#ifdef EwGfxWarpPerspNativeBlend
  #define EwWarpPerspNativeRowBlend                  0
#else
  #define EwGfxWarpPerspNativeBlend                  0
  #define EwWarpPerspNativeRowBlend                  EwWarpNativeRowBlend
#endif

#ifdef EwGfxWarpPerspNativeGradient
  #define EwWarpPerspNativeRowGradient               0
#else
  #define EwGfxWarpPerspNativeGradient               0
  #define EwWarpPerspNativeRowGradient               EwWarpNativeRowGradient
#endif

#ifdef EwGfxWarpPerspNativeGradientBlend
  #define EwWarpPerspNativeRowGradientBlend          0
#else
  #define EwGfxWarpPerspNativeGradientBlend          0
  #define EwWarpPerspNativeRowGradientBlend \
    EwWarpNativeRowGradientBlend
#endif

#ifdef EwGfxWarpPerspNativeFilter
  #define EwWarpPerspNativeRowFilter                 0
#else
  #define EwGfxWarpPerspNativeFilter                 0
  #define EwWarpPerspNativeRowFilter                 EwWarpNativeRowFilter
#endif

#ifdef EwGfxWarpPerspNativeFilterBlend
  #define EwWarpPerspNativeRowFilterBlend            0
#else
  #define EwGfxWarpPerspNativeFilterBlend            0
  #define EwWarpPerspNativeRowFilterBlend            EwWarpNativeRowFilterBlend
#endif

#ifdef EwGfxWarpPerspNativeFilterGradient
  #define EwWarpPerspNativeRowFilterGradient         0
#else
  #define EwGfxWarpPerspNativeFilterGradient         0
  #define EwWarpPerspNativeRowFilterGradient \
    EwWarpNativeRowFilterGradient
#endif

#ifdef EwGfxWarpPerspNativeFilterGradientBlend
  #define EwWarpPerspNativeRowFilterGradientBlend    0
#else
  #define EwGfxWarpPerspNativeFilterGradientBlend    0
  #define EwWarpPerspNativeRowFilterGradientBlend \
    EwWarpNativeRowFilterGradientBlend
#endif

#ifndef EwGfxWarpPerspNativeSolid
  #define EwGfxWarpPerspNativeSolid          EwGfxWarpPerspNativeGradient
#endif

#ifndef EwGfxWarpPerspNativeSolidBlend
  #define EwGfxWarpPerspNativeSolidBlend     EwGfxWarpPerspNativeGradientBlend
#endif

#ifndef EwGfxWarpPerspNativeFilterSolid
  #define EwGfxWarpPerspNativeFilterSolid    EwGfxWarpPerspNativeFilterGradient
#endif

#ifndef EwGfxWarpPerspNativeFilterSolidBlend
  #define EwGfxWarpPerspNativeFilterSolidBlend \
    EwGfxWarpPerspNativeFilterGradientBlend
#endif

#ifdef EwGfxWarpPerspIndex8
  #define EwWarpPerspIndex8Row                       0
#else
  #define EwGfxWarpPerspIndex8                       0
  #define EwWarpPerspIndex8Row                       EwWarpIndex8Row
#endif

#ifdef EwGfxWarpPerspIndex8Blend
  #define EwWarpPerspIndex8RowBlend                  0
#else
  #define EwGfxWarpPerspIndex8Blend                  0
  #define EwWarpPerspIndex8RowBlend                  EwWarpIndex8RowBlend
#endif

#ifdef EwGfxWarpPerspIndex8Gradient
  #define EwWarpPerspIndex8RowGradient               0
#else
  #define EwGfxWarpPerspIndex8Gradient               0
  #define EwWarpPerspIndex8RowGradient               EwWarpIndex8RowGradient
#endif

#ifdef EwGfxWarpPerspIndex8GradientBlend
  #define EwWarpPerspIndex8RowGradientBlend          0
#else
  #define EwGfxWarpPerspIndex8GradientBlend          0
  #define EwWarpPerspIndex8RowGradientBlend \
    EwWarpIndex8RowGradientBlend
#endif

#ifdef EwGfxWarpPerspIndex8Filter
  #define EwWarpPerspIndex8RowFilter                 0
#else
  #define EwGfxWarpPerspIndex8Filter                 0
  #define EwWarpPerspIndex8RowFilter                 EwWarpIndex8RowFilter
#endif

#ifdef EwGfxWarpPerspIndex8FilterBlend
  #define EwWarpPerspIndex8RowFilterBlend            0
#else
  #define EwGfxWarpPerspIndex8FilterBlend            0
  #define EwWarpPerspIndex8RowFilterBlend            EwWarpIndex8RowFilterBlend
#endif

#ifdef EwGfxWarpPerspIndex8FilterGradient
  #define EwWarpPerspIndex8RowFilterGradient         0
#else
  #define EwGfxWarpPerspIndex8FilterGradient         0
  #define EwWarpPerspIndex8RowFilterGradient \
    EwWarpIndex8RowFilterGradient
#endif

#ifdef EwGfxWarpPerspIndex8FilterGradientBlend
  #define EwWarpPerspIndex8RowFilterGradientBlend    0
#else
  #define EwGfxWarpPerspIndex8FilterGradientBlend    0
  #define EwWarpPerspIndex8RowFilterGradientBlend \
    EwWarpIndex8RowFilterGradientBlend
#endif

#ifndef EwGfxWarpPerspIndex8Solid
  #define EwGfxWarpPerspIndex8Solid          EwGfxWarpPerspIndex8Gradient
#endif

#ifndef EwGfxWarpPerspIndex8SolidBlend
  #define EwGfxWarpPerspIndex8SolidBlend     EwGfxWarpPerspIndex8GradientBlend
#endif

#ifndef EwGfxWarpPerspIndex8FilterSolid
  #define EwGfxWarpPerspIndex8FilterSolid    EwGfxWarpPerspIndex8FilterGradient
#endif

#ifndef EwGfxWarpPerspIndex8FilterSolidBlend
  #define EwGfxWarpPerspIndex8FilterSolidBlend \
    EwGfxWarpPerspIndex8FilterGradientBlend
#endif

#ifdef EwGfxWarpPerspAlpha8Gradient
  #define EwWarpPerspAlpha8RowGradient               0
#else
  #define EwGfxWarpPerspAlpha8Gradient               0
  #define EwWarpPerspAlpha8RowGradient               EwWarpAlpha8RowGradient
#endif

#ifdef EwGfxWarpPerspAlpha8GradientBlend
  #define EwWarpPerspAlpha8RowGradientBlend          0
#else
  #define EwGfxWarpPerspAlpha8GradientBlend          0
  #define EwWarpPerspAlpha8RowGradientBlend \
    EwWarpAlpha8RowGradientBlend
#endif

#ifdef EwGfxWarpPerspAlpha8FilterGradient
  #define EwWarpPerspAlpha8RowFilterGradient         0
#else
  #define EwGfxWarpPerspAlpha8FilterGradient         0
  #define EwWarpPerspAlpha8RowFilterGradient \
    EwWarpAlpha8RowFilterGradient
#endif

#ifdef EwGfxWarpPerspAlpha8FilterGradientBlend
  #define EwWarpPerspAlpha8RowFilterGradientBlend    0
#else
  #define EwGfxWarpPerspAlpha8FilterGradientBlend    0
  #define EwWarpPerspAlpha8RowFilterGradientBlend \
    EwWarpAlpha8RowFilterGradientBlend
#endif

#ifndef EwGfxWarpPerspAlpha8Solid
  #define EwGfxWarpPerspAlpha8Solid          EwGfxWarpPerspAlpha8Gradient
#endif

#ifndef EwGfxWarpPerspAlpha8SolidBlend
  #define EwGfxWarpPerspAlpha8SolidBlend     EwGfxWarpPerspAlpha8GradientBlend
#endif

#ifndef EwGfxWarpPerspAlpha8FilterSolid
  #define EwGfxWarpPerspAlpha8FilterSolid    EwGfxWarpPerspAlpha8FilterGradient
#endif

#ifndef EwGfxWarpPerspAlpha8FilterSolidBlend
  #define EwGfxWarpPerspAlpha8FilterSolidBlend \
    EwGfxWarpPerspAlpha8FilterGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XWarpDriver WarpPerspDrivers[] =
{
  EwGfxWarpPerspNative,                EwGfxWarpPerspNativeBlend,
  EwGfxWarpPerspNativeSolid,           EwGfxWarpPerspNativeSolidBlend,
  EwGfxWarpPerspNativeGradient,        EwGfxWarpPerspNativeGradientBlend,
  0,                                   0,
  EwGfxWarpPerspNativeFilter,          EwGfxWarpPerspNativeFilterBlend,
  EwGfxWarpPerspNativeFilterSolid,     EwGfxWarpPerspNativeFilterSolidBlend,
  EwGfxWarpPerspNativeFilterGradient,  EwGfxWarpPerspNativeFilterGradientBlend,
  0,                                   0,
  EwGfxWarpPerspIndex8,                EwGfxWarpPerspIndex8Blend,
  EwGfxWarpPerspIndex8Solid,           EwGfxWarpPerspIndex8SolidBlend,
  EwGfxWarpPerspIndex8Gradient,        EwGfxWarpPerspIndex8GradientBlend,
  0,                                   0,
  EwGfxWarpPerspIndex8Filter,          EwGfxWarpPerspIndex8FilterBlend,
  EwGfxWarpPerspIndex8FilterSolid,     EwGfxWarpPerspIndex8FilterSolidBlend,
  EwGfxWarpPerspIndex8FilterGradient,  EwGfxWarpPerspIndex8FilterGradientBlend,
  0,                                   0,
  0,                                   0,
  EwGfxWarpPerspAlpha8Solid,           EwGfxWarpPerspAlpha8SolidBlend,
  EwGfxWarpPerspAlpha8Gradient,        EwGfxWarpPerspAlpha8GradientBlend,
  0,                                   0,
  0,                                   0,
  EwGfxWarpPerspAlpha8FilterSolid,     EwGfxWarpPerspAlpha8FilterSolidBlend,
  EwGfxWarpPerspAlpha8FilterGradient,  EwGfxWarpPerspAlpha8FilterGradientBlend
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XWarpWorker WarpPerspWorkers[] =
{
  EwWarpPerspNativeRow,                EwWarpPerspNativeRowBlend,
  EwWarpPerspNativeRowGradient,        EwWarpPerspNativeRowGradientBlend,
  EwWarpPerspNativeRowGradient,        EwWarpPerspNativeRowGradientBlend,
  0,                                   0,
  EwWarpPerspNativeRowFilter,          EwWarpPerspNativeRowFilterBlend,
  EwWarpPerspNativeRowFilterGradient,  EwWarpPerspNativeRowFilterGradientBlend,
  EwWarpPerspNativeRowFilterGradient,  EwWarpPerspNativeRowFilterGradientBlend,
  0,                                   0,
  EwWarpPerspIndex8Row,                EwWarpPerspIndex8RowBlend,
  EwWarpPerspIndex8RowGradient,        EwWarpPerspIndex8RowGradientBlend,
  EwWarpPerspIndex8RowGradient,        EwWarpPerspIndex8RowGradientBlend,
  0,                                   0,
  EwWarpPerspIndex8RowFilter,          EwWarpPerspIndex8RowFilterBlend,
  EwWarpPerspIndex8RowFilterGradient,  EwWarpPerspIndex8RowFilterGradientBlend,
  EwWarpPerspIndex8RowFilterGradient,  EwWarpPerspIndex8RowFilterGradientBlend,
  0,                                   0,
  0,                                   0,
  EwWarpPerspAlpha8RowGradient,        EwWarpPerspAlpha8RowGradientBlend,
  EwWarpPerspAlpha8RowGradient,        EwWarpPerspAlpha8RowGradientBlend,
  0,                                   0,
  0,                                   0,
  EwWarpPerspAlpha8RowFilterGradient,  EwWarpPerspAlpha8RowFilterGradientBlend,
  EwWarpPerspAlpha8RowFilterGradient,  EwWarpPerspAlpha8RowFilterGradientBlend
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'warp affine surface' operation.
   Missing functionality will be emulated by the software. */
#ifdef EwGfxWarpAffineNative
  #define EwWarpAffineNativeRow                       0
#else
  #define EwGfxWarpAffineNative                       0
  #define EwWarpAffineNativeRow                       EwWarpNativeRow
#endif

#ifdef EwGfxWarpAffineNativeBlend
  #define EwWarpAffineNativeRowBlend                  0
#else
  #define EwGfxWarpAffineNativeBlend                  0
  #define EwWarpAffineNativeRowBlend                  EwWarpNativeRowBlend
#endif

#ifdef EwGfxWarpAffineNativeGradient
  #define EwWarpAffineNativeRowGradient               0
#else
  #define EwGfxWarpAffineNativeGradient               0
  #define EwWarpAffineNativeRowGradient               EwWarpNativeRowGradient
#endif

#ifdef EwGfxWarpAffineNativeGradientBlend
  #define EwWarpAffineNativeRowGradientBlend          0
#else
  #define EwGfxWarpAffineNativeGradientBlend          0
  #define EwWarpAffineNativeRowGradientBlend \
    EwWarpNativeRowGradientBlend
#endif

#ifdef EwGfxWarpAffineNativeFilter
  #define EwWarpAffineNativeRowFilter                 0
#else
  #define EwGfxWarpAffineNativeFilter                 0
  #define EwWarpAffineNativeRowFilter                 EwWarpNativeRowFilter
#endif

#ifdef EwGfxWarpAffineNativeFilterBlend
  #define EwWarpAffineNativeRowFilterBlend            0
#else
  #define EwGfxWarpAffineNativeFilterBlend            0
  #define EwWarpAffineNativeRowFilterBlend            EwWarpNativeRowFilterBlend
#endif

#ifdef EwGfxWarpAffineNativeFilterGradient
  #define EwWarpAffineNativeRowFilterGradient         0
#else
  #define EwGfxWarpAffineNativeFilterGradient         0
  #define EwWarpAffineNativeRowFilterGradient \
    EwWarpNativeRowFilterGradient
#endif

#ifdef EwGfxWarpAffineNativeFilterGradientBlend
  #define EwWarpAffineNativeRowFilterGradientBlend    0
#else
  #define EwGfxWarpAffineNativeFilterGradientBlend    0
  #define EwWarpAffineNativeRowFilterGradientBlend \
    EwWarpNativeRowFilterGradientBlend
#endif

#ifndef EwGfxWarpAffineNativeSolid
  #define EwGfxWarpAffineNativeSolid        EwGfxWarpAffineNativeGradient
#endif

#ifndef EwGfxWarpAffineNativeSolidBlend
  #define EwGfxWarpAffineNativeSolidBlend   EwGfxWarpAffineNativeGradientBlend
#endif

#ifndef EwGfxWarpAffineNativeFilterSolid
  #define EwGfxWarpAffineNativeFilterSolid  EwGfxWarpAffineNativeFilterGradient
#endif

#ifndef EwGfxWarpAffineNativeFilterSolidBlend
  #define EwGfxWarpAffineNativeFilterSolidBlend \
    EwGfxWarpAffineNativeFilterGradientBlend
#endif

#ifdef EwGfxWarpAffineIndex8
  #define EwWarpAffineIndex8Row                       0
#else
  #define EwGfxWarpAffineIndex8                       0
  #define EwWarpAffineIndex8Row                       EwWarpIndex8Row
#endif

#ifdef EwGfxWarpAffineIndex8Blend
  #define EwWarpAffineIndex8RowBlend                  0
#else
  #define EwGfxWarpAffineIndex8Blend                  0
  #define EwWarpAffineIndex8RowBlend                  EwWarpIndex8RowBlend
#endif

#ifdef EwGfxWarpAffineIndex8Gradient
  #define EwWarpAffineIndex8RowGradient               0
#else
  #define EwGfxWarpAffineIndex8Gradient               0
  #define EwWarpAffineIndex8RowGradient               EwWarpIndex8RowGradient
#endif

#ifdef EwGfxWarpAffineIndex8GradientBlend
  #define EwWarpAffineIndex8RowGradientBlend          0
#else
  #define EwGfxWarpAffineIndex8GradientBlend          0
  #define EwWarpAffineIndex8RowGradientBlend \
    EwWarpIndex8RowGradientBlend
#endif

#ifdef EwGfxWarpAffineIndex8Filter
  #define EwWarpAffineIndex8RowFilter                 0
#else
  #define EwGfxWarpAffineIndex8Filter                 0
  #define EwWarpAffineIndex8RowFilter                 EwWarpIndex8RowFilter
#endif

#ifdef EwGfxWarpAffineIndex8FilterBlend
  #define EwWarpAffineIndex8RowFilterBlend            0
#else
  #define EwGfxWarpAffineIndex8FilterBlend            0
  #define EwWarpAffineIndex8RowFilterBlend            EwWarpIndex8RowFilterBlend
#endif

#ifdef EwGfxWarpAffineIndex8FilterGradient
  #define EwWarpAffineIndex8RowFilterGradient         0
#else
  #define EwGfxWarpAffineIndex8FilterGradient         0
  #define EwWarpAffineIndex8RowFilterGradient \
    EwWarpIndex8RowFilterGradient
#endif

#ifdef EwGfxWarpAffineIndex8FilterGradientBlend
  #define EwWarpAffineIndex8RowFilterGradientBlend    0
#else
  #define EwGfxWarpAffineIndex8FilterGradientBlend    0
  #define EwWarpAffineIndex8RowFilterGradientBlend  \
    EwWarpIndex8RowFilterGradientBlend
#endif

#ifndef EwGfxWarpAffineIndex8Solid
  #define EwGfxWarpAffineIndex8Solid        EwGfxWarpAffineIndex8Gradient
#endif

#ifndef EwGfxWarpAffineIndex8SolidBlend
  #define EwGfxWarpAffineIndex8SolidBlend   EwGfxWarpAffineIndex8GradientBlend
#endif

#ifndef EwGfxWarpAffineIndex8FilterSolid
  #define EwGfxWarpAffineIndex8FilterSolid  EwGfxWarpAffineIndex8FilterGradient
#endif

#ifndef EwGfxWarpAffineIndex8FilterSolidBlend
  #define EwGfxWarpAffineIndex8FilterSolidBlend \
    EwGfxWarpAffineIndex8FilterGradientBlend
#endif

#ifdef EwGfxWarpAffineAlpha8Gradient
  #define EwWarpAffineAlpha8RowGradient               0
#else
  #define EwGfxWarpAffineAlpha8Gradient               0
  #define EwWarpAffineAlpha8RowGradient               EwWarpAlpha8RowGradient
#endif

#ifdef EwGfxWarpAffineAlpha8GradientBlend
  #define EwWarpAffineAlpha8RowGradientBlend          0
#else
  #define EwGfxWarpAffineAlpha8GradientBlend          0
  #define EwWarpAffineAlpha8RowGradientBlend \
    EwWarpAlpha8RowGradientBlend
#endif

#ifdef EwGfxWarpAffineAlpha8FilterGradient
  #define EwWarpAffineAlpha8RowFilterGradient         0
#else
  #define EwGfxWarpAffineAlpha8FilterGradient         0
  #define EwWarpAffineAlpha8RowFilterGradient \
    EwWarpAlpha8RowFilterGradient
#endif

#ifdef EwGfxWarpAffineAlpha8FilterGradientBlend
  #define EwWarpAffineAlpha8RowFilterGradientBlend    0
#else
  #define EwGfxWarpAffineAlpha8FilterGradientBlend    0
  #define EwWarpAffineAlpha8RowFilterGradientBlend \
    EwWarpAlpha8RowFilterGradientBlend
#endif

#ifndef EwGfxWarpAffineAlpha8Solid
  #define EwGfxWarpAffineAlpha8Solid         EwGfxWarpAffineAlpha8Gradient
#endif

#ifndef EwGfxWarpAffineAlpha8SolidBlend
  #define EwGfxWarpAffineAlpha8SolidBlend    EwGfxWarpAffineAlpha8GradientBlend
#endif

#ifndef EwGfxWarpAffineAlpha8FilterSolid
  #define EwGfxWarpAffineAlpha8FilterSolid      \
    EwGfxWarpAffineAlpha8FilterGradient
#endif

#ifndef EwGfxWarpAffineAlpha8FilterSolidBlend
  #define EwGfxWarpAffineAlpha8FilterSolidBlend \
    EwGfxWarpAffineAlpha8FilterGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XWarpDriver WarpAffineDrivers[] =
{
  EwGfxWarpAffineNative,               EwGfxWarpAffineNativeBlend,
  EwGfxWarpAffineNativeSolid,          EwGfxWarpAffineNativeSolidBlend,
  EwGfxWarpAffineNativeGradient,       EwGfxWarpAffineNativeGradientBlend,
  0,                                   0,
  EwGfxWarpAffineNativeFilter,         EwGfxWarpAffineNativeFilterBlend,
  EwGfxWarpAffineNativeFilterSolid,    EwGfxWarpAffineNativeFilterSolidBlend,
  EwGfxWarpAffineNativeFilterGradient, EwGfxWarpAffineNativeFilterGradientBlend,
  0,                                   0,
  EwGfxWarpAffineIndex8,               EwGfxWarpAffineIndex8Blend,
  EwGfxWarpAffineIndex8Solid,          EwGfxWarpAffineIndex8SolidBlend,
  EwGfxWarpAffineIndex8Gradient,       EwGfxWarpAffineIndex8GradientBlend,
  0,                                   0,
  EwGfxWarpAffineIndex8Filter,         EwGfxWarpAffineIndex8FilterBlend,
  EwGfxWarpAffineIndex8FilterSolid,    EwGfxWarpAffineIndex8FilterSolidBlend,
  EwGfxWarpAffineIndex8FilterGradient, EwGfxWarpAffineIndex8FilterGradientBlend,
  0,                                   0,
  0,                                   0,
  EwGfxWarpAffineAlpha8Solid,          EwGfxWarpAffineAlpha8SolidBlend,
  EwGfxWarpAffineAlpha8Gradient,       EwGfxWarpAffineAlpha8GradientBlend,
  0,                                   0,
  0,                                   0,
  EwGfxWarpAffineAlpha8FilterSolid,    EwGfxWarpAffineAlpha8FilterSolidBlend,
  EwGfxWarpAffineAlpha8FilterGradient, EwGfxWarpAffineAlpha8FilterGradientBlend
};


/* For missing graphics subsystem functionality, register alternative software
   emulated functions */
static const XWarpWorker WarpAffineWorkers[] =
{
  EwWarpAffineNativeRow,               EwWarpAffineNativeRowBlend,
  EwWarpAffineNativeRowGradient,       EwWarpAffineNativeRowGradientBlend,
  EwWarpAffineNativeRowGradient,       EwWarpAffineNativeRowGradientBlend,
  0,                                   0,
  EwWarpAffineNativeRowFilter,         EwWarpAffineNativeRowFilterBlend,
  EwWarpAffineNativeRowFilterGradient, EwWarpAffineNativeRowFilterGradientBlend,
  EwWarpAffineNativeRowFilterGradient, EwWarpAffineNativeRowFilterGradientBlend,
  0,                                   0,
  EwWarpAffineIndex8Row,               EwWarpAffineIndex8RowBlend,
  EwWarpAffineIndex8RowGradient,       EwWarpAffineIndex8RowGradientBlend,
  EwWarpAffineIndex8RowGradient,       EwWarpAffineIndex8RowGradientBlend,
  0,                                   0,
  EwWarpAffineIndex8RowFilter,         EwWarpAffineIndex8RowFilterBlend,
  EwWarpAffineIndex8RowFilterGradient, EwWarpAffineIndex8RowFilterGradientBlend,
  EwWarpAffineIndex8RowFilterGradient, EwWarpAffineIndex8RowFilterGradientBlend,
  0,                                   0,
  0,                                   0,
  EwWarpAffineAlpha8RowGradient,       EwWarpAffineAlpha8RowGradientBlend,
  EwWarpAffineAlpha8RowGradient,       EwWarpAffineAlpha8RowGradientBlend,
  0,                                   0,
  0,                                   0,
  EwWarpAffineAlpha8RowFilterGradient, EwWarpAffineAlpha8RowFilterGradientBlend,
  EwWarpAffineAlpha8RowFilterGradient, EwWarpAffineAlpha8RowFilterGradientBlend
};


/* Depending on the capabilities of the underlying graphics subsystem select
   a set of functions for the 'scale surface' operation. For missing scale
   functionality the 'warp affine' operation will be used. */
#ifndef EwGfxScaleNative
  #define EwGfxScaleNative                 EwGfxWarpAffineNative
#endif

#ifndef EwGfxScaleNativeBlend
  #define EwGfxScaleNativeBlend            EwGfxWarpAffineNativeBlend
#endif

#ifndef EwGfxScaleNativeSolid
  #define EwGfxScaleNativeSolid            EwGfxWarpAffineNativeSolid
#endif

#ifndef EwGfxScaleNativeSolidBlend
  #define EwGfxScaleNativeSolidBlend       EwGfxWarpAffineNativeSolidBlend
#endif

#ifndef EwGfxScaleNativeGradient
  #define EwGfxScaleNativeGradient         EwGfxWarpAffineNativeGradient
#endif

#ifndef EwGfxScaleNativeGradientBlend
  #define EwGfxScaleNativeGradientBlend    EwGfxWarpAffineNativeGradientBlend
#endif

#ifndef EwGfxScaleNativeFilter
  #define EwGfxScaleNativeFilter           EwGfxWarpAffineNativeFilter
#endif

#ifndef EwGfxScaleNativeFilterBlend
  #define EwGfxScaleNativeFilterBlend      EwGfxWarpAffineNativeFilterBlend
#endif

#ifndef EwGfxScaleNativeFilterSolid
  #define EwGfxScaleNativeFilterSolid      EwGfxWarpAffineNativeFilterSolid
#endif

#ifndef EwGfxScaleNativeFilterSolidBlend
  #define EwGfxScaleNativeFilterSolidBlend EwGfxWarpAffineNativeFilterSolidBlend
#endif

#ifndef EwGfxScaleNativeFilterGradient
  #define EwGfxScaleNativeFilterGradient   EwGfxWarpAffineNativeFilterGradient
#endif

#ifndef EwGfxScaleNativeFilterGradientBlend
  #define EwGfxScaleNativeFilterGradientBlend \
    EwGfxWarpAffineNativeFilterGradientBlend
#endif

#ifndef EwGfxScaleIndex8
  #define EwGfxScaleIndex8                 EwGfxWarpAffineIndex8
#endif

#ifndef EwGfxScaleIndex8Blend
  #define EwGfxScaleIndex8Blend            EwGfxWarpAffineIndex8Blend
#endif

#ifndef EwGfxScaleIndex8Solid
  #define EwGfxScaleIndex8Solid            EwGfxWarpAffineIndex8Solid
#endif

#ifndef EwGfxScaleIndex8SolidBlend
  #define EwGfxScaleIndex8SolidBlend       EwGfxWarpAffineIndex8SolidBlend
#endif

#ifndef EwGfxScaleIndex8Gradient
  #define EwGfxScaleIndex8Gradient         EwGfxWarpAffineIndex8Gradient
#endif

#ifndef EwGfxScaleIndex8GradientBlend
  #define EwGfxScaleIndex8GradientBlend    EwGfxWarpAffineIndex8GradientBlend
#endif

#ifndef EwGfxScaleIndex8Filter
  #define EwGfxScaleIndex8Filter           EwGfxWarpAffineIndex8Filter
#endif

#ifndef EwGfxScaleIndex8FilterBlend
  #define EwGfxScaleIndex8FilterBlend      EwGfxWarpAffineIndex8FilterBlend
#endif

#ifndef EwGfxScaleIndex8FilterSolid
  #define EwGfxScaleIndex8FilterSolid      EwGfxWarpAffineIndex8FilterSolid
#endif

#ifndef EwGfxScaleIndex8FilterSolidBlend
  #define EwGfxScaleIndex8FilterSolidBlend EwGfxWarpAffineIndex8FilterSolidBlend
#endif

#ifndef EwGfxScaleIndex8FilterGradient
  #define EwGfxScaleIndex8FilterGradient   EwGfxWarpAffineIndex8FilterGradient
#endif

#ifndef EwGfxScaleIndex8FilterGradientBlend
  #define EwGfxScaleIndex8FilterGradientBlend \
    EwGfxWarpAffineIndex8FilterGradientBlend
#endif

#ifndef EwGfxScaleAlpha8Solid
  #define EwGfxScaleAlpha8Solid            EwGfxWarpAffineAlpha8Solid
#endif

#ifndef EwGfxScaleAlpha8SolidBlend
  #define EwGfxScaleAlpha8SolidBlend       EwGfxWarpAffineAlpha8SolidBlend
#endif

#ifndef EwGfxScaleAlpha8Gradient
  #define EwGfxScaleAlpha8Gradient         EwGfxWarpAffineAlpha8Gradient
#endif

#ifndef EwGfxScaleAlpha8GradientBlend
  #define EwGfxScaleAlpha8GradientBlend    EwGfxWarpAffineAlpha8GradientBlend
#endif

#ifndef EwGfxScaleAlpha8FilterSolid
  #define EwGfxScaleAlpha8FilterSolid      EwGfxWarpAffineAlpha8FilterSolid
#endif

#ifndef EwGfxScaleAlpha8FilterSolidBlend
  #define EwGfxScaleAlpha8FilterSolidBlend EwGfxWarpAffineAlpha8FilterSolidBlend
#endif

#ifndef EwGfxScaleAlpha8FilterGradient
  #define EwGfxScaleAlpha8FilterGradient   EwGfxWarpAffineAlpha8FilterGradient
#endif

#ifndef EwGfxScaleAlpha8FilterGradientBlend
  #define EwGfxScaleAlpha8FilterGradientBlend \
    EwGfxWarpAffineAlpha8FilterGradientBlend
#endif


/* Register the functionality provided by the underlying graphics subsystem. */
static const XWarpDriver WarpScaleDrivers[] =
{
  EwGfxScaleNative,                    EwGfxScaleNativeBlend,
  EwGfxScaleNativeSolid,               EwGfxScaleNativeSolidBlend,
  EwGfxScaleNativeGradient,            EwGfxScaleNativeGradientBlend,
  0,                                   0,
  EwGfxScaleNativeFilter,              EwGfxScaleNativeFilterBlend,
  EwGfxScaleNativeFilterSolid,         EwGfxScaleNativeFilterSolidBlend,
  EwGfxScaleNativeFilterGradient,      EwGfxScaleNativeFilterGradientBlend,
  0,                                   0,
  EwGfxScaleIndex8,                    EwGfxScaleIndex8Blend,
  EwGfxScaleIndex8Solid,               EwGfxScaleIndex8SolidBlend,
  EwGfxScaleIndex8Gradient,            EwGfxScaleIndex8GradientBlend,
  0,                                   0,
  EwGfxScaleIndex8Filter,              EwGfxScaleIndex8FilterBlend,
  EwGfxScaleIndex8FilterSolid,         EwGfxScaleIndex8FilterSolidBlend,
  EwGfxScaleIndex8FilterGradient,      EwGfxScaleIndex8FilterGradientBlend,
  0,                                   0,
  0,                                   0,
  EwGfxScaleAlpha8Solid,               EwGfxScaleAlpha8SolidBlend,
  EwGfxScaleAlpha8Gradient,            EwGfxScaleAlpha8GradientBlend,
  0,                                   0,
  0,                                   0,
  EwGfxScaleAlpha8FilterSolid,         EwGfxScaleAlpha8FilterSolidBlend,
  EwGfxScaleAlpha8FilterGradient,      EwGfxScaleAlpha8FilterGradientBlend
};


/* pba */
