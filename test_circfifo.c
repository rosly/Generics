#include "arch.h"

#include <cunit/CUnit.h> /*Required by CUnit functions*/
#include <cunit/Basic.h> /*The results of the Unit Tests are printed to the standard output*/
#include <cunit/TestDB.h> /*This library supports the structure of tests and suites in CUnit*/

#define UT_TEST_CONTINIUES_LOOP_COUNT ((uint32_t)2000000)

/**
  * Table of test inside suite
  */
CU_TestInfo UT_Fifo_Static_Suite[] = {
   { "Invalid parameters", ut_fifo_invalid_param },
   { "Loop test", ut_fifo_loop_test },
   { "Loop test2", ut_fifo_loop_test2 },
   { "Loop test3", ut_fifo_loop_test3 },
   { "Loop test4", ut_fifo_loop_test4 },

   CU_TEST_INFO_NULL,
};

extern void ut_fifo_invalid_param(void)
{
   uint8_t buffer[100];
   uint8_t fifo_data_buffer[100];
   Fifo_Buffer_T fifo_buffer =
   {
      .buffer = fifo_data_buffer,
      .buffer_size = sizeof(fifo_data_buffer),
      .index_write = 0,
      .index_read = 0,
   };

   /* invalid pointer */
   CU_ASSERT_EQUAL( FifoBufferPut(
                       NULL, /* Fifo_Buffer_T *fifo, */
                       buffer, /* void *buffer, */
                       sizeof(buffer) /* int32_t buffer_bytes_count */ ), -1 );

   /* invalid pointer */
   CU_ASSERT_EQUAL( FifoBufferPut(
                       &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                       NULL, /* void *buffer, */
                       sizeof(buffer) /* int32_t buffer_bytes_count */ ), -1 );

   /* invalid buffer size */
   CU_ASSERT_EQUAL( FifoBufferPut(
                       &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                       buffer, /* void *buffer, */
                       0 /* int32_t buffer_bytes_count */ ), -1 );

   /* invalid buffer size */
   CU_ASSERT_EQUAL( FifoBufferPut(
                       &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                       buffer, /* void *buffer, */
                       -1 /* int32_t buffer_bytes_count */ ), -1 );

   /* invalid pointer */
   CU_ASSERT_EQUAL( FifoBufferGet(
                       NULL, /* Fifo_Buffer_T *fifo, */
                       buffer, /* void *buffer, */
                       sizeof(buffer) /* int32_t buffer_bytes_request */ ), -1 );

   /* invalid pointer */
   CU_ASSERT_EQUAL( FifoBufferGet(
                       &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                       NULL, /* void *buffer, */
                       sizeof(buffer) /* int32_t buffer_bytes_request */ ), -1 );

   /* invalid pointer */
   CU_ASSERT_EQUAL( FifoBufferGet(
                       &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                       buffer, /* void *buffer, */
                       0 /* int32_t buffer_bytes_request */ ), -1 );

   /* invalid pointer */
   CU_ASSERT_EQUAL( FifoBufferGet(
                       &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                       buffer, /* void *buffer, */
                       -1 /* int32_t buffer_bytes_request */ ), -1 );
}

extern void ut_fifo_loop_test(void)
{
   uint8_t buffer_upper_check = 0xF0;
   uint8_t buffer[100];
   uint8_t buffer_lower_check = 0xF0;
   uint8_t test_buffer_upper_check = 0xF0;
   uint8_t test_buffer[99];
   uint8_t test_buffer_lower_check = 0xF0;
   uint8_t fifo_data_buffer[100];
   Fifo_Buffer_T fifo_buffer =
   {
      .buffer = fifo_data_buffer,
      .buffer_size = sizeof(fifo_data_buffer),
      .index_write = 0,
      .index_read = 0,
   };

   uint32_t test_loop = 0;
   uint32_t index = 0;
   uint32_t current_complete_index = 0;
   uint32_t complete_index = 0;

   for( index = 0; index < sizeof(buffer); index++ )
   {
      buffer[index] = index;
   }

   printf("000");
   while( test_loop < UT_TEST_CONTINIUES_LOOP_COUNT )
   {
      current_complete_index = (uint32_t)(((float)test_loop / UT_TEST_CONTINIUES_LOOP_COUNT) * 100);
      if( complete_index != current_complete_index )
      {
         printf("\b\b\b%03u", current_complete_index );
         complete_index = current_complete_index;
      }

      CU_ASSERT_EQUAL( FifoBufferPut(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          buffer, /* void *buffer, */
                          sizeof(buffer) /* int32_t buffer_bytes_count */ ), sizeof(fifo_data_buffer) - 1 );

      CU_ASSERT_EQUAL( buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( buffer_lower_check, 0xF0 );

      CU_ASSERT_EQUAL( FifoBufferPut(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          buffer, /* void *buffer, */
                          sizeof(buffer) /* int32_t buffer_bytes_count */ ), 0 );

      CU_ASSERT_EQUAL( buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( buffer_lower_check, 0xF0 );

      memset( test_buffer, 0, sizeof(test_buffer) );
      CU_ASSERT_EQUAL( FifoBufferGet(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          test_buffer, /* void *buffer, */
                          sizeof(test_buffer) /* int32_t buffer_bytes_request */ ), sizeof(test_buffer) );

      CU_ASSERT_EQUAL( test_buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( test_buffer_lower_check, 0xF0 );

      CU_ASSERT_EQUAL( memcmp(buffer, test_buffer, sizeof(test_buffer)), 0 );

      CU_ASSERT_EQUAL( FifoBufferGet(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          test_buffer, /* void *buffer, */
                          sizeof(test_buffer) /* int32_t buffer_bytes_request */ ), 0 );

      test_loop++;
   }

}

extern void ut_fifo_loop_test2(void)
{
   uint8_t buffer_upper_check = 0xF0;
   uint8_t buffer[100];
   uint8_t buffer_lower_check = 0xF0;
   uint8_t test_buffer_upper_check = 0xF0;
   uint8_t test_buffer[49];
   uint8_t test_buffer_lower_check = 0xF0;
   uint8_t fifo_data_buffer[100];
   Fifo_Buffer_T fifo_buffer =
   {
      .buffer = fifo_data_buffer,
      .buffer_size = sizeof(fifo_data_buffer),
      .index_write = 0,
      .index_read = 0,
   };

   uint32_t test_loop = 0;
   uint32_t index = 0;
   uint32_t current_complete_index = 0;
   uint32_t complete_index = 0;

   for( index = 0; index < sizeof(buffer); index++ )
   {
      buffer[index] = index;
   }

   printf("000");
   while( test_loop < UT_TEST_CONTINIUES_LOOP_COUNT )
   {
      current_complete_index = (uint32_t)(((float)test_loop / UT_TEST_CONTINIUES_LOOP_COUNT) * 100);
      if( complete_index != current_complete_index )
      {
         printf("\b\b\b%03u", current_complete_index );
         complete_index = current_complete_index;
      }

      CU_ASSERT_EQUAL( FifoBufferPut(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          buffer, /* void *buffer, */
                          sizeof(test_buffer) /* int32_t buffer_bytes_count */ ), sizeof(test_buffer) );

      CU_ASSERT_EQUAL( buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( buffer_lower_check, 0xF0 );

      CU_ASSERT_EQUAL( FifoBufferPut(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          buffer, /* void *buffer, */
                          sizeof(test_buffer) /* int32_t buffer_bytes_count */ ), sizeof(test_buffer) );

      CU_ASSERT_EQUAL( buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( buffer_lower_check, 0xF0 );

      memset( test_buffer, 0, sizeof(test_buffer) );
      CU_ASSERT_EQUAL( FifoBufferGet(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          test_buffer, /* void *buffer, */
                          sizeof(test_buffer) /* int32_t buffer_bytes_request */ ), sizeof(test_buffer) );

      CU_ASSERT_EQUAL( test_buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( test_buffer_lower_check, 0xF0 );

      CU_ASSERT_EQUAL( memcmp(buffer, test_buffer, sizeof(test_buffer)), 0 );

      memset( test_buffer, 0, sizeof(test_buffer) );
      CU_ASSERT_EQUAL( FifoBufferGet(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          test_buffer, /* void *buffer, */
                          sizeof(test_buffer) /* int32_t buffer_bytes_request */ ), sizeof(test_buffer) );

      CU_ASSERT_EQUAL( test_buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( test_buffer_lower_check, 0xF0 );

      CU_ASSERT_EQUAL( memcmp(buffer, test_buffer, sizeof(test_buffer)), 0 );

      test_loop++;
   }

}

extern void ut_fifo_loop_test3(void)
{
   uint8_t buffer_upper_check = 0xF0;
   uint8_t buffer[2];
   uint8_t buffer_lower_check = 0xF0;
   uint8_t test_buffer_upper_check = 0xF0;
   uint8_t test_buffer[2];
   uint8_t test_buffer_lower_check = 0xF0;
   uint8_t fifo_data_buffer[2];
   Fifo_Buffer_T fifo_buffer =
   {
      .buffer = fifo_data_buffer,
      .buffer_size = sizeof(fifo_data_buffer),
      .index_write = 0,
      .index_read = 0,
   };

   uint32_t test_loop = 0;
   uint32_t index = 0;
   uint32_t current_complete_index = 0;
   uint32_t complete_index = 0;

   for( index = 0; index < sizeof(buffer); index++ )
   {
      buffer[index] = index;
   }

   printf("000");
   while( test_loop < UT_TEST_CONTINIUES_LOOP_COUNT )
   {
      current_complete_index = (uint32_t)(((float)test_loop / UT_TEST_CONTINIUES_LOOP_COUNT) * 100);
      if( complete_index != current_complete_index )
      {
         printf("\b\b\b%03u", current_complete_index );
         complete_index = current_complete_index;
      }

      CU_ASSERT_EQUAL( FifoBufferPut(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          buffer, /* void *buffer, */
                          sizeof(buffer) /* int32_t buffer_bytes_count */ ), 1 );

      CU_ASSERT_EQUAL( buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( buffer_lower_check, 0xF0 );

      CU_ASSERT_EQUAL( FifoBufferPut(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          buffer, /* void *buffer, */
                          sizeof(buffer) /* int32_t buffer_bytes_count */ ), 0 );

      CU_ASSERT_EQUAL( buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( buffer_lower_check, 0xF0 );

      memset( test_buffer, 0, sizeof(test_buffer) );
      CU_ASSERT_EQUAL( FifoBufferGet(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          test_buffer, /* void *buffer, */
                          sizeof(test_buffer) /* int32_t buffer_bytes_request */ ), 1 );

      CU_ASSERT_EQUAL( test_buffer_upper_check, 0xF0 );
      CU_ASSERT_EQUAL( test_buffer_lower_check, 0xF0 );

      CU_ASSERT_EQUAL( buffer[0], test_buffer[0] );

      memset( test_buffer, 0, sizeof(test_buffer) );
      CU_ASSERT_EQUAL( FifoBufferGet(
                          &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                          test_buffer, /* void *buffer, */
                          sizeof(test_buffer) /* int32_t buffer_bytes_request */ ), 0 );

      test_loop++;
   }

}

extern void ut_fifo_loop_test4(void)
{
   uint8_t buffer_upper_check = 0xF0;
   uint8_t buffer[100];
   uint8_t buffer_lower_check = 0xF0;
   uint8_t fifo_data_buffer[100];
   Fifo_Buffer_T fifo_buffer =
   {
      .buffer = fifo_data_buffer,
      .buffer_size = sizeof(fifo_data_buffer),
      .index_write = 0,
      .index_read = 0,
   };

   uint32_t test_loop = 0;
   uint32_t index = 0;
   uint32_t current_complete_index = 0;
   uint32_t complete_index = 0;

   uint8_t sender_state = 0;
   uint8_t receiver_state = 0;
   uint32_t writed_bytes = 0;
   uint32_t to_write_bytes = 0;
   uint32_t to_read_bytes = 0;

   printf("000");
   while( test_loop < UT_TEST_CONTINIUES_LOOP_COUNT )
   {
      current_complete_index = (uint32_t)(((float)test_loop / UT_TEST_CONTINIUES_LOOP_COUNT) * 100);
      if( complete_index != current_complete_index )
      {
         printf("\b\b\b%03u", current_complete_index );
         complete_index = current_complete_index;
      }

      do
      {
         if( writed_bytes < (sizeof(buffer) - 1) )
         {
            to_write_bytes = random() % (sizeof(buffer) - writed_bytes - 1);
            if( 0 == to_write_bytes )
            {
               break;
            }

            for(index = 0; index < to_write_bytes; index++)
            {
               buffer[index] = sender_state++;
            }

            CU_ASSERT_EQUAL( FifoBufferPut(
                                &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                                buffer, /* void *buffer, */
                                to_write_bytes /* int32_t buffer_bytes_count */ ), to_write_bytes );

            writed_bytes += to_write_bytes;

            CU_ASSERT_EQUAL( buffer_upper_check, 0xF0 );
            CU_ASSERT_EQUAL( buffer_lower_check, 0xF0 );
         }
      }while(0);

      do
      {
         if( writed_bytes > 0 )
         {
            bool_t test = True_v;

            to_read_bytes = random() % writed_bytes;
            if( 0 == to_read_bytes )
            {
               break;
            }

            memset( buffer, 0, sizeof(buffer) );
            CU_ASSERT_EQUAL( FifoBufferGet(
                                &fifo_buffer, /* Fifo_Buffer_T *fifo, */
                                buffer, /* void *buffer, */
                                to_read_bytes /* int32_t buffer_bytes_request */ ), to_read_bytes );

            CU_ASSERT_EQUAL( buffer_upper_check, 0xF0 );
            CU_ASSERT_EQUAL( buffer_lower_check, 0xF0 );

            for(index = 0; index < to_read_bytes; index++)
            {
               if( buffer[index] != receiver_state++ )
               {
                  test = False_v;
               }
            }

            CU_ASSERT_EQUAL( True_v, test );

            writed_bytes -= to_read_bytes;
         }
      }while(0);

      test_loop++;
   }
}

int main()
{
   CU_ErrorCode error;

   srandom(time(NULL));
   printf("First random = %li", random());

   if(CU_initialize_registry())
   {
      return -1; // CU_get_error();
   }

   error = CU_register_suites(UT_Fifo_Suites);

   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();

   return CU_get_number_of_tests_failed();;
}

