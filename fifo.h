typedef struct Fifo_Buffer_Tag
{
   uint8_t *buffer;
   int32_t buffer_size;
   int32_t index_write;
   int32_t index_read;
} Fifo_Buffer_T;

extern Return_Code_T FifoBufferInit(Fifo_Buffer_T *fifo, void* buffer, int32_t buffer_size);

extern int32_t FifoBufferPut(Fifo_Buffer_T *fifo, void *buffer, int32_t buffer_bytes_count);

extern int32_t FifoBufferGet(Fifo_Buffer_T *fifo, void *buffer, int32_t buffer_bytes_request);

extern int32_t FifoBufferToFile(Fifo_Buffer_T *fifo, int file, int32_t transfer_bytes_request);

extern int32_t FifoBufferFromFile(Fifo_Buffer_T *fifo, int file, int32_t buffer_bytes_request);

