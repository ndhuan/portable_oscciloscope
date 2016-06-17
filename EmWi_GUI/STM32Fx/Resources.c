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

#include "ewlocale.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "Resources.h"

/* Compressed strings for the language 'Default'. */
static const unsigned long _StringsDefault0[] =
{
  0x00000056, /* ratio 93.02 % */
  0x18005300, 0x1A000548, 0x80006500, 0x80070000, 0x186F001C, 0x80087422, 0x0A3C800E,
  0x60009C01, 0x00186284, 0x014E266D, 0x9E800690, 0x96171B89, 0x0A51939C, 0x82002280,
  0xF1922000, 0x00270009, 0x02C80098, 0x9FC0C5C0, 0x00020303, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 0, 0 };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0002 };

/* Initializer for the class 'Resources::Bitmap' */
void ResourcesBitmap__Init( ResourcesBitmap _this, XObject aLink, unsigned long aArg )
{
  /* At first initialize the super class ... */
  CoreResource__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ResourcesBitmap );

  /* ... and initialize objects, variables, properties, etc. */
  _this->NoOfFrames = 1;

  /* Call the user defined constructor */
  ResourcesBitmap_Init( _this, aArg );
}

/* Re-Initializer for the class 'Resources::Bitmap' */
void ResourcesBitmap__ReInit( ResourcesBitmap _this )
{
  /* At first re-initialize the super class ... */
  CoreResource__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Resources::Bitmap' */
void ResourcesBitmap__Done( ResourcesBitmap _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( ResourcesBitmap );

  /* Call the user defined destructor of the class */
  ResourcesBitmap_Done( _this );

  /* Don't forget to deinitialize the super class ... */
  CoreResource__Done( &_this->_Super );
}

/* Garbage Collector method for the class 'Resources::Bitmap' */
void ResourcesBitmap__Mark( ResourcesBitmap _this )
{
  /* Give the super class a chance to mark its objects and references */
  CoreResource__Mark( &_this->_Super );
}

/* 'C' function for method : 'Resources::Bitmap.Done()' */
void ResourcesBitmap_Done( ResourcesBitmap _this )
{
  XUInt32 handle;

  if ( _this->bitmap == 0 )
    return;

  handle = _this->bitmap;
  EwFreeBitmap((XBitmap*)handle );
  _this->bitmap = 0;
  _this->FrameSize = _Const0000;
  _this->FrameDelay = 0;
  _this->NoOfFrames = 1;
  _this->Animated = 0;
}

/* 'C' function for method : 'Resources::Bitmap.Init()' */
void ResourcesBitmap_Init( ResourcesBitmap _this, XUInt32 aArg )
{
  XUInt32 handle;
  XInt32 noOfFrames;
  XPoint frameSize;
  XInt32 frameDelay;

  if ( aArg == 0 )
    return;

  handle = 0;
  noOfFrames = 1;
  frameSize = _Const0000;
  frameDelay = 0;
  {
    /* aArg is a pointer to the memory where the bitmap resource is stored. */
    XBitmap* bmp = EwLoadBitmap((const XBmpRes*)aArg );

    /* After the bitmap has been loaded get the size of the bitmap and store it
       in the folowing variables. */
    if ( bmp )
    {
      noOfFrames = bmp->NoOfFrames;
      frameSize  = bmp->FrameSize;
      frameDelay = bmp->FrameDelay;
    }

    handle = (XUInt32)(void*)bmp;
  }
  _this->bitmap = handle;
  _this->NoOfFrames = noOfFrames;
  _this->FrameSize = frameSize;
  _this->FrameDelay = frameDelay;
  _this->Animated = (XBool)((( _this->bitmap != 0 ) && ( _this->FrameDelay > 0 )) 
  && ( _this->NoOfFrames > 1 ));
}

/* 'C' function for method : 'Resources::Bitmap.OnSetFrameSize()' */
void ResourcesBitmap_OnSetFrameSize( ResourcesBitmap _this, XPoint value )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( value );

  EwThrow( EwLoadString( &_Const0001 ));
}

/* Wrapper function for the virtual method : 'Resources::Bitmap.OnSetFrameSize()' */
void ResourcesBitmap__OnSetFrameSize( void* _this, XPoint value )
{
  ((ResourcesBitmap)_this)->_VMT->OnSetFrameSize((ResourcesBitmap)_this, value );
}

/* 'C' function for method : 'Resources::Bitmap.Update()' */
void ResourcesBitmap_Update( ResourcesBitmap _this )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
}

/* Wrapper function for the virtual method : 'Resources::Bitmap.Update()' */
void ResourcesBitmap__Update( void* _this )
{
  ((ResourcesBitmap)_this)->_VMT->Update((ResourcesBitmap)_this );
}

/* Variants derived from the class : 'Resources::Bitmap' */
EW_DEFINE_CLASS_VARIANTS( ResourcesBitmap )
EW_END_OF_CLASS_VARIANTS( ResourcesBitmap )

/* Virtual Method Table (VMT) for the class : 'Resources::Bitmap' */
EW_DEFINE_CLASS( ResourcesBitmap, CoreResource, "Resources::Bitmap" )
  ResourcesBitmap_OnSetFrameSize,
  ResourcesBitmap_Update,
EW_END_OF_CLASS( ResourcesBitmap )

/* Initializer for the class 'Resources::Font' */
void ResourcesFont__Init( ResourcesFont _this, XObject aLink, unsigned long aArg )
{
  /* At first initialize the super class ... */
  CoreResource__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ResourcesFont );

  /* Call the user defined constructor */
  ResourcesFont_Init( _this, aArg );
}

/* Re-Initializer for the class 'Resources::Font' */
void ResourcesFont__ReInit( ResourcesFont _this )
{
  /* At first re-initialize the super class ... */
  CoreResource__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Resources::Font' */
void ResourcesFont__Done( ResourcesFont _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( ResourcesFont );

  /* Call the user defined destructor of the class */
  ResourcesFont_Done( _this );

  /* Don't forget to deinitialize the super class ... */
  CoreResource__Done( &_this->_Super );
}

/* Garbage Collector method for the class 'Resources::Font' */
void ResourcesFont__Mark( ResourcesFont _this )
{
  /* Give the super class a chance to mark its objects and references */
  CoreResource__Mark( &_this->_Super );
}

/* 'C' function for method : 'Resources::Font.Done()' */
void ResourcesFont_Done( ResourcesFont _this )
{
  XUInt32 handle;

  if ( _this->font == 0 )
    return;

  handle = _this->font;
  EwFreeFont((XFont*)handle );
  _this->font = 0;
  _this->Ascent = 0;
  _this->Descent = 0;
  _this->Leading = 0;
}

/* 'C' function for method : 'Resources::Font.Init()' */
void ResourcesFont_Init( ResourcesFont _this, XUInt32 aArg )
{
  XUInt32 handle;
  XInt32 ascent;
  XInt32 descent;
  XInt32 leading;

  if ( aArg == 0 )
    return;

  handle = 0;
  ascent = 0;
  descent = 0;
  leading = 0;
  {
    /* aArg is a pointer to a memory where the font resource is stored. */
    XFont* font = EwLoadFont((const XFntRes*)aArg );

    /* After the font has been loaded query its ascent and descent. */
    if ( font )
    {
      ascent  = font->Ascent;
      descent = font->Descent;
      leading = font->Leading;
    }

    handle = (XUInt32)(void*)font;
  }
  _this->font = handle;
  _this->Ascent = ascent;
  _this->Descent = descent;
  _this->Leading = leading;
}

/* 'C' function for method : 'Resources::Font.GetFlowTextSize()' */
XPoint ResourcesFont_GetFlowTextSize( ResourcesFont _this, XString aFlowString )
{
  XInt32 i;
  XInt32 c;
  XInt32 count;
  XInt32 w;
  XInt32 h;

  if ( !EwCompString( aFlowString, 0 ) || ( _this->font == 0 ))
    return _Const0000;

  i = 1;
  c = EwGetStringChar( aFlowString, i );
  count = EwGetStringChar( aFlowString, 0 );
  w = 0;
  h = ((( _this->Ascent + _this->Descent ) + _this->Leading ) * count ) - _this->Leading;

  while ( c > 0 )
  {
    XInt32 cc = c - 2;
    XInt32 tw = 0;

    if ( EwGetStringChar( aFlowString, ( i + c ) - 1 ) == 0x000A )
      cc = cc - 1;

    if ( cc > 0 )
      tw = ResourcesFont_GetTextAdvance( _this, aFlowString, i + 2, cc );

    if ( tw > w )
      w = tw;

    i = i + c;
    c = EwGetStringChar( aFlowString, i );
  }

  return EwNewPoint( w, h );
}

/* 'C' function for method : 'Resources::Font.ParseFlowString()' */
XString ResourcesFont_ParseFlowString( ResourcesFont _this, XString aString, XInt32 
  aOffset, XPoint aSize )
{
  XUInt32 handle;
  XString result;

  if ( aOffset < 0 )
    aOffset = 0;

  if (( _this->font == 0 ) || (( aOffset > 0 ) && ( aOffset >= EwGetStringLength( 
      aString ))))
    return 0;

  handle = _this->font;
  result = 0;
  result = EwParseFlowString((XFont*)handle, aString + aOffset, aSize );
  return result;
}

/* The method GetGlyphAdvance() determines the horizontal advance in pixel of the 
   glyph with the code aCharCode. This metric describes the offset to the next following 
   glyph within a text row. */
XInt32 ResourcesFont_GetGlyphAdvance( ResourcesFont _this, XChar aCharCode )
{
  XInt32 advance;
  XUInt32 handle;

  if (( _this->font == 0 ) || ( aCharCode == 0x0000 ))
    return 0;

  advance = 0;
  handle = _this->font;
  {
    XGlyphMetrics metrics;

    if ( EwGetGlyphMetrics((XFont*)handle, aCharCode, &metrics ))
      advance = metrics.Advance;
  }
  return advance;
}

/* The method GetTextAdvance() calculates the horizontal advance in pixel of a text 
   row to print with this font. This value is calculated by the sum of advance values 
   of all affected glyphs. The text is passed in the parameter aString. The parameter 
   aOffset determines within aString the sign to start the calculation. If aOffset 
   is zero, the calculation starts with the first sign. The parameter aCount determines 
   the max. number of following sigs to calculate the advance value. If aCount is 
   -1, all signs until the end of the string will be evaluated. */
XInt32 ResourcesFont_GetTextAdvance( ResourcesFont _this, XString aString, XInt32 
  aOffset, XInt32 aCount )
{
  XUInt32 handle;
  XInt32 advance;

  if ( aOffset < 0 )
    aOffset = 0;

  if (( _this->font == 0 ) || (( aOffset > 0 ) && ( aOffset >= EwGetStringLength( 
      aString ))))
    return 0;

  handle = _this->font;
  advance = 0;
  advance = EwGetTextAdvance((XFont*)handle, aString + aOffset, aCount );
  return advance;
}

/* Default onget method for the property 'Leading' */
XInt32 ResourcesFont_OnGetLeading( ResourcesFont _this )
{
  return _this->Leading;
}

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetLeading()' */
XInt32 ResourcesFont__OnGetLeading( void* _this )
{
  return ResourcesFont_OnGetLeading((ResourcesFont)_this );
}

/* Default onget method for the property 'Descent' */
XInt32 ResourcesFont_OnGetDescent( ResourcesFont _this )
{
  return _this->Descent;
}

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetDescent()' */
XInt32 ResourcesFont__OnGetDescent( void* _this )
{
  return ResourcesFont_OnGetDescent((ResourcesFont)_this );
}

/* Default onget method for the property 'Ascent' */
XInt32 ResourcesFont_OnGetAscent( ResourcesFont _this )
{
  return _this->Ascent;
}

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetAscent()' */
XInt32 ResourcesFont__OnGetAscent( void* _this )
{
  return ResourcesFont_OnGetAscent((ResourcesFont)_this );
}

/* Variants derived from the class : 'Resources::Font' */
EW_DEFINE_CLASS_VARIANTS( ResourcesFont )
EW_END_OF_CLASS_VARIANTS( ResourcesFont )

/* Virtual Method Table (VMT) for the class : 'Resources::Font' */
EW_DEFINE_CLASS( ResourcesFont, CoreResource, "Resources::Font" )
EW_END_OF_CLASS( ResourcesFont )

/* Bitmap resource : 'Resources::SunkenFrame1' */
EW_DEFINE_BITMAP_RES( ResourcesSunkenFrame1 )
  EW_BITMAP_FRAMES( ResourcesSunkenFrame1, Default, EW_DRIVER_VARIANT_RGBA8888, 72, 24, 0 )
    EW_BITMAP_FRAME( 1, 4, 70, 16, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( ResourcesSunkenFrame1, Default )             /* ratio 22.51 % */
    0x0409FF00, 0xC11FF020, 0x1A0F0581, 0xA110C84C, 0x150F85C3, 0xB1086C4A, 0x1488C4E2,
    0xC8CC5A2F, 0x8DC7A311, 0xEC8A351F, 0xC8E4B208, 0x9A4F2490, 0xA512C94C, 0x552F95CB,
    0xB3096CCA, 0x3498CCE6, 0xC9CCDA6F, 0x9DCFA713, 0xED0A753F, 0xD0E8B409, 0x1A8F44A0,
    0xA914CA4D, 0x954FA5D3, 0xB50A6D4A, 0x54A8D4EA, 0xCACD5AAF, 0x5F40EB15, 0x1AF5FF60,
    0xD9978FFB, 0xFED2BB7F, 0x5FF6C5BB, 0x64FFB7AD, 0xAB07FDC9, 0xAFBB5D2E, 0xC56BFEF2,
    0xBFAADFF7, 0xFE0954FF, 0x3FF0AA97, 0x270F8854, 0x9FF8C53E, 0x218EC7A9, 0x6FFC9A97,
    0x94CAA972, 0xACBE632D, 0xCFE673B9, 0x1E6F419C, 0x69B439ED, 0xA34BA2D2, 0xF35AAD3E,
    0x61915363, 0x1B4D8EBF, 0xB7C52A31, 0x3C0AA70B, 0xBDEF9598, 0xBABEF37D, 0xAC5FEB0B,
    0x2966FFE3, 0xF9AB67FF, 0xD1D05CBF, 0xEBDB1DAA, 0xD6EBAFDF, 0x55FF5FB1, 0xECF6E2D0,
    0x1B07BFDC, 0xB01FFE0F, 0xFAC0FFF3, 0x7C7E8F32, 0xE0C0F630, 0x5F4EFF97, 0xF7F0FDBB,
    0xBF8F9FCB, 0xFFFF7E5F, 0x5FC801E8, 0x680E0480, 0x22057FA0, 0x0A80A078, 0x83E0C836,
    0x1DC7D609, 0x27BA0784, 0x64C2859F, 0x1C858FF8, 0x7BE1D792, 0x5DF7CA1F, 0x485A0C46,
    0x0CFF89CC, 0xC53FE2A3, 0x318FF8B4, 0xCC83FE30, 0x6324FF8C, 0xE4CA3FE3, 0x398EA388,
    0x32CFF32A, 0x2418FE40, 0x43FE4433, 0x34FF3324, 0x4B9264A3, 0x930CD92A, 0xE5233A4F,
    0xF33A523F, 0x5E5833CF, 0xCF962599, 0x9725D968, 0xE609865E, 0xF9965B98, 0xA26299A5,
    0x6C99E649, 0x9AE6D9A6, 0xE5495661, 0x69424E94, 0x1E759D67, 0x40916469, 0x8FA3B32E,
    0xE359FE7D, 0xFA14C78D, 0x162D8C62, 0xC489CFF3, 0x30A193FC, 0x1FD7921B, 0xF791ED89,
    0x262E8B28, 0x3D8D232A, 0x982729DA, 0x4FFA84D0, 0x5A8EA434, 0xFEA834AA, 0xFFAACD33,
    0x3FEAE350, 0xAB0AC4D5, 0x3356B1AC, 0xE4D6ADAB, 0xD6AE4FFA, 0xAFCD73FC, 0xEBFB02BE,
    0x3B12C135, 0x1EC8B16C, 0xCCB0ACAB, 0xB26CDB1A, 0xECEB2ECF, 0x2B52D0B4, 0xEEBCB22D,
    0xB7AD6BAA, 0xABADCB76, 0x2ACAA8D4, 0x1A98D2AA, 0x42A13FCD, 0x72A8AE83, 0x8DA819CA,
    0x0C6A1CC9, 0x38AA8AA6, 0x4E1AA38C, 0x167F511A, 0x3FEF2BDA, 0xA8231E85, 0x7A92609E,
    0x4EE2B92E, 0xDCABEAD3, 0x362C7B66, 0xCD93FF10, 0xAC5312C4, 0x5CFFC5CD, 0xDB3FCDAC,
    0xC7B1CC74, 0x4FFC84DC, 0x2C8CDCC8, 0x9726C932, 0x27CAF28C, 0xCBB2ACB7, 0xF2CCC729,
    0x0CCB34CB, 0xD736CCF3, 0xDBC7728C, 0xC6B3DC7C, 0xF4136715, 0x836310D0, 0x26E0AEAD,
    0xEBC1B4AC, 0xA0675BAA, 0x2FF32233, 0xAA42F8A3, 0x92FC45AF, 0xF38C2FE7, 0x91703C06,
    0xEA9A9F05, 0xDD270AAA, 0x470FAEB0, 0x6BD0B13C, 0xD0318C67, 0xF39CFB3C, 0x7DD738DC,
    0xE33ADDF3, 0x76DEB74D, 0xDC725DE7, 0xF6DD0371, 0x7D1714E0, 0x9349AF74, 0xE9A974BD,
    0x8D3509DA, 0xE28A2F5C, 0x944B57A5, 0xC75B7BF5, 0x04A775E8, 0xC234C967, 0xAB2D9B8A,
    0x4E1AC0C3, 0xC340D0B8, 0x7DDC71ED, 0xEBB7EDEF, 0x3B2EB7B0, 0xBDFFB4DF, 0x5382C7BA,
    0x863FFA7C, 0xE26D9B1F, 0x74CE2AAC, 0x19074FE3, 0xE793D4F5, 0xF1A51E8B, 0xE462BBF5,
    0x79D8EB9B, 0xCD8F9F97, 0xA2B8E8AA, 0xBADABA5D, 0xC636FC5B, 0x7B5C8380, 0xFECFE3EC,
    0xBFE4F9BA, 0x18CFFE1E, 0xEF76DEEF, 0xAAFEFEC0, 0x2E33C2E2, 0x1E7EF16A, 0x94D5391F,
    0xA56FBF2F, 0xE69E7B98, 0xB039C50C, 0xE56C4E79, 0x46C31D0A, 0x99623DA7, 0xFBA87BAE,
    0x97D6C6DE, 0xEFA20ABE, 0xA0CBE582, 0xBEC76EFA, 0x70AFB98D, 0xD21DF311, 0x563C17E6,
    0x4B7EEF0D, 0xF6D49C7A, 0xF9AB3C97, 0x4C006B0F, 0x46BB0091, 0x9019E940, 0x1A54087A,
    0xABD5B4F6, 0x776D19B4, 0x071F03A9, 0x2241A82D, 0x1888AF9E, 0x730519AC, 0xF20836C7,
    0x1BF1842E, 0x67E8B861, 0x2D39FB42, 0x5C83C685, 0xBCA7F88C, 0x522F30F8, 0xF417BC01,
    0xE9A3E86A, 0xA01FEB99, 0xB302DEBB, 0xFDEDC3D7, 0x9C4189D0, 0x689310E0, 0xF1D23AC4,
    0x625B308E, 0x3EF89CFB, 0x7E4B1228, 0xC2A12C53, 0x27C5784E, 0x8D7910A9, 0xFF179FEA,
    0xCDAD3CD7, 0xDC646010, 0x76C29213, 0x6D95EB30, 0xD6443C8D, 0xC1189B03, 0x67AEA98D,
    0x28A243AC, 0x3B4A488F, 0x1E8F5116, 0x8D0823E4, 0x91C44226, 0x3BF56150, 0x98B0F117,
    0x8BCE2D3F, 0x245C2E8B, 0x9EF91B0C, 0x534A694B, 0x9D54EBC7, 0xC6A5953E, 0x097AAD31,
    0xB8B86354, 0x315A4CC5, 0xB3A457B0, 0x29A6B456, 0xEB566ACD, 0x26CAD79A, 0xCDB9B135,
    0x99AB656C, 0xC2E702B6, 0x8430A902, 0x7369B211, 0x82BB92B2, 0x697FAF04, 0xEC8A5FCA,
    0x6412E58E, 0x4BF91422, 0x5E3B9153, 0x4FC53B3B, 0x502A051D, 0x9E13D27A, 0x28D0449A,
    0x6F98894D, 0x13429352, 0x393750CA, 0x5D104E14, 0xDA2543E8, 0xF4153A2E, 0x280D174E,
    0xA9D4829F, 0x9DAA7540, 0xA69C8B5E, 0xF9460970, 0xE5424464, 0xFA77D0B1, 0x490B9EA3,
    0x41F9422A, 0x8A634CA9, 0x28390A1D, 0x29CD3641, 0x20BA774E, 0xA7B4DE9E, 0x4A106D40,
    0x974BCA75, 0x214352F6, 0x9687AA4A, 0x8CFE9EFA, 0xA52D2228, 0x92CE594E, 0xA5AC5D96,
    0x45A96F2D, 0x9752E65C, 0x1FF574E2, 0x2B4E01C2, 0xC11802FC, 0x863082A8, 0xFE6E0DB1,
    0xD05398C1, 0x49AD3666, 0x33595C8D, 0x35D8D5D7, 0xEBCD74AE, 0x9AAAEB5D, 0x3215F8D9,
    0x5A8D857F, 0xDE1BAAD5, 0xAC37D59C, 0xF5582B10, 0xC4E3D5F2, 0x69CB3922, 0x139E2D8E,
    0x858AAA9D, 0x4EE5332C, 0x15E2B657, 0x2B567ACE, 0x686CDDA0, 0xCFDA3B3B, 0xA4B4F68A,
    0x5369AD45, 0xBAD5DACB, 0x55B0B4B6, 0x3B5B6C6D, 0xF6CAD7DB, 0xADC5B4B6, 0xDBCB736D,
    0xB6B6FADD, 0x406D95C0, 0x00000000, 0x00000000

  EW_BITMAPS_TABLE( ResourcesSunkenFrame1 )
    EW_BITMAP( ResourcesSunkenFrame1, Default )
EW_END_OF_BITMAP_RES( ResourcesSunkenFrame1 )

/* Table with links to derived variants of the bitmap resource : 'Resources::SunkenFrame1' */
EW_RES_WITHOUT_VARIANTS( ResourcesSunkenFrame1 )

/* Embedded Wizard */
