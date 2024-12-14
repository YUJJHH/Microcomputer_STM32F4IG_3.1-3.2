///////////////////////////////////////////////////////////////////////////////////
//      ������ : ����������
//      ��������:  ����(0~6��)�� ��ġ�� ���������Ϳ���, ��ǥ�� ����ġ�� �Է��Ͽ� 
//                    ��ǥ������ �̵��ϵ��� �����ϴ� ���α׷� �ۼ�
//      ����� �ϵ����(���) : GPIO, EXTI, GLCD 
//      ������ :    2023.5.24     
//      ������ Ŭ���� :    ����Ϲ�
//                    �й�:      2019130027 
//                    �̸�:       ����ȯ
//////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"
#include "GLCD.h"

//15~31�� ���ο� �Լ� ����
void UP0_1(void);       //0������ 1������ �̵�
void UP1_2(void);       //1������ 2������ �̵�
void UP2_3(void);
void UP3_4(void);
void UP4_5(void);
void UP5_6(void);

void DOWN6_5(void);     //6������ 5������ �̵�
void DOWN5_4(void);
void DOWN4_3(void);
void DOWN3_2(void);
void DOWN2_1(void);
void DOWN1_0(void);

void BEEP_3(void); // ���� 3�� �Լ�
void BEEP_2(void); //���� 2�� �Լ�

void _GPIO_Init(void);
void _EXTI_Init(void);

void DisplayInitScreen(void);
uint16_t KEY_Scan(void);
void BEEP(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint8_t	SW7_Flag=0;

int main(void)
{
	LCD_Init();	// LCD ��� �ʱ�ȭ
	DelayMS(10);
	_GPIO_Init();	// GPIO �ʱ�ȭ
	

	DisplayInitScreen();	// LCD �ʱ�ȭ��

	GPIOG->ODR &= ~0x00FF;	// �ʱⰪ: LED0~7 Off
	GPIOG->ODR |= 0x0001;	// GPIOG->ODR.0 'H'(LED ON)
    
        _EXTI_Init();	// EXTI �ʱ�ȭ
        
	while(1)
	{
	
          if((GPIOG->ODR & 0x0001)==0x0001) //LED0 �� ON�϶� ����
               {             
                  switch(KEY_Scan())
                  {
                     case 0xFD00:  //SW1 ON
                             BEEP();
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"1");
                             UP0_1();                         //1���� LED0 ����, LED1 ��  (0������ 1������ �̵�)
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	                     //���� 3ȸ
                         break;
                         
                    case 0xFB00:  //SW2 ON 
                             BEEP();
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"2");
                             UP0_1();                                //0������ 1������ �̵�
                             UP1_2();                                //1������ 2������ �̵�
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xF700:  //SW3  ON
                             BEEP();
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"3");
                             UP0_1();                                  //0������ 1������ �̵�
                             UP1_2();                                 //1������ 2������ �̵�
                             UP2_3();                                 //2������ 3������ �̵�
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xEF00:  //SW4 ON
                             BEEP();
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"4");
                             UP0_1();                                 //0������ 1������ �̵�
                             UP1_2();                                 //1������ 2������ �̵�
                             UP2_3();                                 //2������ 3������ �̵�
                             UP3_4();                                 //3������ 4������ �̵�
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xDF00:  //SW5 ON
                             BEEP();
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"5");
                             UP0_1();                                 //0������ 1������ �̵�
                             UP1_2();                                 //1������ 2������ �̵�
                             UP2_3();                                 //2������ 3������ �̵�
                             UP3_4();                                 //3������ 4������ �̵�
                             UP4_5();                                 //4������ 5������ �̵�
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                            break;
                            
                     case 0xBF00:  //SW6 ON
                             BEEP();
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"6");
                             UP0_1();                                 //0������ 1������ �̵�
                             UP1_2();                                 //1������ 2������ �̵�
                             UP2_3();                                 //2������ 3������ �̵�
                             UP3_4();                                 //3������ 4������ �̵�
                             UP4_5();                                 //4������ 5������ �̵�
                             UP5_6();                                 //5������ 6������ �̵�
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                        }  //switch(KEY_Scan())
                 }
          
           if((GPIOG->ODR & 0x0002)==0x0002) //LED1�� ON�϶� ����
               {             
                  switch(KEY_Scan())
                  {
                    case 0xFE00:  //SW0 ON
                             BEEP();
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"0");
                             DOWN1_0();                                   //1������ 0������ �̵�
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xFB00:  //SW2 ON 
                             BEEP();
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"2");
                             UP1_2();                                    //1������ 2������ �̵�
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xF700:  //SW3  ON
                             BEEP();
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"3");
                             UP1_2();                                    //1������ 2������ �̵�
                             UP2_3();                                    //2������ 3������ �̵�
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xEF00:  //SW4 ON
                             BEEP();
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"4");
                             UP1_2(); 
                             UP2_3(); 
                             UP3_4(); 
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xDF00:  //SW5 ON
                             BEEP();
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"5");
                             UP1_2(); 
                             UP2_3(); 
                             UP3_4(); 
                             UP4_5(); 
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                            break;
                            
                     case 0xBF00:  //SW6 ON
                             BEEP();
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"6");
                             UP1_2(); 
                             UP2_3(); 
                             UP3_4(); 
                             UP4_5(); 
                             UP5_6(); 
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                        }  //switch(KEY_Scan())
                 }
            if((GPIOG->ODR & 0x0004)==0x0004) //LED2�� ON�϶� ����
               {             
                  switch(KEY_Scan())
                  {
                     case 0xFE00:  //SW0 ON
                             BEEP();
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"0");
                             DOWN2_1();                     //2������ 1������
                             DOWN1_0();                    //1������ 0������
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xFD00:  //SW1 ON 
                             BEEP();
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"1");
                             DOWN2_1();  
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xF700:  //SW3  ON
                             BEEP();
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"3");
                             UP2_3(); 
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xEF00:  //SW4 ON
                             BEEP();
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"4");
                             UP2_3(); 
                             UP3_4(); 
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xDF00:  //SW5 ON
                             BEEP();
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"5");
                             UP2_3(); 
                             UP3_4(); 
                             UP4_5(); 
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                            break;
                            
                     case 0xBF00:  //SW6 ON
                             BEEP();
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"6");
                             UP2_3(); 
                             UP3_4(); 
                             UP4_5(); 
                             UP5_6(); 
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                        }  //switch(KEY_Scan())
                 }
          if((GPIOG->ODR & 0x0008)) //LED3�� ON�϶� ����
               {             
                  switch(KEY_Scan())
                  {
                      case 0xFE00:  //SW0  ON
                             BEEP();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"0");
                             DOWN3_2();                      //3������ 2������
                             DOWN2_1();                      //2������ 1������
                             DOWN1_0();                     //1������ 0������
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                     case 0xFD00:  //SW1 ON
                             BEEP();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"1");
                             DOWN3_2(); 
                             DOWN2_1();   
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xFB00:  //SW2 ON 
                             BEEP();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"2");
                             DOWN3_2();  
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                        
                     case 0xEF00:  //SW4 ON
                             BEEP();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"4");
                             UP3_4(); 
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xDF00:  //SW5 ON
                             BEEP();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"5");
                             UP3_4(); 
                             UP4_5(); 
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                            break;
                            
                     case 0xBF00:  //SW6 ON
                             BEEP();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"6");
                             UP3_4(); 
                             UP4_5(); 
                             UP5_6(); 
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                        }  //switch(KEY_Scan())
                 }
            if((GPIOG->ODR & 0x0010)) //LED4�� ON�϶� ����
               {             
                  switch(KEY_Scan())
                  {
                     case 0xFE00:  //SW0  ON
                             BEEP();
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"0");
                             DOWN4_3();                     //4������ 3������
                             DOWN3_2();                      //3������ 2������
                             DOWN2_1();                      //2������ 1������
                             DOWN1_0();                     //1������ 0������
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"-");
                               BEEP_3();	
                         break;
                     case 0xFD00:  //SW1 ON
                             BEEP();
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"1");
                             DOWN4_3();
                             DOWN3_2(); 
                             DOWN2_1();   
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xFB00:  //SW2 ON 
                             BEEP();
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"2");
                             DOWN4_3();
                             DOWN3_2();  
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
             
                     case 0xF700:  //SW3 ON
                             BEEP();
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"3");
                             DOWN4_3();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                     case 0xDF00:  //SW5 ON
                             BEEP();
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"5");
                             UP4_5(); 
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                            break;
                            
                     case 0xBF00:  //SW6 ON
                             BEEP();
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"6");
                             UP4_5(); 
                             UP5_6(); 
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                        }  //switch(KEY_Scan())
                 }
            if((GPIOG->ODR & 0x0020)) //LED5�� ON�϶� ����
               {             
                  switch(KEY_Scan())
                  {
                         case 0xFE00:  //SW0  ON
                             BEEP();
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"0");
                             DOWN5_4();                      //5������ 4������
                             DOWN4_3();                      //4������ 3������
                             DOWN3_2();                      //3������ 2������
                             DOWN2_1();                      //2������ 1������
                             DOWN1_0();                      //1������ 0������
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                      case 0xFD00:  //SW1 ON
                             BEEP();
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"1");
                             DOWN5_4();
                             DOWN4_3();
                             DOWN3_2(); 
                             DOWN2_1();   
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"-");
                            BEEP_3();	
                         break;
                         
                      case 0xFB00:  //SW2 ON 
                             BEEP();
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"2");
                             DOWN5_4();
                             DOWN4_3();
                             DOWN3_2();  
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
             
                       case 0xF700:  //SW3 ON
                             BEEP();
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"3");
                             DOWN5_4();
                             DOWN4_3();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                      case 0xEF00:  //SW4 ON
                             BEEP();
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"4");
                             DOWN5_4();
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                           break;
                            
                      case 0xBF00:  //SW6 ON
                             BEEP();
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"6");
                             UP5_6(); 
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                        }  //switch(KEY_Scan())
                 }
           if((GPIOG->ODR & 0x0040)) //LED6�� ON�϶� ����
               {             
                  switch(KEY_Scan())
                  {
                       case 0xFE00:  //SW0  ON
                             BEEP();
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"0");
                             DOWN6_5();                      //6������ 5������
                             DOWN5_4();                      //5������ 4������
                             DOWN4_3();                      //4������ 3������
                             DOWN3_2();                      //3������ 2������
                             DOWN2_1();                      //2������ 1������
                             DOWN1_0();                      //1������ 0������
                             LCD_DisplayText(1,8,"0");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                      case 0xFD00:  //SW1 ON
                             BEEP();
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"1");
                             DOWN6_5();
                             DOWN5_4();
                             DOWN4_3();
                             DOWN3_2(); 
                             DOWN2_1();   
                             LCD_DisplayText(1,8,"1");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                      case 0xFB00:  //SW2 ON 
                             BEEP();
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"2");
                             DOWN6_5();
                             DOWN5_4();
                             DOWN4_3();
                             DOWN3_2();  
                             LCD_DisplayText(1,8,"2");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
             
                       case 0xF700:  //SW3 ON
                             BEEP();
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"3");
                             DOWN6_5();
                             DOWN5_4();
                             DOWN4_3();
                             LCD_DisplayText(1,8,"3");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                         
                      case 0xEF00:  //SW4 ON
                             BEEP();
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"4");
                             DOWN6_5();
                             DOWN5_4();
                             LCD_DisplayText(1,8,"4");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                            break;
                            
                      case 0xDF00:  //SW5 ON
                             BEEP();
                             LCD_DisplayText(1,8,"6");
                             LCD_DisplayText(2,8,"5");
                             DOWN6_5();
                             LCD_DisplayText(1,8,"5");
                             LCD_DisplayText(2,8,"-");
                             BEEP_3();	
                         break;
                        }  //switch(KEY_Scan())
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
	//LCD_DisplayText(0,0,"EXTI Test");  	// Title

	LCD_SetBackColor(RGB_YELLOW);	//���ڹ��� : Yellow
     
        //�߰�(����)
        LCD_SetBrushColor(RGB_YELLOW);
        LCD_DrawFillRect(0,0,160,128);
        LCD_SetTextColor(RGB_BLUE);
        LCD_DisplayText(0,0,"MECHA Elevator(YJH)");
        
         LCD_SetTextColor(RGB_BLACK);
         LCD_DisplayText(1,0,"Cur FL:");
         LCD_DisplayText(2,0,"Des FL:");
        
          LCD_SetTextColor(RGB_RED);
          LCD_DisplayText(1,7," 0");
         LCD_DisplayText(2,8,"-");
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
}	


void _EXTI_Init(void)
{
	RCC->AHB1ENR 	|= 0x00000080;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= 0x00004000;	// Enable System Configuration Controller Clock
			// 0b 0000 0000 0000 0000 0100 0000 0000 0000
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH PIN8~PIN15 Input mode (reset state)				 
	
	SYSCFG->EXTICR[3] |= 0x7000;	// EXTI15(PH15)�� ���� �ҽ� �Է��� GPIOH�� ����
					
	EXTI->FTSR |= 0x00008000;	// EXTI15: Falling Trigger Enable 

	EXTI->IMR   |= 0x00008000;	// EXTI15 ���ͷ�Ʈ mask (Interrupt Enable) ����
		
	NVIC->ISER[1] |= ( 1<<(40-32)  );	// Enable 'Global Interrupt EXTI15'
					// Vector table Position ����
		
}
void EXTI15_10_IRQHandler(void)
{	
            if(EXTI->PR & 0x8000)			// EXTI15 Interrupt Pending(�߻�) ����?
              {
         
              //  EXTI->PR |= 0x8000;		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)           
                GPIOG->ODR ^= 0x0080;	// LED7 Toggle
                BEEP_2();
                DelayMS(5000);
                GPIOG->ODR ^= 0x0080;	// LED7 Toggle
                BEEP_2();   
                EXTI->PR |= 0x8000;
                }
}


/* EXTI5~9 ���ͷ�Ʈ �ڵ鷯(ISR: Interrupt Service Routine) */
/*void EXTI9_5_IRQHandler(void)
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
		GPIOG->ODR ^= 0x02;		// LED1 Toggle
	}
}*/

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

void BEEP_3(void)  //���� 3ȸ
{ 	
	GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
	DelayMS(30);		// Delay 30 ms
	GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
        DelayMS(30);
        GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
	DelayMS(30);		// Delay 30 ms
	GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
        DelayMS(30);
        GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
	DelayMS(30);		// Delay 30 ms
	GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
}

void BEEP_2(void)  //���� 2ȸ
{ 	
	GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
	DelayMS(30);		// Delay 30 ms
	GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
        DelayMS(30);
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

//���ο� �Լ� ����
//1���� ���
void UP0_1(void)  //0������ 1������
{
     DelayMS(1000);   
     GPIOG->ODR &=~0x0001;                                   
     GPIOG->ODR |=0x0002;
}
void UP1_2(void)  //1������ 2������
{
    DelayMS(1000);   
    GPIOG->ODR &=~0x0002;                                   
    GPIOG->ODR |=0x0004;
}
void UP2_3(void)  //2������ 3������  
{
    DelayMS(1000);   
    GPIOG->ODR &=~0x0004;                                   
    GPIOG->ODR |=0x0008;
}
void UP3_4(void)    //3������ 4������
{
    DelayMS(1000);   
    GPIOG->ODR &=~0x0008;                                   
    GPIOG->ODR |=0x0010;
}
void UP4_5(void)  //4������ 5������
{
    DelayMS(1000);   
    GPIOG->ODR &=~0x0010;                                   
    GPIOG->ODR |=0x0020;
}
void UP5_6(void)  //5������ 6������
{
    DelayMS(1000);   
    GPIOG->ODR &=~0x0020;                                   
    GPIOG->ODR |=0x0040;
}

//1���� �ϰ�
void DOWN1_0(void)  //1������ 0������
{
        DelayMS(1000);   
     GPIOG->ODR &=~0x0002;                                   
     GPIOG->ODR |=0x0001;
}
void  DOWN2_1(void)  //2������ 1������
{
        DelayMS(1000);   
     GPIOG->ODR &=~0x0004;                                   
     GPIOG->ODR |=0x0002;
}
void  DOWN3_2(void)  //3������ 2������
{
        DelayMS(1000);   
     GPIOG->ODR &=~0x0008;                                   
     GPIOG->ODR |=0x0004;
}
void DOWN4_3(void)  //4������ 3������
{
       DelayMS(1000);   
     GPIOG->ODR &=~0x0010;                                   
     GPIOG->ODR |=0x0008; 
}
void DOWN5_4(void)  //5������ 4������
{
        DelayMS(1000);   
     GPIOG->ODR &=~0x0020;                                   
     GPIOG->ODR |=0x0010;
}
void DOWN6_5(void)  //6������ 5������
{
      DelayMS(1000);   
     GPIOG->ODR &=~0x0040;                                   
     GPIOG->ODR |=0x0020;
}