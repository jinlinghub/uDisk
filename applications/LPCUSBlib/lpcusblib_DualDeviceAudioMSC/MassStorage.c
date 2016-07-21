
#include "MassStorage.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/** nxpUSBlib Mass Storage Class driver interface configuration and state information. This structure is
 *  passed to all Mass Storage Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_MS_Device_t Disk_MS_Interface = {
	.Config = {
		.InterfaceNumber           = 0,

		.DataINEndpointNumber      = MASS_STORAGE_IN_EPNUM,
		.DataINEndpointSize        = MASS_STORAGE_IO_EPSIZE,
		.DataINEndpointDoubleBank  = false,

		.DataOUTEndpointNumber     = MASS_STORAGE_OUT_EPNUM,
		.DataOUTEndpointSize       = MASS_STORAGE_IO_EPSIZE,
		.DataOUTEndpointDoubleBank = false,

		.TotalLUNs                 = TOTAL_LUNS,
		.PortNumber = MASS_STORAGE_CORENUM,
	},
};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/


/* HW set up function */
void MassStorageDeviceSetupHardware(void)
{

#ifdef CFG_SDCARD

	SDMMCSetupHardware();
	SDMMCAcquire();
#endif
	
	USB_Init(Disk_MS_Interface.Config.PortNumber, USB_MODE_Device);
}

/* HW set up function */
void MassStorageDeviceShutdownHardware(void)
{
	USB_Disable(Disk_MS_Interface.Config.PortNumber, USB_MODE_Device);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief Event handler for the library USB Configuration Changed event
 * @return Nothing
 */
void EVENT_USB_Device_MassStorage_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= MS_Device_ConfigureEndpoints(&Disk_MS_Interface);
}

/**
 * @brief Event handler for the library USB Control Request reception event
 * @return	Nothing
 */
void EVENT_USB_Device_MassStorage_ControlRequest(void)
{
	MS_Device_ProcessControlRequest(&Disk_MS_Interface);
}

/**
 * @brief Mass Storage class driver callback function
 * @return Nothing
 * @note   The reception of SCSI commands from the host, which must be processed
 */
bool CALLBACK_MS_Device_SCSICommandReceived(USB_ClassInfo_MS_Device_t *const MSInterfaceInfo)
{
	bool CommandSuccess;

	CommandSuccess = SCSI_DecodeSCSICommand(MSInterfaceInfo);
	return CommandSuccess;
}
