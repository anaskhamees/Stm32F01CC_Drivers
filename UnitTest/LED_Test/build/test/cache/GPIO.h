#include "src/Std_Libraries.h"
typedef struct

{

    void* GPIO_Port;

    uint32_t GPIO_Pin;

    uint32_t GPIO_Mode;

    uint32_t GPIO_Speed;









    uint32_t GPIO_AF ;

} GPIO_CFG_t;

ErrorStatus_t GPIO_InitPin(GPIO_CFG_t* GPIO_Config);

ErrorStatus_t GPIO_SetPinValue(void* GPIO_Port,uint32_t GPIO_Pin,uint32_t GPIO_PinState);

ErrorStatus_t GPIO_GetPinValue(void* GPIO_Port,uint32_t GPIO_Pin,uint32_t* GPIO_PinState);
