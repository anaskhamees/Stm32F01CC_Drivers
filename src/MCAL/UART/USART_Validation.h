



/********************************************************************************************************/
/*                              Status register (USART_SR)                                              */                                                                             
/********************************************************************************************************/
#define PE          (0UL)
#define FE          (1UL)
#define NF          (2UL)
#define ORE         (3UL)
#define IDLE        (4UL)
#define RXNE        (5UL)
#define TC          (6UL)
#define TXE         (7UL)
#define LBD         (8UL)
#define CTS         (9UL)
/* From bit 10 to 31 is reserved*/
/********************************************************************************************************/
/*                              Control register 1 (USART_CR1)                                          */                                                                            
/********************************************************************************************************/
#define SBK         (0UL)   /* Send Break         */
#define RWU         (1UL)   /* Receiver WakeUp    */
#define RE          (2UL)   /* Receiver Enable    */
#define TE          (3UL)   /* Transmitter Enable */
#define IDLEIE      (4UL)   /* IDEL Interrupt Enable        */
#define RXNEIE      (5UL)   /* Read Data Register Not Empty Interrupt Enable */
#define TCIE        (6UL)   /* Transmission Complete Interrupt Enable        */
#define TXEIE       (7UL)   /* Transmit Data Register Empty Interrupt Enable */
#define PEIE        (8UL)   /* Parity Error Interrupt Enable */
#define PS          (9UL)   /* Parity Selection (Even / Odd) */
#define PCE         (10UL)  /* Parity Control Enable         */
#define WAKE        (11UL)  /* WAKE Up Method                */
#define M           (12UL)  /* Word Length                   */
#define UE          (13UL)  /* USART Enable                  */
//#define OVER8       (15UL)  /* Over Sampling Mode: 8 or 16 Samples on 1 bit Time    */

/********************************************************************************************************/
/*                              Control register 3 (USART_CR3)                                          */                                                                          
/********************************************************************************************************/
#define EIE         (0UL)   /* Error interrupt enable */
#define IREN        (1UL)   /* Infrared Data Association mode enable */
#define IRLP        (2UL)   /* Infrared Data Association low-power  */
#define HDSEL       (3UL)   /* Half-duplex selection  */
#define NACK        (4UL)   /* Smartcard NACK enable  */
#define SCEN        (5UL)   /* Smartcard mode enable  */
#define DMAR        (6UL)   /* DMA Enable Reciever    */
#define DMAT        (7UL)   /* DMA Enable Transmitter */
#define RTSE        (8UL)   /* Request to Send Enable */
#define CTSE        (9UL)   /* Clear to Send Enable   */
#define CTSIE       (10UL)  /* Clear to Send Interrupt enable */
#define ONEBIT      (11UL)  /* One sample bit method enable: 1 for one sample bit, 0 for three sample bit method */

/********************************************************************************************************/
/*                             MACROs Fun Validation                                                    */                                                                          
/********************************************************************************************************/


#define IS_DATA_BITS_VALID(dataBits)            (((dataBits)==USART_DATA_BITS_9)||((dataBits)==USART_DATA_BITS_8))

#define IS_OVER_SAMPLE_VALID(overSampling)      (((overSampling)==USART_OVERSAMPLING_8)||((overSampling)==USART_OVERSAMPLING_16))

#define IS_BIT_SAMPLE_METHOD(BitSampleMethod)   (((BitSampleMethod)==USART_SAMPLE_BIT1)||((BitSampleMethod)==USART_SAMPLE_BIT3))

#define IS_STOP_BITS_VALID(stopBits)            (((stopBits)==UART_STOP_BITS_HALF)    ||\
                                                 ((stopBits)==UART_STOP_BITS_ONE)     ||\
                                                 ((stopBits)==UART_STOP_BITS_ONE_HALF)||\
                                                 ((stopBits)==UART_STOP_BITS_TWO)       \
                                                )
