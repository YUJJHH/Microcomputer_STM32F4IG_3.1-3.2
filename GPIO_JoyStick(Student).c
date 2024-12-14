//************************************//
// GPIO(INPUT mode: Joystick) �ǽ�
//

#include "stm32f4xx.h"
#include "GLCD.h"

#define SW0_PUSH        0xFE00  //PH8
#define SW1_PUSH        0xFD00  //PH9
#define SW2_PUSH        0xFB00  //PH10
#define SW3_PUSH        0xF700  //PH11

// NO Joy-Button : 0x03E0 : 0000 0011 1110 0000 
			//Bit No      FEDC BA98 7654 3210
#define NAVI_PUSH	0x03C0	        //PI5 0000 0011 1100 0000 
#define NAVI_UP		0x03A0	//PI6 0000 0011 1010 0000 
#define NAVI_DOWN	0x0360	        //PI7 0000 0011 0110 0000 
#define NAVI_RIGHT	0x02E0	        //PI8 0000 0010 1110 0000 
#define NAVI_LEFT	0x01E0	        //PI9 0000 0001 1110 0000  9�� ��Ʈ�� 0(on)

void _GPIO_Init(void);
uint16_t KEY_Scan(void);
uint16_t JOY_Scan(void);

void LCD_UDLR(void); 

void BEEP(void);
void DisplayInitScreen(void);
void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint8_t	SW0_Flag, SW1_Flag;

int main(void)
{
	_GPIO_Init();		// GPIO (LED & SW) �ʱ�ȭ
	LCD_Init();		// LCD ��� �ʱ�ȭ
	DelayMS(100);
	BEEP();

	GPIOG->ODR = 0xFF00;	// LED �ʱⰪ: LED0~7 Off
	DisplayInitScreen();	// LCD �ʱ�ȭ��
 
        
        LCD_UDLR();
        
	while(1)
	{
		/*switch(JOY_Scan())  //0x03E0
		{
			case  0x01E0 : // NAVI_LEFT (PI9)
                                   LCD_SetTextColor(RGB_BLACK);
                                    LCD_DisplayChar(4,5,'U');
                                     LCD_DisplayText(5,4,"LPR");
                                      LCD_DisplayChar(6,5,'D');
                          
                                  LCD_SetTextColor(RGB_RED);
				 LCD_DisplayChar(5,4,'L');
			break;
                case  NAVI_RIGHT:  //������ PI8
                          if((GPIOI->IDR & 0x0100)==0)
                         {   LCD_UDLR();
                          
                            LCD_SetTextColor(RGB_RED);
				 LCD_DisplayChar(5,6,'R');
                         }
                         else
                         {  
                           LCD_SetTextColor(RGB_BLACK);
                         LCD_DisplayChar(5,6,'R');
                         }
                                 break;
                case NAVI_PUSH :
                          LCD_UDLR();
                          
                            LCD_SetTextColor(RGB_RED);
				 LCD_DisplayChar(5,5,'P');
                                 break;
                                 
                                 
                case  NAVI_UP :
                   LCD_UDLR();
                          
                            LCD_SetTextColor(RGB_RED);
				 LCD_DisplayText(4,5,"U");
                                 break;
                
                case NAVI_DOWN:
                   LCD_UDLR();
                          
                            LCD_SetTextColor(RGB_RED);
				 LCD_DisplayChar(6,5,'D');
                                 break;
		}  // switch(JOY_Scan())
                
                */
              /*  
               if((GPIOI->IDR & 0x0100)==0)  //������ PI8
                         { //  LCD_UDLR();
                          
                            LCD_SetTextColor(RGB_RED);
				 LCD_DisplayChar(5,6,'R');
                         }
                         else
                         {  
                           LCD_SetTextColor(RGB_BLACK);
                         LCD_DisplayChar(5,6,'R');
                         }
                 
                    if((GPIOI->IDR & 0x0200)==0)  //��PI9
                         {   //LCD_UDLR();
                          
                            LCD_SetTextColor(RGB_RED);
                            LCD_DisplayChar(5,4,'L');
                         }
                         else
                         {  
                           LCD_SetTextColor(RGB_BLACK);
                         LCD_DisplayChar(5,4,'L');
                         }
                 
                 if((GPIOI->IDR & 0x0040)==0)  //��
                         {   //LCD_UDLR();
                          
                            LCD_SetTextColor(RGB_RED);
                                  LCD_DisplayChar(4,5,'U');
                         }
                         else
                         {  
                           LCD_SetTextColor(RGB_BLACK);
                              LCD_DisplayChar(4,5,'U');
                         }
               
                if((GPIOI->IDR & 0x0080)==0)  //�Ʒ�
                         {   //LCD_UDLR();
                          
                            LCD_SetTextColor(RGB_RED);
                                LCD_DisplayChar(6,5,'D');
                         }
                         else
                         {  
                           LCD_SetTextColor(RGB_BLACK);
                              LCD_DisplayChar(6,5,'D');
                         }
               
                
              */
		 
                
                
                switch(JOY_Scan())
		{
			case 0x01E0  :	// NAVI_LEFT  (PI9)
				if (SW0_Flag==0) {
					GPIOG->ODR |= 0x0001;	// LED0 ON		
					LCD_DisplayText(1,0,"0.LED0 ON ");
					SW0_Flag = 1;
				}
				else {
					GPIOG->ODR &= ~0x0001;	// LED0 OFF
					LCD_DisplayText(1,0,"0.LED0 OFF");
					SW0_Flag = 0;
				}
			break;
			case 0x02E0:	// NAVI_RIGHT
				if (SW1_Flag==0) {
					GPIOG->ODR |= 0x0002;	// LED1 ON
					LCD_DisplayText(2,0,"1.LED1 ON ");
					SW1_Flag = 1;
				}
				else {
					GPIOG->ODR &= ~0x0002; // LED1 OFF
					LCD_DisplayText(2,0,"1.LED1 OFF");
					SW1_Flag = 0;
				}
			break;
		}  // switch(JOY_Scan())
                
                
	}  // while(1)
}

void LCD_UDLR(void)  //���̽�ƽ �⺻��
{
                              LCD_SetTextColor(RGB_BLACK);
                                    LCD_DisplayChar(4,5,'U');
                                     LCD_DisplayText(5,4,"LPR");
                                      LCD_DisplayChar(6,5,'D');
}



/* GPIO (GPIOG(LED), GPIOH(Switch), GPIOF(Buzzer), GPIOI(Joystick)) �ʱ� ����	*/
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
	RCC->AHB1ENR	|= 0x00000100;	// RCC_AHB1ENR GPIOI 8�� ��Ʈ Enable
	GPIOI->MODER	&=~0x000FFC00;	// GPIOI 5~9 : Input mode (reset state)
	  //0b 0000 0000 0000 1111 1111 1100 0000 0000 
        GPIOI->PUPDR	&= ~0x000FFC00;	// GPIOI 5~9 : Floating input (No Pull-up, pull-down) (reset state)
}	

void DisplayInitScreen(void)
{
	LCD_Clear(RGB_WHITE);		// ȭ�� Ŭ����
	LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
	LCD_SetBackColor(RGB_GREEN);	// ���ڹ��� : Green
	LCD_SetTextColor(RGB_BLACK);	// ���ڻ� : Black
	LCD_DisplayText(0,0,"MENU");	// Title

	LCD_SetBackColor(RGB_YELLOW);	//���ڹ��� : Yellow
          
	LCD_DisplayText(1,0,"0.LED0 Off");
	LCD_DisplayText(2,0,"1.LED1 Off");
        
        LCD_SetBackColor(RGB_WHITE);  //���ڹ�� ��� ,���ڻ��� ����
        LCD_DisplayText(4,5,"U");
        LCD_DisplayText(5,4,"L");
          LCD_DisplayText(5,5,"P");   //LCD_Displaychar?
         LCD_DisplayText(6,5,"D");
        LCD_DisplayText(5,6,"R");
        
}

uint8_t joy_flag = 0;
uint16_t JOY_Scan(void)	// input joy stick NAVI_* 
{ 
	uint16_t key;
	key = GPIOI->IDR & 0x03E0; //����ũ: �� ��Ʈ�� ���ڴ�	// any key pressed ?
	// 0b 0000 0011 1110 0000
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
	{	if(joy_flag != 0)	// if continuous key, treat as no key input ��� ������ 1���� �ν�
			return 0x03E0;
		else			// if new key,delay for debounce
		{	joy_flag = 1;
			DelayMS(10);
			return key;
		}
	}
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
