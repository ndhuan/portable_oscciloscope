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

#ifndef _FlatVertSlider_H
#define _FlatVertSlider_H

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

#include "_CoreGroup.h"
#include "_CoreSimpleTouchHandler.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"

/* Forward declaration of the class Core::KeyPressHandler */
#ifndef _CoreKeyPressHandler_
  EW_DECLARE_CLASS( CoreKeyPressHandler )
#define _CoreKeyPressHandler_
#endif

/* Forward declaration of the class Core::LayoutContext */
#ifndef _CoreLayoutContext_
  EW_DECLARE_CLASS( CoreLayoutContext )
#define _CoreLayoutContext_
#endif

/* Forward declaration of the class Core::View */
#ifndef _CoreView_
  EW_DECLARE_CLASS( CoreView )
#define _CoreView_
#endif

/* Forward declaration of the class Flat::MinMaxText */
#ifndef _FlatMinMaxText_
  EW_DECLARE_CLASS( FlatMinMaxText )
#define _FlatMinMaxText_
#endif

/* Forward declaration of the class Flat::VertSlider */
#ifndef _FlatVertSlider_
  EW_DECLARE_CLASS( FlatVertSlider )
#define _FlatVertSlider_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* Vertical slider widget with a flat design. The widget allows the user to change 
   a value by moving a thumb in vertical direction. */
EW_DEFINE_FIELDS( FlatVertSlider, CoreGroup )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( CurrentValue,    XInt32 )
  EW_VARIABLE( startValue,      XInt32 )
  EW_PROPERTY( Outlet,          XRef )
  EW_PROPERTY( OnApply,         XSlot )
  EW_PROPERTY( OnChange,        XSlot )
  EW_OBJECT  ( SimpleTouchHandler, CoreSimpleTouchHandler )
  EW_OBJECT  ( Rectangle,       ViewsRectangle )
  EW_OBJECT  ( Thumb,           ViewsImage )
  EW_OBJECT  ( ThumbText,       ViewsText )
  EW_OBJECT  ( MoveText,        ViewsText )
  EW_PROPERTY( DotPosition,     XInt32 )
  EW_PROPERTY( Divisor,         XInt32 )
  EW_PROPERTY( MinText,         FlatMinMaxText )
  EW_PROPERTY( MaxText,         FlatMinMaxText )
  EW_PROPERTY( SliderSize,      XInt32 )
  EW_PROPERTY( BackColor,       XColor )
  EW_PROPERTY( ItemColor,       XColor )
  EW_PROPERTY( TextColor,       XColor )
  EW_PROPERTY( ItemColorActive, XColor )
  EW_PROPERTY( Inverse,         XBool )
  EW_PROPERTY( ShowCurrent,     XBool )
EW_END_OF_FIELDS( FlatVertSlider )

/* Virtual Method Table (VMT) for the class : 'Flat::VertSlider' */
EW_DEFINE_METHODS( FlatVertSlider, CoreGroup )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( Draw,              void )( CoreGroup _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreView _this, CoreEvent aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreGroup _this, XRect aArea, XInt32 
    aFinger, XInt32 aStrikeCount, CoreView aDedicatedView )
  EW_METHOD( ArrangeView,       XPoint )( CoreRectView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreRectView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreRectView _this )
  EW_METHOD( ChangeViewState,   void )( CoreGroup _this, XSet aSetState, XSet aClearState )
  EW_METHOD( OnSetBounds,       void )( CoreGroup _this, XRect value )
  EW_METHOD( OnSetFocus,        void )( CoreGroup _this, CoreView value )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( UpdateLayout,      void )( FlatVertSlider _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( FlatVertSlider _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( FlatVertSlider )

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visiblity or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void FlatVertSlider_UpdateViewState( FlatVertSlider _this, XSet aState );

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void FlatVertSlider_UpdateLayout( FlatVertSlider _this, XPoint aSize );

/* 'C' function for method : 'Flat::VertSlider.OnSetCurrentValue()' */
void FlatVertSlider_OnSetCurrentValue( FlatVertSlider _this, XInt32 value );

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void FlatVertSlider_onPressSlot( FlatVertSlider _this, XObject sender );

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void FlatVertSlider_onDragSlot( FlatVertSlider _this, XObject sender );

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void FlatVertSlider_onReleaseSlot( FlatVertSlider _this, XObject sender );

/* Method to format a string based on the given aValue and the current @DotPosition 
   and @Divisor. */
XString FlatVertSlider_formatValueString( FlatVertSlider _this, XInt32 aValue );

#ifdef __cplusplus
  }
#endif

#endif /* _FlatVertSlider_H */

/* Embedded Wizard */
