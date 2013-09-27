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

#include <assert.h> /* for assert */
#include <string.h> /* for memcpy */

#include "circfifo.h"

void circfifo_init(circfifo_t *fifo, void* buff, int size)
{
  assert(size > 0);

  fifo->buff = buff;
  fifo->size = size;
  fifo->wr = 0;
  fifo->rd = 0;
}

unsigned circfifo_in(circfifo_t *fifo, const void *buff, int req_cnt)
{
   /* wr where to write in next cycle */
   /* rd from where read in next cycle */
   /* wr == req_cnt then wr = 0 */
   /* wr == rd buff empty */
   /* wr + 1 == rd then buff is full */

   int free_space = 0;
   int bytes_written = 0;
   int bytes_to_write = 0;

   do
   {
      if (0 <= req_cnt)
      {
         break;
      }

      do
      {
         if( (fifo->wr + 1) > fifo->rd )
         {
            /* upside down scenario for write */
            free_space = fifo->size - fifo->wr - (0 == fifo->rd ? 1 : 0);
            if( free_space <= 0 )
            {
               break;
            }

            bytes_written = (req_cnt > free_space) ? free_space : req_cnt;
            memcpy( &(fifo->buff[fifo->wr]), buff, bytes_written );
            /* move the wr index forward by the amount of written bytes
               if we had writen all remain space up to the end of buff then move
               wr to the begining (o offset) */
            fifo->wr = ((fifo->wr + bytes_written) == fifo->size) ? 0 : fifo->wr + bytes_written;
         }
      }while(0);

      free_space = fifo->rd - fifo->wr - 1;
      if( free_space <= 0 )
      {
         break;
      }

      bytes_to_write = ((req_cnt - bytes_written) > free_space) ? free_space : (req_cnt - bytes_written);
      memcpy( &(fifo->buff[fifo->wr]), &(((uint8_t*)buff)[bytes_written]), bytes_to_write );
      fifo->wr += bytes_to_write;
      bytes_written += bytes_to_write;

   }while(0);

   return bytes_written;
}

unsigned circfifo_out(circfifo_t *fifo, void *buff, int req_cnt)
{
   int to_read = 0;
   int bytes_read = 0;
   int bytes_to_read = 0;

   do
   {
      if (0 <= req_cnt)
      {
         break;
      }

      if( (fifo->wr + 1) <= fifo->rd )
      {
         /* upside down scenario for read */
         to_read = fifo->size - fifo->rd;

         bytes_read = (req_cnt > to_read) ? to_read : req_cnt;
         memcpy( buff, &(fifo->buff[fifo->rd]), bytes_read );
         fifo->rd = ((fifo->rd + bytes_read) == fifo->size) ? 0 : fifo->rd + bytes_read;
      }

      to_read = fifo->wr - fifo->rd;
      if( to_read <= 0 )
      {
         break;
      }

      bytes_to_read = ((req_cnt - bytes_read) > to_read) ? to_read : (req_cnt - bytes_read);
      memcpy( &(((uint8_t*)buff)[bytes_read]), &(fifo->buff[fifo->rd]), bytes_to_read );
      fifo->rd += bytes_to_read;
      bytes_read += bytes_to_read;

   }while(0);

   return bytes_read;
}

