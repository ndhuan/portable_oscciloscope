#ifndef __MY_USBH_H
#define __MY_USBH_H

typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_RUNNING    
}MSC_ApplicationTypeDef;

void USBH_Config(void);
void Save_Picture(void);

#endif
