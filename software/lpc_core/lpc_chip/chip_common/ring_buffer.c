/*
 * @brief Common ring buffer support functions
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

#include "ring_buffer.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize ring buffer */
void RingBuffer_Init(RINGBUFF_T *RingBuff, void *buffer, int itemSize, int count)
{
	RingBuff->bufferBase = RingBuff->bufferIn = RingBuff->bufferOut = buffer;
	RingBuff->bufferLast = RingBuff->bufferBase + (itemSize * count);
	RingBuff->count = count;
	RingBuff->itemSize = itemSize;
	RingBuff->used = 0;
}

/* Return empty status of ring buffer */
void RingBuffer_Insert8(RINGBUFF_T *RingBuff, uint8_t data8)
{
	*RingBuff->bufferIn = data8;
	RingBuff->used++;
	RingBuff->bufferIn++;
	if (RingBuff->bufferIn >= RingBuff->bufferLast) {
		RingBuff->bufferIn = RingBuff->bufferBase;
	}
}

/* Insert a block of 8-bit values in ring buffer */
void RingBuffer_InsertMult8(RINGBUFF_T *RingBuff, const uint8_t *block8, int num)
{
	while (num > 0) {
		RingBuffer_Insert8(RingBuff, *block8);
		block8++;
		num--;
	}
}

/* Insert 16-bit value in ring buffer */
void RingBuffer_Insert16(RINGBUFF_T *RingBuff, uint16_t data16)
{
	uint16_t *buff16 = (uint16_t *) RingBuff->bufferIn;
	*buff16 = data16;
	RingBuff->used++;
	buff16++;
	RingBuff->bufferIn = (uint8_t *) buff16;
	if (RingBuff->bufferIn >= RingBuff->bufferLast) {
		RingBuff->bufferIn = RingBuff->bufferBase;
	}
}

/* Insert a block of 16-bit values in ring buffer */
void RingBuffer_InsertMult16(RINGBUFF_T *RingBuff, const uint16_t *block16, int num)
{
	while (num > 0) {
		RingBuffer_Insert16(RingBuff, *block16);
		block16++;
		num--;
	}
}

/* Insert 32-bit value in ring buffer */
void RingBuffer_Insert32(RINGBUFF_T *RingBuff, uint32_t data32)
{
	uint32_t *buff32 = (uint32_t *) RingBuff->bufferIn;
	*buff32 = data32;
	RingBuff->used++;
	buff32++;
	RingBuff->bufferIn = (uint8_t *) buff32;
	if (RingBuff->bufferIn >= RingBuff->bufferLast) {
		RingBuff->bufferIn = RingBuff->bufferBase;
	}
}

/* Insert a block of 32-bit values in ring buffer */
void RingBuffer_InsertMult32(RINGBUFF_T *RingBuff, const uint32_t *block32, int num)
{
	while (num > 0) {
		RingBuffer_Insert32(RingBuff, *block32);
		block32++;
		num--;
	}
}

/* Pop an 8-bit value from the ring buffer */
uint8_t RingBuffer_Pop8(RINGBUFF_T *RingBuff)
{
	uint8_t data;

	data = *RingBuff->bufferOut;
	RingBuff->used--;
	RingBuff->bufferOut++;
	if (RingBuff->bufferOut >= RingBuff->bufferLast) {
		RingBuff->bufferOut = RingBuff->bufferBase;
	}

	return data;
}

/* Pop a block of 8-bit values from the ring buffer */
int RingBuffer_PopMult8(RINGBUFF_T *RingBuff, uint8_t *block8, int num)
{
	int popped = 0, count;

	/* Copy data up to size in buffer or passed size of buffer,
	   whichever is smaller */
	count = RingBuffer_GetCount(RingBuff);
	if (count > 0) {
		if (count > num) {
			count = num;
		}

		while (count > 0) {
			*block8 = RingBuffer_Pop8(RingBuff);
			count--;
			popped++;
		}
	}

	return popped;
}

/* Pop a 16-bit value from the ring buffer */
uint16_t RingBuffer_Pop16(RINGBUFF_T *RingBuff)
{
	uint16_t data, *buff16 = (uint16_t *) RingBuff->bufferOut;

	data = *buff16;
	RingBuff->used--;
	buff16++;
	if ((uint8_t *) buff16 >= RingBuff->bufferLast) {
		RingBuff->bufferOut = RingBuff->bufferBase;
	}
	else {
		RingBuff->bufferOut = (uint8_t *) buff16;
	}

	return data;
}

/* Pop a block of 16-bit values from the ring buffer */
int RingBuffer_PopMult16(RINGBUFF_T *RingBuff, uint16_t *block16, int num)
{
	int popped = 0, count;

	/* Copy data up to size in buffer or passed size of buffer,
	   whichever is smaller */
	count = RingBuffer_GetCount(RingBuff);
	if (count > 0) {
		if (count > num) {
			count = num;
		}

		while (count > 0) {
			*block16 = RingBuffer_Pop16(RingBuff);
			count--;
			popped++;
		}
	}

	return popped;
}

/* Pop a 32-bit value from the ring buffer */
uint32_t RingBuffer_Pop32(RINGBUFF_T *RingBuff)
{
	uint32_t data, *buff32 = (uint32_t *) RingBuff->bufferOut;

	data = *buff32;
	RingBuff->used--;
	data++;
	if ((uint8_t *) data >= RingBuff->bufferLast) {
		RingBuff->bufferOut = RingBuff->bufferBase;
	}
	else {
		RingBuff->bufferOut = (uint8_t *) data;
	}

	return data;
}

/* Pop a block of 32-bit values from the ring buffer */
int RingBuffer_PopMult32(RINGBUFF_T *RingBuff, uint32_t *block32, int num)
{
	int popped = 0, count;

	/* Copy data up to size in buffer or passed size of buffer,
	   whichever is smaller */
	count = RingBuffer_GetCount(RingBuff);
	if (count > 0) {
		if (count > num) {
			count = num;
		}

		while (count > 0) {
			*block32 = RingBuffer_Pop32(RingBuff);
			count--;
			popped++;
		}
	}

	return popped;
}
