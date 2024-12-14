/////////////////////////////////////////////////////////////
// ������: TP2_���� �����
// ��������: �������� �����ڸ� �̿��� ����� ��� �� ���� ���� ��带 ���� ������ ����� ���� ����  
// ����� �ϵ����(���): GPIO(LED, SW, Buzzer, Joystick), GLCD, EXTI(interrupt), FRAM
// ������: 2023. X. X
// ������ Ŭ����: X���Ϲ�
// �й�: XXXXXXXXXX
// �̸�: XXX
///////////////////////////////////////////////////////////////
#include "stm32f4xx.h"
#include "GLCD.h"
#include "FRAM.h"

#define RGB_PINK GET_RGB(255,192,203)

void _GPIO_Init(void);
void _EXTI_Init(void);

void DisplayInitScreen(void);
uint16_t KEY_Scan(void);
uint16_t JOY_Scan(void);
void BEEP(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint8_t	SW2_Flag =0 , SW3_Flag = 0 ,SW4_Flag=0, SW5_Flag=0;
uint8_t 	opr_A =0, opr_B =0;
uint8_t	escape =0;
unsigned char opr;
int result;

int main(void)
{
	LCD_Init();	// LCD ��� �ʱ�ȭ
	DelayMS(10);
	
	
	_GPIO_Init();	// GPIO �ʱ�ȭ
	_EXTI_Init();	// EXTI �ʱ�ȭ
	Fram_Init();                    // FRAM �ʱ�ȭ H/W �ʱ�ȭ
	Fram_Status_Config();   // FRAM �ʱ�ȭ S/W �ʱ�ȭ

	DisplayInitScreen();	// LCD �ʱ�ȭ��
	int temp;
	result = Fram_Read (531);	//Fram�� ����� ����� �ҷ���
	temp = result;
	opr = Fram_Read(530);		// Fram�� ����� ������ �ҷ���
	LCD_SetFont(&Gulim10);	
	LCD_SetBackColor(RGB_PINK);
	if(opr =='+' || opr =='-'||opr =='x'||opr =='&'||opr =='|'||opr =='^')	//Fram�� �ҷ��� �� ���
		LCD_DrawChar(76,52, opr);
	else opr ='+';				//Fram�� �ҷ��ð� ������ �⺻�� ���� 
	LCD_SetFont(&Gulim7);		// ��Ʈ : ���� 7
	LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : Yellow
	if (result >=8)
	{
		LCD_DrawChar(118,41, '1');
		result -=8;
	}
	else 	LCD_DrawChar(118,41, '0');
	if (result >=4)
	{
		LCD_DrawChar(118,56, '1');
		result -=4;
	}
	else LCD_DrawChar(118,56, '0');
	if (result >=2)
	{
		LCD_DrawChar(118,71, '1');
		result -=2;
	}
	else LCD_DrawChar(118,71, '0');

	if (result ==1)
	{
		LCD_DrawChar(118,86, '1');
	}
	else LCD_DrawChar(118,86, '0');
	result = temp;
	GPIOG->ODR &= ~0x00FF;	// �ʱⰪ: LED0~7 Off
    
	while(1)
	{	
		LCD_SetFont(&Gulim7);		// ��Ʈ : ���� 7
		LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : Yellow
		LCD_SetTextColor(RGB_BLACK);
		escape = 0;
		switch(KEY_Scan())
		{
			case 0xFB00:		//SW2
				BEEP();
				if(SW2_Flag == 0)
				{
					SW2_Flag =1;
					opr_A +=2;
				}
				else 
				{
					SW2_Flag =0;	
					opr_A -=2;
				}
				LCD_DrawChar(35,25,SW2_Flag+0x30);
			break;
			case 0xF700: 		//SW3
				BEEP();
				if(SW3_Flag == 0)
				{
					SW3_Flag =1;
					opr_A +=1;
				}
				else 
				{
					SW3_Flag =0;	
					opr_A -=1;
				}	
				LCD_DrawChar(35,40,SW3_Flag+0x30);	
			break;
			case 0xEF00:		//SW4
				BEEP(); 
				if(SW4_Flag == 0)
				{
					SW4_Flag =1;
					opr_B +=2;
				}
				else 
				{
					SW4_Flag =0;	
					opr_B -=2;
				}
				LCD_DrawChar(35,70,SW4_Flag+0x30);
			break;
			case 0xDF00: 		//SW5
				BEEP();
				if(SW5_Flag == 0)
				{
					SW5_Flag =1;
					opr_B +=1;
				}
				else 
				{
					SW5_Flag =0;	
					opr_B -=1;
				}
				LCD_DrawChar(35,85,SW5_Flag+0x30);
			break;
		}
			
	}
}

/* GLCD �ʱ�ȭ�� ���� �Լ� */
void DisplayInitScreen(void)
{
	LCD_Clear(RGB_WHITE);		// ȭ�� Ŭ����
	LCD_SetFont(&Gulim7);		// ��Ʈ : ���� 7
	LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : Yellow
	LCD_SetTextColor(RGB_BLACK);	// ���ڻ� : Black
	LCD_SetPenColor(RGB_GREEN);
	LCD_DrawRectangle(60,20, 40,80);
	LCD_SetPenColor(RGB_BLUE);
	LCD_DrawHorLine(45,30,15);
	LCD_DrawHorLine(45,45,15);
	LCD_DrawHorLine(45,75,15);
	LCD_DrawHorLine(45,90,15);
	
	LCD_DrawHorLine(100,26,15);
	LCD_DrawHorLine(100,46,15);
	LCD_DrawHorLine(100,61,15);
	LCD_DrawHorLine(100,76,15);
	LCD_DrawHorLine(100,91,15);
	LCD_SetPenColor(RGB_BLACK);
	LCD_SetBrushColor(RGB_YELLOW);

	LCD_DrawRectangle(32,23,13,13);
	LCD_DrawFillRect(33,24,12,12);
	LCD_DrawRectangle(32,38,13,13);
	LCD_DrawFillRect(33,39,12,12);
	LCD_DrawRectangle(32,68,13,13);
	LCD_DrawFillRect(33,69,12,12);
	LCD_DrawRectangle(32,83,13,13);
	LCD_DrawFillRect(33,84,12,12);

	LCD_DrawRectangle(115,19,13,13);
	LCD_DrawFillRect(116,20,12,12);
	LCD_DrawRectangle(115,39,13,13);
	LCD_DrawFillRect(116,40,12,12);
	LCD_DrawRectangle(115,54,13,13);
	LCD_DrawFillRect(116,55,12,12);
	LCD_DrawRectangle(115,69,13,13);
	LCD_DrawFillRect(116,70,12,12);
	LCD_DrawRectangle(115,84,13,13);
	LCD_DrawFillRect(116,85,12,12);	


	LCD_DrawChar(35,25,'0');
	LCD_DrawChar(35,40,'0');
	LCD_DrawChar(35,70,'0');
	LCD_DrawChar(35,85,'0');

	LCD_DrawChar(118,21, '+');
	LCD_DrawChar(118,41, '0');
	LCD_DrawChar(118,56, '0');
	LCD_DrawChar(118,71, '0');
	LCD_DrawChar(118,86, '0');

	LCD_DrawRectangle(72,50, 16,20);
	LCD_DrawRectangle(70,82, 20,15);
	LCD_DrawFillRect(71,83, 19,14);

	LCD_DrawText(73, 85,"+0");
	LCD_SetBrushColor(RGB_PINK);	
	LCD_DrawFillRect(73,51,15,19);
	LCD_SetFont(&Gulim10);	
	LCD_SetBackColor(RGB_PINK);
	LCD_DrawChar(76,52, '+');
	
	LCD_SetBackColor(RGB_WHITE);
	LCD_SetPenColor(RGB_GREEN);	
	LCD_DrawRectangle(12, 28, 16,18);
	LCD_DrawChar(16,29,'A');
	
	LCD_DrawRectangle(12, 73, 16,18);
	LCD_DrawChar(16,74,'B');
	LCD_DrawRectangle(133, 44, 16,18);
	LCD_DrawChar(137,45,'C');
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

	//Joy Stick SW(PORT I) ����
	RCC->AHB1ENR	|= 0x000000100;	// RCC_AHB1ENR GPIOI Enable
	GPIOI->MODER	&= ~0x000FFC00;	// GPIOI 5~9 : Input mode (reset state)
	GPIOI->PUPDR	&= ~0x000FFC00;	// GPIOI 5~9 : Floating input (No Pull-up, pull-down) (reset state)
}	

/* EXTI (EXTI8(GPIOH.8, SW0), EXTI9(GPIOH.9, SW1)) �ʱ� ����  */
void _EXTI_Init(void)
{
	RCC->AHB1ENR 	|= 0x00000080;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= 0x00004000;	// Enable System Configuration Controller Clock
	RCC->AHB1ENR	|= 0x000000100;	// RCC_AHB1ENR GPIOI Enable
	
	GPIOI->MODER	&= ~0x000FFC00;	// GPIOI 5~9 : Input mode (reset state)
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH PIN8~PIN15 Input mode (reset state)				 
	
	SYSCFG->EXTICR[1] |= 0x0800;	// EXTII6�� ���� �ҽ� �Է��� GPIOI�� ����
	SYSCFG->EXTICR[2] |= 0x0078;	// EXTI8�� ���� �ҽ� �Է��� GPIOH�� ����, EXTII9�� ���� �ҽ� �Է��� GPIOI�� ����
	SYSCFG->EXTICR[3] |= 0x0700;	// EXTI14�� ���� �ҽ� �Է��� GPIOH�� ����,

	
	EXTI->FTSR |= 0x4340;		// EXTI6,8,9,14: Falling Trigger Enable  
	EXTI->IMR  |= 0x4340;		 //EXTI6,8,9,14 ���ͷ�Ʈ mask (Interrupt Enable) ����
		
	NVIC->ISER[0] |= ( 1<<23  );	// Enable 'Global Interrupt EXTI6,8,9'
	NVIC->ISER[1] |= ( 1<<8  );	// Enable 'Global Interrupt EXTI14'
					// Vector table Position ����

	NVIC->IP[23] =0xF0;	
	NVIC->IP[40] = 0xE0; 	//High Priority 
}

/* EXTI5~9 ���ͷ�Ʈ �ڵ鷯(ISR: Interrupt Service Routine) */
void EXTI9_5_IRQHandler(void)
{		
	if(EXTI->PR & 0x0040)	// EXTI6 Interrupt Pending(�߻�) ����?
	{
		EXTI->PR |= 0x0040;	// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		int temp;
		BEEP();
		GPIOG->ODR |= 0x80;	//LED7 ON
		/*���� ���� ��� On*/
		while(!escape) 	//Ż�� ���� 1�Ǳ� ������ �ݺ�
		{
			LCD_DrawText(73, 85,"+1");
			if(result <15)
				result++;
			else result =0;
			temp = result;
			if (result >=8)
			{
					LCD_DrawChar(118,41, '1');
					result -=8;
			}
			else 	LCD_DrawChar(118,41, '0');

			if (result >=4)
			{
				LCD_DrawChar(118,56, '1');
				result -=4;
			}
			else LCD_DrawChar(118,56, '0');

			if (result >=2)
			{
				LCD_DrawChar(118,71, '1');
				result -=2;
			}
			else LCD_DrawChar(118,71, '0');


			if (result ==1)
			{
				LCD_DrawChar(118,86, '1');
			}
			else LCD_DrawChar(118,86, '0');
			result =temp;
			Fram_Write(531, result);
			DelayMS(500);
		}
		LCD_DrawText(73, 85,"+0");
		BEEP();
		DelayMS(500);
		BEEP();
		DelayMS(500);
		BEEP();
		GPIOG->ODR &= ~0x80;	//LED7 OFF
	
	}
	if(EXTI->PR & 0x0100)	// EXTI8 Interrupt Pending(�߻�) ����?
	{
		EXTI->PR |=  0x0100;	// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		BEEP();
		/*������ ����*/
		if (opr == '^')
			opr  = '+';
		else if (opr == '+')
			opr = '-';
		else if (opr == '-')
			opr = 'x';
		else if (opr == 'x')
			opr = '&';
		else if (opr == '&')
			opr = '|';
		else if (opr == '|')
			opr = '^';
		LCD_SetFont(&Gulim10);	
		LCD_SetBackColor(RGB_PINK);
		LCD_DrawChar(76,52, opr);
		Fram_Write(530, opr);
	}
	
	if(EXTI->PR & 0x0200)	// EXTI9 Interrupt Pending(�߻�) ����?
	{
		EXTI->PR |= 0x0200;	// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		BEEP();
		LCD_DrawChar(118,21, '+');
		/*�����ڿ� ���� ����*/
		if(opr =='+')
			result = opr_A+ opr_B;
		if(opr =='-')
		{
			if(opr_A < opr_B)	//������ �����ϰ��
			{
				result = opr_B - opr_A;
				LCD_DrawChar(118,21, '-');	
			}
			else result = opr_A -opr_B;

		}
		if(opr =='x')			
			result = opr_A * opr_B;
		if(opr =='&')
			result = opr_A & opr_B;
		if(opr =='|')
			result = opr_A | opr_B;
		if(opr =='^')
			result = opr_A ^ opr_B;
		Fram_Write(531, result);	//����� Fram 531������ ����

		/*10������ 2������ LCD�� ���*/
		if (result >=8)
		{
				LCD_DrawChar(118,41, '1');
				result -=8;
		}
		else 	LCD_DrawChar(118,41, '0');

		if (result >=4)
		{
			LCD_DrawChar(118,56, '1');
			result -=4;
		}
		else LCD_DrawChar(118,56, '0');

		if (result >=2)
		{
			LCD_DrawChar(118,71, '1');
			result -=2;
		}
		else LCD_DrawChar(118,71, '0');


		if (result ==1)
		{
			LCD_DrawChar(118,86, '1');
		}
		else LCD_DrawChar(118,86, '0');
		result = Fram_Read(531);
	}



}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & 0x4000)	// EXTI14 Interrupt Pending(�߻�) ����?
	{
		EXTI->PR|= 0x4000;		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		BEEP();
		DelayMS(1000);
		escape = 1;	//Ż�� ���� ����

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

uint8_t joy_flag = 0;
uint16_t JOY_Scan(void)	// input joy stick NAVI_* 
{ 
	uint16_t key;
	key = GPIOI->IDR & 0x03E0;	// any key pressed ?
	if(key == 0x03E0)		// if no key, check key off
	{	if(joy_flag == 0)
			return key;
		else
		{	DelayMS(10);
			joy_flag = 0;
			return key;
		}
	}
	else				// if key input, check continuous key
	{	if(joy_flag != 0)	// if continuous key, treat as no key input
			return 0x03E0;
		else			// if new key,delay for debounce
		{	joy_flag = 1;
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
