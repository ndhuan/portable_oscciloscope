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
*   The header 'ewextfnt' declares the platform dependent representation of a
*   font resource. This includes data structures to store the attributes of a
*   font resource and a set of macros to cover these structures.
*
*   This header file provides the interface between the platform independent
*   Graphics Engine and the platform specific representation of resources as
*   they will be generated by the Embedded Wizard resource converters of the
*   particular target system.
*
*******************************************************************************/

#ifndef EWEXTFNT_H
#define EWEXTFNT_H


#ifdef __cplusplus
  extern "C" 
  {
#endif


/* The following is an identification number for font resources. */
#define EW_MAGIC_NO_FONT  0x666E7464


/*******************************************************************************
* TYPE:
*   XFntGlyphRes
*
* DESCRIPTION:
*   The XFntGlyphRes structure describes a single glyph of a font resource. The
*   structure stores the glyph metrics and the bit-offset to the location where
*   the glyph pixel data starts. Within a font, all glyphs share a common pixel
*   memory area.
*
* ELEMENTS:
*   CharCode - Character code of the glyph.
*   OriginX,
*   OriginY  - Offset in pixel to the top-left corner of the glyph, relative to
*     the printing position on the text base-line.
*   Width,
*   Height   - Size in pixel of the glyph (= size of the ink box).
*   Advance  - Advance in horizontal direction added to the printing position.
*   Pixel    - Bit offset to the compressed pixel data of the glyph stored in
*     a linear order in the font's pixel memory area.
*
*******************************************************************************/
typedef struct
{ 
  unsigned short    CharCode;
  signed short      OriginX;
  signed short      OriginY;
  signed short      Width;
  signed short      Height;
  signed short      Advance;
  unsigned long     Pixel;
} XFntGlyphRes;


/*******************************************************************************
* TYPE:
*   XFntRes
*
* DESCRIPTION:
*   The structure XFntRes describes the attributes of a single font resource as
*   it will be stored in the code memory.
*
* ELEMENTS:
*   MagicNo     - Unique ID of this resource type. It exists for verification
*     purpose only.
*   Ascent      - Ascent of the font (means the area above the baseline).
*   Descent     - Descent of the font (means the area below the baseline).
*   Leading     - Leading of the font (additional distance between two rows).
*   NoOfColors  - Quality of the rasterized pixel information. Only the values
*     2, 4 or 16 are valid.
*   DefChar     - Code of the character to use instead of missing characters.
*   NoOfGlyphs  - Number of glyphs within this font resource.
*   Glyphs      - Table containing the description of all glyphs. The glyphs 
*     are sorted by their character codes.
*   Pixel       - Pointer to the memory area containing the compressed pixel 
*     data of all glyphs.
*
*******************************************************************************/
typedef struct XFntRes
{ 
  unsigned long       MagicNo;
  int                 Ascent;
  int                 Descent;
  int                 Leading;
  int                 NoOfColors;
  unsigned short      DefChar;
  int                 NoOfGlyphs;
  const XFntGlyphRes* Glyphs;
  const void*         Pixel;
} XFntRes;


/*******************************************************************************
* MACRO:
*   EW_DECLARE_FONT_RES
*   EW_DEFINE_FONT_RES
*   EW_END_OF_FONT_RES
*   EW_GLYPH
*   EW_FONT_PIXEL
*
* DESCRIPTION:
*   Following macros are used to build up the structure of a font resource in
*   a better readable way. These macros are used by the Embedded Wizard font
*   resource converter.
*
*******************************************************************************/
#define EW_DECLARE_FONT_RES( aName )                                           \
  extern const XVariant aName;

#define EW_DEFINE_FONT_RES( aName, aAscent, aDescent, aLeading, aNoOfColors,   \
  aDefChar, aNoOfGlyphs )                                                      \
  extern const unsigned long ____##aName[];                                    \
  extern const XFntGlyphRes ___##aName[];                                      \
  static const XFntRes __##aName =                                             \
  {                                                                            \
    EW_MAGIC_NO_FONT,                                                          \
    aAscent,                                                                   \
    aDescent,                                                                  \
    aLeading,                                                                  \
    aNoOfColors,                                                               \
    aDefChar,                                                                  \
    aNoOfGlyphs,                                                               \
    ___##aName,                                                                \
    ____##aName                                                                \
  };                                                                           \
  const XFntGlyphRes ___##aName[] =                                            \
  {

#define EW_GLYPH( aCode, aOriginX, aOriginY, aWidth, aHeight, aAdvance,        \
  aPixel )                                                                     \
  {                                                                            \
    aCode,                                                                     \
    aOriginX,                                                                  \
    aOriginY,                                                                  \
    aWidth,                                                                    \
    aHeight,                                                                   \
    aAdvance,                                                                  \
    aPixel                                                                     \
  }

#define EW_FONT_PIXEL( aName, aSize )                                          \
    { 0, 0, 0, 0, 0, 0, aSize }                                                \
  };                                                                           \
  const unsigned long ____##aName[] =                                          \
  {

#define EW_END_OF_FONT_RES( aName )                                            \
  };                                                                           \
  static const XResource _##aName[] =                                          \
  {                                                                            \
    { Default, &__##aName }                                                    \
  };


#ifdef __cplusplus
  }
#endif

#endif /* EWEXTFNT_H */


/* pba, msy */
