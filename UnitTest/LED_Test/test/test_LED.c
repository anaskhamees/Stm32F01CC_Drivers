#ifdef TEST

#include "unity.h"
/*Create Mocks for GPIO*/
#include "mock_GPIO.h"
#include "LED.h"
#include "LED_CFG.h"


extern LedCFG_t LEDs[_Led_Number];

void setUp(void)
{
}

void tearDown(void)
{
}

void test_LED_Init_ValidParameters(void)
{
	GPIO_InitPin_IgnoreAndReturn(OK);
	GPIO_SetPinValue_IgnoreAndReturn(OK);
	TEST_ASSERT_EQUAL(OK,LED_Init());
}

void test_LED_Init_InvalidParameters(void)
{
	GPIO_InitPin_IgnoreAndReturn(NOK);
	GPIO_SetPinValue_IgnoreAndReturn(NOK);
	TEST_ASSERT_EQUAL(NOK,LED_Init());
}

void test_LED_SetState_InvalidLedName(void)
{
	uint32_t Led=_Led_Number+1;
	TEST_ASSERT_EQUAL(WRONG_PARAMETER,LED_SetState(Led,LED_ON));
}

void test_LED_SetState_ValidLedName(void)
{
	if(_Led_Number!=0)
	{
		uint32_t Led=_Led_Number-1;
		GPIO_SetPinValue_IgnoreAndReturn(OK);
		TEST_ASSERT_EQUAL(OK,LED_SetState(Led,LED_ON));
	}
}

void test_LED_SetState_InvalidState(void)
{
	if(_Led_Number!=0)
	{
		uint32_t Led=_Led_Number-1;
		GPIO_SetPinValue_IgnoreAndReturn(OK);
		uint8_t InvalidState=5;
		TEST_ASSERT_EQUAL(WRONG_PARAMETER,LED_SetState(Led,InvalidState));
	}

}

void test_LED_SetState_ValidState(void)
{
	if(_Led_Number!=0)
	{
		uint32_t Led=_Led_Number-1;
		GPIO_SetPinValue_IgnoreAndReturn(OK);
		TEST_ASSERT_EQUAL(OK,LED_SetState(Led,LED_ON));
	}
}


#endif // TEST
