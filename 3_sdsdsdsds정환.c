///////////////////////////////////////////////////////////////////////////////////
//      ������ : Ŀ�����Ǳ�
//      ��������:  GPIO�� �̿��� Ŀ�����Ǳ��� �Ӻ���� ���� 
//                     ������ �ְ� Ŀ�Ǽ��� ��ư�� ������ Ŀ�ǰ� ������ �� ó�� LED�� Ȱ���� ����
//      ����� �ϵ����(���) : SW, LED,���� , GPIO, 
//      ������ :    2023.5.05     
//      ������ Ŭ���� :    ����Ϲ�
//                    �й�:      2019130027 
//                    �̸�:       ����ȯ
//////////////////////////////////////////////////////////////////////////////
#include "stm32f4xx.h"

void _GPIO_Init(void);
uint16_t KEY_Scan(void);
void BEEP(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

int main(void)
{
	DelayMS(100);
	_GPIO_Init();
	BEEP();

	GPIOG->ODR = ~0x00FF;	// �ʱⰪ: LED0~7 Off
      
       
	while(1)
	{
              if( !((GPIOG->ODR & 0x0001) == 0x0001))     // LED0 �� OFF �� ��  ����ġ0 �� �Է��� ���� 
              {         switch(KEY_Scan())
                        {
                            case 0xFE00   : 	//SW0 ���� ����
                                    GPIOG->ODR |= 0x0001;	//  LED0 (PG0)  ON
                                    BEEP();                        //���� 1ȸ
                                    break;                          
                          }
              }     
              else  //LED0 �� ON�� �� ����
              {                                                 
                              switch(KEY_Scan())
                              { 
                                  case 0xF700 :        //SW3, �ͽ�Ŀ��
                                            GPIOG->ODR |= 0x0008;         //�ͽ�Ŀ�� LED 3 on
                                                BEEP();                            //���� 1ȸ
                                      //�� ���� 
                                               DelayMS(1000);                    //1����
                                            GPIOG->ODR |= 0x0080;         //�� LED7 ON  
                                                DelayMS(500);                     //0.5����
                                            GPIOG->ODR &= ~0x0080;      //�� LED7 OFF        
                                                DelayMS(500);                    //0.5����
                                     //���� ����
                                            GPIOG->ODR |= 0x0040;         //���� LED6 ON    
                                                DelayMS(500);                    //0.5����
                                            GPIOG->ODR &= ~0x0040;      //���� LED6 OFF 
                                                DelayMS(500);                    //0.5����
                                            GPIOG->ODR |= 0x0040;         //���� LED6 ON    
                                                DelayMS(500);                    //0.5����
                                            GPIOG->ODR &= ~0x0040;      //���� LED6 OFF 
                                                DelayMS(500);                    //0.5���� 
                                      //ũ�� ����
                                            GPIOG->ODR |= 0x0020;         //ũ�� LED5  ON    
                                                DelayMS(500);                    //0.5����
                                            GPIOG->ODR &= ~0x0020;      //ũ�� LED5  OFF
                                                DelayMS(500);                    //0.5����
                                            GPIOG->ODR |= 0x0020;        //ũ�� LED5  ON    
                                                DelayMS(500);                   //0.5����
                                            GPIOG->ODR &= ~0x0020;     //ũ�� LED5  OFF
                                                DelayMS(500);                    //0.5����
                                        //�� ����
                                            GPIOG->ODR |= 0x0010;        //�� LED4 ON    
                                                DelayMS(500);                    //0.5����
                                            GPIOG->ODR &= ~0x0010;     //�� LED4 OFF 
                                                DelayMS(500);                    //0.5���� 
                                            GPIOG->ODR |= 0x0010;        //�� LED4 ON    
                                                DelayMS(500);                    //0.5����
                                            GPIOG->ODR &= ~0x0010;     //�� LED4 OFF 
                                                DelayMS(500);                    //0.5����   
                                            GPIOG->ODR |= 0x0010;        //�� LED4 ON    
                                                DelayMS(500);                    //0.5����
                                            GPIOG->ODR &= ~0x0010;     //�� LED4 OFF 
                                                DelayMS(500);                    //0.5����   
                                    //���� 3�� 0.5�� ����
                                           GPIOF->ODR |=  0x0200;	        // PF9 'H' Buzzer on
                                                DelayMS(500);			//0.5����
                                           GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off 
                                               DelayMS(500);                     //0.5����   
                                           GPIOF->ODR |=  0x0200;	        // PF9 'H' Buzzer on
                                                DelayMS(500);			//0.5����
                                           GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off   
                                                DelayMS(500);                    //0.5����   
                                           GPIOF->ODR |=  0x0200;	        // PF9 'H' Buzzer on
                                                DelayMS(500);			//0.5����
                                           GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off   
                      
                                                    DelayMS(1000);	              //1�� �� ��緥�� �ʱ�ȭ
                                           GPIOG->ODR &= ~0x000F;
                                             break;
                                        
                                 case 0xFB00 :                       //SW2, ����Ŀ��
                                            GPIOG->ODR |= 0x0004;       //LED2 ON
                                            BEEP();                                //���� 1ȸ

                                        //�� ���� 
                                               DelayMS(1000);                 //1����
                                            GPIOG->ODR |= 0x0080;      //�� LED7 ON  
                                                DelayMS(500);                  //0.5����
                                            GPIOG->ODR &= ~0x0080;   //�� LED7 OFF        
                                                DelayMS(500);                  //0.5����
                                     //���� ����
                                            GPIOG->ODR |= 0x0040;      //���� LED6 ON    
                                                DelayMS(500);                  //0.5����
                                            GPIOG->ODR &= ~0x0040;   //���� LED6 OFF 
                                                DelayMS(500);                  //0.5����
                                            GPIOG->ODR |= 0x0040;      //���� LED6 ON    
                                                DelayMS(500);                  //0.5����
                                            GPIOG->ODR &= ~0x0040;   //���� LED6 OFF 
                                                DelayMS(500);                  //0.5���� 
                                     
                                        //�� ����
                                            GPIOG->ODR |= 0x0010;      //�� LED4 ON    
                                                DelayMS(500);                  //0.5����
                                            GPIOG->ODR &= ~0x0010;  //�� LED4 OFF 
                                                DelayMS(500);                 //0.5���� 
                                            GPIOG->ODR |= 0x0010;     //�� LED4 ON    
                                                DelayMS(500);                 //0.5����
                                            GPIOG->ODR &= ~0x0010;  //�� LED4 OFF 
                                                DelayMS(500);                 //0.5����   
                                            GPIOG->ODR |= 0x0010;     //�� LED4 ON    
                                                DelayMS(500);                 //0.5����
                                            GPIOG->ODR &= ~0x0010;  //�� LED4 OFF 
                                                DelayMS(500);                 //0.5����   
                                    //���� 3�� 0.5�� ����
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);			 //0.5����  
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off 
                                               DelayMS(500);                      //0.5����   
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);		         //0.5����  	
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off   
                                                DelayMS(500);                     //0.5����   
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);		         //0.5����  	
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off   
                      
                                                    DelayMS(1000);	              //1�� �� ��緥�� �ʱ�ȭ
                                           GPIOG->ODR &= ~0x000F;
                                            break;
                                                      
                                case 0xFD00 :          //SW1, ��Ŀ��
                                            GPIOG->ODR |= 0x0002;    //LED1 ON
                                            BEEP();                              //���� 1ȸ
                                     //�� ���� 
                                               DelayMS(1000);                 //1����
                                            GPIOG->ODR |= 0x0080;     //�� LED7 ON  
                                                DelayMS(500);                 //0.5����
                                            GPIOG->ODR &= ~0x0080;  //�� LED7 OFF        
                                                DelayMS(500);                 //0.5����
                                    
                                        //�� ����
                                            GPIOG->ODR |= 0x0010;     //�� LED4 ON    
                                                DelayMS(500);                 //0.5����
                                            GPIOG->ODR &= ~0x0010;  //�� LED4 OFF 
                                                DelayMS(500);                 //0.5���� 
                                            GPIOG->ODR |= 0x0010;     //�� LED4 ON    
                                                DelayMS(500);                 //0.5����
                                            GPIOG->ODR &= ~0x0010;  //�� LED4 OFF 
                                                DelayMS(500);                 //0.5����   
                                            GPIOG->ODR |= 0x0010;     //�� LED4 ON    
                                                DelayMS(500);                 //0.5����
                                            GPIOG->ODR &= ~0x0010;  //�� LED4 OFF 
                                                DelayMS(500);                 //0.5����   
                                   
                                          //���� 3�� 0.5�� ����
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);			 //0.5����
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off 
                                                DelayMS(500);                     //0.5����   
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);			 //0.5����
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off   
                                                DelayMS(500);                     //0.5����   
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);			 //0.5����
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off   */
                      
                                                    DelayMS(1000);	              //1�� �� ��緥�� �ʱ�ȭ
                                           GPIOG->ODR &= ~0x000F;
                                            break;
                               }
                       
                }
            
	}//while(1)
}

/* Switch�� �ԷµǾ������� ���ο� � switch�� �ԷµǾ������� ������ return�ϴ� �Լ�  */ 
uint8_t key_flag = 0;
uint16_t KEY_Scan(void)	// input key SW0 - SW7 
{ 
	uint16_t key;
	key = GPIOH->IDR & 0xFF00;	// any key pressed ?
	if(key == 0xFF00)		// if no key, check key off
	{	if(key_flag == 0)
			return key;
		else
		{	DelayMS(10);
			key_flag = 0;
			return key;
		}
	}
	else				// if key input, check continuous key
	{	if(key_flag != 0)	// if continuous key, treat as no key input
			return 0xFF00;
		else			// if new key,delay for debounce
		{	key_flag = 1;
			DelayMS(10);
 			return key;
		}
	}
}

/* GPIO (GPIOG(LED), GPIOH(Switch), GPIOF(Buzzer)) �ʱ� ����	*/
void _GPIO_Init(void)
{
	// LED (GPIO G) ���� : Output mode
	RCC->AHB1ENR	|=  0x00000040;	// RCC_AHB1ENR : GPIOG(bit#6) Enable							
	GPIOG->MODER 	|=  0x00005555;	// GPIOG 0~7 : Output mode (0b01)						
	GPIOG->OTYPER	&= ~0x00FF;	// GPIOG 0~7 : Push-pull  (GP8~15:reset state)	
	GPIOG->OSPEEDR 	|=  0x00005555;	// GPIOG 0~7 : Output speed 25MHZ Medium speed 
   
	// SW (GPIO H) ���� : Input mode 
	RCC->AHB1ENR    |=  0x00000080;	// RCC_AHB1ENR : GPIOH(bit#7) Enable							
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH 8~15 : Input mode (reset state)				
	GPIOH->PUPDR 	&= ~0xFFFF0000;	// GPIOH 8~15 : Floating input (No Pull-up, pull-down) :reset state

	// Buzzer (GPIO F) ���� : Output mode 
	RCC->AHB1ENR	|=  0x00000020; // RCC_AHB1ENR : GPIOF(bit#5) Enable							
	
        GPIOF->MODER 	&= ~0x000C0000;
        GPIOF->MODER 	|= 0x00040000;	// GPIOF 9 : Output mode (0b01)						
	GPIOF->OTYPER 	&= ~0x0200;	// GPIOF 9 : Push-pull  	
 	GPIOF->OSPEEDR 	|=  0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 
}	

void BEEP(void)			/* beep for 30 ms */   //������ �︲
{ 	
	GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
	DelayMS(30);			// Delay 30 ms
	GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
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