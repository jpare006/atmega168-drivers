#include "unity.h"

#include "adc.h"
#include "mock_atmega168_adc.h"

uint8_t expectedData;

void setUp(void)
{
	expectedData = 200;
}

void tearDown(void)
{
}

void test_AdcInit(void)
{
    create_Expect();

    adc_init();
}

void test_AdcRead(void)
{
	uint8_t actualData;

	read_ExpectAndReturn(expectedData);

	actualData = adc_read();
	TEST_ASSERT_EQUAL_UINT8(expectedData, actualData);
}