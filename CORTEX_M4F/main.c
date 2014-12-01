#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "stm32f429i_discovery.h"


void USART1_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, *s);
        s++;
    }
}




void I2C_Config(void)
{


    /* 
    *         SCL = PB6
    *         SDA = PB7
    */


  GPIO_InitTypeDef GPIO_InitStructure;

  
  /* Enable IOE_I2C and IOE_I2C_GPIO_PORT & Alternate Function clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOB , ENABLE);

  
  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); 
    
  /* SCL and SDA pins configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the I2C event priority */
  NVIC_InitStructure.NVIC_IRQChannel                   = I2C1_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel                   = I2C1_ER_IRQn;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the I2C peripheral */
  I2C_DeInit( I2C1);
        
  I2C_InitTypeDef I2C_InitStructure;


  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x05;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
  /* Initialize the I2C peripheral */
  I2C_Init( I2C1 , &I2C_InitStructure);

 
       
  I2C_ITConfig(I2C1,I2C_IT_EVT , ENABLE);
  I2C_ITConfig(I2C1,I2C_IT_BUF , ENABLE);
  I2C_Cmd( I2C1 , ENABLE);
  
}







void I2C1_EV_IRQHandler(void)
{

    switch(I2C_GetLastEvent( I2C1))
    {

    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED :

        /* get I2C address

            Do nothing
      
        */         
        
        break;
    case I2C_EVENT_SLAVE_BYTE_RECEIVED:

    /*
        BYTE_RECEIVED

        put the byte on UART1

    */
        USART_SendData(USART1, I2C_ReceiveData(I2C1));
        
        break;
        case I2C_EVENT_SLAVE_STOP_DETECTED:

    /*
        BYTE_RECEIVED

        put the byte on UART1

    */
        break;

      default:
        break;
    }
     
}

void I2C1_ER_IRQHandler(void)
{
  
  
  if (I2C_GetITStatus(I2C1, I2C_IT_AF)) 
  {
    USART1_puts("I2C_IT_AF");
    I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
  }
  else if(I2C_GetITStatus(I2C1, I2C_IT_BERR))
  {
    USART1_puts("I2C1, I2C_IT_BERR");
  }
  else if(I2C_GetITStatus(I2C1, I2C_IT_ARLO))
  {
    USART1_puts("I2C_IT_ARLO");
  }
    else if(I2C_GetITStatus(I2C1, I2C_IT_OVR))
  {
    USART1_puts("I2C_IT_OVR");
  }
    else if(I2C_GetITStatus(I2C1, I2C_IT_PECERR))
  {
    USART1_puts("I2C_IT_PECERR");
  }
    else if(I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT))
  {
    USART1_puts("I2C_IT_TIMEOUT");
  }
  else if(I2C_GetITStatus(I2C1, I2C_IT_SMBALERT))
  {
    USART1_puts("I2C_IT_SMBALERT");
  }
    
}




void RCC_Configuration(void)
{
      /* --------------------------- System Clocks Configuration -----------------*/
      /* USART1 clock enable */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
      /* GPIOA clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // USART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // USART1_RX
}
 

 void USART1_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 9600 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}


int main(){

	RCC_Configuration();
  	GPIO_Configuration();
  	USART1_Configuration();
 	
	I2C_Config();
	

	
    	while(1)
    	{

    		
    	}	


}
