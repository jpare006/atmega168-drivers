#include "unity.h"

#include "uart.h"
#include "mock_atmega168_uart.h"
#include "CircularBuffer.h"


#define BAUD 9600

char data;
char data_arr[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

char string[] = "Hello, World!";
char stringKeyboard[] = "Hello, World!\n";



//fill buffer with num data element by simulating ISR being called
static void fill_buffer(int num)
{
    for(int i = 0; i < num; ++i)
    {
        receive_ExpectAndReturn(data + i);
        uart_rx_complete_ISR();
    }
}

static void fill_buffer_with_string(int option)
{
	if(option == MCU)
	{
		for(int i = 0; i < sizeof(string); ++i)
	    {
	        receive_ExpectAndReturn(string[i]);
	        uart_rx_complete_ISR();
	    }	
	}
	else
	{
		for(int i = 0; i < sizeof(stringKeyboard) - 1; ++i)
	    {
	        receive_ExpectAndReturn(stringKeyboard[i]);
	        uart_rx_complete_ISR();
	    }
	}
}


void setUp(void)
{
	data = 'a';
	create_Ignore();
	uart_init(BAUD);
}

void tearDown(void)
{
	destroy_Ignore();
	uart_terminate();
}

void test_uart_InitializationAndTerminationOfUart(void)
{
    //set expectations
    create_Expect(BAUD);
    destroy_Expect();

    //test expectations
    uart_init(BAUD);
    uart_terminate();
}

void test_IsSendReady_ReadyImmediatly(void)
{
	is_send_ready_ExpectAndReturn(TRUE);

	TEST_ASSERT_TRUE(uart_send_ready());
}

void test_IsSendReady_NotReady(void)
{
	is_send_ready_ExpectAndReturn(FALSE);

	TEST_ASSERT_FALSE(uart_send_ready());
}

void test_Send_SendChar_ReadyImmediately(void)
{
	is_send_ready_ExpectAndReturn(TRUE);
	send_Expect(data);

	uart_send(data);
}

void test_Send_SendChar_NotReadyImmediately(void)
{
	is_send_ready_ExpectAndReturn(FALSE);
	is_send_ready_ExpectAndReturn(FALSE);
	is_send_ready_ExpectAndReturn(FALSE);
	is_send_ready_ExpectAndReturn(FALSE);
	is_send_ready_ExpectAndReturn(TRUE);
	send_Expect(data);

	uart_send(data);
}

void test_SendString_FuncReceivesCString_Success(void)
{
	int status;
	int string_sz = sizeof(string); 

	for (int i = 0; i < string_sz; ++i)
	{
        is_send_ready_ExpectAndReturn(TRUE);
		send_Expect(string[i]);
	}
	status = uart_send_string(string, string_sz);
	TEST_ASSERT_EQUAL(SUCCESS, status);
}

void test_SendString_InputCharArrayWithNoNullChar_Fail(void)
{
	int status;
	int string_sz = 10;

	status = uart_send_string(data_arr, string_sz);
	TEST_ASSERT_EQUAL(FAIL, status);
}

void test_SendString_EmptyString_Fail(void)
{
	int status;
	char emptyString[5] = {};

	status = uart_send_string(emptyString, 5);

	TEST_ASSERT_EQUAL(FAIL, status);
}

void test_ISR_InterruptServiceRoutineCalled(void)
{
    receive_ExpectAndReturn(data);

	uart_rx_complete_ISR();
}

void test_Available_ZeroBytesAvailable(void)
{
    uint8_t num_bytes;

    num_bytes = uart_available();
    TEST_ASSERT_EQUAL(0, num_bytes);
}

void test_Available_MultipleBytesAvailable(void)
{
    uint8_t num_bytes;
    fill_buffer(10);

    num_bytes = uart_available();
    TEST_ASSERT_EQUAL(10, num_bytes);
}

void test_Read_ReadChar_Success(void)
{
	int status;
	uint8_t byte;

    //fill buffer with one data element by simulating ISR being called
    receive_ExpectAndReturn(data);
    uart_rx_complete_ISR();


	status = uart_read(&byte);

	TEST_ASSERT_EQUAL(SUCCESS, status);
	TEST_ASSERT_EQUAL(data, byte);
}

void test_Read_ReadChar_Fail(void)
{
	int status;
	uint8_t dummy;

	status = uart_read(&dummy);
	TEST_ASSERT_EQUAL(FAIL, status);
}

void test_ReadString_NullCharReceived_Success(void)
{
	int size_read;
	char read_buffer[CBUF_SIZE] = {};
	fill_buffer_with_string(MCU);

	size_read = uart_read_string(read_buffer, MCU);

	int i = 0;
	int complete = 0;
	while(!complete)
	{
		TEST_ASSERT_EQUAL_CHAR(string[i], read_buffer[i]);

		if (read_buffer[i] == '\0') {complete = 1;}

		++i;
	}

	TEST_ASSERT_EQUAL(sizeof(string), size_read);
}

void test_ReadString_NullCharNeverReceived_OverflowOccurs_Fail(void)
{
	int size_read;
	char read_buffer[CBUF_SIZE] = {};
	fill_buffer(CBUF_SIZE);

	size_read = uart_read_string(read_buffer, MCU);

	TEST_ASSERT_EQUAL(CHAR_NOT_FOUND, size_read);
}

void test_ReadString_ReadFail_RestoreDataPreviouslyPresentInBuffer(void)
{
	int cnt = 10;
	//simulate 10 characters being present in circular buffer
	//then overflowing buffer so read string returns
	fill_buffer(cnt);
	fill_buffer(CBUF_SIZE - cnt);

	//call uart_read_string and test for error code
	int status;
	char read_buffer[CBUF_SIZE] = {};
	status = uart_read_string(read_buffer, MCU);

	TEST_ASSERT_EQUAL(CHAR_NOT_FOUND, status);

	//make sure data that was in buffer prior to uart_read_string is still there
	char test_read;
	for(int i = 0; i < cnt; ++i)
	{
		//read one char at a time and compare to expected
		uart_read(&test_read);
		TEST_ASSERT_EQUAL_CHAR(data + i, test_read);
	}
}

void test_ReadString_GetOptionToChooseKeyboardAsInputMethod_Success(void)
{
	int size_read;
	char read_buffer[CBUF_SIZE] = {};
	fill_buffer_with_string(KEYBOARD);

	size_read = uart_read_string(read_buffer, KEYBOARD);

	int i = 0;
	int complete = 0;
	while(!complete)
	{
		TEST_ASSERT_EQUAL_CHAR(stringKeyboard[i], read_buffer[i]);

		if (read_buffer[i] == '\n') {complete = 1;}

		++i;
	}

	TEST_ASSERT_EQUAL(sizeof(stringKeyboard) - 1, size_read);
}

void test_ReadString_UnkownInputMethod(void)
{
	int size_read;
	char read_buffer[CBUF_SIZE] = {};

	fill_buffer_with_string(MCU);
	size_read = uart_read_string(read_buffer, UNKNOWN);

	TEST_ASSERT_EQUAL(UNKNOWN, size_read);
}

void test_flush(void)
{
	fill_buffer(2);
	TEST_ASSERT_EQUAL(2, uart_available());

	uart_flush();
	TEST_ASSERT_EQUAL(0, uart_available());
}
