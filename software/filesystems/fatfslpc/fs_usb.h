
#ifndef __FS_USB_H_
#define __FS_USB_H_

#ifdef __cplusplus
extern "C" {
#endif

DSTATUS USB_disk_reset(void);

DSTATUS USB_disk_initialize (void);

DSTATUS USB_disk_status (void);

DRESULT USB_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
);

DRESULT USB_disk_write (
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
);

DRESULT USB_disk_ioctl (
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ifndef __FS_USB_H_ */
