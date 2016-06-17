#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "def.h"
#include "save.h"
#include "color.h"
#include "color2.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "my_usbh.h"
const uint32_t aBMPHeader1[14]=         
{0x26E64D42, 0x0000000B, 0x00360000, 0x00280000, 0x02440000, 0x01A40000, 0x00010000, 
 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

const uint32_t aBMPHeader2[14]=         
{0x13A64D42, 0x00000004, 0x00360000, 0x00280000, 0x01A40000, 0x00D40000, 0x00010000, 
 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

extern FIL MyFile;           /* File object */ 
extern MSC_ApplicationTypeDef Appli_state;
 
static uint32_t Radius = 2;
static uint32_t x = 0, y = 0;
static TS_StateTypeDef  TS_State;

static void Draw_Menu(void);
static void GetPosition(void);
static void Prepare_Picture(void);
static void Update_Color(void);
static void Update_Size(uint8_t size);

void UI_Config()
{
  /*##-1- LCD Initialization #################################################*/ 
  /* Initialize the LCD */
  BSP_LCD_Init();
  
  /* Foreground Layer Initialization */
  BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_LAYER1);
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(1);
  /* Clear the LCD Foreground layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  /* Enable the color Key for foreground layer */   
  BSP_LCD_SetColorKeying(1, LCD_COLOR_WHITE);
	//BSP_LCD_SetColorKeying(1,LCD_COLOR_BLACK);
  BSP_LCD_SetLayerVisible(1, DISABLE);
  
  /* Background Layer Initialization */
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER0);
  
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(0);
  
  /* Enable the LCD */
  BSP_LCD_DisplayOn();
  
  /* Clear the LCD Background layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
 
  ft5336_ts_drv.Init(TS_I2C_ADDRESS);
  /*##-2- Touch screen initialization ########################################*/
  
  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}
/**
  * @brief  Clock Config.
  * @param  hltdc: LTDC handle
  * @note   This API is called by BSP_LCD_Init()
  * @retval None
  */
void BSP_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
  static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;

  if(ft5336_ts_drv.ReadID(TS_I2C_ADDRESS) == FT5336_ID_VALUE)
  {
    /* AMPIRE480272 LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
    periph_clk_init_struct.PLLSAI.PLLSAIR = 5;
    periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
  }
  else
  {
    /* AMPIRE640480 LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 38.4/2 = 19.2 Mhz */
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
    periph_clk_init_struct.PLLSAI.PLLSAIR = 5;
    periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
  }
}
/**
  * @brief  Configures and gets Touch screen position.
  * @param  None
  * @retval None
  */
static void GetPosition(void)
{
  static uint32_t color_width;  
  static uint32_t color;
  
  if (BSP_LCD_GetXSize() == 640)
  {
    color_width = 36;
  }
  else
  {
    color_width = 19;
  }
  
  /* Get Touch screen position */
  BSP_TS_GetState(&TS_State); 

  /* Read the coordinate */
  if(ft5336_ts_drv.ReadID(TS_I2C_ADDRESS) == FT5336_ID_VALUE)
  {
		x = TS_State.touchX[0];
		y = TS_State.touchY[0];
  }
  
  if ((TS_State.touchDetected) & (x > (67 + Radius)) & (y > (7 + Radius) ) & ( x < (BSP_LCD_GetXSize()-(7  + Radius )) ) & (y < (BSP_LCD_GetYSize()-(67 + Radius )) ))
  {
    BSP_LCD_FillCircle((x), (y), Radius);
  }
  else if ((TS_State.touchDetected) & (x > 0 ) & ( x < 50 ))
  { 
    if ((TS_State.touchDetected) & ( y > 0 ) & ( y < color_width ))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & ( y > color_width ) & (y < (2 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (2 * color_width)) & (y < (3 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (3 * color_width)) & (y < (4 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (4 * color_width)) & (y < (5 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (5 * color_width)) &(y < (6 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (6 * color_width)) &(y < (7 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (7 * color_width)) & (y < (8 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (8 * color_width)) & (y < (9 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (9 * color_width)) & (y < (10 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (10 * color_width)) & (y < (11 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
      Update_Size(Radius);
    }
    else if ((TS_State.touchDetected) & (y > (11 * color_width)) & (y < (12 * color_width)))
    {
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      Update_Size(Radius);
    }    
    else if ((TS_State.touchDetected) &  (y > (12 * color_width)) & (y < (13 * color_width)))
    {
      /* Get the current text color */
      color = BSP_LCD_GetTextColor();
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Clear the working window */
      BSP_LCD_FillRect(68, 8, (BSP_LCD_GetXSize() - 75), (BSP_LCD_GetYSize() - 75));
      BSP_LCD_SetTextColor(color);
    }
    else
    {
      x = 0;
      y = 0;
    }
    Update_Color();    
  }
  else if ((TS_State.touchDetected) & (x > 70 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 120 ))
  {    
    Radius = 20;
    Update_Size(Radius);
  }
  else if ((TS_State.touchDetected) & (x > 120 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 170 ))
  {    
    Radius = 15;
    Update_Size(Radius);
  }
  else if ((TS_State.touchDetected) & (x > 170 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 220 ))
  {    
    Radius = 10;
    Update_Size(Radius);
  }
  else if ((TS_State.touchDetected) & (x > 220 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 270 ))
  {    
    Radius = 5;
    Update_Size(Radius);
  }
  else if ((TS_State.touchDetected) & (x > 270 ) & (y > (12 * color_width)) & (y < (13 * color_width)) & ( x < 320 ))
  {    
    Radius = 2;
    Update_Size(Radius);
  }  
  else if ((TS_State.touchDetected) & (((x > (BSP_LCD_GetXSize()-5) ) & (y > (12 * color_width)) & (y < (13 * color_width))) | (( x < 55 ) & ( y < 5 ))))
  {    
  TS_State.touchX[0] = 0;
  TS_State.touchY[0] = 0;
  }  
  else if ((TS_State.touchDetected) & (x > 320) & (y > (BSP_LCD_GetYSize() - 50)) & (x < 370) & (y < BSP_LCD_GetYSize() ))
  {   
    //Save_Picture();
  }    
}

/**
  * @brief  Draws the menu.
  * @param  None
  * @retval None
  */
static void Draw_Menu(void)
{ 
  /* Set background Layer */
  BSP_LCD_SelectLayer(0);
  
	/* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);  

  if (BSP_LCD_GetXSize() == 640)
  {  
    /* Draw color image */
    BSP_LCD_DrawBitmap(0, 0, (uint8_t *)color);
  }
  else
  {
    /* Draw color image */
    BSP_LCD_DrawBitmap(0, 0, (uint8_t *)color2);
  }
  
  /* Draw save image */
  BSP_LCD_DrawBitmap(310, (BSP_LCD_GetYSize() - 50), (uint8_t *)save);

  /* Set Black as text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  /* Draw working window */
  BSP_LCD_DrawRect(61, 0, (BSP_LCD_GetXSize() - 61), (BSP_LCD_GetYSize() - 60));
  //BSP_LCD_DrawRect(63, 3, (BSP_LCD_GetXSize() - 66), (BSP_LCD_GetYSize() - 66));
  //BSP_LCD_DrawRect(65, 5, (BSP_LCD_GetXSize() - 70), (BSP_LCD_GetYSize() - 70));
  //BSP_LCD_DrawRect(67, 7, (BSP_LCD_GetXSize() - 74), (BSP_LCD_GetYSize() - 74));
  
  /* Draw size icons */
  BSP_LCD_FillRect(60, (BSP_LCD_GetYSize() - 48), 250, 48);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(95, (BSP_LCD_GetYSize() - 24), 20);
  BSP_LCD_FillCircle(145, (BSP_LCD_GetYSize() - 24), 15);
  BSP_LCD_FillCircle(195, (BSP_LCD_GetYSize() - 24), 10);
  BSP_LCD_FillCircle(245, (BSP_LCD_GetYSize() - 24), 5);
  BSP_LCD_FillCircle(295, (BSP_LCD_GetYSize() - 24), 2);  
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font8);
  BSP_LCD_DisplayStringAt(360, (BSP_LCD_GetYSize() - 55), (uint8_t *)"Selected Color  Size", LEFT_MODE);  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
  BSP_LCD_FillRect(380, (BSP_LCD_GetYSize() - 40), 30, 30);  
  BSP_LCD_FillCircle(450, (BSP_LCD_GetYSize()- 24), Radius); 
}


/**
  * @brief  Update the selected color
  * @param  None
  * @retval None
  */
static void Update_Color(void)
{
  static uint32_t color;
  
  /* Get the current text color */
  color = BSP_LCD_GetTextColor();
  /* Update the selected color icon */
  BSP_LCD_FillRect(380, (BSP_LCD_GetYSize()-40), 30, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);    
  BSP_LCD_DrawRect(380, (BSP_LCD_GetYSize()-40), 30, 30);
  BSP_LCD_SetTextColor(color);  
}

/**
  * @brief  Updates the selected size
  * @param  size: Size to be updated
  * @retval None
  */
static void Update_Size(uint8_t size)
{
  static uint32_t color;
  
  /* Get the current text color */ 
  color = BSP_LCD_GetTextColor();
  /* Update the selected size icon */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(450, (BSP_LCD_GetYSize()-24), 20);
  BSP_LCD_SetTextColor(color);  
  BSP_LCD_FillCircle(450, (BSP_LCD_GetYSize()-24), size);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);    
  BSP_LCD_DrawCircle(450, (BSP_LCD_GetYSize()-24), size);
  BSP_LCD_SetTextColor(color);  
}
/**
  * @brief  Prepares the picture to be Saved in USB Disk.
  * @param  None
  * @retval None
  */
static void Prepare_Picture(void) 
{ 
  static DMA2D_HandleTypeDef hdma2d_eval;
  uint32_t address1 = CONVERTED_FRAME_BUFFER;
  uint32_t address2 = LCD_FRAME_BUFFER_LAYER0;
  uint32_t index = 0;
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_OUTPUT_RGB888;
  hdma2d_eval.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d_eval.LayerCfg[1].InputOffset = 60;
  
  hdma2d_eval.Instance = DMA2D; 

  /* Bypass the bitmap header */
  address2 += ((BSP_LCD_GetXSize() * (BSP_LCD_GetYSize() - 61) + 60) * 4);  
  
  /* Convert picture to RGB888 pixel format */
  for(index=0; index < (BSP_LCD_GetYSize() - 60); index++)
  { 
    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK) 
    {
      if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK) 
      {
        if (HAL_DMA2D_Start(&hdma2d_eval, address2, address1, (BSP_LCD_GetXSize() - 60), 1) == HAL_OK)
        {
          /* Polling For DMA transfer */  
          HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
        }
      }
    }    
    /* Increment the source and destination buffers */
    address1 += ((BSP_LCD_GetXSize() - 60)*3);
    address2 -= BSP_LCD_GetXSize()*4;
  }
}
/**
  * @brief  Save the picture in USB Disk.
  * @param  None
  * @retval None
  */
void Save_Picture(void)
{ 
  FRESULT res1, res2;    /* FatFs function common result code */
  uint32_t byteswritten; /* File write count */
  
  BSP_LCD_SetLayerVisible(1, ENABLE);//??????????????????????????????????????????????????????????????????????????????
  BSP_LCD_SetColorKeying(1, LCD_COLOR_WHITE);//??????????????????????????????????????????????????????????????????????
  /* Set foreground Layer */
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font20);
  
  /* Turn LED1 and LED3 Off */
  BSP_LED_Off(LED1);
//  BSP_LED_Off(LED3);
  
  if (Appli_state == APPLICATION_RUNNING)
  {
    BSP_LCD_DisplayStringAt(10, (BSP_LCD_GetYSize()-100), (uint8_t *)"Saving ... ", RIGHT_MODE);
    
    /*##-1- Prepare the image to be saved ####################################*/
    Prepare_Picture();
    
    /*##-2- Create and Open a new bmp file object with write access ##########*/
    if(f_open(&MyFile, "image.bmp", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
      /* 'image.bmp' file Open for write Error */
      while(1);
    }
    else
    {
      /*##-3- Write data to the BMP file #####################################*/
      /* Write the BMP header */
      if (BSP_LCD_GetXSize() == 640)
      {  
        /* if ampire 640x480 LCD is used */
        res1 = f_write(&MyFile, (uint32_t *)aBMPHeader1, 54, (void *)&byteswritten);
      }
      else
      {  
        /* if ampire 480x272 LCD is used */
        res1 = f_write(&MyFile, (uint32_t *)aBMPHeader2, 54, (void *)&byteswritten);
      }        
      /* Write the bmp file */
      res2 = f_write(&MyFile, (uint32_t *)CONVERTED_FRAME_BUFFER, ((BSP_LCD_GetYSize()-60)*(BSP_LCD_GetXSize()-60)*3), (void *)&byteswritten);
      
      if((res1 != FR_OK) || (res2 != FR_OK) || (byteswritten == 0))
      {
        /* 'image' file Write or EOF Error */
//        BSP_LED_On(LED3);
        BSP_LCD_DisplayStringAt(10, (BSP_LCD_GetYSize()-100), (uint8_t *)" Aborted...", RIGHT_MODE);
        /* Wait for 2s */
        HAL_Delay(2000);
        /* Disable the Layer 2 */
        BSP_LCD_SetLayerVisible(1, DISABLE);
        /* Select Layer 1 */
        BSP_LCD_SelectLayer(0);
      }
      else
      {
        /*##-4- Close the open text file #####################################*/
        f_close(&MyFile);
        
        /* Success of the demo: no error occurrence */
        BSP_LED_On(LED1);
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
        BSP_LCD_DisplayStringAt(10, (BSP_LCD_GetYSize()-100), (uint8_t *)" Saved     ", RIGHT_MODE);
        /* Wait for 2s */
        HAL_Delay(2000);
        /* Disable the Layer 2 */
        BSP_LCD_SetLayerVisible(1, DISABLE);
        /* Select Layer 1 */
        BSP_LCD_SelectLayer(0);
      }
    }
  }
  else
  {
    /* USB not connected */
    BSP_LCD_DisplayStringAt(10, (BSP_LCD_GetYSize()-100), (uint8_t *)"USB KO... ", RIGHT_MODE);
    /* Wait for 2s */
    HAL_Delay(2000);
    /* Disable the Layer 2 */
    BSP_LCD_SetLayerVisible(1, DISABLE);
    /* Select Layer 1 */
    BSP_LCD_SelectLayer(0);
  }
}
