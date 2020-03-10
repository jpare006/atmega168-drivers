#include "unity.h"

#include "timer.h"
#include "mock_atmega168_timer.h"

uint16_t period;

void setUp(void)
{
	period = 50;
	create_timer_Ignore();
	on_Ignore();

	timer_init(period);
	timer_on();
}

void tearDown(void)
{
}

void test_timer_Init(void)
{
    create_timer_Expect();

    timer_init(period);

    TEST_ASSERT_TRUE(TimerFlag == 0);
}

void test_timer_On(void)
{
	on_Expect();

	timer_on();
}

void test_timer_Off()
{
	off_Expect();

	timer_off();
}

void test_timer_ISR_SetsFlagToOne_AfterperiodAmountOfMillisecondsHaveElapsed(void)
{
	TEST_ASSERT_TRUE(TimerFlag == 0);

	//make sure flag remains zero
	for(int i = 0; i < (period - 1); ++i)
	{
		TimerISR();
		TEST_ASSERT_TRUE(TimerFlag == 0);
	}
	//until TimerISR has been called period amount of times
	TimerISR();
	TEST_ASSERT_TRUE(TimerFlag == 1);
}

void test_timer_ISR_countdownVariableResetAfterReachingZero(void)
{
	TEST_ASSERT_TRUE(TimerFlag == 0);

	// call TimerISR period amount of times to ensure internal variable
	// reaches zero
	for(int i = 0; i < period; ++i) {TimerISR();}
	//at this point TimerFlag should = 1
	TEST_ASSERT_TRUE(TimerFlag == 1);
	//reset flag and call TimerISR period amount of times again
	TimerFlag = 0;
	for(int i = 0; i < period; ++i) {TimerISR();}

	//if internal variable was reset, TimerFlag should = 1
	TEST_ASSERT_TRUE(TimerFlag == 1);
}