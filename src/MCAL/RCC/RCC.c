/*
 * File Name: RCC.c
 * Layer	: MCAL
 * Module	: RCC
 * Version  : V 0.0
 * Created	:  Feb 11, 2024
 * Author	: Anas Khamees
 */

/************************************************Includes************************************************/
#include "RCC.h"
/************************************************Defines*************************************************/
/*============================================== Masks =================================================*/
#define MASK_SYSCLK_BITs_CLEAR (0x00000003UL)
/************************************************Types***************************************************/

/************************************************Variables***********************************************/
 static volatile RCC_Registers_t*const RCC_Registers=(RCC_Registers_t*)RCC_BASE_ADDRESS;

 /*****************************************Static Functions Prototype*************************************/

/*********************************************APIs Implementation****************************************/


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
ErrorStatus_t RCC_EnableClock(uint32_t *CLK_Src_Register,uint32_t CLK_SourceMask)
{
	ErrorStatus_t ReturnState;
	uint32_t RegisterValue=0;
	if((CLK_Src_Register != RCC_HSI_HSE_PLL_PLLI2S_REGISTER &&
	         CLK_Src_Register != RCC_LSE_REGISTER &&
	         CLK_Src_Register != RCC_LSI_REGISTER) ||
	        (CLK_SourceMask != MASK_HSI_ON_OFF &&
	         CLK_SourceMask != MASK_HSE_ON_OFF &&
	         CLK_SourceMask != MASK_PLL_ON_OFF &&
	         CLK_SourceMask != MASK_PLLI2S_ON_OFF &&
	         CLK_SourceMask != MASK_LSE_ON_OFF &&
	         CLK_SourceMask != MASK_LSI_ON_OFF))
	{
		ReturnState=WRONG_PARAMETER;
	}
	else
	{
		RegisterValue=*CLK_Src_Register;
		RegisterValue&=~CLK_SourceMask;
		RegisterValue|=CLK_SourceMask;
		*CLK_Src_Register=RegisterValue;
		ReturnState=OK;
	}

	return ReturnState;
}

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
ErrorStatus_t RCC_DisableClock(uint32_t *CLK_Src_Register,uint32_t CLK_SourceMask)
{
	ErrorStatus_t ReturnState;
	uint32_t RegisterValue=0;
	if((CLK_Src_Register != RCC_HSI_HSE_PLL_PLLI2S_REGISTER &&
	         CLK_Src_Register != RCC_LSE_REGISTER &&
	         CLK_Src_Register != RCC_LSI_REGISTER) ||
	        (CLK_SourceMask != MASK_HSI_ON_OFF &&
	         CLK_SourceMask != MASK_HSE_ON_OFF &&
	         CLK_SourceMask != MASK_PLL_ON_OFF &&
	         CLK_SourceMask != MASK_PLLI2S_ON_OFF &&
	         CLK_SourceMask != MASK_LSE_ON_OFF &&
	         CLK_SourceMask != MASK_LSI_ON_OFF))
	{
		ReturnState=WRONG_PARAMETER;
	}
	else
	{
		RegisterValue=*CLK_Src_Register;
		RegisterValue&=~CLK_SourceMask;
		*CLK_Src_Register=RegisterValue;
		ReturnState=OK;
	}

	return ReturnState;
}

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
ErrorStatus_t RCC_CheckReadyCLK(uint32_t CLK_Source)
{
	ErrorStatus_t ReturnState;
	if((CLK_Source !=MASK_HSI_READY)&&(CLK_Source !=MASK_HSE_READY)&&(CLK_Source !=MASK_PLL_READY))
	{
		ReturnState=WRONG_PARAMETER;
	}
	else if((RCC_Registers->RCC_CR) & CLK_Source)
	{
		ReturnState=OK;
	}
	else
	{
		ReturnState=NOT_READY;
	}
	return ReturnState;
}

/**
 * @brief :This function selects the system clock source by modifying the corresponding bits in the RCC_CFGR register.
 * @param :SYSCLK The desired system clock source to be selected.
 * @note  :User should select the following Options only:
 * 		   - MASK_SYSCLK_HSI
 * 		   - MASK_SYSCLK_HSE
 * 		   - MASK_SYSCLK_PLL
 * @return: ErrorStatus_t Returns OK if the system clock source was successfully selected, otherwise returns NOK.
 */
ErrorStatus_t RCC_Select_SYSCLK(uint32_t SYSCLK)
{
	ErrorStatus_t ReturnState;
	uint32_t RegisterValue=0;

	if((SYSCLK <MASK_SYSCLK_HSI)||(SYSCLK>MASK_SYSCLK_PLL)) /* If user pass Wrong Arguments*/
	{
		ReturnState=WRONG_PARAMETER;
	}
	else
	{
		RegisterValue=RCC_Registers->RCC_CFGR;
		RegisterValue &=~MASK_SYSCLK_BITs_CLEAR;
		RegisterValue |=SYSCLK;
		RCC_Registers->RCC_CFGR=RegisterValue;
		ReturnState=OK;
	}

	return ReturnState;
}

//Credits to El-NOBY and 3aw

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
ErrorStatus_t RCC_ControlPeripheralCLK(uint32_t* BusRegister,uint32_t Peripheral,uint32_t PeripheralCFG)
{
	ErrorStatus_t ReturnState;
	uint32_t RegisterValue=0;
	/* Validation-Statement : To check the Passed Parameters*/
	if(((BusRegister!=RCC_AHB1_BUS_REGISTER )&&(BusRegister!=RCC_AHB2_BUS_REGISTER )&&(BusRegister!=RCC_APB1_BUS_REGISTER )&&
		(BusRegister!=RCC_APB2_BUS_REGISTER ))||((PeripheralCFG!=RCC_PERIPHERAL_OFF)&&(PeripheralCFG!=RCC_PERIPHERAL_ON)))
	{
		ReturnState=WRONG_PARAMETER;
	}
	else
	{
		RegisterValue=*BusRegister;  		 	   /* Read the current value of the register                                 */
		RegisterValue&=~((1UL)<<Peripheral);       /* Clear the bit corresponding to the specified peripheral in the register*/
		RegisterValue|=PeripheralCFG<<Peripheral;  /* Modify the bit based on the PeripheralCFG parameter                    */
		*BusRegister=RegisterValue;				   /* Write the modified value back to the register                          */
		ReturnState=OK;
	}

	return ReturnState;
}

/**
 * @brief : Configures the PLL (Phase-Locked Loop) according to the provided configuration (M,N,P,Q,SRC).
 * @param : PLL_Config Pointer to a structure containing PLL configuration parameters.
 * @return: ErrorStatus_t Returns OK if PLL configuration is successful, otherwise returns an error status.
 *         - OK: Successful configuration.
 *         - NULL_POINTER: The PLL configuration pointer is NULL.
 *         - WrongPLLM_PARAMETER: Invalid PLLM parameter. PLLM must be between 2 and 63.
 *         - WrongPLLN_PARAMETER: Invalid PLLN parameter. PLLN must be between 192 and 432.
 *         - WrongPLLP_PARAMETER: Invalid PLLP parameter. PLLP must be DIV2, DIV4, DIV6, or DIV8.
 *         - WrongPLLSRC_PARAMETER: Invalid PLLSRC parameter. PLLSRC must be HSE or HSI.
 *         - WrongPLLQ_PARAMETER: Invalid PLLQ parameter. PLLQ must be between 2 and 15.
 */
ErrorStatus_t RCC_ConfigurePLL(PLLCFG_t* PLL_Config)
{
	ErrorStatus_t ReturnState;
	uint32_t RegisterValue=0;
	 if(!PLL_Config)
	 {
		 ReturnState=NULL_POINTER;
	 }

	 else if(PLL_Config->PLLM < 2 || PLL_Config->PLLM > 63)
	 {
		 ReturnState=WrongPLLM_PARAMETER;
	 }

	 else if(PLL_Config->PLLN < 192 || PLL_Config->PLLN > 432)
	 {
		 ReturnState=WrongPLLN_PARAMETER;
	 }

	 else if ((PLL_Config->PLLP != RCC_PLLP_DIV2) && (PLL_Config->PLLP != RCC_PLLP_DIV4) &&
	     (PLL_Config->PLLP != RCC_PLLP_DIV6 )&& (PLL_Config->PLLP != RCC_PLLP_DIV8))
	 {
		 ReturnState=WrongPLLP_PARAMETER;
	 }

	 else if ((PLL_Config->PLLSRC != RCC_PLLSRC_HSE) && (PLL_Config->PLLSRC != RCC_PLLSRC_HSI))
	 {
		 ReturnState=WrongPLLSRC_PARAMETER;
	 }

	 else if (PLL_Config->PLLQ < 2 || PLL_Config->PLLQ > 15)
	 {
		 ReturnState=WrongPLLQ_PARAMETER;
	 }

	 else
	 {
		 /*
		  * In Data sheet
		  * 00: PLLP = 2
		  * 01: PLLP = 4
		  * 10: PLLP = 6
		  * 11: PLLP = 8
		  * Loc_PLLP is a variable to Mapping
		  * 	> 2 to 0
		  * 	> 4 to 1
		  * 	> 6 to 2
		  * 	> 8 to 3
		  */
		 uint32_t Loc_PLLP=((PLL_Config->PLLP)/2)-1;
		 RegisterValue=RCC_Registers->RCC_PLLCFGR;
		 RegisterValue&=RCC_PLL_CFG_MASK;							/* Clear All Configuration Bits						     */
		 RegisterValue|=(PLL_Config->PLLM)<<RCC_PLLM_START_BIT; 	/*Assign M Configuration to corresponding bits position  */
		 RegisterValue|=(PLL_Config->PLLN)<<RCC_PLLN_START_BIT; 	/*Assign N Configuration to corresponding bits position  */
		 RegisterValue|=(Loc_PLLP)<<RCC_PLLP_START_BIT;				/*Assign P Configuration to corresponding bits position  */
		 RegisterValue|=(PLL_Config->PLLSRC)<<RCC_PLLSRC_START_BIT; /*Assign SRC Configuration to corresponding bits position*/
		 RegisterValue|=(PLL_Config->PLLQ)<<RCC_PLLQ_START_BIT;		/*Assign Q Configuration to corresponding bits position  */
		 RCC_Registers->RCC_PLLCFGR=RegisterValue;
		 ReturnState=OK;
	 }
	 return ReturnState;
}

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
ErrorStatus_t RCC_ConfigurePreScalarLowSpeedAPB(uint32_t APB1_Prescalar)
{
	ErrorStatus_t ReturnState;
	uint32_t RegisterValue=0;

	if(APB1_Prescalar!=RCC_APB1_PRE_DIV2 && APB1_Prescalar!=RCC_APB1_PRE_DIV4&&
	   APB1_Prescalar!=RCC_APB1_PRE_DIV8 && APB1_Prescalar!=RCC_APB1_PRE_DIV16)
	{
		ReturnState=WRONG_PARAMETER;
	}
	else
	{
		RegisterValue=RCC_Registers->RCC_CFGR;
		RegisterValue &=~RCC_APB1_PRESCALAR_MASK;
		RegisterValue|=APB1_Prescalar;
		RCC_Registers->RCC_CFGR=RegisterValue;
		ReturnState=OK;
	}

	 return ReturnState;
}

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
ErrorStatus_t RCC_ConfigurePreScalarHighSpeedAPB(uint32_t APB2_Prescalar)
{
	ErrorStatus_t ReturnState;
	uint32_t RegisterValue=0;
	if(APB2_Prescalar!=RCC_APB2_PRE_DIV2 && APB2_Prescalar!=RCC_APB2_PRE_DIV4&&
	   APB2_Prescalar!=RCC_APB2_PRE_DIV8 && APB2_Prescalar!=RCC_APB2_PRE_DIV16)
	{
		ReturnState=WRONG_PARAMETER;
	}
	else
	{
		RegisterValue=RCC_Registers->RCC_CFGR;   	/* Read The register Value 						*/
		RegisterValue &=~RCC_APB2_PRESCALAR_MASK;	/* Clear the APB2_Prescalar Bits in the Register*/
		RegisterValue|=APB2_Prescalar;				/* Modify the New Passed Value 					*/
		RCC_Registers->RCC_CFGR=RegisterValue;		/* Write the new vlaue on the register			*/
		ReturnState=OK;
	}

	 return ReturnState;
}

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
ErrorStatus_t RCC_ConfigurePreScalarAHB(uint8_t AHB_Prescalar)
{
	ErrorStatus_t ReturnState;
	uint32_t RegisterValue=0;
	if(AHB_Prescalar!=RCC_AHB_PRE_DIV2  &&AHB_Prescalar!=RCC_AHB_PRE_DIV4 &&AHB_Prescalar!=RCC_AHB_PRE_DIV8  &&
	   AHB_Prescalar!=RCC_AHB_PRE_DIV16 &&AHB_Prescalar!=RCC_AHB_PRE_DIV64&&AHB_Prescalar!=RCC_AHB_PRE_DIV128&&
	   AHB_Prescalar!=RCC_AHB_PRE_DIV256&&AHB_Prescalar!=RCC_AHB_PRE_DIV512)
	{
		ReturnState=WRONG_PARAMETER;
	}
	else
	{
		RegisterValue=RCC_Registers->RCC_CFGR;
		RegisterValue &=~RCC_AHB_PRESCALAR_MASK;
		RegisterValue|=AHB_Prescalar;
		RCC_Registers->RCC_CFGR=RegisterValue;
		ReturnState=OK;
	}

	 return ReturnState;
}
