#include "src/LED_CFG.h"
#include "src/LED.h"
#include "build/test/mocks/mock_GPIO.h"
#include "C:/Users/anask/.local/share/gem/ruby/3.0.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"










extern LedCFG_t LEDs[_Led_Number];



void setUp(void)

{

}



void tearDown(void)

{

}



void test_LED_Init_ValidParameters(void)

{

 GPIO_InitPin_CMockIgnoreAndReturn(22, OK);

 GPIO_SetPinValue_CMockIgnoreAndReturn(23, OK);

 UnityAssertEqualNumber((UNITY_INT)((OK)), (UNITY_INT)((LED_Init())), (((void *)0)), (UNITY_UINT)(24), UNITY_DISPLAY_STYLE_INT);

}



void test_LED_Init_InvalidParameters(void)

{

 GPIO_InitPin_CMockIgnoreAndReturn(29, NOK);

 GPIO_SetPinValue_CMockIgnoreAndReturn(30, NOK);

 UnityAssertEqualNumber((UNITY_INT)((NOK)), (UNITY_INT)((LED_Init())), (((void *)0)), (UNITY_UINT)(31), UNITY_DISPLAY_STYLE_INT);

}



void test_LED_SetState_InvalidLedName(void)

{

 uint32_t Led=_Led_Number+1;

 UnityAssertEqualNumber((UNITY_INT)((WRONG_PARAMETER)), (UNITY_INT)((LED_SetState(Led,(0x00000001UL)))), (((void *)0)), (UNITY_UINT)(37), UNITY_DISPLAY_STYLE_INT);

}



void test_LED_SetState_ValidLedName(void)

{

 if(_Led_Number!=0)

 {

  uint32_t Led=_Led_Number-1;

  GPIO_SetPinValue_CMockIgnoreAndReturn(45, OK);

  UnityAssertEqualNumber((UNITY_INT)((OK)), (UNITY_INT)((LED_SetState(Led,(0x00000001UL)))), (((void *)0)), (UNITY_UINT)(46), UNITY_DISPLAY_STYLE_INT);

 }

}



void test_LED_SetState_InvalidState(void)

{

 if(_Led_Number!=0)

 {

  uint32_t Led=_Led_Number-1;

  GPIO_SetPinValue_CMockIgnoreAndReturn(55, OK);

  uint8_t InvalidState=5;

  UnityAssertEqualNumber((UNITY_INT)((WRONG_PARAMETER)), (UNITY_INT)((LED_SetState(Led,InvalidState))), (((void *)0)), (UNITY_UINT)(57), UNITY_DISPLAY_STYLE_INT);

 }



}



void test_LED_SetState_ValidState(void)

{

 if(_Led_Number!=0)

 {

  uint32_t Led=_Led_Number-1;

  GPIO_SetPinValue_CMockIgnoreAndReturn(67, OK);

  UnityAssertEqualNumber((UNITY_INT)((OK)), (UNITY_INT)((LED_SetState(Led,(0x00000001UL)))), (((void *)0)), (UNITY_UINT)(68), UNITY_DISPLAY_STYLE_INT);

 }

}
