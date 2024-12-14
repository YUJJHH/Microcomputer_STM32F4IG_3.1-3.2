///////////////////////////////////////////////////////////////////////////////////
//      ������ :  TP2. ���� �����
//      ��������:  ����(2��Ʈ) ������ �ϴ� ���� ���α׷� ����
//      ����� �ϵ����(���) : GPIO, EXTI, GLCD , FRAM, JoyStick
//      ������ :    2023.6.09    
//      ������ Ŭ���� :    ����Ϲ�
//                    �й�:      2019130027 
//                    �̸�:       ����ȯ
//////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"
#include "GLCD.h"
#include "FRAM.h"

void _GPIO_Init(void);
void _EXTI_Init(void);

void DisplayInitScreen(void);
uint16_t KEY_Scan(void);
void BEEP(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint8_t	 SW2_Flag, SW3_Flag, SW4_Flag, SW5_Flag;

int Aopnd1, Aopnd2, Bopnd1, Bopnd2;  
 //A�� MSB  ,LSB ,   B�� MSB,    LSB

int AopndSUM, BopndSUM;
//AopndSUM = Aopnd1 + Aopnd2
//BopndSUM = Bopnd1 + Bopnd2

unsigned char Operator='+'; //������ ����

int Cresult; //�����

int Cre1, Cre2, Cre3, Cre4;
//C�� MSB����    ~  LSB

int ExoFlag; //���Ӹ�� Ż�� ���

int main(void)
{
	LCD_Init();	// LCD ��� �ʱ�ȭ
	DelayMS(10);
	_GPIO_Init();	// GPIO (LED & SW) �ʱ�ȭ
	_EXTI_Init();	// EXTI �ʱ�ȭ

        Fram_Init();                    // FRAM �ʱ�ȭ H/W �ʱ�ȭ
	Fram_Status_Config();   // FRAM �ʱ�ȭ S/W �ʱ�ȭ
 
        
        DisplayInitScreen();	// LCD �ʱ�ȭ��

	GPIOG->ODR &= ~0x00FF;		// �ʱⰪ: LED0~7 Off
   
        Operator=Fram_Read(530);  //������ ���尪 ����
        LCD_SetFont(&Gulim10);		// ��Ʈ : ���� 10
        LCD_SetBackColor(GET_RGB(255,192,203));	// ���ڹ��� :��ȫ��
        LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
       if(Operator=='+')
        {
            LCD_DisplayChar(2,10,'+');
        }
        else  if(Operator=='-')
        {
            LCD_DisplayChar(2,10,'-');
        }
         else  if(Operator=='x')
        {
            LCD_DisplayChar(2,10,'x');
        }
        else  if(Operator=='&')
        {
            LCD_DisplayChar(2,10,'&');
        }
        else  if(Operator=='|')
        {
            LCD_DisplayChar(2,10,'|');
        }
        else  if(Operator=='^')
        {
            LCD_DisplayChar(2,10,'^');
        }
       
        Cresult=Fram_Read(531);  //����� ���尪 ����
        LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
        LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
        LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
        switch (Cresult)
                        {
                         case 0:    //��� = 0 �̸�
                                Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                                LCD_DisplayChar(2,15,Cre1+0x30);              //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 1:    //��� = 1 �̸�
                                Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 2:    //��� = 2 �̸�
                                Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                                LCD_DisplayChar(2,15,Cre1+0x30);  
                                LCD_DisplayChar(4,15,Cre2+0x30);             //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 3:    //��� = 3 �̸�
                                Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 4:    //��� = 4 �̸�
                                Cre1=0;    Cre2=1;   Cre3=0;    Cre4=0;     //0100
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 5:    //��� = 5 �̸�
                                Cre1=0;    Cre2=1;   Cre3=0;    Cre4=1;     //0101
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 6:    //��� = 6 �̸�
                                Cre1=0;    Cre2=1;   Cre3=1;    Cre4=0;     //0110
                                LCD_DisplayChar(2,15,Cre1+0x30);             //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB  
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 7:    //��� = 7 �̸�
                                Cre1=0;    Cre2=1;   Cre3=1;    Cre4=1;     //0111
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 8:    //��� = 8 �̸�
                                Cre1=1;    Cre2=0;   Cre3=0;    Cre4=0;     //1000
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 9:    //��� = 9 �̸�
                                Cre1=1;    Cre2=0;   Cre3=0;    Cre4=1;     //1001
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                        case 10:    //��� = 10 �̸�
                                Cre1=1;    Cre2=0;   Cre3=1;    Cre4=0;     //1010
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 11:    //��� = 11 �̸�
                                Cre1=1;    Cre2=0;   Cre3=1;    Cre4=1;     //1011
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 12:    //��� = 12 �̸�
                                Cre1=1;    Cre2=1;   Cre3=0;    Cre4=0;     //1100
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                             case 13:    //��� = 13 �̸�
                                Cre1=1;    Cre2=1;   Cre3=0;    Cre4=1;     //1101
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                             case 14:    //��� = 14 �̸�
                                Cre1=1;    Cre2=1;   Cre3=1;    Cre4=0;     //1110
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                             case 15:    //��� = 15 �̸�
                                Cre1=1;    Cre2=1;   Cre3=1;    Cre4=1;     //1111
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                        }
        
	while(1)
	{
		switch(KEY_Scan())  
		{
                      case 0xFB00:              //SW2  ON
                         if(SW2_Flag==0)        
                         { 
                              BEEP();             //���� 1ȸ
                              Aopnd1=1;        //1����
                              LCD_SetFont(&Gulim8);		       // ��Ʈ : ���� 8      
                              LCD_SetTextColor(RGB_BLACK);        //���ڻ� :����
                              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                              LCD_DisplayChar(1,4,Aopnd1+0x30);   //LCD�� 1ǥ��
                    
                              SW2_Flag=1;
                          }
                          else
                          {  
                              BEEP();              //���� 1ȸ
                              Aopnd1=0;         //0����
                              LCD_SetFont(&Gulim8);		       // ��Ʈ : ���� 8      
                              LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
                              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                              LCD_DisplayChar(1,4,Aopnd1+0x30);  //LCD�� 0ǥ��
                    
                              SW2_Flag=0;
                          }
                          break;
                    case 0xF700:                 //SW3  ON
                        if(SW3_Flag==0)        
                        { 
                              BEEP();              //���� 1ȸ
                              Aopnd2=1;             //1����
                              LCD_SetFont(&Gulim8);		       // ��Ʈ : ���� 8      
                              LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
                              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                              LCD_DisplayChar(3,4,Aopnd2+0x30);  //LCD�� 1ǥ��
                      
                              SW3_Flag=1;
                              
                          }
                          else
                          {   
                              BEEP();              //���� 1ȸ
                              Aopnd2=0;            //0����
                              LCD_SetFont(&Gulim8);		       // ��Ʈ : ���� 8      
                              LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
                              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                              LCD_DisplayChar(3,4,Aopnd2+0x30);  //LCD�� 0ǥ��
                      
                              SW3_Flag=0;
                          }
                        break;
                    case 0xEF00:              //SW4 ON
                          if(SW4_Flag==0)        
                          { 
                              BEEP();             //���� 1ȸ
                              Bopnd1=1;         //1����
                              LCD_SetFont(&Gulim8);		       // ��Ʈ : ���� 8      
                              LCD_SetTextColor(RGB_BLACK);        //���ڻ� :����
                              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                              LCD_DisplayChar(6,4,Bopnd1+0x30);   //LCD�� 1ǥ��
                      
                              SW4_Flag=1;
                          }
                          else
                          {  
                              BEEP();              //���� 1ȸ 
                              Bopnd1=0;            //0����
                              LCD_SetFont(&Gulim8);		       // ��Ʈ : ���� 8      
                              LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
                              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                              LCD_DisplayChar(6,4,Bopnd1+0x30);    //LCD�� 0ǥ��
                      
                              SW4_Flag=0;
                          }
                        break;
                    case 0xDF00:                 //SW5  ON
                        if(SW5_Flag==0)        
                          { 
                              BEEP();              //���� 1ȸ
                              Bopnd2=1;          //1����
                              LCD_SetFont(&Gulim8);		       // ��Ʈ : ���� 8      
                              LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
                              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                              LCD_DisplayChar(8,4,Bopnd2+0x30);   //LCD�� 1ǥ��
                      
                              SW5_Flag=1;
                         }
                         else
                         {   
                              BEEP();              //���� 1ȸ
                              Bopnd2=0;        //0����
                              LCD_SetFont(&Gulim8);		       // ��Ʈ : ���� 8      
                              LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
                              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                              LCD_DisplayChar(8,4,Bopnd2+0x30);   //LCD�� 0ǥ��
                      
                              SW5_Flag=0;
                          }
                        break;
                 }
                AopndSUM = 2*Aopnd1 + Aopnd2;     //A,B�� MSB�� 2^1 �̹Ƿ� Aopnd1�� Bopnd1�� *2 ��
                BopndSUM = 2*Bopnd1 + Bopnd2;
           
              
	}
}

/* GLCD �ʱ�ȭ�� ���� �Լ� */
void DisplayInitScreen(void)
{
	LCD_Clear(RGB_WHITE);		// ȭ�� Ŭ����
	LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
	LCD_SetBackColor(RGB_WHITE);	// ���ڹ��� : ���
        LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����

//��� �κ�
	LCD_SetPenColor(RGB_GREEN);
        LCD_DrawRectangle(55,8,48,106);  //��� ū �ʷϻ� �簢��
        LCD_SetPenColor(RGB_BLACK);
        LCD_DrawRectangle(72,31,20,20);  //��� ������ �ִ� �簢��
       LCD_SetBrushColor(GET_RGB(255,192,203)); // ��ȫ��
        LCD_DrawFillRect(73,32,19,19);   //��� ������ �ִ� �簢�� ����

        LCD_DrawRectangle(70,86,20,17);  //���� ���� ��� �簢��
        LCD_SetBrushColor(RGB_YELLOW);
        LCD_DrawFillRect(71,87,19,16);   //���� ���� ��� �簢�� ����

        LCD_SetFont(&Gulim10);		// ��Ʈ : ���� 10
        LCD_SetBackColor(GET_RGB(255,192,203));	// ���ڹ��� :��ȫ��
        LCD_DisplayChar(2,10,Operator);          //������ ǥ��

        LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
	LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����
        LCD_DisplayText(7,9,"+0");

//���� �κ�
        LCD_DrawRectangle(28,11,17,17);  //A�� �簢��
        LCD_DrawRectangle(28,36,17,17);  //A��  �簢��
        LCD_DrawRectangle(28,76,17,17);  //B�� �簢��
        LCD_DrawRectangle(28,99,17,17);  //B��  �簢��
        LCD_SetBrushColor(RGB_YELLOW);
        LCD_DrawFillRect(29,12,16,16); //A�� �簢��
        LCD_DrawFillRect(29,37,16,16);  //A�� �簢��
        LCD_DrawFillRect(29,77,16,16);  //B�� �簢��
        LCD_DrawFillRect(29,100,16,16);   //B�� �簢��
        LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8      
        LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
        LCD_DisplayChar(1,4,Aopnd1+0x30);
        LCD_DisplayChar(3,4,Aopnd2+0x30);
        LCD_DisplayChar(6,4,Bopnd1+0x30);
        LCD_DisplayChar(8,4,Bopnd2+0x30);
        
        LCD_SetPenColor(RGB_GREEN);
        LCD_DrawRectangle(3,16,20,20);  //A �簢��  
        LCD_DrawRectangle(3,82,20,20);  //B �簢��   
        LCD_SetFont(&Gulim10);		// ��Ʈ : ���� 10
        LCD_SetBackColor(RGB_WHITE);	// ���ڹ��� : ���
        LCD_DisplayChar(1,1,'A');
        LCD_DisplayChar(5,1,'B');

        LCD_SetPenColor(RGB_BLUE);
        LCD_DrawHorLine(28+18, 21, 9);  //���� ����簢�� 4���� ū �簢��  ���ἱ
        LCD_DrawHorLine(28+18, 45, 9);
        LCD_DrawHorLine(28+18, 84, 9);
        LCD_DrawHorLine(28+18,108, 9);

//������ �κ�
        LCD_SetBrushColor(RGB_YELLOW);
        LCD_DrawFillRect(116,1,16,16);  //C��  �簢�� ��ȣ 
        LCD_DrawFillRect(116,24,16,15);  //C�� �簢�� 1
        LCD_DrawFillRect(116,49,16,16);  //C��  �簢�� 2
        LCD_DrawFillRect(116,77,16,16);  //C�� �簢�� 3
        LCD_DrawFillRect(116,100,16,16);  //C��  �簢�� 4

        LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
        LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
        LCD_DisplayChar(0,15,'+');
        LCD_DisplayChar(2,15,Cre1+0x30);  
        LCD_DisplayChar(4,15,Cre2+0x30);
        LCD_DisplayChar(6,15,Cre3+0x30);
        LCD_DisplayChar(8,15,Cre4+0x30);
     
        LCD_SetPenColor(RGB_BLACK);
        LCD_DrawRectangle(115,0,17,17);  //C��  �簢�� ��ȣ 
        LCD_DrawRectangle(115,23,17,17);  //C�� �簢�� 1
        LCD_DrawRectangle(115,48,17,17);  //C��  �簢�� 2
        LCD_DrawRectangle(115,76,17,17);  //C�� �簢�� 3
        LCD_DrawRectangle(115,99,17,17);  //C��  �簢�� 4
       
        LCD_SetPenColor(RGB_BLUE);
        LCD_DrawHorLine(104, 9, 11);  //������ ����簢�� 5���� ū �簢��  ���ἱ
        LCD_DrawHorLine(104, 33, 11);
        LCD_DrawHorLine(104, 55,11);
        LCD_DrawHorLine(104,84, 11);
        LCD_DrawHorLine(104,108, 11);

        LCD_SetFont(&Gulim10);		// ��Ʈ : ���� 10
        LCD_SetBackColor(RGB_WHITE);	// ���ڹ��� : ���
        LCD_DisplayChar(3,18,'C');

        LCD_SetPenColor(RGB_GREEN);
        LCD_DrawRectangle(137,49,20,20);  //C �簢��  

     

}

/* GPIO (GPIOG(LED), GPIOH(Switch), GPIOF(Buzzer)) �ʱ� ����	*/
void _GPIO_Init(void)
{
	// LED (GPIO G) ����
    	RCC->AHB1ENR	|=  0x00000040;	// RCC_AHB1ENR : GPIOG(bit#6) Enable							
	GPIOG->MODER 	|=  0x00005555;	// GPIOG 0~7 : Output mode (0b01)						
	GPIOG->OTYPER	&= ~0x00FF;	// GPIOG 0~7 : Push-pull  (GP8~15:reset state)	
 	GPIOG->OSPEEDR 	|=  0x00005555;	// GPIOG 0~7 : Output speed 25MHZ Medium speed 
   
	// SW (GPIO H) ���� 
	RCC->AHB1ENR    |=  0x00000080;	// RCC_AHB1ENR : GPIOH(bit#7) Enable							
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH 8~15 : Input mode (reset state)				
	GPIOH->PUPDR 	&= ~0xFFFF0000;	// GPIOH 8~15 : Floating input (No Pull-up, pull-down) :reset state

	// Buzzer (GPIO F) ���� 
    	RCC->AHB1ENR	|=  0x00000020; // RCC_AHB1ENR : GPIOF(bit#5) Enable							
	GPIOF->MODER 	|=  0x00040000;	// GPIOF 9 : Output mode (0b01)						
	GPIOF->OTYPER 	&= ~0x0200;	// GPIOF 9 : Push-pull  	
 	GPIOF->OSPEEDR 	|=  0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 
        
        //Joy Stick SW(PORT I) ����
	RCC->AHB1ENR	|= 0x00000100;	// RCC_AHB1ENR GPIOI 8�� ��Ʈ Enable
	GPIOI->MODER	&=~0x000FFC00;	// GPIOI 5~9 : Input mode (reset state)
        GPIOI->PUPDR	&= ~0x000FFC00;	// GPIOI 5~9 : Floating input (No Pull-up, pull-down) (reset state)
}	

/* EXTI (EXTI8(GPIOH.8, SW0), EXTI9(GPIOH.9, SW1)) �ʱ� ����  */
void _EXTI_Init(void)
{
    	RCC->AHB1ENR 	|= 0x00000080;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= 0x00004000;	// Enable System Configuration Controller Clock
	
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH PIN8~PIN15 Input mode (reset state)			
        	      	
// EXTI9, 14 ����     , PH9, PH14
	SYSCFG->EXTICR[2] |= 0x0070; 	// EXTI9�� ���� �ҽ� �Է��� GPIOH�� ����
	SYSCFG->EXTICR[3] |= 0x0700; 	// EXTI14�� ���� �ҽ� �Է��� GPIOH�� ����

	EXTI->FTSR |= 0x004200;		// EXTI9,14: Falling Trigger Enable
 	EXTI->IMR  |= 0x004200;  	// EXTI9,14 ���ͷ�Ʈ mask (Interrupt Enable) ����
		
	NVIC->ISER[0] |= (1 << 23);	   // Enable 'Global Interrupt EXTI9'
        NVIC->ISER[1] |= (1 << (40-32));	 // Enable 'Global Interrupt EXTI14'
     
    //���̽�ƽ  ��  PI6,  ����Ʈ  PI8        , EXTI6, EXTI8
      
        RCC->AHB1ENR 	|= 0x00000100;	  // RCC_AHB1ENR GPIOI Enable
	RCC->APB2ENR 	|= 0x00004000;	  // Enable System Configuration Controller Clock
			
	GPIOI->MODER 	&= ~ 0x00033000;	// GPIOI PIN6, PIN8 Input mode (reset state)		 
        SYSCFG->EXTICR[1] |= 0x0800;	       //EXTI6�� ����
        SYSCFG->EXTICR[2] |= 0x0008;        //EXTI8�� ����
        
        EXTI->FTSR |=  0x00000140 ;// EXTI6,8: Falling Trigger Enable 

	EXTI->IMR   |= 0x00000140 ;	//EXTI6,8 : ���ͷ�Ʈ mask (Interrupt Enable) ����
	NVIC->ISER[0] |= ( 1<<23  );     // Enable 'Global Interrupt EXTI6, EXTI8'
        
        NVIC->IP[23]= 0xF0;  // EXTI9_5 ,  Ligh Priority     EXTI6���� ����   
	NVIC->IP[40]= 0xE0;  // EXTI15_10    How Priority    EXTI14���� ���� , �� ���� �켱����
       
}

/* EXTI5~9 ISR */
void EXTI9_5_IRQHandler(void)		
{
      if(EXTI->PR & 0x0100)                   // EXTI8 Interrupt Pending(�߻�) ����?  ,PI8
	{
		EXTI->PR |= 0x0100; 		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
		
                BEEP();
                LCD_SetFont(&Gulim10);		// ��Ʈ : ���� 10
                LCD_SetTextColor(RGB_BLACK);      // ���ڹ��� :����
                LCD_SetBackColor(GET_RGB(255,192,203));	// ���ڹ��� :��ȫ��
                if(Operator=='+')
                {
                      Operator='-';
                      LCD_DisplayChar(2,10,Operator); 
                      Fram_Write(530,Operator);         //������ ����
                }
                 else if(Operator=='-')
                {
                      Operator='x';
                      LCD_DisplayChar(2,10,Operator);    
                      Fram_Write(530,Operator);         //������ ����
                }
		else if(Operator=='x')
                {
                      Operator='&';
                      LCD_DisplayChar(2,10,Operator);
                      Fram_Write(530,Operator);         //������ ����
                }
                else if(Operator=='&')
                {
                      Operator='|';
                      LCD_DisplayChar(2,10,Operator);
                      Fram_Write(530,Operator);         //������ ����
                }
                else if(Operator=='|')
                {
                      Operator='^';
                      LCD_DisplayChar(2,10,Operator);
                      Fram_Write(530,Operator);         //������ ����
                }
                else if(Operator=='^')
                {
                      Operator='+';
                      LCD_DisplayChar(2,10,Operator);
                      Fram_Write(530,Operator);         //������ ����
                }
                EXTI->PR |= 0x0100; 		// Pending bit Clear
	}
        if(EXTI->PR & 0x0200)                   // EXTI9 Interrupt Pending(�߻�) ����?  , PH9, SW1 
	{
		EXTI->PR |= 0x0200; 
                BEEP();      //���� 1ȸ
                if(Operator=='+')   //�����ڰ� + �� ���
                {
                      LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                      LCD_SetTextColor(RGB_BLACK);      // ���ڹ��� :����
                      LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C��  ��ȣ �簢��
                      
                      Cresult = AopndSUM + BopndSUM;   //���� : �ּڰ�:0 ,�ִ�:6
                      Fram_Write(531,Cresult);         //����� ����
                      switch (Cresult)
                      {
                       case 0:    //��� = 0 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //��� = 1 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //��� = 2 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //��� = 3 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 4:    //��� = 4 �̸�
                              Cre1=0;    Cre2=1;   Cre3=0;    Cre4=0;     //0100
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 5:    //��� = 5 �̸�
                              Cre1=0;    Cre2=1;   Cre3=0;    Cre4=1;     //0101
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 6:    //��� = 6 �̸�
                              Cre1=0;    Cre2=1;   Cre3=1;    Cre4=0;     //0110
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }
                else if(Operator=='-')   //�����ڰ� -�� ���
                {
                      LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                      LCD_SetTextColor(RGB_BLACK);      // ���ڹ��� :����
                      LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
            
                         if(AopndSUM >= BopndSUM)         //A�� �� Ŭ ��� , A=B �� ���
                          {
                              LCD_DisplayChar(0,15,'+');
                              LCD_SetPenColor(RGB_BLACK);
                              LCD_DrawRectangle(115,0,17,17);  //C��  ��ȣ �簢��
                              
                              Cresult = AopndSUM - BopndSUM;   // : �ּڰ�:0 ,�ִ�:3
                              Fram_Write(531,Cresult);         //����� ����
                              switch (Cresult)
                              {
                               case 0:    //��� = 0 �̸�
                                      Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 1:    //��� = 1 �̸�
                                      Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 2:    //��� = 2 �̸�
                                      Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 3:    //��� = 3 �̸�
                                      Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                                }
                          }
                         else if(AopndSUM < BopndSUM)         //B�� �� Ŭ ���
                          {
                              LCD_DisplayChar(0,15,'-');
                              LCD_SetPenColor(RGB_BLACK);
                              LCD_DrawRectangle(115,0,17,17);  //C��  ��ȣ �簢��
                              
                              Cresult = BopndSUM - AopndSUM;   // : �ּڰ�:0 ,�ִ�:3
                              Fram_Write(531,Cresult);         //����� ����
                              switch (Cresult)
                              {
                               case 0:    //��� = 0 �̸�
                                      Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 1:    //��� = 1 �̸�
                                      Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 2:    //��� = 2 �̸�
                                      Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 3:    //��� = 3 �̸�
                                      Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                                }
                          }
                }
                else if(Operator=='x')   //�����ڰ� x �� ���
                {
                      LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                      LCD_SetTextColor(RGB_BLACK);      // ���ڹ��� :����
                      LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C��  ��ȣ �簢��
                      
                      Cresult = AopndSUM * BopndSUM;   //  �ּڰ�:0 ,�ִ�:9
                      Fram_Write(531,Cresult);         //����� ����
                      switch (Cresult)
                      {
                       case 0:    //��� = 0 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //��� = 1 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //��� = 2 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //��� = 3 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 4:    //��� = 4 �̸�
                              Cre1=0;    Cre2=1;   Cre3=0;    Cre4=0;     //0100
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 5:    //��� = 5 �̸�
                              Cre1=0;    Cre2=1;   Cre3=0;    Cre4=1;     //0101
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 6:    //��� = 6 �̸�
                              Cre1=0;    Cre2=1;   Cre3=1;    Cre4=0;     //0110
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 7:    //��� = 7 �̸�
                              Cre1=0;    Cre2=1;   Cre3=1;    Cre4=1;     //0111
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 8:    //��� = 8 �̸�
                              Cre1=1;    Cre2=0;   Cre3=0;    Cre4=0;     //1000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 9:    //��� = 9 �̸�
                              Cre1=1;    Cre2=0;   Cre3=0;    Cre4=1;     //1001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }
                 else if(Operator=='&')   //�����ڰ� & �� ���
                {
                      LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                      LCD_SetTextColor(RGB_BLACK);      // ���ڹ��� :����
                      LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C��  ��ȣ �簢��
                      
                      Cresult = AopndSUM & BopndSUM;   //  �ּڰ�:0 ,�ִ�:3
                      Fram_Write(531,Cresult);         //����� ����
                      switch (Cresult)
                      {
                       case 0:    //��� = 0 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //��� = 1 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //��� = 2 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //��� = 3 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }
               else if(Operator=='|')   //�����ڰ� | �� ���
                {
                      LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                      LCD_SetTextColor(RGB_BLACK);      // ���ڹ��� :����
                      LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C��  ��ȣ �簢��
                      
                      Cresult = AopndSUM | BopndSUM;   //  �ּڰ�:0 ,�ִ�:3
                      Fram_Write(531,Cresult);         //����� ����
                      switch (Cresult)
                      {
                       case 0:    //��� = 0 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //��� = 1 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //��� = 2 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //��� = 3 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }
                else if(Operator=='^')   //�����ڰ� ^ �� ���
                {
                      LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����  
                      LCD_SetTextColor(RGB_BLACK);      // ���ڹ��� :����
                      LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C��  ��ȣ �簢��
                      
                      Cresult = AopndSUM ^ BopndSUM;   //  �ּڰ�:0 ,�ִ�:3
                      Fram_Write(531,Cresult);         //����� ����
                      switch (Cresult)
                      {
                       case 0:    //��� = 0 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //��� = 1 �̸�
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //��� = 2 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //��� = 3 �̸�
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }EXTI->PR |= 0x0200; 	// Pending bit Clear
        }
        if(EXTI->PR & 0x0040)                   // EXTI6 Interrupt Pending(�߻�) ����?  ,PI6: �����������
	 {
              EXTI->PR |= 0x0040; 		// Pending bit Clear (clear�� ���ϸ� ���ͷ�Ʈ ������ �ٽ� ���ͷ�Ʈ �߻�)
              LCD_SetTextColor(RGB_BLACK);       //���ڻ� :����
              LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
              LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : �����
              LCD_DisplayText(7,9,"+1");             //+1ǥ��
              BEEP();                //���� 1ȸ
              GPIOG->ODR |= 0x0080;         //LED7 on
              
              ExoFlag=0;
              while(ExoFlag== 0)
              {
                  Cresult++;     // ��� 1�� ����
                  
                  if(Cresult==16)      //���=16 �̸� 0����
                  {
                    Cresult=0;
                  }
                  Fram_Write(531,Cresult);         //����� ����
                     switch (Cresult)
                        {
                         case 0:    //��� = 0 �̸�
                                Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 1:    //��� = 1 �̸�
                                Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 2:    //��� = 2 �̸�
                                Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 3:    //��� = 3 �̸�
                                Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 4:    //��� = 4 �̸�
                                Cre1=0;    Cre2=1;   Cre3=0;    Cre4=0;     //0100
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 5:    //��� = 5 �̸�
                                Cre1=0;    Cre2=1;   Cre3=0;    Cre4=1;     //0101
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 6:    //��� = 6 �̸�
                                Cre1=0;    Cre2=1;   Cre3=1;    Cre4=0;     //0110
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 7:    //��� = 7 �̸�
                                Cre1=0;    Cre2=1;   Cre3=1;    Cre4=1;     //0111
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 8:    //��� = 8 �̸�
                                Cre1=1;    Cre2=0;   Cre3=0;    Cre4=0;     //1000
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 9:    //��� = 9 �̸�
                                Cre1=1;    Cre2=0;   Cre3=0;    Cre4=1;     //1001
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                        case 10:    //��� = 10 �̸�
                                Cre1=1;    Cre2=0;   Cre3=1;    Cre4=0;     //1010
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 11:    //��� = 11 �̸�
                                Cre1=1;    Cre2=0;   Cre3=1;    Cre4=1;     //1011
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 12:    //��� = 12 �̸�
                                Cre1=1;    Cre2=1;   Cre3=0;    Cre4=0;     //1100
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 13:    //��� = 13 �̸�
                                Cre1=1;    Cre2=1;   Cre3=0;    Cre4=1;     //1101
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 14:    //��� = 14 �̸�
                                Cre1=1;    Cre2=1;   Cre3=1;    Cre4=0;     //1110
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 15:    //��� = 15 �̸�
                                Cre1=1;    Cre2=1;   Cre3=1;    Cre4=1;     //1111
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD�� ����� ǥ��  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                          break;
                        }
                  DelayMS(500);
              } 
              //while�� Ż��
              LCD_DisplayText(7,9,"+0");             //+0ǥ��
              BEEP();                //���� 3ȸ
              DelayMS(50);
              BEEP();                
              DelayMS(50);
              BEEP();                
              GPIOG->ODR &= ~0x0080;         //LED7 off
                EXTI->PR |= 0x0040; 		// Pending bit Clear
        }
}

void EXTI15_10_IRQHandler(void)		
{
	if(EXTI->PR & 0x4000)                   // EXTI14 Interrupt Pending(�߻�) ����?  ,���� ���� ��� Ż��
	{
            	
            BEEP();                //����1ȸ
            DelayMS(1000);     //1����
            ExoFlag=1;  	   // Ż�� flag ����
            EXTI->PR |= 0x4000;       // Pending bit Clear
	}
}
                
/* Switch�� �ԷµǾ����� ���ο� � switch�� �ԷµǾ������� ������ return�ϴ� �Լ�  */ 
uint8_t key_flag = 0;
uint16_t KEY_Scan(void)	// input key SW0 - SW7 
{ 
	uint16_t key;
	key = GPIOH->IDR & 0xFF00;	// any key pressed ?
	if(key == 0xFF00)		// if no key, check key off
	{  	if(key_flag == 0)
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
        DelayUS(1000);         		// 1000us => 1ms
}

void DelayUS(unsigned short wUS)
{
    volatile int Dly = (int)wUS*17;
    for(; Dly; Dly--);
}
