/*
 * File Name: RCC.h
 * Layer	: MCAL
 * Module	: RCC
 * Version  : V 0.0
 * Created	:  Feb 11, 2024
 * Author	: Anas Khamees
 */

#ifndef RCC_H_
#define RCC_H_

/******************************************************************************************/
/*************************************** Includes *****************************************/
/******************************************************************************************/
#include"Std_Lib/Std_Libraries.h"

/******************************************************************************************/
/**************************************** Defines *****************************************/
/******************************************************************************************/

#define RCC_BASE_ADDRESS		(0x40023800UL)
#define RCC_CR_OFFSET			(0x00UL)
#define RCC_CSR_OFFSET			(0x74UL)
#define RCC_BDCR_OFFSET			(0x70UL)
#define RCC_AHB1ENR_OFFSET		(0x30UL)
#define RCC_AHB2ENR_OFFSET		(0x34UL)
#define RCC_APB1ENR_OFFSET		(0x40UL)
#define RCC_APB2ENR_OFFSET		(0x44UL)
/*================== Masks to Control the Clock Sources in Stm32F401FCC==================*/
#define MASK_HSI_ON_OFF		(0x00000001UL)
#define MASK_HSE_ON_OFF		(0x00010000UL)
#define MASK_PLL_ON_OFF     (0x01000000UL)
#define MASK_PLLI2S_ON_OFF  (0x04000000UL)
#define MASK_LSE_ON_OFF		(0x00000001UL)
#define MASK_LSI_ON_OFF		(0x00000001UL)
/*============== Defines For Hold the Addresses of the Clock Sources Registers==================*/
 #define RCC_HSI_HSE_PLL_PLLI2S_REGISTER	(uint32_t*)(RCC_BASE_ADDRESS+RCC_CR_OFFSET)
 #define RCC_LSE_REGISTER					(uint32_t*)(RCC_BASE_ADDRESS+RCC_BDCR_OFFSET)
 #define RCC_LSI_REGISTER					(uint32_t*)(RCC_BASE_ADDRESS+RCC_CSR_OFFSET)
/*========================== Masks of System CLK Selection ====================================*/
/*
> 00: HSI oscillator selected as system clock
> 01: HSE oscillator selected as system clock
> 10: PLL selected as system clock
> 11: not allowed
*/
#define MASK_SYSCLK_HSI        (0x00000000UL)
#define MASK_SYSCLK_HSE   	   (0x00000001UL)
#define MASK_SYSCLK_PLL  	   (0x00000002UL)
/*=========================== Masks To Check CLK Source READY flag ===============================*/
#define MASK_HSI_READY		(0x00000002UL)
#define MASK_HSE_READY      (0x00020000UL)
#define MASK_PLL_READY      (0x02000000UL)
/*================================= PLL Clock Source Options =====================================*/
#define RCC_PLLSRC_HSI 	   (0)
#define RCC_PLLSRC_HSE 	   (1)
/*======================================= PLL Configurations =====================================*/
#define RCC_PLL_CFG_MASK 		(0xF0BC8000UL) //To Clear all Configuration Bits of PLL
/*========================== Options of P-parameter of PLL Configurations=========================*/
#define RCC_PLLP_DIV2			2
#define RCC_PLLP_DIV4			4
#define RCC_PLLP_DIV6			6
#define RCC_PLLP_DIV8			8
/*==== Helper Defines for shifting the PLL parameters in Corresponding positions in the register=======*/
#define RCC_PLLM_START_BIT     0
#define RCC_PLLN_START_BIT     6
#define RCC_PLLP_START_BIT     16
#define RCC_PLLSRC_START_BIT   22
#define RCC_PLLQ_START_BIT     24
/*============================= Options for Enable /Disable Peripherals Clock =========================*/
#define RCC_PERIPHERAL_OFF    (0UL)
#define RCC_PERIPHERAL_ON     (1UL)
/*=================================== The Peripherals on AHB1 bus=====================================*/
#define RCC_AHB1ENR_DMA2EN					22
#define RCC_AHB1ENR_DMA1EN					21
#define RCC_AHB1ENR_CRCEN					12
#define RCC_AHB1ENR_GPIOHEN					7
#define RCC_AHB1ENR_GPIOEEN					4
#define RCC_AHB1ENR_GPIODEN					3
#define RCC_AHB1ENR_GPIOCEN					2
#define RCC_AHB1ENR_GPIOBEN					1
#define RCC_AHB1ENR_GPIOAEN					0
/*=================================== The Peripherals on AHB2 bus=====================================*/
#define RCC_AHB2ENR_OTGFSEN					7
/*=================================== The Peripherals on APB1 bus=====================================*/
#define RCC_APB1ENR_PWREN					28
#define RCC_APB1ENR_I2C3EN					23
#define RCC_APB1ENR_I2C2EN					22
#define RCC_APB1ENR_I2C1EN					21
#define RCC_APB1ENR_USART2EN				17
#define RCC_APB1ENR_SPI3EN					15
#define RCC_APB1ENR_SPI2EN					14
#define RCC_APB1ENR_WWDGEN					11
#define RCC_APB1ENR_TIM5EN					3
#define RCC_APB1ENR_TIM4EN					2
#define RCC_APB1ENR_TIM3EN					1
#define RCC_APB1ENR_TIM2EN					0
/*=================================== The Peripherals on APB2 bus=====================================*/
#define RCC_APB2ENR_TIM11EN					18
#define RCC_APB2ENR_TIM10EN					17
#define RCC_APB2ENR_TIM9EN					16
#define RCC_APB2ENR_SYSCFGEN				14
#define RCC_APB2ENR_SPI4EN					13
#define RCC_APB2ENR_SPI1EN					12
#define RCC_APB2ENR_SDIOEN					11
#define RCC_APB2ENR_ADC1EN					8
#define RCC_APB2ENR_USART6EN				5
#define RCC_APB2ENR_USART1EN				4
#define RCC_APB2ENR_TIM1EN				 	0
 /*================= Defines For Hold the Addresses of the Registers of Peripheral Buses================*/
 #define RCC_AHB1_BUS_REGISTER 	(uint32_t*)(RCC_BASE_ADDRESS+RCC_AHB1ENR_OFFSET)
 #define RCC_AHB2_BUS_REGISTER 	(uint32_t*)(RCC_BASE_ADDRESS+RCC_AHB2ENR_OFFSET)
 #define RCC_APB1_BUS_REGISTER 	(uint32_t*)(RCC_BASE_ADDRESS+RCC_APB1ENR_OFFSET)
 #define RCC_APB2_BUS_REGISTER 	(uint32_t*)(RCC_BASE_ADDRESS+RCC_APB2ENR_OFFSET)

/*========================= Options for Configure PreScalar of Low Speed APB1 Bus=====================*/

#define RCC_APB1_PRE_DIV2	(0x00040000) /*(4<<10)*/
#define RCC_APB1_PRE_DIV4	(0x00050000) /*(5<<10)*/
#define RCC_APB1_PRE_DIV8	(0x00060000) /*(6<<10)*/
#define RCC_APB1_PRE_DIV16	(0x00070000) /*(7<<10)*/

/*========================= Options for Configure PreScalar of High Speed APB2 Bus=====================*/
//not to exceed 84 MHz
#define RCC_APB2_PRE_DIV2	(0x00008000) /*(4<<13) See To data sheet to understand*/
#define RCC_APB2_PRE_DIV4	(0x00280000) /*(5<<13)*/
#define RCC_APB2_PRE_DIV8	(0x00300000) /*(6<<13)*/
#define RCC_APB2_PRE_DIV16	(0x00380000) /*(7<<13)*/

/*========================= Options for Configure PreScalar of High Speed AHB Bus=====================*/
#define RCC_AHB_PRE_DIV2	(0x00000080)	/*(8<<4) */
#define RCC_AHB_PRE_DIV4	(0x00000090)	/*(9<<4) */
#define RCC_AHB_PRE_DIV8	(0x000000A0)	/*(10<<4)*/
#define RCC_AHB_PRE_DIV16	(0x000000B0)	/*(11<<4)*/
#define RCC_AHB_PRE_DIV64	(0x000000C0)	/*(12<<4)*/
#define RCC_AHB_PRE_DIV128	(0x000000D0)	/*(13<<4)*/
#define RCC_AHB_PRE_DIV256	(0x000000E0)	/*(14<<4)*/
#define RCC_AHB_PRE_DIV512	(0x000000F0)	/*(15<<4)*/

/*===================== Masks to Clear the Configuration Bits of APB/AHB Prescalar Registers===========*/
#define RCC_APB1_PRESCALAR_MASK (0x00001C00UL) // ~ Toggle it to clear APB(LOW Speed) bits
#define RCC_APB2_PRESCALAR_MASK (0x0000E000UL) // ~ Toggle it to clear APB(HIGH Speed) bits
#define RCC_AHB_PRESCALAR_MASK  (0x000000F0UL) // ~ Toggle it to clear AHB bits


/******************************************************************************************/
/*************************************** Data Types ***************************************/
/******************************************************************************************/
typedef struct{
	uint32_t RCC_CR      ;
	uint32_t RCC_PLLCFGR ;
	uint32_t RCC_CFGR    ;
	uint32_t RCC_CIR     ;
	uint32_t RCC_AHB1RSTR;
	uint32_t RCC_AHB2RSTR;
	uint32_t Reserved0x18;
	uint32_t Reserved0x1C;
	uint32_t RCC_APB1RSTR;
	uint32_t RCC_APB2RSTR;
	uint32_t Reserved0x28;
	uint32_t Reserved0x2C;
	uint32_t RCC_AHB1ENR ;
	uint32_t RCC_AHB2ENR ;
	uint32_t Reserved0x38;
	uint32_t Reserved0x3C;
	uint32_t RCC_APB1ENR ;
	uint32_t RCC_APB2ENR ;
	uint32_t Reserved0x48;
	uint32_t Reserved0x4C;
	uint32_t RCC_AHB1LPENR;
	uint32_t RCC_AHB2LPENR;
	uint32_t Reserved0x58;
	uint32_t Reserved0x5C;
	uint32_t RCC_APB1LPENR;
	uint32_t RCC_APB2LPENR;
	uint32_t Reserved0x68;
	uint32_t Reserved0x6C;
	uint32_t RCC_BDCR	 ;
	uint32_t RCC_CSR	 ;
	uint32_t Reserved0x78;
	uint32_t Reserved0x7C;
	uint32_t RCC_SSCGR	 ;
	uint32_t RCC_PLLI2SCFGR;
	uint32_t Reserved0x88;
	uint32_t RCC_DCKCFGR;
}RCC_Registers_t;

typedef struct {
	 uint32_t PLLM;   /*PLLM Value must be between 2 and 63*/
	 uint32_t PLLP;   /*PLLP must be RCC_PLLP_DIV2, RCC_PLLP_DIV4, RCC_PLLP_DIV6, or RCC_PLLP_DIV8*/
	 uint32_t PLLN;   /*PLLN Value must be between 192 and 432*/
	 uint32_t PLLSRC; /*PLLSRC must be RCC_PLLSRC_HSI or RCC_PLLSRC_HSE*/
	 uint32_t PLLQ;   /*PLLQ Value must be between 2 and 15*/
}PLLCFG_t;

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------  API'S -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief :Enable a clock source by modifying the control bits in the specified register.
 * @param :CLK_Src_Register is a Pointer to the register containing clock source control bits.
 * User should select from the following Options ONLY:
 * - RCC_HSI_HSE_PLL_PLLI2S_REGISTER (RCC_CR Register)
 * - RCC_LSE_REGISTER (RCC_BDCR Register)
 * - RCC_LSI_REGISTER (RCC_CSR Register)
 * @param : CLK_SourceMask is a Mask value indicating the clock source to enable or disable.
 * User should select from the following Options ONLY:
 * - MASK_HSI_ON_OFF
 * - MASK_HSE_ON_OFF
 * - MASK_PLL_ON_OFF
 * - MASK_PLLI2S_ON_OFF
 * - MASK_LSE_ON_OFF
 * - MASK_LSI_ON_OFF
 * @return: ErrorStatus_t Returns OK if the clock source was successfully enabled, otherwise returns NOK.
 */
ErrorStatus_t RCC_EnableClock(uint32_t *CLK_Src_Register,uint32_t CLK_SourceMask);
/**
 * @brief :Disable a clock source by modifying the control bits in the specified register.
 * @param :CLK_Src_Register is a Pointer to the register containing clock source control bits.
 * User should select from the following Options ONLY:
 * - RCC_HSI_HSE_PLL_PLLI2S_REGISTER (RCC_CR Register)
 * - RCC_LSE_REGISTER (RCC_BDCR Register)
 * - RCC_LSI_REGISTER (RCC_CSR Register)
 * @param : CLK_SourceMask is a Mask value indicating the clock source to enable or disable.
 * User should select from the following Options ONLY:
 * - MASK_HSI_ON_OFF
 * - MASK_HSE_ON_OFF
 * - MASK_PLL_ON_OFF
 * - MASK_PLLI2S_ON_OFF
 * - MASK_LSE_ON_OFF
 * - MASK_LSI_ON_OFF
 * @return: ErrorStatus_t Returns OK if the clock source was successfully  disabled, otherwise returns NOK.
 */
ErrorStatus_t RCC_DisableClock(uint32_t *CLK_Src_Register,uint32_t CLK_SourceMask);

/**
 * @brief :This function selects the system clock source by modifying the corresponding bits in the RCC_CFGR register.
 * @param :SYSCLK The desired system clock source to be selected.
 * @note  :User should select the following Options only:
 * 		   - MASK_SYSCLK_HSI
 * 		   - MASK_SYSCLK_HSE
 * 		   - MASK_SYSCLK_PLL
 * @return: ErrorStatus_t Returns OK if the system clock source was successfully selected, otherwise returns NOK.
 */
ErrorStatus_t RCC_Select_SYSCLK(uint32_t SYSCLK);
/**
 * @brief : This function checks the Ready flag of The Clock Sources in the RCC_CR register
 * @param : CLK_Source > The clock source to check its Ready Flag.
 * @note  : User should Select the Following Options Only:
 * 			- MASK_HSI_READY
 *       	- MASK_HSE_READY
 *          - MASK_PLL_READY
 * @return: ErrorStatus_t Returns :
 * 			- OK if the specified clock source is ready,
 * 			- NOT_READY if it is not ready,
 * 			- WRONG_PARAMETER if an invalid clock source is provided.
 */
ErrorStatus_t RCC_CheckReadyCLK(uint32_t CLK_Source);
/**
 * @brief : Configures the PLL (Phase-Locked Loop) according to the provided configuration (M,N,P,Q,SRC).
 * @param : PLL_Config Pointer to a structure containing PLL configuration parameters.
 * @return: ErrorStatus_t Returns OK if PLL configuration is successful, otherwise returns an error status.
 *         - OK: Successful configuration.
 *         - NULL_POINTER: The PLL configuration pointer is NULL.
 *         - WrongPLLM_PARAMETER: Invalid PLLM parameter. PLLM must be between 2 and 63.
 *         - WrongPLLN_PARAMETER: Invalid PLLN parameter. PLLN must be between 192 and 432.
 *         - WrongPLLP_PARAMETER: Invalid PLLP parameter. PLLP must be RCC_PLLP_DIV2, RCC_PLLP_DIV4, RCC_PLLP_DIV6, or RCC_PLLP_DIV8.
 *         - WrongPLLSRC_PARAMETER: Invalid PLLSRC parameter. PLLSRC must be RCC_PLLSRC_HSI or RCC_PLLSRC_HSE.
 *         - WrongPLLQ_PARAMETER: Invalid PLLQ parameter. PLLQ must be between 2 and 15.
 */
ErrorStatus_t RCC_ConfigurePLL(PLLCFG_t* PLL_Config);
/**
 * @brief : This function allows enabling or disabling the clock for a peripheral.
 * @param : BusRegister is a Pointer to the bus register which controlling the clock of the peripheral.
 * @note  : User should select the following options only depending on where the peripheral is located in the register:
 *       - For peripherals in the AHB1 bus register, use RCC_AHB1_BUS_REGISTER .
 *       - For peripherals in the AHB2 bus register, use RCC_AHB2_BUS_REGISTER .
 *       - For peripherals in the APB1 bus register, use RCC_APB1_BUS_REGISTER .
 *       - For peripherals in the APB2 bus register, use RCC_APB2_BUS_REGISTER .
 * @param : Peripheral is the Bit position in the Register of the peripheral whose clock is to be controlled.
 * @note  : User should Pass Specific Options > @ref : Peripherals Clock Enable /Disable in RCC.h
 * @param : PeripheralCFG is a Configuration for the peripheral clock (ON or OFF).
 * @note  : User should Select The following options Only For enabling or disabling a peripheral
 * 			- RCC_PERIPHERAL_OFF
 * 			- RCC_PERIPHERAL_ON
 * @return: ErrorStatus_t Status of the operation (OK if successful, WRONG_PARAMETER if invalid
 *         parameters are provided).
 */
ErrorStatus_t RCC_ControlPeripheralCLK(uint32_t *BusRegister,uint8_t Peripheral,uint8_t PeripheralCFG);

/**
  * @brief : Configures the prescaler for the Low Speed APB1 bus.
  * @param  APB1_Prescalar: Specifies the prescaler value for the APB1 bus.
  * @note  : User should select one of the following options only:
  *            - RCC_APB1_PRE_DIV2: Prescaler set to divide by 2.
  *            - RCC_APB1_PRE_DIV4: Prescaler set to divide by 4.
  *            - RCC_APB1_PRE_DIV8: Prescaler set to divide by 8.
  *            - RCC_APB1_PRE_DIV16: Prescaler set to divide by 16.
  * @return: An ErrorStatus enumeration value:
  *            - OK: No error.
  *            - WRONG_PARAMETER: The provided prescaler value is invalid.
  */
ErrorStatus_t RCC_ConfigurePreScalarLowSpeedAPB(uint32_t APB1_Prescalar);

/**
  * @brief  Configures the prescaler for the High Speed APB2 bus.
  * @param  APB2_Prescalar: Specifies the prescaler value for the APB2 bus.
  * @note   User should select one of the following options only:
  *            - RCC_APB2_PRE_DIV2: Prescaler set to divide by 2.
  *            - RCC_APB2_PRE_DIV4: Prescaler set to divide by 4.
  *            - RCC_APB2_PRE_DIV8: Prescaler set to divide by 8.
  *            - RCC_APB2_PRE_DIV16: Prescaler set to divide by 16.
  * @return An ErrorStatus enumeration value:
  *            - OK: No error.
  *            - WRONG_PARAMETER: The provided prescaler value is invalid.
  */
ErrorStatus_t RCC_ConfigurePreScalarHighSpeedAPB(uint32_t APB2_Prescalar);
/*
  * @brief  Configures the prescalar for the AHB bus.
  * @param  AHB_Prescalar: specifies the AHB prescalar value.
  *          This parameter can be one of the following values Only:
  *            @arg RCC_AHB_PRE_DIV2: AHB prescalar set to divide by 2
  *            @arg RCC_AHB_PRE_DIV4: AHB prescalar set to divide by 4
  *            @arg RCC_AHB_PRE_DIV8: AHB prescalar set to divide by 8
  *            @arg RCC_AHB_PRE_DIV16: AHB prescalar set to divide by 16
  *            @arg RCC_AHB_PRE_DIV64: AHB prescalar set to divide by 64
  *            @arg RCC_AHB_PRE_DIV128: AHB prescalar set to divide by 128
  *            @arg RCC_AHB_PRE_DIV256: AHB prescalar set to divide by 256
  *            @arg RCC_AHB_PRE_DIV512: AHB prescalar set to divide by 512
  * @return An ErrorStatus enumeration value:
  *           - OK: AHB prescalar configured successfully
  *           - WRONG_PARAMETER: AHB_Prescalar parameter is invalid
  */
ErrorStatus_t RCC_ConfigurePreScalarAHB(uint8_t AHB_Prescalar);

#endif /* RCC_H_ */
