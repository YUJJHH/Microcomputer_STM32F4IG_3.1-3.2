//************************************//
// EXTI �ǽ�(EXTI8, 9)
// SW0(PH8) �Է�: EXTI8 �߻�
// SW1(PH9) �Է�: EXTI9 �߻�

#include "stm32f4xx.h"
#include "GLCD.h"

void _GPIO_Init(void);
void _EXTI_Init(void);

void DisplayInitScreen(void);
uint16_t KEY_Scan(void);
void BEEP(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint8_t	SW0_Flag, SW1_Flag, SW3_Flag;

int main(void)
{
	LCD_Init();	// LCD ��� �ʱ�ȭ
	DelayMS(10);
	_GPIO_Init();	// GPIO �ʱ�ȭ
	_EXTI_Init();	// EXTI �ʱ�ȭ

	DisplayInitScreen();	// LCD �ʱ�ȭ��

	GPIOG->ODR &= ~0x00FF;	// �ʱⰪ: LED0~7 Off
	GPIOG->ODR |= 0x0001;	// GPIOG->ODR.0 'H'(LED ON)
    
	while(1)
	{
		
        if((GPIOH -> IDR & 0x8000)==0)
                {
              
                  	GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        
                         GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                         GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x80;	// LED7Toggle
                        DelayMS(500);
                     
                }
        
        
              //EXTI Example(1) : SW0�� High���� Low�� �� ��(Falling edge Trigger mode) EXTI8 ���ͷ�Ʈ �߻�, LED0 toggle
		if(SW0_Flag)	// ���ͷ�Ʈ (EXTI8) �߻�
		{
			   GPIOG->ODR ^= 0x01;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x01;	// LED7Toggle
                        DelayMS(500);
                         GPIOG->ODR ^= 0x01;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x01;	// LED7Toggle
                        DelayMS(500);
                
                
             //   GPIOG->ODR ^= 0x01;	// LED0 Toggle
			SW0_Flag = 0;
                        
		}
		if(SW3_Flag)	// ���ͷ�Ʈ (EXTI8) �߻�
		{
			GPIOG->ODR ^= 0x08;	// LED3 Toggle
			SW3_Flag = 0;
		}

	}
}

/* GLCD �ʱ�ȭ�� ���� �Լ� */
void DisplayInitScreen(void)
{
	LCD_Clear(RGB_WHITE);		// ȭ�� Ŭ����
	LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
	LCD_SetBackColor(RGB_GREEN);	// ���ڹ��� : Green
	LCD_SetTextColor(RGB_BLACK);	// ���ڻ� : Black
	LCD_DisplayText(0,0,"EXTI Test");  	// Title

	LCD_SetBackColor(RGB_YELLOW);	//���ڹ��� : Yellow
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
	RCC->AHB1ENR	|=  0x00000020;	// RCC_AHB1ENR : GPIOF(bit#5) Enable							
	GPIOF->MODER 	|=  0x00040000;	// GPIOF 9 : Output mode (0b01)						
	GPIOF->OTYPER 	&= ~0x0200;	// GPIOF 9 : Push-pull  	
	GPIOF->OSPEEDR 	|=  0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 
        
        //����
        RCC->AHB1ENR	|=  0x00000100;
        GPIOI->MODER 	&= ~ 0x000FFC00;	
        GPIOI->PUPDR 	&= ~ 0x000FFC00;
        
        
       
}	

/* EXTI (EXTI8(GPIOH.8, SW0), EXTI9(GPIOH.9, SW1)) �ʱ� ����  */
void _EXTI_Init(void)
{
	RCC->AHB1ENR 	|= 0x00000080;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= 0x00004000;	// Enable System Configuration Controller Clock
			// 0b 0000 0000 0000 0000 0100 0000 0000 0000
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH PIN8~PIN15 Input mode (reset state)				 
	
	SYSCFG->EXTICR[2] |= 0x0077;	// EXTI8,9�� ���� �ҽ� �Է��� GPIOH�� ����
					// EXTI8 <- PH8, EXTI9 <- PH9 
					// EXTICR3(EXTICR[2])�� �̿� 
					// reset value: 0x0000	
	
	EXTI->FTSR |= 0x00000100;	// EXTI8: Falling Trigger Enable 
	// 0b 0000 0000 0000 0000 0000 0001 0000 0000
	EXTI->RTSR |= 0x00000200;	// EXTI9: Rising Trigger  Enable 
	EXTI->IMR   |= 0x00000300;	// EXTI8,9 ���ͷ�Ʈ mask (Interrupt Enable) ����
		
	NVIC->ISER[0] |= ( 1<<23  );	// Enable 'Global Interrupt EXTI8,9'
					// Vector table Position ����
		// 0b 0000 0000 1000 0000 0000 0000 0000 0000
			// 0x00800000

	SYSCFG->EXTICR[2] |= 0x7000;	// EXTI11�� ���� �ҽ� �Է��� GPIOH�� ����
	SYSCFG->EXTICR[3] |= 0x0007;	// EXTI12�� ���� �ҽ� �Է��� GPIOH�� ����
	EXTI->FTSR |= 0x00001800;	// EXTI11,12: Falling Trigger Enable 
	EXTI->IMR   |= 0x00001800;	// EXTI11,12 ���ͷ�Ʈ mask (Interrupt Enable) ����
	NVIC->ISER[1] |= 0x00000100;	// Enable 'Global Interrupt EXTI11,12'
				// (1<< (40-32));
	// 0b 0000 0000 0000 0000 0000 0001 0000 0000
        
        //���� �� PI6, �ٿ�  PI7         >> EXTI6, EXTI7
      
        RCC->AHB1ENR 	|= 0x00000100;	
	RCC->APB2ENR 	|= 0x00004000;	
			
	GPIOI->MODER 	&= ~ 0x000FFC00;		
        SYSCFG->EXTICR[1] |= 0x8800;	
        
        EXTI->FTSR |=  0x000000C0 ;// EXTI5,6: Falling Trigger Enable 
	// 0b 0000 0000 0000 0000 0000 0001 0000 0000
	
	EXTI->IMR   |= 0x000000C0 ;	// 
	NVIC->ISER[0] |= ( 1<<23  );
}
void EXTI15_10_IRQHandler(void)
{		
	if(EXTI->PR & 0x0800)			// EXTI11 Interrupt Pending(�߻�) ����?
	{
		EXTI->PR |= 0x0800;		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		SW3_Flag = 1;			// SW3_Flag: EXTI11�� �߻��Ǿ����� �˸��� ���� ���� ����(main���� mission�� ���) 
		LCD_DisplayText(2,0,"EXTI11");
	}

	//EXTI Example(2) : SW4�� Low���� High�� �� ��(Rising edge Trigger mode) EXTI9 ���ͷ�Ʈ �߻�, LED1 toggle
	else if(EXTI->PR & 0x1000)		// EXTI12 Interrupt Pending(�߻�) ����?
	{
		EXTI->PR |= 0x1000;		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		LCD_DisplayText(2,0,"EXTI12");
		GPIOG->ODR ^= 0x10;		// LED4 Toggle
	}
}
/* EXTI5~9 ���ͷ�Ʈ �ڵ鷯(ISR: Interrupt Service Routine) */
void EXTI9_5_IRQHandler(void)
{		
	if(EXTI->PR & 0x0100)			// EXTI8 Interrupt Pending(�߻�) ����?
		// 0x0100(EXTI8 �߻�) or 0x0000(EXTI8 �̹߻�)
	{
		EXTI->PR |= 0x0100;		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		SW0_Flag = 1;			// SW0_Flag: EXTI8�� �߻��Ǿ����� �˸��� ���� ���� ����(main���� mission�� ���) 
		LCD_DisplayText(1,0,"EXTI8");
                
              
	}
	//EXTI Example(2) : SW1�� Low���� High�� �� ��(Rising edge Trigger mode) EXTI9 ���ͷ�Ʈ �߻�, LED1 toggle
	else if(EXTI->PR & 0x0200)		// EXTI9 Interrupt Pending(�߻�) ����?
	{
		EXTI->PR |= 0x0200;		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		LCD_DisplayText(1,0,"EXTI9");
	//	GPIOG->ODR ^= 0x02;		// LED1 Toggle
                
                 GPIOG->ODR ^= 0x02;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x02;	// LED7Toggle
                        DelayMS(500);
                         GPIOG->ODR ^= 0x02;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x02;	// LED7Toggle
                        DelayMS(500);
	}
        //���̽�ƽ EXTI6,7
        else if(EXTI->PR & 0x0040) //EXTI6 ��
        {
            EXTI->PR |= 0x0040;	
            
              GPIOG->ODR ^= 0x40;	// LED6Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x40;	// LED7Toggle
                        DelayMS(500);
                         GPIOG->ODR ^= 0x40;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x40;	// LED7Toggle
                        DelayMS(500);
        }
        
        else if(EXTI->PR & 0x0080)  //EXTI7  �Ʒ�
        {
            EXTI->PR |= 0x0080;	
            
              GPIOG->ODR ^= 0x20;	// LED5Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x20;	// LED7Toggle
                        DelayMS(500);
                         GPIOG->ODR ^= 0x20;	// LED7Toggle
                        DelayMS(500);
                        GPIOG->ODR ^= 0x20;	// LED7Toggle
                        DelayMS(500);
        }
        
}

/* Switch�� �ԷµǾ����� ���ο� � switch�� �ԷµǾ������� ������ return�ϴ� �Լ�  */ 
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

/* Buzzer: Beep for 30 ms */
void BEEP(void)			
{ 	
	GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
	DelayMS(30);		// Delay 30 ms
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
