///////////////////////////////////////////////////////////////////////////////////
//      ������ :  TP1. Two Elevators
//      ��������:  6�� �ǹ��� �δ��� ����������(Left ����������(L-E), Right ����������(R-E)) ��ġ �Ͽ�
//                   ��������� ��ǥ�� ����ġ(1~6��)�� �����ϸ� ���������Ͱ� �̵��ϵ��� ���α׷� �ۼ�
//      ����� �ϵ����(���) : GPIO, EXTI, GLCD 
//      ������ :    2023.6.01     
//      ������ Ŭ���� :    ����Ϲ�
//                    �й�:      2019130027 
//                    �̸�:       ����ȯ
//////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"
#include "GLCD.h"
#include "FRAM.h"

void ELE_L_UP(void); //���� ���� �ö�  
void ELE_L_DOWN(void); //���� ���� ������ 
void ELE_R_UP(void); //������ ���� �ö�  
void ELE_R_DOWN(void); //������ ���� ������  

void BEEP_3(void); // ���� 3�� �Լ�


void _GPIO_Init(void);
void _EXTI_Init(void);

void DisplayInitScreen(void);
uint16_t KEY_Scan(void);
void BEEP(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint8_t	SW0_Flag;

int  START_FL=1;  //����� ����
int  DESTI_FL=1;  //��ǥ�� ����

int y1=89;  //���� ���������� ��ǥ��
int y2=89;  //������ ���������� ��ǥ��

int Y1=1;  //���� ���������� ���� �� ��
int Y2=1;  //������ ���������� ���� �� ��




int main(void)
{
	LCD_Init();	// LCD ��� �ʱ�ȭ
	DelayMS(10);
	_GPIO_Init();	// GPIO �ʱ�ȭ
	_EXTI_Init();	// EXTI �ʱ�ȭ

      	Fram_Init();                    // FRAM �ʱ�ȭ H/W �ʱ�ȭ
	Fram_Status_Config();   // FRAM �ʱ�ȭ S/W �ʱ�ȭ
        
	DisplayInitScreen();	// LCD �ʱ�ȭ��

	GPIOG->ODR &= ~0x00FF;	// �ʱⰪ: LED0~7 Off
	GPIOG->ODR |= 0x0080;	// �ʱ� LED7 on ����
    
          Y1=Fram_Read(2023);       //���� ����������  ���尪 ����
        LCD_SetTextColor(RGB_RED);  
        LCD_SetPenColor(RGB_BLUE);
        LCD_SetBrushColor(RGB_BLUE);
        
        if(Y1==1)        //���� ���������Ϳ� 1�� ����
        {
            y1=89;
            LCD_DrawRectangle(20, y1, 10, 12);
            LCD_DrawFillRect(20, y1, 10, 12);
            START_FL=Y1;
            DESTI_FL=Y1;     
            LCD_DisplayChar(6,6,START_FL+0x30);
            LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==2)      //���� ���������Ϳ�  2�� ����
        {
            y1=77;
            LCD_DrawRectangle(20, y1, 10, 24);
            LCD_DrawFillRect(20, y1, 10, 24);
            START_FL=Y1;
            DESTI_FL=Y1;     
            LCD_DisplayChar(6,6,START_FL+0x30);
            LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==3)    //���� ���������Ϳ� 3�� ����
        { 
             y1=65;
             LCD_DrawRectangle(20, y1, 10, 36);
             LCD_DrawFillRect(20, y1, 10, 36);
             START_FL=Y1;
             DESTI_FL=Y1;     
             LCD_DisplayChar(6,6,START_FL+0x30);
             LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==4)      //���� ���������Ϳ� 4�� ����
        { 
           y1=53;
           LCD_DrawRectangle(20, y1, 10, 48);
           LCD_DrawFillRect(20, y1, 10, 48);
           START_FL=Y1;
           DESTI_FL=Y1;     
           LCD_DisplayChar(6,6,START_FL+0x30);
           LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==5)     //���� ���������Ϳ� 5�� ����
        { 
           y1=41;
           LCD_DrawRectangle(20, y1, 10, 60);
           LCD_DrawFillRect(20, y1, 10, 60);
           START_FL=Y1;
           DESTI_FL=Y1;     
           LCD_DisplayChar(6,6,START_FL+0x30);
           LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==6)        //���� ���������Ϳ� 6�� ����
        { 
            y1=29;
            LCD_DrawRectangle(20, y1, 10, 72);
            LCD_DrawFillRect(20, y1, 10, 72);
            START_FL=Y1;        
            DESTI_FL=Y1;     
            LCD_DisplayChar(6,6,START_FL+0x30);
             LCD_DisplayChar(6,10,START_FL+0x30);
         }
        
         Y2=Fram_Read(2024);           //������ ���������� ���尪 ����
         LCD_SetPenColor(RGB_GREEN); 
         LCD_SetBrushColor(RGB_GREEN);
         
        if(Y2==1)          //������ ���������Ϳ�  1�� ����
        {
              y2=89;
              LCD_DrawRectangle(110, y2, 10, 12);
               LCD_DrawFillRect(110, y2, 10, 12);
        }
        else if(Y2==2)        //������ ���������Ϳ� 2�� ����
        {
             y2=77;
             LCD_DrawRectangle(110, y2, 10, 24);
             LCD_DrawFillRect(110, y2, 10, 24);
        }
        else if(Y2==3) //������ ���������Ϳ�  3�� ����
        { 
               y2=65;
               LCD_DrawRectangle(110, y2, 10, 36);
                LCD_DrawFillRect(110, y2, 10, 36);
        }
        else if(Y2==4) //������ ���������Ϳ�  4�� ����
        { 
             y2=53;
             LCD_DrawRectangle(110, y2, 10, 48);
             LCD_DrawFillRect(110, y2, 10, 48);
        }
        else if(Y2==5) //������ ���������Ϳ�  5�� ����
        { 
              y2=41;
               LCD_DrawRectangle(110, y2, 10, 60);
               LCD_DrawFillRect(110, y2, 10, 60);
        }
        else if(Y2==6) //������ ���������Ϳ� 6�� ����
        { 
                y2=29;
                LCD_DrawRectangle(110, y2, 10, 72);
                LCD_DrawFillRect(110, y2, 10, 72);
         }
        
        
	while(1)
	{
                   switch(KEY_Scan())
                   {
                      case 0xFD00:                                        //SW1 ON
                             GPIOG->ODR |= 0x0002;                 //LED1 on
                             GPIOG->ODR &=~ 0x0004;              //LED2 off
                             BEEP();                                         //���� 1ȸ
                             START_FL++;                                  //����� ����
                          
                             if(START_FL==7)
                            { 
                                START_FL=1;
                            }
                             LCD_SetTextColor(RGB_RED);
                           
                             LCD_DisplayChar(6,6,START_FL+0x30);
                        break;
                     case 0xFB00:                                              //SW2 ON
                            GPIOG->ODR |= 0x0004;                      //LED2 on
                            GPIOG->ODR &=~ 0x0002;                   //LED1 off
                            BEEP();                                             //���� 1ȸ
                            DESTI_FL++;                                       //��ǥ�� ����
                             
                            if(DESTI_FL==7)
                            { 
                                DESTI_FL=1;
                            }
                             LCD_SetTextColor(RGB_RED);
                           
                             LCD_DisplayChar(6,10,DESTI_FL+0x30);
                         break;        
                    }//switch(KEY_Scan())
     
	}
        
        
}

/* GLCD �ʱ�ȭ�� ���� �Լ� */
void DisplayInitScreen(void)
{
	LCD_Clear(RGB_WHITE);		// ȭ�� Ŭ����
	LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
	LCD_SetBackColor(RGB_WHITE);	//���ڹ��� : ���
	LCD_SetTextColor(RGB_BLACK);	// ���ڻ� : Black
        LCD_DisplayText(0,0,"MC-Elevator(YJH)");
        
 //01234567890123456789       
 //       6    FL   6 
        
        LCD_SetTextColor(RGB_BLUE);
        LCD_DisplayText(2,4,"6");
        LCD_SetTextColor(RGB_RED);
        LCD_DisplayText(2,7,"FL");
        LCD_SetTextColor(RGB_GREEN);
        LCD_DisplayText(2,12,"6");
        
        LCD_SetTextColor(RGB_BLUE);
        LCD_DisplayText(3,4,"5");
        LCD_SetTextColor(RGB_GREEN);
        LCD_DisplayText(3,12,"5");
        
        LCD_SetTextColor(RGB_BLUE);
        LCD_DisplayText(4,4,"4");
        LCD_SetTextColor(RGB_BLUE);
        LCD_DisplayText(4,7,"L-E");
        LCD_SetTextColor(RGB_GREEN);
        LCD_DisplayText(4,12,"4");
        
         LCD_SetTextColor(RGB_BLUE);
        LCD_DisplayText(5,4,"3");
        LCD_SetTextColor(RGB_RED);
        LCD_DisplayText(5,8,"S");
        LCD_SetTextColor(RGB_GREEN);
        LCD_DisplayText(5,12,"3");
        
         LCD_SetTextColor(RGB_BLUE);
        LCD_DisplayText(6,4,"2");
        LCD_SetTextColor(RGB_RED);
        LCD_DisplayText(6,6,"  > 1");
         LCD_DisplayChar(6,6,START_FL+0x30);
        LCD_SetTextColor(RGB_GREEN);
        LCD_DisplayText(6,12,"2");
        
        LCD_SetTextColor(RGB_BLUE);
        LCD_DisplayText(7,4,"1");
        LCD_SetTextColor(RGB_GREEN);
        LCD_DisplayText(7,12,"1");
        
      //�������(�簢��)
        LCD_SetPenColor(RGB_BLUE);
	LCD_DrawRectangle(20, y1, 10, 12);
	LCD_SetBrushColor(RGB_BLUE);
	LCD_DrawFillRect(20, y1, 10, 12);
        
        LCD_SetPenColor(RGB_GREEN);
	LCD_DrawRectangle(110,  y2, 10, 12);
	LCD_SetBrushColor(RGB_GREEN);
	LCD_DrawFillRect(110,  y2, 10, 12);
       
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
        SYSCFG->EXTICR[2] |= 0x0007;	// EXTI8(PH8)�� ���� �ҽ� �Է��� GPIOH�� ����
					
	EXTI->FTSR |= 0x00008000;	// EXTI15: Falling Trigger Enable 
        EXTI->FTSR |= 0x00000100;	// EXTI8: Falling Trigger Enable

	 
         EXTI->IMR   |= 0x00000100;	// EXTI8 ���ͷ�Ʈ mask (Interrupt Enable) ����
		
	NVIC->ISER[1] |= ( 1<<(40-32)  );	// Enable 'Global Interrupt EXTI15'  
					
        NVIC->ISER[0] |= ( 1<<23 );	// Enable 'Global Interrupt EXTI8'
        
        NVIC->IP[23]= 0xF0;  // EXTI9_5 ,  Low Priority     EXTI8���� ����   
       
        NVIC->IP[40]= 0xE0;  // EXTI15_10    High Priority    EXTI15���� �� ���� �켱����
        
}

void EXTI9_5_IRQHandler(void)
{		
	 
      if(EXTI->PR & 0x0100)			// EXTI8 Interrupt Pending(�߻�) ����?
      {
          EXTI->IMR   |= 0x00008000;	// EXTI15- ���ͷ�Ʈ mask (Interrupt Enable) ����: �����忡�� enable
          EXTI->PR |= 0x0100;		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)	
        
          LCD_SetTextColor(RGB_RED);     
          LCD_DisplayText(2,7,"EX");        //������ EX ǥ��
          LCD_DisplayText(5,8,"U");         //UP
        
         BEEP();  //���� 1ȸ
         GPIOG->ODR ^= 0x01;		// LED0 Toggle
         GPIOG->ODR &=~ 0xFE;		// LED7~1 T��� OFF
                
                if(abs(Y1-START_FL) <= abs(Y2-START_FL))  //������� ���� ����� ���������Ͱ� ���� ���������� �϶�  &�Ÿ� ���̰� ������ ���� �켱 
                {  
                      LCD_SetTextColor(RGB_BLUE);
                      LCD_DisplayText(4,7,"L-E");                       //���� ���������� ���� ǥ��
                      DelayMS(500);
                          if(Y1<START_FL)                                 //���������Ͱ� ����� �ؿ� ��ġ
                          {
                                  if(START_FL <DESTI_FL)             //������<����� < ��ǥ��  
                                  {
                                  
                                            while(START_FL!=Y1)          //������� �����ϸ� while�� Ż��
                                              {
                                                  y1=y1-12;                     //��ǥ�� -12
                                                  Y1=Y1+1;                      //�� +1      
                                                  ELE_L_UP();                //���� ���� ���
                                                  DelayMS(500);   
                                              }      
                                        //������� �����ϸ�
                                                LCD_SetTextColor(RGB_RED);     
                                                LCD_DisplayText(5,8,"S");           // STOP
                                                DelayMS(500);                            //0.5���� 
                                                LCD_DisplayText(5,8,"U");              // UP
                                                DelayMS(500);                             //0.5���� 
                                                 
                                          while(DESTI_FL!=Y1)     //�������� �����ϸ� while�� Ż��
                                              {
                                                      y1=y1-12;               //��ǥ�� -12
                                                      Y1=Y1+1;               //�� +1
                                                      ELE_L_UP();         //���� ���� ���
                                                     DelayMS(500); 
                                              }  
                                       //�������� �����ϸ�
                                             Fram_Write(2023,Y1);                              //2023������ ������ ����
                                             LCD_SetTextColor(RGB_RED);       
                                             LCD_DisplayText(5,8,"S");                   //S ǥ��
                                             DelayMS(500);                                   //0.5����
                                             LCD_DisplayText(2,7,"FL");                  //FL ���
                                             BEEP_3();                                          //���� 3��
                                             GPIOG->ODR |= 0x80;		            // LED7 ON
                                             GPIOG->ODR &=~ 0x7F;	            // LED6~0 ��� OFF
                                      
                                  }
                                 else if(START_FL > DESTI_FL)     //������ < ����� > ������  
                                  {
                                             while(START_FL!=Y1)     //������� �����ϸ� while�� Ż��
                                              {
                                                    y1=y1-12;             //��ǥ�� -12
                                                    Y1=Y1+1;              //�� +1      
                                                    ELE_L_UP();         //���� ���� ���
                                                    DelayMS(500);   
                                              }
                                     //������� �����ϸ�
                                               LCD_SetTextColor(RGB_RED);     
                                               LCD_DisplayText(5,8,"S");             // STOP
                                               DelayMS(500);                            //0.5���� 
                                               LCD_DisplayText(5,8,"U");              // UP
                                               DelayMS(500);                             //0.5���� 
                                                 
                                             while(DESTI_FL!=Y1)     //��ǥ���� �����ϸ� while�� Ż��
                                              {
                                                     ELE_L_DOWN();    //���� ���� �ϰ�
                                                     y1=y1+12;           //��ǥ�� +12
                                                     Y1=Y1-1;           //�� -1
                                                     DelayMS(500); 
                                              }  
                                      //�������� �����ϸ�
                                             Fram_Write(2023,Y1);                           //2023������ ������ ����
                                             LCD_SetTextColor(RGB_RED);       
                                             LCD_DisplayText(5,8,"S");                    //S ǥ��
                                             DelayMS(500);                                   //0.5����
                                             LCD_DisplayText(2,7,"FL");                 //FL ���
                                             BEEP_3();                                        //���� 3��
                                             GPIOG->ODR |= 0x80;		           // LED7 ON
                                             GPIOG->ODR &=~ 0x7F;		   // LED6~0 ��� OFF
                                      

                                      
                                  }
                                  else if(START_FL == DESTI_FL)     //������ <����� = ��ǥ��  : ���������Ͱ� ���������� �̵�
                                  {
                                             while(START_FL!=Y1)               //������� �����ϸ� while�� Ż��
                                              {
                                                     y1=y1-12;                      //��ǥ�� -12
                                                     Y1=Y1+1;                       //�� +1
                                                     ELE_L_UP();                   //���� ���� ���
                                                     DelayMS(500);                //0.5���� 
                                              
                                              }   
                                          //������� �����ϸ� 1���� ����ó��
                                              DelayMS(500);    
                                              Fram_Write(2023,Y1);                              //2023������ ������ ����
                                              LCD_SetTextColor(RGB_RED);       
                                              LCD_DisplayText(5,8,"S");                        //S ǥ��
                                              DelayMS(500);                                       //0.5����
                                              LCD_DisplayText(2,7,"FL");                      //FL ���
                                              BEEP_3();                                             //���� 3��
                                              GPIOG->ODR |= 0x80;		                 // LED7 ON
                                              GPIOG->ODR &=~ 0x7F;		         // LED6~0 ��� OFF
                                   }
                         }
                         else if(Y1>START_FL)                              //���������Ͱ� ����� ���� ��ġ
                         {
                                 if(START_FL <DESTI_FL)                 //������ > ����� < ������  
                                 {
                                           while(START_FL!=Y1)           //������� �����ϸ� while�� Ż��
                                              {
                                                  ELE_L_DOWN();           //���� ���� �ϰ�
                                                  y1=y1+12;                  //��ǥ�� +12
                                                  Y1=Y1-1;                   //�� -1
                                                  DelayMS(500);   
                                              }     
                                           //������� �����ϸ�
                                                 LCD_SetTextColor(RGB_RED);     
                                                 LCD_DisplayText(5,8,"S");             // STOP
                                                 DelayMS(500);                             //0.5���� 
                                                 LCD_DisplayText(5,8,"U");              // UP
                                                 DelayMS(500);                             //0.5���� 

                                           while(DESTI_FL!=Y1)     //��ǥ���� �����ϸ� while�� Ż��
                                              {
                                                     y1=y1-12;            //��ǥ�� -12
                                                     Y1=Y1+1;              //�� +1
                                                     ELE_L_UP();         //���� ���� ���
                                                     DelayMS(500); 
                                              }  
                                          //�������� �����ϸ�
                                             Fram_Write(2023,Y1);                              //2023������ ������ ����
                                             LCD_SetTextColor(RGB_RED);       
                                             LCD_DisplayText(5,8,"S");                  //S ǥ��
                                             DelayMS(500);                                 //0.5����
                                             LCD_DisplayText(2,7,"FL");                //FL ���
                                             BEEP_3();                                       //���� 3��
                                             GPIOG->ODR |= 0x80;		          // LED7 ON
                                             GPIOG->ODR &=~ 0x7F;	          // LED6~0 ��� OFF

                                 }
                                else  if(START_FL >DESTI_FL)     //������ > ����� > ������  
                                 {
                                          while(START_FL!=Y1)            //������� �����ϸ� while�� Ż��
                                              {
                                                      ELE_L_DOWN();         //���� ���� �ϰ�
                                                      y1=y1+12;               //��ǥ�� +12
                                                      Y1=Y1-1;                //�� -1
                                                      DelayMS(500);   
                                              }
                                         //������� �����ϸ�
                                                  LCD_SetTextColor(RGB_RED);    
                                                  LCD_DisplayText(5,8,"S");               // STOP
                                                  DelayMS(500);                               //0.5����           
                                                  LCD_DisplayText(5,8,"U");               //0.5���� UP
                                                  DelayMS(500);                               //0.5���� 
                                                   
                                            while(DESTI_FL!=Y1)              //��ǥ���� �����ϸ� while�� Ż��
                                                {
                                                      ELE_L_DOWN();             //���� ���� �ϰ�
                                                      y1=y1+12;                   //��ǥ�� +12
                                                      Y1=Y1-1;                     //�� -1
                                                      DelayMS(500);
                                                }
                                                 //��ǥ���� �����ϸ�
                                                Fram_Write(2023,Y1);                              //2023������ ��ǥ�� ����
                                                LCD_SetTextColor(RGB_RED);       
                                                LCD_DisplayText(5,8,"S");                       //S ǥ��
                                                DelayMS(500);                                       //0.5����
                                                LCD_DisplayText(2,7,"FL");                      //FL ���
                                                BEEP_3();                                             //���� 3��
                                                GPIOG->ODR |= 0x80;		           // LED7 ON
                                                GPIOG->ODR &=~ 0x7F;		           // LED6~0 ��� OFF
                                        
                                 }
                                 else  if(START_FL == DESTI_FL)     //������ > ����� = ��ǥ��  
                                 { 
                                           while(START_FL!=Y1)              //������� �����ϸ� while�� Ż��
                                                {     
                                                      ELE_L_DOWN();           //���� ���� �ϰ�
                                                      y1=y1+12;                 //��ǥ�� +12
                                                      Y1=Y1-1;                  //�� -1
                                                      DelayMS(500);  
                                                }
                                            //������� �����ϸ�  1���� ���� ó�� 
                                               Fram_Write(2023,Y1);                               //2023������ ��ǥ�� ����
                                               DelayMS(500);                                         //0.5���� 
                                               LCD_SetTextColor(RGB_RED);       
                                               LCD_DisplayText(5,8,"S");                        //S ǥ��
                                               DelayMS(500);
                                               LCD_DisplayText(2,7,"FL");                      //0.5���� FL ���
                                               BEEP_3();                                             //���� 3��
                                               GPIOG->ODR |= 0x80;		          // LED7 ON
                                               GPIOG->ODR &=~ 0x7F;		          // LED6~0 ��� OFF
                                  }
                        }
                        else if(Y1==START_FL)                      //  ����� = ������
                        {
                                      
                                   if(START_FL <DESTI_FL)                    // ����� = ������  < ��ǥ��  
                                   {
                                             DelayMS(500);                       //��342�� 0.5��  + ���� 0.5�� =1�� �� �̵� ��ǥ�� �̵�
                                            while(DESTI_FL!=Y1)              //��ǥ���� �����ϸ� while�� Ż��
                                             {
                                                    y1=y1-12;                     //��ǥ�� -12
                                                    Y1=Y1+1;                      //�� +1
                                                    ELE_L_UP();                 //���� ���� ���
                                                    DelayMS(500); 
                                             }
                                        //��ǥ���� �����ϸ�
                                                 Fram_Write(2023,Y1);                //2023������ ��ǥ�� ����
                                                 LCD_SetTextColor(RGB_RED);       
                                                 LCD_DisplayText(5,8,"S");         //S ǥ��
                                                 DelayMS(500);
                                                 LCD_DisplayText(2,7,"FL");        //0.5���� FL ���
                                                 BEEP_3();                                //���� 3��
                                                 GPIOG->ODR |= 0x80;		// LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;		// LED6~0T��� OFF

                                   }
                                   else if(START_FL >DESTI_FL)     //���� ���� ����� = ������  >  ��ǥ��
                                   {
                                             DelayMS(500);                       //��342�� 0.5��  + ���� 0.5�� =1�� �� �̵� ��ǥ�� �̵�
                                              while(DESTI_FL!=Y1)                 //��ǥ���� �����ϸ� while�� Ż��
                                              {
                                                     ELE_L_DOWN();                  //���� ���� �ϰ�
                                                     y1=y1+12;                         //��ǥ�� +12
                                                     Y1=Y1-1;                          //�� -1
                                                     DelayMS(500);  
                                              }
                                                 //��ǥ���� �����ϸ�
                                                  Fram_Write(2023,Y1);                   //2023������ ��ǥ�� ����
                                                  LCD_SetTextColor(RGB_RED);       
                                                  LCD_DisplayText(5,8,"S");            //S ǥ��
                                                  DelayMS(500);
                                                  LCD_DisplayText(2,7,"FL");           //0.5���� FL ���
                                                  BEEP_3();                                 //���� 3��
                                                  GPIOG->ODR |= 0x80;		// LED7 ON
                                                  GPIOG->ODR &=~ 0x7F;		// LED6~0T��� OFF
                                   }
                                   else if(START_FL ==DESTI_FL)     //���� ���� ����� = ������  = ��ǥ��  : 1���� ���� ó��
                                   {
                                            
                                        DelayMS(500);                                //��342�� 0.5��  + ���� 0.5�� =1�� ��
                                        Fram_Write(2023,Y1);                         //2023������ ��ǥ�� ����
                                                   
                                        LCD_SetTextColor(RGB_RED);        
                                        LCD_DisplayText(5,8,"S");                  //S ǥ��
                                        DelayMS(500);
                                        LCD_DisplayText(2,7,"FL");                //0.5���� FL ���
                                        BEEP_3();                                     //���� 3��
                                        GPIOG->ODR |= 0x80;		         // LED7 ON
                                        GPIOG->ODR &=~ 0x7F;	  	 // LED6~0T��� OFF
                                     }
                        }
                }
                else  if(abs(Y1-START_FL) > abs(Y2-START_FL))            //������� ���� ����� ���������Ͱ� ������ ���������� ���
                {
                      LCD_SetTextColor(RGB_GREEN);
                      LCD_DisplayText(4,7,"R-E");                                        //������ ���������� ���� ǥ��  
                      DelayMS(500);
                          if(Y2<START_FL)                                                   //���������Ͱ� ����� �ؿ� ��ġ : ������ < ����� 
                          {
                                    if(START_FL <DESTI_FL)                               //������ < ����� < ��ǥ��  
                                    {
                                               while(START_FL!=Y2)                         //������� �����ϸ� while�� Ż��
                                               { 
                                                        y2=y2-12;                             //��ǥ�� -12
                                                        Y2=Y2+1;                             //�� +1
                                                        ELE_R_UP();                         //������ ���� ���
                                                        DelayMS(500);                       //0.5���� 
                                                } 
                                           //������� �����ϸ�
                                                      LCD_SetTextColor(RGB_RED);    
                                                      LCD_DisplayText(5,8,"S");           // STOP ǥ��
                                                      DelayMS(500);                           //0.5���� 
                                                      LCD_DisplayText(5,8,"U");            //0.5���� UP
                                                      DelayMS(500);                          //0.5���� 
                                              while(DESTI_FL != Y2)                          //��ǥ���� �����ϸ� while�� Ż��
                                                 {   
                                                         y2=y2-12;                                //��ǥ�� -12
                                                         Y2=Y2+1;                                //�� +1
                                                         ELE_R_UP();                           //������ ���� ���
                                                         DelayMS(500);                          //0.5���� 
                                                 }
                                            //��ǥ���� �����ϸ�
                                                  Fram_Write(2024,Y2);                    //2024������ ��ǥ�� ����
                                                 LCD_SetTextColor(RGB_RED);    
                                                 LCD_DisplayText(5,8,"S");                //Stop ǥ��
                                                 DelayMS(500);                         
                                                 LCD_DisplayText(2,7,"FL");              //0.5���� FL ���
                                                 BEEP_3();                                  //���� 3��
                                                 GPIOG->ODR |= 0x80;		        // LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;		// LED6~0T��� OFF
                                   }  
                                   else if(START_FL > DESTI_FL)                    //  ������ < ����� > ��ǥ��  
                                   {
                                              while(START_FL!=Y2)                     //������� �����ϸ� while�� Ż��
                                               { 
                                                      y2=y2-12;                         //��ǥ�� -12
                                                      Y2=Y2+1;                           //�� +1
                                                      ELE_R_UP();                     //������ ���� ���
                                                      DelayMS(500);                   //0.5����                             
                                               }     
                                           //������� �����ϸ�
                                                       LCD_SetTextColor(RGB_RED);    
                                                       LCD_DisplayText(5,8,"S");                    // STOP ǥ��
                                                       DelayMS(500);                                 //0.5����
                                                       LCD_DisplayText(5,8,"U");                  // UP
                                                       DelayMS(500);                                 //0.5����
                                              while(DESTI_FL != Y2)                                       //��ǥ���� �����ϸ� while�� Ż��
                                               {
                                                        ELE_R_DOWN();                                     //������ ���� �ϰ�
                                                        DelayMS(500);                                       //0.5���� 
                                                        y2=y2+12;                                          //��ǥ�� +12
                                                        Y2=Y2-1;                                            //�� -1
                                                }
                                           //��ǥ���� �����ϸ�
                                                 Fram_Write(2024,Y2);                //2024������ ��ǥ�� ����
                                                 LCD_SetTextColor(RGB_RED);    
                                                 LCD_DisplayText(5,8,"S");               //Stop ǥ��
                                                 DelayMS(500);                         
                                                 LCD_DisplayText(2,7,"FL");              //0.5���� FL ���
                                                 BEEP_3();                                   //���� 3��
                                                 GPIOG->ODR |= 0x80;		        // LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;		// LED6~0T��� OFF
                                  }
                                  else if(START_FL == DESTI_FL)                    //������ < ����� = ��ǥ��  
                                  {
                                            while(START_FL!=Y2)                       //������� �����ϸ� while�� Ż��
                                              {
                                                    y2=y2-12;                                    //��ǥ�� -12
                                                    Y2=Y2+1;                                    //�� +1
                                                    ELE_R_UP();                               //������ ���� ���
                                                    DelayMS(500);                           //0.5���� 
                                              
                                              }
                                        //������� �����ϸ�  1���� ���� ó�� 
                                              Fram_Write(2024,Y2);                      //2024������ ��ǥ�� ����
                                              DelayMS(500);                                     //0.5���� 
                                              LCD_SetTextColor(RGB_RED);       
                                              LCD_DisplayText(5,8,"S");                      //Stop ǥ��
                                              DelayMS(500);
                                              LCD_DisplayText(2,7,"FL");                    //0.5���� FL ���
                                              BEEP_3();                                         //���� 3��
                                              GPIOG->ODR |= 0x80;		           // LED7 ON
                                              GPIOG->ODR &=~ 0x7F;		           // LED6~0T��� OFF
                                   }
                      }
                      else if(Y2>START_FL)                                //������ > �����
                      {
                                  if(START_FL <DESTI_FL)                    //������ > ����� < ��ǥ��  
                                   {
                                           while(START_FL!=Y2)                //������� �����ϸ� while�� Ż��
                                              {
                                                       ELE_R_DOWN();           //������ ���� �ϰ�
                                                       y2=y2+12;                  //��ǥ�� +12
                                                       Y2=Y2-1;                   //�� -1
                                                       DelayMS(500);   
                                                      
                                              }
                                           //������� �����ϸ�
                                                LCD_SetTextColor(RGB_RED);    
                                                LCD_DisplayText(5,8,"S");                // STOP ǥ��
                                                DelayMS(500);                               //0.5����
                                                LCD_DisplayText(5,8,"U");                // UP
                                                DelayMS(500);                               //0.5����
                                              
                                             while(DESTI_FL!=Y2)                             //��ǥ���� �����ϸ� while�� Ż��
                                              {
                                                       y2=y2-12;                                //��ǥ�� -12
                                                       Y2=Y2+1;                                 //�� +1
                                                       ELE_R_UP();                             //������ ���� ���
                                                       DelayMS(500);                             //0.5���� 
                                              }
                                        //��ǥ���� �����ϸ�
                                             Fram_Write(2024,Y2);                          //2024������ ��ǥ�� ����
                                             LCD_SetTextColor(RGB_RED);        
                                             LCD_DisplayText(5,8,"S");                  // STOP ǥ��
                                             DelayMS(500);
                                             LCD_DisplayText(2,7,"FL");                //0.5���� FL ���
                                             BEEP_3();                                      //���� 3��
                                             GPIOG->ODR |= 0x80;		        // LED7 ON
                                             GPIOG->ODR &=~ 0x7F;	          	// LED6~0T��� OFF
                                      
                                  }
                                  else  if(START_FL >DESTI_FL)                //������ > ����� > ��ǥ��  
                                  {
                                            while(START_FL!=Y2)                 //������� �����ϸ� while�� Ż��
                                            {
                                                       ELE_R_DOWN();                  //������ ���� �ϰ�
                                                       y2=y2+12;                         //��ǥ�� +12
                                                       Y2=Y2-1;                         //�� -1
                                                       DelayMS(500);   
                                                          
                                           }
                                    //������� �����ϸ�
                                              LCD_SetTextColor(RGB_RED);    
                                              LCD_DisplayText(5,8,"S");                 // STOP ǥ��
                                              DelayMS(500);                                //0.5����
                                              LCD_DisplayText(5,8,"U");                 // UP
                                              DelayMS(500);                               //0.5����    
                                                     
                                            while(DESTI_FL!=Y2)                       //��ǥ���� �����ϸ� while�� Ż��
                                            {
                                                       ELE_R_DOWN();                   //������ ���� �ϰ�
                                                       y2=y2+12;                         //��ǥ�� +12
                                                       Y2=Y2-1;                         //�� -1
                                                       DelayMS(500);
                                             }
                                       //��ǥ���� �����ϸ�
                                                 Fram_Write(2024,Y2);                          //2024������ ��ǥ�� ����
                                                 LCD_SetTextColor(RGB_RED);        
                                                 LCD_DisplayText(5,8,"S");                  //STOP ǥ��
                                                 DelayMS(500);
                                                 LCD_DisplayText(2,7,"FL");                //0.5���� FL ���
                                                 BEEP_3();                                      //���� 3��
                                                 GPIOG->ODR |= 0x80;		           // LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;	           // LED6~0T��� OFF
                                  
                                  }
                                  else  if(START_FL == DESTI_FL)     //������ > ����� = ��ǥ��  
                                  { 
                                            while(START_FL!=Y2)                   //������� �����ϸ� while�� Ż��
                                            {
                                                   ELE_R_DOWN();                   //������ ���� �ϰ�
                                                   y2=y2+12;                          //��ǥ�� +12
                                                   Y2=Y2-1;                           //�� -1
                                                   DelayMS(500);  
                                            }
                                        //������� �����ϸ� 1���� ����ó��
                                                  Fram_Write(2024,Y2);                      //2024������ ��ǥ�� ����
                                                  DelayMS(500);                               //0.5���� 
                                                   LCD_SetTextColor(RGB_RED);       
                                                   LCD_DisplayText(5,8,"S");            //STOP ǥ��
                                                   DelayMS(500);
                                                   LCD_DisplayText(2,7,"FL");           //0.5���� FL ���
                                                   BEEP_3();                                 //���� 3��
                                                   GPIOG->ODR |= 0x80;		// LED7 ON
                                                   GPIOG->ODR &=~ 0x7F;		// LED6~0T��� OFF
                                  }
                        }
                         else if(Y2==START_FL)                      //  �����=������ 
                        {
                                  if(START_FL <DESTI_FL)             // �����=������  < ��ǥ��  
                                  {
                                         DelayMS(500);                       //��342�� 0.5��  + ���� 0.5�� =1�� �� �̵� ��ǥ�� �̵�
                                            while(DESTI_FL!=Y2)         //��ǥ���� �����ϸ� while�� Ż��
                                            {
                                                     y2=y2-12;              //��ǥ�� -12
                                                     Y2=Y2+1;               //�� +1
                                                     ELE_R_UP();           //������ ���� ���
                                                     DelayMS(500);          //0.5����  
                                             }
                                       //��ǥ���� �����ϸ�   
                                                   Fram_Write(2024,Y2);                       //2024������ ��ǥ�� ����
                                            
                                                   LCD_SetTextColor(RGB_RED);        
                                                   LCD_DisplayText(5,8,"S");               //STOP ǥ��
                                                   DelayMS(500);
                                                   LCD_DisplayText(2,7,"FL");              //0.5���� FL ���
                                                   BEEP_3();                                   //���� 3��
                                                   GPIOG->ODR |= 0x80;		  // LED7 ON
                                                   GPIOG->ODR &=~ 0x7F;		  // LED6~0T��� OFF
  
                                  }
                                  else if(START_FL >DESTI_FL)     // �����=������  > ��ǥ��  
                                  {
                                           DelayMS(500);                       //��342�� 0.5��  + ���� 0.5�� =1�� �� �̵� ��ǥ�� �̵� 
                                              while(DESTI_FL!=Y2)
                                             {
                                                    ELE_R_DOWN();                   //������ ���� �ϰ�
                                                    y2=y2+12;                          //��ǥ�� +12
                                                    Y2=Y2-1;                           //�� -1
                                                    DelayMS(500);  
                                              }
                                   //��ǥ���� �����ϸ�          
                                                     Fram_Write(2024,Y2);                         //2024������ ��ǥ�� ����
                           
                                                      LCD_SetTextColor(RGB_RED);       
                                                      LCD_DisplayText(5,8,"S");                     //STOP ǥ��
                                                      DelayMS(500);
                                                      LCD_DisplayText(2,7,"FL");                  //0.5���� FL ���
                                                      BEEP_3();                                       //���� 3��
                                                      GPIOG->ODR |= 0x80;		          // LED7 ON
                                                      GPIOG->ODR &=~ 0x7F;		         // LED6~0T��� OFF
                                   }
                                    else if(START_FL ==DESTI_FL)     // �����=������  = ��ǥ��  : 1���� ���� ó��
                                   {
                                                DelayMS(500);                       //��342�� 0.5��  + ���� 0.5�� =1�� �� ���� ó��
                                                Fram_Write(2024,Y2);               //2024������ ��ǥ�� ����
                                                
                                                 LCD_SetTextColor(RGB_RED);       
                                                 LCD_DisplayText(5,8,"S");          //STOP ǥ��
                                                 DelayMS(500);
                                                 LCD_DisplayText(2,7,"FL");         //0.5���� FL ���
                                                 BEEP_3();                              //���� 3��
                                                 GPIOG->ODR |= 0x80;		   // LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;	   // LED6~0T��� OFF
                                   }
                        }
            }//������ ���������Ϳ� ���� if��  
              
               EXTI->PR |= 0x0100;		// Pending bit Clear 
               EXTI->IMR   &=~ 0x00008000;	// EXTI15- ���ͷ�Ʈ disable , ������ �߿����� �ߴܸ�带 �����ϱ� ����
	}
	
}

void EXTI15_10_IRQHandler(void)
{	
          
            if( EXTI->PR & 0x8000)			// EXTI15 Interrupt Pending(�߻�) ����?
              {
         
               EXTI->PR |= 0x8000;		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)           
              LCD_SetTextColor(RGB_RED);     
              LCD_DisplayText(2,7,"HD");        //�ߴܸ�� HD ǥ��
               
         
               GPIOG->ODR &= ~0x0081;	// LED7 , LED0  OFF
               GPIOG->ODR |= 0x0040;	// LED6  ON
              
                GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
                DelayMS(500);		// Delay   500ms
                GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
                DelayMS(500);		// Delay   500ms
                GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
                DelayMS(500);		// Delay   500ms
                GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
                DelayMS(500);		// Delay   500ms
                 GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
                DelayMS(500);		// Delay   500ms
                GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
                DelayMS(500);		// Delay   500ms
                 GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
                DelayMS(500);		// Delay   500ms
                GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
                DelayMS(500);		// Delay   500ms
                 GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
                DelayMS(500);		// Delay   500ms
                GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
                DelayMS(500);		// Delay   500ms
                
                LCD_DisplayText(2,7,"EX");        //��� EX�� ����
                        
                GPIOG->ODR &= ~0x00C0;	// LED7 , LED6  OFF
                GPIOG->ODR |= 0x0001;	// LED0  ON
                
                EXTI->PR |= 0x8000;        // Pending bit Clear
                       
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



void ELE_L_UP(void)        //���� ���� �ö�
{
       LCD_SetPenColor(RGB_BLUE);       
       LCD_DrawRectangle(20, y1, 10, 12);
       LCD_SetBrushColor(RGB_BLUE);
       LCD_DrawFillRect(20, y1, 10, 12);
}
void ELE_L_DOWN(void) //���� ���� ������
{
       LCD_SetPenColor(RGB_WHITE);         
       LCD_DrawRectangle(20, y1, 10, 12);
       LCD_SetBrushColor(RGB_WHITE);
       LCD_DrawFillRect(20, y1, 10, 12);
}
void ELE_R_UP(void) //������ ���� �ö�
{
      LCD_SetPenColor(RGB_GREEN);         
      LCD_DrawRectangle(110, y2, 10, 12);
      LCD_SetBrushColor(RGB_GREEN);
      LCD_DrawFillRect(110, y2, 10, 12);
}
void ELE_R_DOWN(void) //������ ���� ������
{
      LCD_SetPenColor(RGB_WHITE);         
      LCD_DrawRectangle(110, y2, 10, 12);
      LCD_SetBrushColor(RGB_WHITE);
      LCD_DrawFillRect(110, y2, 10, 12);
}