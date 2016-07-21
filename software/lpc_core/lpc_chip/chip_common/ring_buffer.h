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

#ifndef __RING_BUFFER_H_
#define __RING_BUFFER_H_

#include "lpc_types.h"

/** @defgroup Ring_Buffer CHIP: Simple ring buffer implementation
 * @ingroup CHIP_Common
 * @{
 */

/**
 * @brief Ring buffer structure
 */
typedef struct {
	uint8_t *bufferBase, *bufferLast;
	uint8_t *bufferIn, *bufferOut;
	int count, used, itemSize;
} RINGBUFF_T;

/**
 * @brief	Initialize ring buffer
 * @param	RingBuff	: Pointer to ring buffer to initialize
 * @param	buffer		: Pointer to buffer to associate with RingBuff
 * @param	itemSize	: Size of each buffer item size (1, 2 or 4 bytes)
 * @param	count		: Size of ring buffer
 * @return	Nothing
 */
void RingBuffer_Init(RINGBUFF_T *RingBuff, void *buffer, int itemSize, int count);

/**
 * @brief	Resets the ring buffer to empty
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Nothing
 */
STATIC INLINE void RingBuffer_Flush(RINGBUFF_T *RingBuff)
{
	RingBuffer_Init(RingBuff, RingBuff->bufferBase, RingBuff->itemSize, RingBuff->count);
}

/**
 * @brief	Return size the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Size of the ring buffer in bytes
 */
STATIC INLINE int RingBuffer_GetSize(RINGBUFF_T *RingBuff)
{
	return RingBuff->count;
}

/**
 * @brief	Return number of items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Number of items in the ring buffer
 */
STATIC INLINE int RingBuffer_GetCount(RINGBUFF_T *RingBuff)
{
	return RingBuff->used;
}

/**
 * @brief	Return number of free items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Number of free items in the ring buffer
 */
STATIC INLINE int RingBuffer_GetFree(RINGBUFF_T *RingBuff)
{
	return RingBuff->count - RingBuff->used;
}

/**
 * @brief	Return number of items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	true if the ring buffer is full, otherwise false
 */
STATIC INLINE bool RingBuffer_IsFull(RINGBUFF_T *RingBuff)
{
	return (bool) (RingBuff->used >= RingBuff->count);
}

/**
 * @brief	Return empty status of ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	true if the ring buffer is empty, otherwise false
 */
STATIC INLINE bool RingBuffer_IsEmpty(RINGBUFF_T *RingBuff)
{
	return (bool) (RingBuff->used == 0);
}

/**
 * @brief	Return not empty status of ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	true if the ring buffer has data, otherwise false
 */
STATIC INLINE bool RingBuffer_NotEmpty(RINGBUFF_T *RingBuff)
{
	return (bool) (RingBuff->used != 0);
}

/**
 * @brief	Insert a single 8-bit value in ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data8		: Byte to insert in ring buffer
 * @return	Nothing
 * @note	Ring buffer will overflow(wraP) without an error
 *			if data is pushed into the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
void RingBuffer_Insert8(RINGBUFF_T *RingBuff, uint8_t data8);

/**
 * @brief	Insert a block of 8-bit values in ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	block8		: Pointer to block of 8-bit data
 * @param	num			: Number of 8-bit data values to insert
 * @return	Nothing
 * @note	Ring buffer will overflow(wraP) without an error
 *			if data is pushed into the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
void RingBuffer_InsertMult8(RINGBUFF_T *RingBuff, const uint8_t *block8, int num);

/**
 * @brief	Insert 16-bit value in ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data16		: 16-bit value to insert in ring buffer
 * @return	Nothing
 * @note	Ring buffer will overflow(wrap) without an error
 *			if data is pushed into the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
void RingBuffer_Insert16(RINGBUFF_T *RingBuff, uint16_t data16);

/**
 * @brief	Insert a block of 16-bit values in ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	block16		: Pointer to block of 16-bit data
 * @param	num			: Number of 16-bit data values to insert
 * @return	Nothing
 * @note	Ring buffer will overflow(wraP) without an error
 *			if data is pushed into the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
void RingBuffer_InsertMult16(RINGBUFF_T *RingBuff, const uint16_t *block16, int num);

/**
 * @brief	Insert 32-bit value in ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data32		: 32-bit value to insert in ring buffer
 * @return	Nothing
 * @note	Ring buffer will overflow(wrap) without an error
 *			if data is pushed into the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
void RingBuffer_Insert32(RINGBUFF_T *RingBuff, uint32_t data32);

/**
 * @brief	Insert a block of 32-bit values in ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	block32		: Pointer to block of 32-bit data
 * @param	num			: Number of 32-bit data values to insert
 * @return	Nothing
 * @note	Ring buffer will overflow(wraP) without an error
 *			if data is pushed into the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
void RingBuffer_InsertMult32(RINGBUFF_T *RingBuff, const uint32_t *block32, int num);

/**
 * @brief	Pop a 8-bit value from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	8-bit value popped from the ring buffer
 * @note	Ring buffer will overflow(wrap) without an error
 *			if data is popped from the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
uint8_t RingBuffer_Pop8(RINGBUFF_T *RingBuff);

/**
 * @brief	Pop a block of 8-bit values from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	block8		: Pointer to 8-bit buffer to fill
 * @param	num			: Size of the passed 8-bit buffer in bytes
 * @return	Number of 8-bit values placed into the buffer
 * @note	Ring buffer will overflow(wrap) without an error
 *			if data is popped from the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
int RingBuffer_PopMult8(RINGBUFF_T *RingBuff, uint8_t *block8, int num);

/**
 * @brief	Pop a 16-bit value from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	16-bit value popped from the ring buffer
 * @note	Ring buffer will overflow(wrap) without an error
 *			if data is popped from the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
uint16_t RingBuffer_Pop16(RINGBUFF_T *RingBuff);

/**
 * @brief	Pop a block of 16-bit values from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	block16		: Pointer to 16-bit buffer to fill
 * @param	num			: Size of the passed 16-bit buffer in bytes
 * @return	Number of 16-bit values placed into the buffer
 * @note	Ring buffer will overflow(wrap) without an error
 *			if data is popped from the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
int RingBuffer_PopMult16(RINGBUFF_T *RingBuff, uint16_t *block16, int num);

/**
 * @brief	Pop a 32-bit value from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	32-bit value popped from the ring buffer
 * @note	Ring buffer will overflow(wrap) without an error
 *			if data is popped from the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
uint32_t RingBuffer_Pop32(RINGBUFF_T *RingBuff);

/**
 * @brief	Pop a block of 32-bit values from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	block32		: Pointer to 32-bit buffer to fill
 * @param	num			: Size of the passed 32-bit buffer in bytes
 * @return	Number of 32-bit values placed into the buffer
 * @note	Ring buffer will overflow(wrap) without an error
 *			if data is popped from the ring buffer beyond the
 *			ring buffer size. Use RingBuffer_IsFull() or
 *			RingBuffer_GetCount() to determine if the ring
 *			buffer is full prior to this call if needed.
 */
int RingBuffer_PopMult32(RINGBUFF_T *RingBuff, uint32_t *block32, int num);

/**
 * @}
 */

#endif /* __RING_BUFFER_H_ */
