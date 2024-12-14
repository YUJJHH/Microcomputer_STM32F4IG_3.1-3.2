//**************************************************************************//
// GLCD �ǽ�(����, �׸�)
// 	LCD_Init()(calling in GLCD_Display.c, called in GLCD.c)
//		ILI_Init() in calling in GLCD.c, called in ILI3168B.c
//			FSMC_GPIO_Config() in ILI3168B.c   
//			FSMC_LCD_Init() in ILI3168B.c
//	DisplayInitScreen();

#include "stm32f4xx.h"
#include "GLCD.h"

#define NAVI_PUSH	0x03C0	//PI5 0000 0011 1100 0000 
#define NAVI_UP		0x03A0	//PI6 0000 0011 1010 0000 
#define NAVI_DOWN	0x0360	//PI7 0000 0011 0110 0000 
#define NAVI_RIGHT	0x02E0	//PI8 0000 0010 1110 0000 
#define NAVI_LEFT	0x01E0	//PI9 0000 0001 1110 0000  9�� ��Ʈ�� 0(on)




void _GPIO_Init(void);
uint16_t KEY_Scan(void);
uint16_t JOY_Scan(void);

void BEEP(void);
void DisplayInitScreen(void);
void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint8_t	SW0_Flag, SW1_Flag; 	// Switch �Է��� Ȧ����°����? ¦����°����? �� �˱����� ���� 
int i,x=80,y=64;
int main(void)
{
	_GPIO_Init(); 		// GPIO (LED, SW, Buzzer) �ʱ�ȭ
	LCD_Init();		// LCD ��� �ʱ�ȭ
	DelayMS(100);       //LCD ���
	BEEP();

	GPIOG->ODR &= ~0x00FF;	// LED �ʱⰪ: LED0~7 Off
	DisplayInitScreen();	// LCD �ʱ�ȭ��
 
	/* ����ǥ�ÿ����� ���� ����ϴ� �Լ��� */ 
	//void LCD_DrawHorLine(UINT16 x, UINT16 y, UINT16 width)  
	//void LCD_DrawVerLine(UINT16 x, UINT16 y, UINT16 height)
	//void LCD_DrawRectangle(UINT16 x, UINT16 y, UINT16 width, UINT16 height)  
	//void LCD_DrawFillRect(UINT16 x, UINT16 y, UINT16 width, UINT16 height)  
	//void LCD_DrawLine(UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)
	
	while(1)
	{
	      
  
                switch(JOY_Scan())  //�Ǹ���
		{
			case NAVI_UP    :	
				 LCD_SetBrushColor(RGB_WHITE);
                              LCD_DrawFillRect(116, 101, 10,30);

                          
			break;
			case NAVI_DOWN:	
				 LCD_SetBrushColor(RGB_RED);
                              LCD_DrawFillRect(116, 91, 10, 30);
                          
			break;
                        
                       
		}  // switch(JOY_Scan())
                  switch(KEY_Scan())
                  {
                    case 0xFE00:
                               LCD_SetBrushColor(RGB_RED);
                              LCD_DrawFillRect(116, 91, 10, 30);
                              DelayMS(300);
                               LCD_SetBrushColor(RGB_WHITE);
                              LCD_DrawFillRect(116, 101, 10,30);
                              
                              

                         break;
                 
                  case 0xEF00: 	//SW4
				GPIOG->BSRRL = 0x0010; 	//  LED 0 ON
				BEEP();
			
				break;
                                case 0xDF00: 	//SW5
				GPIOG->BSRRH = 0x0010; 	//  LED 0 ON
				BEEP();
			
				break;
                  
                  }
                






                   /*	   switch(JOY_Scan())               // ���̽�ƽ���� �� �����̰�
		{
			case 0x01E0  :	// NAVI_LEFT  (PI9)
				if (SW0_Flag==0) {
				// LCD_DrawHorLine(80, 64, 1) ;
                                  // LCD_DrawFillRect(79,63,2,2);
                                  GPIOG->ODR |= 0x0001;		// LED0 ON	
                                  SW0_Flag = 1;
				}
                                else
                                { GPIOG->ODR &= ~0x0001;	
                                  SW0_Flag = 0;
                                }
                                x--;
                                 LCD_SetBrushColor(RGB_BLUE);
                                LCD_DrawFillRect(x-1,y-1,2,2);
                                
                                LCD_SetBrushColor(RGB_WHITE);
                                LCD_DrawFillRect(x-1+2,y-1,2,2);
                                
                                //  LCD_SetPenColor(RGB_BLACK); 
                                //LCD_DrawHorLine(80-x, 64, 1) ;
                                 
                               //  LCD_SetPenColor(RGB_WHITE);
                            // LCD_DrawHorLine(80-x+1, 64, 1) ;
                                 
                                    LCD_DisplayChar(1,12,x+0x30);
			break;
			case 0x02E0:	// NAVI_RIGHT
				x++;
                                 LCD_SetBrushColor(RGB_BLUE);
                                LCD_DrawFillRect(79+x,63,2,2);
                                
                                LCD_SetBrushColor(RGB_WHITE);
                                LCD_DrawFillRect(79+x-2,63,2,2);                       
                   break;
                   case NAVI_UP:	// NAVI_
				y--;
                                 LCD_SetBrushColor(RGB_BLUE);
                                LCD_DrawFillRect(x-1,y-1,2,2);
                                
                                LCD_SetBrushColor(RGB_WHITE);
                                LCD_DrawFillRect(x-1,y-1+2,2,2);                       
                   break;
                   
                      case NAVI_DOWN:	// NAVI_
				y++;
                                 LCD_SetBrushColor(RGB_BLUE);
                                LCD_DrawFillRect(79,63-y,2,2);
                                
                                LCD_SetBrushColor(RGB_WHITE);
                                LCD_DrawFillRect(79,63-y+2,2,2);                       
                   break;
                   
                   
                }*/
          
              switch(KEY_Scan())	// �Էµ� Switch ���� �з� 
		{
			case 0xFE00 : 	//SW0
				if (SW0_Flag==0) {
					GPIOG->ODR |= 0x0001;		// LED0 ON		
					LCD_DisplayText(1,5,"ON ");	// Text(String) "ON " ǥ��
					SW0_Flag = 1;          //���� ON�ڿ� �ٿ��
                                        
				}
				else {
					GPIOG->ODR &= ~0x0001;		// LED0 OFF
					LCD_DisplayText(1,5,"OFF");	// Text(String) "OFF" ǥ��
					SW0_Flag = 0;
                                    
				}
			break;
			case 0xFD00 : 	//SW1
				if (SW1_Flag==0) {
				GPIOG->ODR ^= 0x0002;	
                                //GPIOG->BSRRL = 0x0002;		// LED1 ON		
					LCD_DisplayChar(2,5,'O');	// Charater 'O' ǥ��
					SW1_Flag = 1;
				}
				else {
				GPIOG->ODR ^= 0x0002;
                                //GPIOG->BSRRH = 0x0002;		// LED1 OFF
					LCD_DisplayChar(2,5,0x58);	// Charater 'X' ǥ��
									// 'X'=0x58 (ASCII code)
					SW1_Flag = 0;
				}	
                                i++;   
                                LCD_DisplayChar(2,8,i+0x30);
                                if(i==10) i=0;
			break;
		}  // switch(KEY_Scan())
	}  // while(1)
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
	RCC->AHB1ENR	|= 0x00000100;	// RCC_AHB1ENR GPIOI 8�� ��Ʈ Enable
	GPIOI->MODER	&=~0x000FFC00;	// GPIOI 5~9 : Input mode (reset state)
	  //0b 0000 0000 0000 1111 1111 1100 0000 0000 
        GPIOI->PUPDR	&= ~0x000FFC00;	// GPIOI 5~9 : Floating input (No Pull-up, pull-down) (reset state)
}

/* GLCD �ʱ�ȭ�� ���� �Լ� */
void DisplayInitScreen(void)
{
	LCD_Clear(RGB_GRAY);		// ȭ�� Ŭ����
	LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
	LCD_SetBackColor(RGB_GREEN);	// ���ڹ��� : Green
	LCD_SetTextColor(RGB_BLACK);	// ���ڻ� : Black
	LCD_DisplayText(0,0,"GLCD Test");	// Title

	LCD_SetBackColor(RGB_YELLOW);	//���ڹ��� : Yellow
          
	LCD_DisplayText(1,0,"LED0 OFF");
	LCD_DisplayText(2,0,"LED1 X");

	/* �׸�������� ���� ����ϴ� �Լ��� */
	//void LCD_SetPenColor(UINT32 Color); //���(�׸����)����
	//void LCD_DrawHorLine(UINT16 x, UINT16 y, UINT16 width)  // ���� �׸���
	//void LCD_DrawVerLine(UINT16 x, UINT16 y, UINT16 height) // ������ �׸���
	//void LCD_DrawRectangle(UINT16 x, UINT16 y, UINT16 width, UINT16 height) // �簢�� �׸���  
	//void LCD_DrawFillRect(UINT16 x, UINT16 y, UINT16 width, UINT16 height)  // �簢��(fille) �׸���
	//void LCD_DrawLine(UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)  // ���� �׸���
    	
	/* �׸��� �׽�Ʈ�� ����� */
/*	LCD_SetPenColor(RGB_RED);
	LCD_DrawHorLine(0, 60, 100);
	LCD_SetPenColor(RGB_BLUE);
	LCD_DrawVerLine(0, 60, 110);
	LCD_SetPenColor(RGB_RED);
	LCD_DrawRectangle(10, 70, 30, 20);
	LCD_SetBrushColor(RGB_GRAY);
	LCD_DrawFillRect(50, 70, 40, 30);
	LCD_SetPenColor(RGB_BLACK);  
	LCD_DrawLine(0, 60, 100, 100);  */
//����
        LCD_SetBackColor(RGB_BLACK);	// ���ڹ��� : ����
 	LCD_SetTextColor(RGB_YELLOW);	// ���ڻ� : ���
        LCD_DisplayText(3,0,"LCE test");  //
//P
#define  RGB_VIOLET   GET_RGB(255,0,255)   
         //
         LCD_SetPenColor(RGB_BLACK);
         LCD_SetBrushColor(RGB_VIOLET);
         
         LCD_DrawRectangle(10, 70, 20, 20);
         LCD_DrawFillRect(10, 70, 20, 20);
         LCD_DrawLine(10, 70, 10,110);
         
           LCD_SetPenColor(RGB_BLACK);
          LCD_DrawHorLine(10, 70, 20) ;
    //U     
          LCD_SetBrushColor(GET_RGB(255,192,203)); //��ũ
          LCD_DrawFillRect(30, 90, 20, 20);
           LCD_DrawRectangle(30, 90, 20, 20);
            LCD_SetPenColor(RGB_WHITE);
            LCD_DrawHorLine(31, 90, 20) ;  //����  ,��
            
            //
            LCD_SetPenColor(RGB_BLACK);
             LCD_SetBrushColor(RGB_BLUE);
           //LCD_DrawRectangle(80,64,1,1);
             LCD_DrawFillRect(79,63,2,2);
           //  LCD_DrawHorLine(80, 64, 1) ;  //�� �ϳ���  �����̵��� ����
       
             
     //        �Ǹ���
             
             LCD_SetPenColor(RGB_BLUE);
	LCD_SetBrushColor(RGB_VIOLET);	
	LCD_DrawRectangle(110, 40, 20, 50);
        LCD_DrawFillRect(111, 41, 19, 49);
        
           LCD_SetPenColor(RGB_BLACK);
           LCD_DrawLine(110, 40, 130,90);
            LCD_DrawLine(130, 40, 110,90);
            
            LCD_SetBrushColor(RGB_RED);
             LCD_DrawFillRect(116, 91, 10, 10);
             
             LCD_DrawPixel(140, 120);
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
