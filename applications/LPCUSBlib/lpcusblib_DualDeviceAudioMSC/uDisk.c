/*
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "led.h"
//#include "AudioOutput.h"
#include "MassStorage.h"
#include "MassStorageHost.h"
#include "MassStorageHost.h"
#include "param.h"

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	Board_Init();

	DEBUGOUT("\r\n\n\n\n\n========= uDisk test start ==========\r\n");
	DEBUGOUT("CPU speed = %d MHz\n\r", Chip_Clock_GetRate(CLK_MX_MXCORE)/1000000);
	
	MassStorageDeviceSetupHardware();
	MassStorageHostSetupHardware();
	
	for (;; ) 
	{
		volatile int USB0_CurrentMode = USB_GetCurrentMode(MASS_STORAGE_CORENUM);
		
		switch(USB0_CurrentMode)
		{
			case USB_MODE_Device:
			{
				// Run the device mode and MSC class stacks
				MS_Device_USBTask(&Disk_MS_Interface);
				USB_USBTask(MASS_STORAGE_CORENUM, USB_MODE_Device);
			}
			break;
			
      case USB_MODE_Host :
      {
        MS_Host_USBTask(&FlashDisk_MS_Interface);
        USB_USBTask(FLASH_DISK_CORENUM,USB_MODE_Host);
      }
      break;
			default :
			{
				DEBUGOUT("\n=========USB Mode Not used==============");
		
      }
			break;
		}
//TODO_JL
/*
    main()
    {
    BoardInit();
//    SDMMCInit();
//    MassStorageDeviceSetupHardware();
//    MassStorageHostSetupHardware();
    for(;;)
    {
      if(uDiskFlag.jl_USBSelect == USBCommModeIdle)
      {
         if(Insert==getUsbStatus(USB1)
         {
            SDMMCInit();
            MassStorageHostSetupHardware();
            uDiskFlag.jl_USBSelect = USBCommModePhone;
         }
      }
    
      if(uDiskFlag.jl_USBSelect != USBCommModePC)
      {
        if(Insert==getUsbStatus(USB0))
        {
          if(uDiskFlag.jl_USBSelect == USBCommModePhone)
            UnMassStorageHostSetupHardware();
          SDMMCInit();
          MassStorageDeviceSetupHardware();
          uDiskFlag.jl_USBSelect = USBCommModePC;
        }
      }
    
      switch(uDiskFlag.jl_USBSelect)
      {
        case USBCommModePC :
        {
           MassStorageHostDeal();
        }break;
        
        case USBCommModePhone :
        {
          switch(uDiskFlag.jl_USBPhoneMode)
          {
            case USBPhoneAndroid :
            {
               AndroidCommDeal();
            }break;
            
            case USBPhoneIOS :
            {
              IOSCommDeal();
            }break;
          }
        }
      } end of switch() 
    } end of for()
  } end of main()
		
*/
	}
}
