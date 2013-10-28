/*
 * This file is a part of Generics project
 * Copyright (c) 2013, Radoslaw Biernaki <radoslaw.biernacki@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3) No personal names or organizations' names associated with the 'Generics' project
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE GENERICS PROJET AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __CIRCFIFO_H_
#define __CIRCFIFO_H_ 1

#include "arch.h"

typedef struct circfifo_tag
{
   /** pointer to buffer for data storadge */
   uint8_t *buff;
   /** size of the buffer in bytes */
   int size;
   /** index where to write in next cycle */
   int wr;
   /** index from where read in next cycle */
   int rd;

   /**
    * wr == req_cnt then wr = 0
    * wr == rd buff empty
    * wr + 1 == rd then buff is full */
} circfifo_t;

void circfifo_init(circfifo_t *fifo, void* buff, int size);

/**
 * Function writes bytes into fifo from passed buff
 * \return Number of bytes written into buff
 */
unsigned circfifo_in(circfifo_t *fifo, const void *buff, int req_cnt);

/**
 * Function reads bytes from fifo and stores it in buff
 * \return Number of bytes written into buff is returned
 */
unsigned circfifo_out(circfifo_t *fifo, void *buff, int req_cnt);

#endif /* __CIRCFIFO_H_ */

