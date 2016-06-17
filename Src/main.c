/**
  ******************************************************************************
  * @file    Display/LTDC_Paint/Src/main.c 
  * @author  MCD Application Team
  * @version V1.0.4
  * @date    22-April-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

tlsf_t MemPool;

/* Defines for the pyhiscal dimension and color format of the LCD framebuffer */
#define FRAME_BUFFER_WIDTH    480
#define FRAME_BUFFER_HEIGHT   272 
/* Select LTDC color format and depth according makefile settings */
#if EW_FRAME_BUFFER_COLOR_FORMAT_Index8
  #define FRAME_BUFFER_FORMAT   LTDC_PIXEL_FORMAT_L8
  #define FRAME_BUFFER_DEPTH    1
#elif (EW_FRAME_BUFFER_COLOR_FORMAT_LumA44)
  #define FRAME_BUFFER_FORMAT   LTDC_PIXEL_FORMAT_AL44
  #define FRAME_BUFFER_DEPTH    1
#elif (EW_FRAME_BUFFER_COLOR_FORMAT_RGBA4444)
  #define FRAME_BUFFER_FORMAT   LTDC_PIXEL_FORMAT_ARGB4444
  #define FRAME_BUFFER_DEPTH    2
#elif (EW_FRAME_BUFFER_COLOR_FORMAT_RGB565)
  #define FRAME_BUFFER_FORMAT   LTDC_PIXEL_FORMAT_RGB565
  #define FRAME_BUFFER_DEPTH    2
#elif (EW_FRAME_BUFFER_COLOR_FORMAT_RGB888)
  #define FRAME_BUFFER_FORMAT   LTDC_PIXEL_FORMAT_RGB888
  #define FRAME_BUFFER_DEPTH    3
#elif (EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888)
  #define FRAME_BUFFER_FORMAT   LTDC_PIXEL_FORMAT_ARGB8888
  #define FRAME_BUFFER_DEPTH    4
#else
  #error The given FRAME_BUFFER_COLOR_FORMAT is not supported!
#endif
/* Calculated addresses for framebuffer(s) and memory manager */
#define FRAME_BUFFER_SIZE     FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * FRAME_BUFFER_DEPTH
#define FRAME_BUFFER_ADDR     (void*)(LCD_FB_START_ADDRESS)

#ifdef EW_USE_DOUBLE_BUFFER
  #define DOUBLE_BUFFER_ADDR  (void*)(FRAME_BUFFER_ADDR + FRAME_BUFFER_SIZE)
  #define DOUBLE_BUFFER_SIZE  FRAME_BUFFER_SIZE
#else  
  #define DOUBLE_BUFFER_ADDR  (void*)(0)
  #define DOUBLE_BUFFER_SIZE  0
#endif

#define MEMORY_POOL_ADDR      (void*)((unsigned char *)FRAME_BUFFER_ADDR + FRAME_BUFFER_SIZE + DOUBLE_BUFFER_SIZE)
#define MEMORY_POOL_SIZE      0x800000 - FRAME_BUFFER_SIZE - DOUBLE_BUFFER_SIZE 

#undef USE_TERMINAL_INPUT	
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern const uint16_t BUTTON_PIN[];

static uint32_t duty=0;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
/*******************************************************************************
* FUNCTION:
*   TermGetCode
*
* DESCRIPTION:
*   The function TermGetCode reads the next EmWi key code from the console.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns the next EmWi key code or CoreKeyCodeNoKey if no key code available.
*
*******************************************************************************/
#ifdef USE_TERMINAL_INPUT
static XEnum TermGetCode( void )
{
  switch ( UartGetCharacter())
  {
    case 0x65 : xputs("Key 'Exit' pressed\n");  return CoreKeyCodeExit;
    case 0x38 : xputs("Key 'Up' pressed\n");    return CoreKeyCodeUp;
    case 0x32 : xputs("Key 'Down' pressed\n");  return CoreKeyCodeDown;
    case 0x36 : xputs("Key 'Right' pressed\n"); return CoreKeyCodeRight;
    case 0x34 : xputs("Key 'Left' pressed\n");  return CoreKeyCodeLeft;
    case 0x35 : xputs("Key 'OK' pressed\n");    return CoreKeyCodeOk;
    case 0x6D : xputs("Key 'Menu' pressed\n");  return CoreKeyCodeMenu;
    case 0x70 : xputs("Key 'Power' pressed\n"); return CoreKeyCodePower;
  }
  return CoreKeyCodeNoKey;
}
#endif
/*******************************************************************************
* FUNCTION:
*   Update
*
* DESCRIPTION:
*   The function Update performs the screen update of the dirty area.
*
* ARGUMENTS:
*   aViewPort    - Viewport used for the screen update.
*   aApplication - Root object used for the screen update.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
static void Update( XViewport* aViewport, CoreRoot aApplication )
{
  XBitmap*       bitmap     = EwBeginUpdate( aViewport );
  GraphicsCanvas canvas     = EwNewObject( GraphicsCanvas, 0 );
  XRect          updateRect = {{ 0, 0 }, { 0, 0 }};

  /* let's redraw the dirty area of the screen. Cover the returned bitmap 
     objects within a canvas, so Mosaic can draw to it. */
  if ( bitmap && canvas )
  {
    GraphicsCanvas__AttachBitmap( canvas, (XUInt32)bitmap );
    updateRect = CoreRoot__UpdateGE20( aApplication, canvas );
    GraphicsCanvas__DetachBitmap( canvas );
  }

  /* complete the update */
  if ( bitmap )
    EwEndUpdate( aViewport, updateRect );
}
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  CoreRoot   rootObject;
  XViewport* viewport;
  XEnum      cmd = CoreKeyCodeNoKey;

  int        touched = 0;
  XPoint     touchPos;
	/* Configure the MPU attributes as Write Through */
  MPU_Config();
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 200 MHz */
  SystemClock_Config(); 
  
  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_SDRAM_Init();
  SystemTick_Config();
  //USBH_Config();
	//init button
	BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
	//init uart
	UART_Config();	
  /* initialize LCD */
  xputs( "Initialize LCD...                            " );
  LCD_Config(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, FRAME_BUFFER_FORMAT, FRAME_BUFFER_ADDR );
  xputs( "[OK]\n" );
  /* initialize touchscreen */
  Touch_Config( FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT );	
  /* initialize tlsf memory manager */
  /* please note, that the first part of SDRAM is reserved for framebuffer */
  xputs( "Initialize Memory Manager...                 " );
  MemPool = tlsf_create_with_pool( MEMORY_POOL_ADDR, MEMORY_POOL_SIZE );
  xputs( "[OK]\n" );	
	xputs("MemoryPool at address 0x%08X size 0x%08X\n", MEMORY_POOL_ADDR, MEMORY_POOL_SIZE );
  /* initialize the Graphics Engine and Runtime Environment */
  xputs( "Initialize Graphics Engine...                " );
  if ( !EwInitGraphicsEngine( 0 ))
    return 2;
  xputs( "[OK]\n" );
  /* create the applications root object ... */
  xputs( "Create EmWi Root Object...                   " );
  rootObject = (CoreRoot)EwNewObjectIndirect( EwApplicationClass, 0 );
  EwLockObject( rootObject );
  CoreRoot__Initialize( rootObject, EwScreenSize );
  xputs( "[OK]\n" );
  /* create Embedded Wizard viewport object to provide uniform access to the
     framebuffer */
  xputs( "Create EmWi Viewport...                      " );
  viewport = EwInitViewport( EwScreenSize, EwNewRect( 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT ), 
    0, 255, FRAME_BUFFER_ADDR, DOUBLE_BUFFER_ADDR, (void*)FRAME_BUFFER_FORMAT, 0 );  
  xputs( "[OK]\n" );
	
	//init test pulse
	PWM_Config();
	PWM_Set(20000,duty);			
	//init adc
	ADC_Config();
	ADC_Start();

  /* Infinite loop */  
  while (1)
  {    
    int timers  = 0;
    int signals = 0;
    int events  = 0;
    //USBH_Process(&hUSB_Host);		
    /* receive touch inputs and provide the application with them */
    if ( GetTouchPosition( &touchPos ))
    {
      /* begin of touch cycle */
      if ( touched == 0 )
        CoreRoot__DriveCursorHitting( rootObject, 1, 0, touchPos );

      /* movement during touch cycle */
      else if ( touched == 1 )
        CoreRoot__DriveCursorMovement( rootObject, touchPos );

      touched = 1;
      events  = 1;
    }		
    /* end of touch cycle */
    else if ( touched == 1 )
    {
      CoreRoot__DriveCursorHitting( rootObject, 0, 0, touchPos );
      touched = 0;
      events  = 1;
    }	
    /* process expired timers */
    timers = EwProcessTimers();

    /* process the pending signals */
    signals = EwProcessSignals();
    /* refresh the screen, if something has changed and draw its content */
    if ( timers || signals || events )
    {
      BSP_LED_On( LED1 );
      Update( viewport, rootObject );
      BSP_LED_Off( LED1 );

      /* after each processed message start the garbage collection */
      EwReclaimMemory();

      /* show the memory statistic */
      #ifdef EW_PRINT_MEMORY_USAGE
        EwPrintProfilerStatistic( 0 );
      #endif
    }		
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  //BSP_LED_On(LED3);
  while(1)
  {
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 8
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN[TAMPER_BUTTON_PIN]);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	duty += 10;
	if (duty > 100)
		duty = 0;
	PWM_Set(20000,duty);
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
