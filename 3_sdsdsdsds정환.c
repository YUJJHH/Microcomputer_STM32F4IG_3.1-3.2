///////////////////////////////////////////////////////////////////////////////////
//      과제명 : 커피자판기
//      과제개요:  GPIO를 이용한 커피자판기의 임베디드 제어 
//                     동전을 넣고 커피선택 버튼을 누르면 커피가 나오는 것 처럼 LED를 활용해 구현
//      사용한 하드웨어(기능) : SW, LED,부저 , GPIO, 
//      제출일 :    2023.5.05     
//      제출자 클래스 :    목요일반
//                    학번:      2019130027 
//                    이름:       유정환
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

	GPIOG->ODR = ~0x00FF;	// 초기값: LED0~7 Off
      
       
	while(1)
	{
              if( !((GPIOG->ODR & 0x0001) == 0x0001))     // LED0 이 OFF 일 때  스위치0 의 입력을 받음 
              {         switch(KEY_Scan())
                        {
                            case 0xFE00   : 	//SW0 코인 넣음
                                    GPIOG->ODR |= 0x0001;	//  LED0 (PG0)  ON
                                    BEEP();                        //부저 1회
                                    break;                          
                          }
              }     
              else  //LED0 이 ON일 때 동작
              {                                                 
                              switch(KEY_Scan())
                              { 
                                  case 0xF700 :        //SW3, 믹스커피
                                            GPIOG->ODR |= 0x0008;         //믹스커피 LED 3 on
                                                BEEP();                            //부저 1회
                                      //컵 나옴 
                                               DelayMS(1000);                    //1초후
                                            GPIOG->ODR |= 0x0080;         //컵 LED7 ON  
                                                DelayMS(500);                     //0.5초후
                                            GPIOG->ODR &= ~0x0080;      //컵 LED7 OFF        
                                                DelayMS(500);                    //0.5초후
                                     //슈거 나옴
                                            GPIOG->ODR |= 0x0040;         //슈거 LED6 ON    
                                                DelayMS(500);                    //0.5초후
                                            GPIOG->ODR &= ~0x0040;      //슈거 LED6 OFF 
                                                DelayMS(500);                    //0.5초후
                                            GPIOG->ODR |= 0x0040;         //슈거 LED6 ON    
                                                DelayMS(500);                    //0.5초후
                                            GPIOG->ODR &= ~0x0040;      //슈거 LED6 OFF 
                                                DelayMS(500);                    //0.5초후 
                                      //크림 나옴
                                            GPIOG->ODR |= 0x0020;         //크림 LED5  ON    
                                                DelayMS(500);                    //0.5초후
                                            GPIOG->ODR &= ~0x0020;      //크림 LED5  OFF
                                                DelayMS(500);                    //0.5초후
                                            GPIOG->ODR |= 0x0020;        //크림 LED5  ON    
                                                DelayMS(500);                   //0.5초후
                                            GPIOG->ODR &= ~0x0020;     //크림 LED5  OFF
                                                DelayMS(500);                    //0.5초후
                                        //물 나옴
                                            GPIOG->ODR |= 0x0010;        //물 LED4 ON    
                                                DelayMS(500);                    //0.5초후
                                            GPIOG->ODR &= ~0x0010;     //물 LED4 OFF 
                                                DelayMS(500);                    //0.5초후 
                                            GPIOG->ODR |= 0x0010;        //물 LED4 ON    
                                                DelayMS(500);                    //0.5초후
                                            GPIOG->ODR &= ~0x0010;     //물 LED4 OFF 
                                                DelayMS(500);                    //0.5초후   
                                            GPIOG->ODR |= 0x0010;        //물 LED4 ON    
                                                DelayMS(500);                    //0.5초후
                                            GPIOG->ODR &= ~0x0010;     //물 LED4 OFF 
                                                DelayMS(500);                    //0.5초후   
                                    //부저 3번 0.5초 간격
                                           GPIOF->ODR |=  0x0200;	        // PF9 'H' Buzzer on
                                                DelayMS(500);			//0.5초후
                                           GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off 
                                               DelayMS(500);                     //0.5초후   
                                           GPIOF->ODR |=  0x0200;	        // PF9 'H' Buzzer on
                                                DelayMS(500);			//0.5초후
                                           GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off   
                                                DelayMS(500);                    //0.5초후   
                                           GPIOF->ODR |=  0x0200;	        // PF9 'H' Buzzer on
                                                DelayMS(500);			//0.5초후
                                           GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off   
                      
                                                    DelayMS(1000);	              //1초 후 모든램프 초기화
                                           GPIOG->ODR &= ~0x000F;
                                             break;
                                        
                                 case 0xFB00 :                       //SW2, 수거커피
                                            GPIOG->ODR |= 0x0004;       //LED2 ON
                                            BEEP();                                //부저 1회

                                        //컵 나옴 
                                               DelayMS(1000);                 //1초후
                                            GPIOG->ODR |= 0x0080;      //컵 LED7 ON  
                                                DelayMS(500);                  //0.5초후
                                            GPIOG->ODR &= ~0x0080;   //컵 LED7 OFF        
                                                DelayMS(500);                  //0.5초후
                                     //슈거 나옴
                                            GPIOG->ODR |= 0x0040;      //슈거 LED6 ON    
                                                DelayMS(500);                  //0.5초후
                                            GPIOG->ODR &= ~0x0040;   //슈거 LED6 OFF 
                                                DelayMS(500);                  //0.5초후
                                            GPIOG->ODR |= 0x0040;      //슈거 LED6 ON    
                                                DelayMS(500);                  //0.5초후
                                            GPIOG->ODR &= ~0x0040;   //슈거 LED6 OFF 
                                                DelayMS(500);                  //0.5초후 
                                     
                                        //물 나옴
                                            GPIOG->ODR |= 0x0010;      //물 LED4 ON    
                                                DelayMS(500);                  //0.5초후
                                            GPIOG->ODR &= ~0x0010;  //물 LED4 OFF 
                                                DelayMS(500);                 //0.5초후 
                                            GPIOG->ODR |= 0x0010;     //물 LED4 ON    
                                                DelayMS(500);                 //0.5초후
                                            GPIOG->ODR &= ~0x0010;  //물 LED4 OFF 
                                                DelayMS(500);                 //0.5초후   
                                            GPIOG->ODR |= 0x0010;     //물 LED4 ON    
                                                DelayMS(500);                 //0.5초후
                                            GPIOG->ODR &= ~0x0010;  //물 LED4 OFF 
                                                DelayMS(500);                 //0.5초후   
                                    //부저 3번 0.5초 간격
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);			 //0.5초후  
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off 
                                               DelayMS(500);                      //0.5초후   
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);		         //0.5초후  	
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off   
                                                DelayMS(500);                     //0.5초후   
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);		         //0.5초후  	
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off   
                      
                                                    DelayMS(1000);	              //1초 후 모든램프 초기화
                                           GPIOG->ODR &= ~0x000F;
                                            break;
                                                      
                                case 0xFD00 :          //SW1, 블랙커피
                                            GPIOG->ODR |= 0x0002;    //LED1 ON
                                            BEEP();                              //부저 1회
                                     //컵 나옴 
                                               DelayMS(1000);                 //1초후
                                            GPIOG->ODR |= 0x0080;     //컵 LED7 ON  
                                                DelayMS(500);                 //0.5초후
                                            GPIOG->ODR &= ~0x0080;  //컵 LED7 OFF        
                                                DelayMS(500);                 //0.5초후
                                    
                                        //물 나옴
                                            GPIOG->ODR |= 0x0010;     //물 LED4 ON    
                                                DelayMS(500);                 //0.5초후
                                            GPIOG->ODR &= ~0x0010;  //물 LED4 OFF 
                                                DelayMS(500);                 //0.5초후 
                                            GPIOG->ODR |= 0x0010;     //물 LED4 ON    
                                                DelayMS(500);                 //0.5초후
                                            GPIOG->ODR &= ~0x0010;  //물 LED4 OFF 
                                                DelayMS(500);                 //0.5초후   
                                            GPIOG->ODR |= 0x0010;     //물 LED4 ON    
                                                DelayMS(500);                 //0.5초후
                                            GPIOG->ODR &= ~0x0010;  //물 LED4 OFF 
                                                DelayMS(500);                 //0.5초후   
                                   
                                          //부저 3번 0.5초 간격
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);			 //0.5초후
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off 
                                                DelayMS(500);                     //0.5초후   
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);			 //0.5초후
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off   
                                                DelayMS(500);                     //0.5초후   
                                           GPIOF->ODR |=  0x0200;	         // PF9 'H' Buzzer on
                                                DelayMS(500);			 //0.5초후
                                           GPIOF->ODR &= ~0x0200;	 // PF9 'L' Buzzer off   */
                      
                                                    DelayMS(1000);	              //1초 후 모든램프 초기화
                                           GPIOG->ODR &= ~0x000F;
                                            break;
                               }
                       
                }
            
	}//while(1)
}

/* Switch가 입력되었는지를 여부와 어떤 switch가 입력되었는지의 정보를 return하는 함수  */ 
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

/* GPIO (GPIOG(LED), GPIOH(Switch), GPIOF(Buzzer)) 초기 설정	*/
void _GPIO_Init(void)
{
	// LED (GPIO G) 설정 : Output mode
	RCC->AHB1ENR	|=  0x00000040;	// RCC_AHB1ENR : GPIOG(bit#6) Enable							
	GPIOG->MODER 	|=  0x00005555;	// GPIOG 0~7 : Output mode (0b01)						
	GPIOG->OTYPER	&= ~0x00FF;	// GPIOG 0~7 : Push-pull  (GP8~15:reset state)	
	GPIOG->OSPEEDR 	|=  0x00005555;	// GPIOG 0~7 : Output speed 25MHZ Medium speed 
   
	// SW (GPIO H) 설정 : Input mode 
	RCC->AHB1ENR    |=  0x00000080;	// RCC_AHB1ENR : GPIOH(bit#7) Enable							
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH 8~15 : Input mode (reset state)				
	GPIOH->PUPDR 	&= ~0xFFFF0000;	// GPIOH 8~15 : Floating input (No Pull-up, pull-down) :reset state

	// Buzzer (GPIO F) 설정 : Output mode 
	RCC->AHB1ENR	|=  0x00000020; // RCC_AHB1ENR : GPIOF(bit#5) Enable							
	
        GPIOF->MODER 	&= ~0x000C0000;
        GPIOF->MODER 	|= 0x00040000;	// GPIOF 9 : Output mode (0b01)						
	GPIOF->OTYPER 	&= ~0x0200;	// GPIOF 9 : Push-pull  	
 	GPIOF->OSPEEDR 	|=  0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 
}	

void BEEP(void)			/* beep for 30 ms */   //부저가 울림
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
// 보충 설명자료
// 다음은 stm32f4xx.h에 있는 RCC관련 주요 선언문임 
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