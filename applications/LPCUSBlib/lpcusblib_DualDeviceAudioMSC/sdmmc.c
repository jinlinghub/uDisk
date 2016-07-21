/*
 * @brief SDMMC access functions
 *
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

#include <stdio.h>
#include <string.h>
#include "board.h"
#include "sdmmc.h"
#include "led.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
unsigned char MMC_disk_reset(void);

#ifdef CFG_SDCARD
/* SDMMC card info structure */
mci_card_struct sdcardinfo;
static volatile int32_t sdio_wait_exit = 0;
#endif

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

#ifdef CFG_SDCARD

/* Delay callback for timed SDIF/SDMMC functions */
static void sdmmc_waitms(uint32_t time)
{
	int32_t curr = (int32_t) Chip_RIT_GetCounter(LPC_RITIMER);
	int32_t final = curr + ((SystemCoreClock / 1000) * time);

	/* If the value is zero let us not worry about it */
	if (!time) {
		return;
	}

	if ((final < 0) && (curr > 0)) {
		while (Chip_RIT_GetCounter(LPC_RITIMER) < (uint32_t) final) {}
	}
	else {
		while ((int32_t) Chip_RIT_GetCounter(LPC_RITIMER) < final) {}
	}
}

/*  Sets up the SD event driven wakeup */
static void sdmmc_setup_wakeup(uint32_t bits)
{
	/* Wait for IRQ - for an RTOS, you would pend on an event here with a IRQ based wakeup. */
	NVIC_ClearPendingIRQ(SDIO_IRQn);
	sdio_wait_exit = 0;
	Chip_SDMMC_SetIntMask(LPC_SDMMC, bits);
	NVIC_EnableIRQ(SDIO_IRQn);
}

/* A better wait callback for SDMMC driven by the IRQ flag */
static uint32_t sdmmc_irq_driven_wait(void)
{
	uint32_t status;

	/* Wait for event, would be nice to have a timeout, but keep it  simple */
	while (sdio_wait_exit == 0) {}

	/* Get status and clear interrupts */
	status = Chip_SDMMC_GetIntStatus(LPC_SDMMC);

	return status;
}

#endif


/* HW set up function */
void SDMMCSetupHardware(void)
{

//	DEBUGSTR("SDMMCSetupHardware()\r\n");

#ifdef CFG_SDCARD
	memset(&sdcardinfo, 0, sizeof(sdcardinfo));
	sdcardinfo.evsetup_cb = sdmmc_setup_wakeup;
	sdcardinfo.waitfunc_cb = sdmmc_irq_driven_wait;
	sdcardinfo.msdelay_func = sdmmc_waitms;

	/*  SD/MMC initialization */
	Board_SDMMC_Init();

	/* The SDIO driver needs to know the SDIO clock rate */
	Chip_SDMMC_Init(LPC_SDMMC);

	/* Wait for a card to be inserted */
#ifndef BOARD_NGX_XPLORER_18304330
	/* NGX board ignored SD_CD pin */
	/* Wait for a card to be inserted (note CD is not on the SDMMC power rail and can be polled without enabling SD slot power */
	while (Chip_SDMMC_CardNDetect(LPC_SDMMC) == 0) {}
#endif

#endif
	MMC_disk_reset();
}

/* HW set up function */
void SDMMCShutdownHardware(void)
{
	Chip_SDMMC_DeInit(LPC_SDMMC);
}

void Green2Flashes()
{
	volatile int i, j, k;
	
	Board_LED_Set(GreenLED, LEDOFF);
	for(j = 0; j < 10000; j++) for(k = 0; k < 100; k++);
	for(i = 0; i < 2; i++)
	{
		Board_LED_Set(GreenLED, LEDON);
		for(j = 0; j < 10000; j++) for(k = 0; k < 200; k++);
		Board_LED_Set(GreenLED, LEDOFF);
		for(j = 0; j < 10000; j++) for(k = 0; k < 200; k++);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

#ifdef CFG_SDCARD

void SDMMCAcquire(void)
{
	/* Acquire the card once ready */
	if (!Chip_SDMMC_Acquire(LPC_SDMMC, &sdcardinfo)) {
		DEBUGOUT("Card Acquire failed...\r\n");
		while (1) 
		{
			volatile int j, k;

			Green2Flashes();
			for(j = 0; j < 10000; j++) for(k = 0; k < 1000; k++);
		}
	}
}

/**
 * @brief	SDIO controller interrupt handler
 * @return	Nothing
 */
void SDIO_IRQHandler(void)
{
	/* All SD based register handling is done in the callback
	   function. The SDIO interrupt is not enabled as part of this
	   driver and needs to be enabled/disabled in the callbacks or
	   application as needed. This is to allow flexibility with IRQ
	   handling for applicaitons and RTOSes. */
	/* Set wait exit flag to tell wait function we are ready. In an RTOS,
	   this would trigger wakeup of a thread waiting for the IRQ. */
	NVIC_DisableIRQ(SDIO_IRQn);
	sdio_wait_exit = 1;
}

#endif
