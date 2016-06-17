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

#ifndef _SteelRotaryKnob_H
#define _SteelRotaryKnob_H

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
#include "_CoreKeyPressHandler.h"
#include "_CoreRotateTouchHandler.h"
#include "_ViewsImage.h"
#include "_ViewsWarpImage.h"

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

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif

/* Forward declaration of the class Steel::RotaryKnob */
#ifndef _SteelRotaryKnob_
  EW_DECLARE_CLASS( SteelRotaryKnob )
#define _SteelRotaryKnob_
#endif


/* Rotary know widget with a steel design. The widget allows the user to change 
   a value by rotating a knob arround the widgets center. */
EW_DEFINE_FIELDS( SteelRotaryKnob, CoreGroup )
  EW_OBJECT  ( Image,           ViewsImage )
  EW_OBJECT  ( Needle,          ViewsWarpImage )
  EW_OBJECT  ( RotateTouchHandler, CoreRotateTouchHandler )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( CurrentValue,    XInt32 )
  EW_PROPERTY( Outlet,          XRef )
  EW_PROPERTY( OnApply,         XSlot )
  EW_PROPERTY( OnChange,        XSlot )
  EW_VARIABLE( startValue,      XInt32 )
  EW_OBJECT  ( KeyHandlerUp,    CoreKeyPressHandler )
  EW_OBJECT  ( KeyHandlerDown,  CoreKeyPressHandler )
EW_END_OF_FIELDS( SteelRotaryKnob )

/* Virtual Method Table (VMT) for the class : 'Steel::RotaryKnob' */
EW_DEFINE_METHODS( SteelRotaryKnob, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( CoreGroup _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( SteelRotaryKnob _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( SteelRotaryKnob )

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
void SteelRotaryKnob_UpdateViewState( SteelRotaryKnob _this, XSet aState );

/* 'C' function for method : 'Steel::RotaryKnob.OnSetCurrentValue()' */
void SteelRotaryKnob_OnSetCurrentValue( SteelRotaryKnob _this, XInt32 value );

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void SteelRotaryKnob_onStartSlot( SteelRotaryKnob _this, XObject sender );

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void SteelRotaryKnob_onRotateSlot( SteelRotaryKnob _this, XObject sender );

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void SteelRotaryKnob_onEndSlot( SteelRotaryKnob _this, XObject sender );

/* This internal slot method is used to receive the corresponding signal form the 
   keyboard handler. */
void SteelRotaryKnob_onPressKeySlot( SteelRotaryKnob _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _SteelRotaryKnob_H */

/* Embedded Wizard */
