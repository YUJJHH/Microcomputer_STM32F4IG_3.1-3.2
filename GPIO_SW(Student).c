//************************************//
// GPIO(INPUT mode:SWITCH) �ǽ�
//	

#include "stm32f4xx.h"

void _GPIO_Init(void);
void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

int main(void)
{
	_GPIO_Init();		// GPIO(LED(OUTPUT), SWITCH(INPUT)) �ʱ�ȭ
	GPIOG->ODR &= ~0x00FF;	// �ʱⰪ: LED0~7 Off
    
	//GPIO LED0 ON (Test 1)
	GPIOG->ODR |=0x0001; 	// GPIOG->ODR.0  (G0)  'H'(LED ON)
    
	while(1)
	{
		
              GPIOB->ODR |= 0x00FF; // LED0 ON	
        
        
        
          //GPIO SW1 �Է����� LED1 On/Off (Test 2)     
		if((GPIOH->IDR & 0x0200) == 0)	// SW1�� ������ ��(SW1 = LOW) (GPIOH->IDR.9 = 'L' ?) 
		{
			GPIOG->ODR |=0x0002;	// LED1 On (GPIOG->ODR.1 'H')
		}
		else
		{
			GPIOG->ODR &= ~0x0002;	// LED1 Off (GPIOG->ODR.1 'L')
		}
	
		//GPIO SW2�Է����� LED2 1���� Off (Test 3)
		if((GPIOH->IDR & 0x0400) == 0)	// SW2�� ������ ��(SW2 = LOW) (GPIOH->IDR.10 = 'L' ?) 
		{
			GPIOG->ODR |=  0x0004;	// LED2 On (GPIOG->ODR.2 'H')
			DelayMS(1000);		// Delay (1000ms  = 1s)
			GPIOG->ODR &= ~0x0004;	// LED2 Off (GPIOG->ODR.2 'L') 
                        DelayMS(1000);	
                        
                }
      
                if((GPIOH->IDR & 0x8000) == 0)	// SW7�� ������ ��(SW2 = LOW) (GPIOH->IDR.10 = 'L' ?) 
		{
			GPIOG->ODR |=  0x00F0;	// LED8 7 6 5 On (GPIOG->ODR.2 'H')
			DelayMS(1000);		// Delay (1000ms  = 1s)
			GPIOG->ODR &= ~0x00F0;	// LED8 7 6 5Off (GPIOG->ODR.2 'L') 
                        DelayMS(1000);	
                        
                }
                
                
                
	}
}

/* GPIO (GPIOG(LED), GPIOH(Switch)) �ʱ� ����	*/
void _GPIO_Init(void)
{


//pb7

        RCC->AHB1ENR	|=  (1<<1);	// RCC_AHB1ENR : GPIOG(bit#6) Enable							
	GPIOB->MODER 	|=  0x00005555;	// GPIOG 0~7 : Output mode (0b01)						
	GPIOB->OTYPER	&= ~0x00FF;	// GPIOG 0~7 : Push-pull  (GP8~15:reset state)	
	GPIOB->OSPEEDR 	|=  0x00005555;	// GPIOG 0~7 : Output speed 25MHZ Medium speed 
    
/*
	// LED (GPIO G) ���� : Output mode
	RCC->AHB1ENR	|=  0x00000040;	// RCC_AHB1ENR : GPIOG(bit#6) Enable							
	GPIOG->MODER 	|=  0x00005555;	// GPIOG 0~7 : Output mode (0b01)						
	GPIOG->OTYPER	&= ~0x00FF;	// GPIOG 0~7 : Push-pull  (GP8~15:reset state)	
	GPIOG->OSPEEDR 	|=  0x00005555;	// GPIOG 0~7 : Output speed 25MHZ Medium speed 
    
	// SW(GPIO H) ���� : Input mode
	RCC->AHB1ENR    |=  0x00000080;	// RCC_AHB1ENR : GPIOH(bit#7)�� Clock Enable							
	GPIOH->MODER    &= ~0xFFFF0000;	// GPIOH 8~15 : Input mode (reset state),  8��Ʈ�� 0����				
	GPIOH->PUPDR     &= ~0xFFFF0000;	// GPIOH 8~15 : Floating input (No Pull-up, pull-down) :reset state

      //  GPIOH->PUPDR     &= ~0x0000000C;

*/
}	

void DelayMS(unsigned short wMS)
{
	register unsigned short i;
	for (i=0; i<wMS; i++)
		DelayUS(1000);	// 1000us => 1ms
}

void DelayUS(unsigned short wUS)
{
	volatile int Dly = (int)wUS*17;
	for(; Dly; Dly--);
}

/**************************************************************************
// ���� �����ڷ�
// ������ stm32f4xx.h�� �ִ� RCC���� �ֿ� ������ 
#define PERIPH_BASE           ((uint32_t)0x40000000) // Peripheral base address in the alias region                                 

// Peripheral memory map  
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000)

// AHB1 peripherals  
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE            (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASE            (AHB1PERIPH_BASE + 0x2000)
#define GPIOJ_BASE            (AHB1PERIPH_BASE + 0x2400)
#define GPIOK_BASE            (AHB1PERIPH_BASE + 0x2800)
#define CRC_BASE              (AHB1PERIPH_BASE + 0x3000)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00)

typedef struct
{
  __IO uint32_t MODER;    // GPIO port mode register,               Address offset: 0x00       
  __IO uint32_t OTYPER;   // GPIO port output type register,        Address offset: 0x04       
  __IO uint32_t OSPEEDR;  // GPIO port output speed register,       Address offset: 0x08       
  __IO uint32_t PUPDR;    // GPIO port pull-up/pull-down register,  Address offset: 0x0C       
  __IO uint32_t IDR;      // GPIO port input data register,         Address offset: 0x10       
  __IO uint32_t ODR;      // GPIO port output data register,        Address offset: 0x14       
  __IO uint16_t BSRRL;    // GPIO port bit set/reset low register,  Address offset: 0x18       
  __IO uint16_t BSRRH;    // GPIO port bit set/reset high register, Address offset: 0x1A       
  __IO uint32_t LCKR;     // GPIO port configuration lock register, Address offset: 0x1C       
  __IO uint32_t AFR[2];   // GPIO alternate function registers,     Address offset: 0x20-0x24  
} GPIO_TypeDef;

#define GPIOA 	((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB	((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE  	((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF   ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG   ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH   ((GPIO_TypeDef *) GPIOH_BASE)
#define GPIOI   ((GPIO_TypeDef *) GPIOI_BASE)
#define GPIOJ   ((GPIO_TypeDef *) GPIOJ_BASE)
#define GPIOK   ((GPIO_TypeDef *) GPIOK_BASE)

*/ 