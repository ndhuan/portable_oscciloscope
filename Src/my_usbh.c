#include "ff_gen_drv.h"
#include "usbh_diskio.h"

USBH_HandleTypeDef  hUSB_Host;
FATFS USBDISK_FatFs;  /* File system object for USB Disk logical drive */
FIL MyFile;           /* File object */
char USB_Path[4];     /* USB Disk logical drive path */
/* Variable to save the state of USB */
typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_RUNNING    
}MSC_ApplicationTypeDef;
MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
void USBH_Config(void)
{
  /*##-3- USB Initialization #################################################*/ 
  /* Init Host Library */
  if (USBH_Init(&hUSB_Host, USBH_UserProcess, 0) != USBH_OK)
  {
    /* USB Initialization Error */
    while(1);
  }
  
  /* Add Supported Class */
  USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
  
  /* Start Host Process */
  if (USBH_Start(&hUSB_Host) != USBH_OK)
  {
    /* USB Initialization Error */
    while(1);
  }
  
  /*##-4- Link the USB Mass Storage disk I/O driver ##########################*/
  if(FATFS_LinkDriver(&USBH_Driver, USB_Path) != 0) 
  {
    /* FatFs Initialization Error */
    while(1);
  }
  
  /*##-5- Register the file system object to the FatFs module ################*/
  if(f_mount(&USBDISK_FatFs, (TCHAR const*)USB_Path, 0) != FR_OK)
  {
    /* FatFs Initialization Error */
    while(1);
  }	
}
/**
  * @brief  User Process
  * @param  None
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{  
  switch (id)
  { 
  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_IDLE;
    if (f_mount(&USBDISK_FatFs, "", 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      while(1);
    }
    break;
    
  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_RUNNING;
    break;
  }
}


