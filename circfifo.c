#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int FifoBufferInit(Fifo_Buffer_T *fifo, void* buff, int size)
{
  assert(size > 0);

  fifo->buff = buff;
  fifo->size = size;
  fifo->wr = 0;
}

/**
 * Function writes bytes into fifo from passed buff
 * \return Number of bytes written into buff is returned
 */
int FifoBufferPut(Fifo_Buffer_T *fifo, void *buff, int req_cnt)
{
   /* wr where to write in next cycle */
   /* rd from where read in next cycle */
   /* wr == req_cnt then wr = 0 */
   /* wr == rd buff empty */
   /* wr + 1 == rd then buff is full */

   int free_space = 0;
   int bytes_written = -1;
   int bytes_to_write = 0;

   do
   {
      if (req_cnt <= 0)
      {
         break;
      }

      bytes_written = 0;

      do
      {
         if( (fifo->wr + 1) > fifo->rd )
         {
            /* upside down scenario for write */
            free_space = fifo->size - fifo->wr - (0 == fifo->rd ? 1 : 0);
            if( 0 >= free_space )
            {
               break;
            }

            bytes_written = (req_cnt > free_space) ? free_space : req_cnt;
            memcpy( &(fifo->buff[fifo->wr]), buff, bytes_written );
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

/**
 * Function reads bytes from fifo and stores it in buff
 * \return Number of bytes written into buff is returned
 */
int FifoBufferGet(Fifo_Buffer_T *fifo, void *buff, int req_cnt)
{
   int to_read = 0;
   int bytes_readed = -1;
   int bytes_to_read = 0;

   do
   {
      if (req_cnt <= 0)
      {
         break;
      }

      bytes_readed = 0;

      if( (fifo->wr + 1) <= fifo->rd )
      {
         /* upside down scenario for read */
         to_read = fifo->size - fifo->rd;

         bytes_readed = (req_cnt > to_read) ? to_read : req_cnt;
         memcpy( buff, &(fifo->buff[fifo->rd]), bytes_readed );
         fifo->rd = ((fifo->rd + bytes_readed) == fifo->size) ? 0 : fifo->rd + bytes_readed;
      }

      to_read = fifo->wr - fifo->rd;
      if( to_read <= 0 )
      {
         break;
      }

      bytes_to_read = ((req_cnt - bytes_readed) > to_read) ? to_read : (req_cnt - bytes_readed);
      memcpy( &(((uint8_t*)buff)[bytes_readed]), &(fifo->buff[fifo->rd]), bytes_to_read );
      fifo->rd += bytes_to_read;
      bytes_readed += bytes_to_read;

   }while(0);

   return bytes_readed;
}

