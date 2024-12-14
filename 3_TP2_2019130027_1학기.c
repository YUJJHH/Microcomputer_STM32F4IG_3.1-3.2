///////////////////////////////////////////////////////////////////////////////////
//      과제명 :  TP2. 이진 연산기
//      과제개요:  이진(2비트) 연산을 하는 계산기 프로그램 제작
//      사용한 하드웨어(기능) : GPIO, EXTI, GLCD , FRAM, JoyStick
//      제출일 :    2023.6.09    
//      제출자 클래스 :    목요일반
//                    학번:      2019130027 
//                    이름:       유정환
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
 //A의 MSB  ,LSB ,   B의 MSB,    LSB

int AopndSUM, BopndSUM;
//AopndSUM = Aopnd1 + Aopnd2
//BopndSUM = Bopnd1 + Bopnd2

unsigned char Operator='+'; //연산자 변수

int Cresult; //결과값

int Cre1, Cre2, Cre3, Cre4;
//C의 MSB부터    ~  LSB

int ExoFlag; //연속모드 탈출 명령

int main(void)
{
	LCD_Init();	// LCD 모듈 초기화
	DelayMS(10);
	_GPIO_Init();	// GPIO (LED & SW) 초기화
	_EXTI_Init();	// EXTI 초기화

        Fram_Init();                    // FRAM 초기화 H/W 초기화
	Fram_Status_Config();   // FRAM 초기화 S/W 초기화
 
        
        DisplayInitScreen();	// LCD 초기화면

	GPIOG->ODR &= ~0x00FF;		// 초기값: LED0~7 Off
   
        Operator=Fram_Read(530);  //연산자 저장값 리드
        LCD_SetFont(&Gulim10);		// 폰트 : 굴림 10
        LCD_SetBackColor(GET_RGB(255,192,203));	// 글자배경색 :분홍색
        LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
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
       
        Cresult=Fram_Read(531);  //결과값 저장값 리드
        LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
        LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
        LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
        switch (Cresult)
                        {
                         case 0:    //결과 = 0 이면
                                Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                                LCD_DisplayChar(2,15,Cre1+0x30);              //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 1:    //결과 = 1 이면
                                Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 2:    //결과 = 2 이면
                                Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                                LCD_DisplayChar(2,15,Cre1+0x30);  
                                LCD_DisplayChar(4,15,Cre2+0x30);             //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 3:    //결과 = 3 이면
                                Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 4:    //결과 = 4 이면
                                Cre1=0;    Cre2=1;   Cre3=0;    Cre4=0;     //0100
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 5:    //결과 = 5 이면
                                Cre1=0;    Cre2=1;   Cre3=0;    Cre4=1;     //0101
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 6:    //결과 = 6 이면
                                Cre1=0;    Cre2=1;   Cre3=1;    Cre4=0;     //0110
                                LCD_DisplayChar(2,15,Cre1+0x30);             //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB  
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 7:    //결과 = 7 이면
                                Cre1=0;    Cre2=1;   Cre3=1;    Cre4=1;     //0111
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 8:    //결과 = 8 이면
                                Cre1=1;    Cre2=0;   Cre3=0;    Cre4=0;     //1000
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 9:    //결과 = 9 이면
                                Cre1=1;    Cre2=0;   Cre3=0;    Cre4=1;     //1001
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                        case 10:    //결과 = 10 이면
                                Cre1=1;    Cre2=0;   Cre3=1;    Cre4=0;     //1010
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 11:    //결과 = 11 이면
                                Cre1=1;    Cre2=0;   Cre3=1;    Cre4=1;     //1011
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 12:    //결과 = 12 이면
                                Cre1=1;    Cre2=1;   Cre3=0;    Cre4=0;     //1100
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                             case 13:    //결과 = 13 이면
                                Cre1=1;    Cre2=1;   Cre3=0;    Cre4=1;     //1101
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                             case 14:    //결과 = 14 이면
                                Cre1=1;    Cre2=1;   Cre3=1;    Cre4=0;     //1110
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4: LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                             case 15:    //결과 = 15 이면
                                Cre1=1;    Cre2=1;   Cre3=1;    Cre4=1;     //1111
                                LCD_DisplayChar(2,15,Cre1+0x30);               //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
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
                              BEEP();             //부저 1회
                              Aopnd1=1;        //1저장
                              LCD_SetFont(&Gulim8);		       // 폰트 : 굴림 8      
                              LCD_SetTextColor(RGB_BLACK);        //글자색 :검정
                              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                              LCD_DisplayChar(1,4,Aopnd1+0x30);   //LCD에 1표시
                    
                              SW2_Flag=1;
                          }
                          else
                          {  
                              BEEP();              //부저 1회
                              Aopnd1=0;         //0저장
                              LCD_SetFont(&Gulim8);		       // 폰트 : 굴림 8      
                              LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
                              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                              LCD_DisplayChar(1,4,Aopnd1+0x30);  //LCD에 0표시
                    
                              SW2_Flag=0;
                          }
                          break;
                    case 0xF700:                 //SW3  ON
                        if(SW3_Flag==0)        
                        { 
                              BEEP();              //부저 1회
                              Aopnd2=1;             //1저장
                              LCD_SetFont(&Gulim8);		       // 폰트 : 굴림 8      
                              LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
                              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                              LCD_DisplayChar(3,4,Aopnd2+0x30);  //LCD에 1표시
                      
                              SW3_Flag=1;
                              
                          }
                          else
                          {   
                              BEEP();              //부저 1회
                              Aopnd2=0;            //0저장
                              LCD_SetFont(&Gulim8);		       // 폰트 : 굴림 8      
                              LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
                              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                              LCD_DisplayChar(3,4,Aopnd2+0x30);  //LCD에 0표시
                      
                              SW3_Flag=0;
                          }
                        break;
                    case 0xEF00:              //SW4 ON
                          if(SW4_Flag==0)        
                          { 
                              BEEP();             //부저 1회
                              Bopnd1=1;         //1저장
                              LCD_SetFont(&Gulim8);		       // 폰트 : 굴림 8      
                              LCD_SetTextColor(RGB_BLACK);        //글자색 :검정
                              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                              LCD_DisplayChar(6,4,Bopnd1+0x30);   //LCD에 1표시
                      
                              SW4_Flag=1;
                          }
                          else
                          {  
                              BEEP();              //부저 1회 
                              Bopnd1=0;            //0저장
                              LCD_SetFont(&Gulim8);		       // 폰트 : 굴림 8      
                              LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
                              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                              LCD_DisplayChar(6,4,Bopnd1+0x30);    //LCD에 0표시
                      
                              SW4_Flag=0;
                          }
                        break;
                    case 0xDF00:                 //SW5  ON
                        if(SW5_Flag==0)        
                          { 
                              BEEP();              //부저 1회
                              Bopnd2=1;          //1저장
                              LCD_SetFont(&Gulim8);		       // 폰트 : 굴림 8      
                              LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
                              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                              LCD_DisplayChar(8,4,Bopnd2+0x30);   //LCD에 1표시
                      
                              SW5_Flag=1;
                         }
                         else
                         {   
                              BEEP();              //부저 1회
                              Bopnd2=0;        //0저장
                              LCD_SetFont(&Gulim8);		       // 폰트 : 굴림 8      
                              LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
                              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                              LCD_DisplayChar(8,4,Bopnd2+0x30);   //LCD에 0표시
                      
                              SW5_Flag=0;
                          }
                        break;
                 }
                AopndSUM = 2*Aopnd1 + Aopnd2;     //A,B의 MSB는 2^1 이므로 Aopnd1과 Bopnd1에 *2 함
                BopndSUM = 2*Bopnd1 + Bopnd2;
           
              
	}
}

/* GLCD 초기화면 설정 함수 */
void DisplayInitScreen(void)
{
	LCD_Clear(RGB_WHITE);		// 화면 클리어
	LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
	LCD_SetBackColor(RGB_WHITE);	// 글자배경색 : 흰색
        LCD_SetTextColor(RGB_BLACK);       //글자색 :검정

//가운데 부분
	LCD_SetPenColor(RGB_GREEN);
        LCD_DrawRectangle(55,8,48,106);  //가운데 큰 초록색 사각형
        LCD_SetPenColor(RGB_BLACK);
        LCD_DrawRectangle(72,31,20,20);  //가운데 연산자 있는 사각형
       LCD_SetBrushColor(GET_RGB(255,192,203)); // 분홍색
        LCD_DrawFillRect(73,32,19,19);   //가운데 연산자 있는 사각형 배경색

        LCD_DrawRectangle(70,86,20,17);  //연속 증가 모드 사각형
        LCD_SetBrushColor(RGB_YELLOW);
        LCD_DrawFillRect(71,87,19,16);   //연속 증가 모드 사각형 배경색

        LCD_SetFont(&Gulim10);		// 폰트 : 굴림 10
        LCD_SetBackColor(GET_RGB(255,192,203));	// 글자배경색 :분홍색
        LCD_DisplayChar(2,10,Operator);          //연산자 표시

        LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
	LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색
        LCD_DisplayText(7,9,"+0");

//왼쪽 부분
        LCD_DrawRectangle(28,11,17,17);  //A쪽 사각형
        LCD_DrawRectangle(28,36,17,17);  //A쪽  사각형
        LCD_DrawRectangle(28,76,17,17);  //B쪽 사각형
        LCD_DrawRectangle(28,99,17,17);  //B쪽  사각형
        LCD_SetBrushColor(RGB_YELLOW);
        LCD_DrawFillRect(29,12,16,16); //A쪽 사각형
        LCD_DrawFillRect(29,37,16,16);  //A쪽 사각형
        LCD_DrawFillRect(29,77,16,16);  //B쪽 사각형
        LCD_DrawFillRect(29,100,16,16);   //B쪽 사각형
        LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8      
        LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
        LCD_DisplayChar(1,4,Aopnd1+0x30);
        LCD_DisplayChar(3,4,Aopnd2+0x30);
        LCD_DisplayChar(6,4,Bopnd1+0x30);
        LCD_DisplayChar(8,4,Bopnd2+0x30);
        
        LCD_SetPenColor(RGB_GREEN);
        LCD_DrawRectangle(3,16,20,20);  //A 사각형  
        LCD_DrawRectangle(3,82,20,20);  //B 사각형   
        LCD_SetFont(&Gulim10);		// 폰트 : 굴림 10
        LCD_SetBackColor(RGB_WHITE);	// 글자배경색 : 흰색
        LCD_DisplayChar(1,1,'A');
        LCD_DisplayChar(5,1,'B');

        LCD_SetPenColor(RGB_BLUE);
        LCD_DrawHorLine(28+18, 21, 9);  //왼쪽 노란사각형 4개와 큰 사각형  연결선
        LCD_DrawHorLine(28+18, 45, 9);
        LCD_DrawHorLine(28+18, 84, 9);
        LCD_DrawHorLine(28+18,108, 9);

//오른쪽 부분
        LCD_SetBrushColor(RGB_YELLOW);
        LCD_DrawFillRect(116,1,16,16);  //C쪽  사각형 부호 
        LCD_DrawFillRect(116,24,16,15);  //C쪽 사각형 1
        LCD_DrawFillRect(116,49,16,16);  //C쪽  사각형 2
        LCD_DrawFillRect(116,77,16,16);  //C쪽 사각형 3
        LCD_DrawFillRect(116,100,16,16);  //C쪽  사각형 4

        LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
        LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
        LCD_DisplayChar(0,15,'+');
        LCD_DisplayChar(2,15,Cre1+0x30);  
        LCD_DisplayChar(4,15,Cre2+0x30);
        LCD_DisplayChar(6,15,Cre3+0x30);
        LCD_DisplayChar(8,15,Cre4+0x30);
     
        LCD_SetPenColor(RGB_BLACK);
        LCD_DrawRectangle(115,0,17,17);  //C쪽  사각형 부호 
        LCD_DrawRectangle(115,23,17,17);  //C쪽 사각형 1
        LCD_DrawRectangle(115,48,17,17);  //C쪽  사각형 2
        LCD_DrawRectangle(115,76,17,17);  //C쪽 사각형 3
        LCD_DrawRectangle(115,99,17,17);  //C쪽  사각형 4
       
        LCD_SetPenColor(RGB_BLUE);
        LCD_DrawHorLine(104, 9, 11);  //오른쪽 노란사각형 5개와 큰 사각형  연결선
        LCD_DrawHorLine(104, 33, 11);
        LCD_DrawHorLine(104, 55,11);
        LCD_DrawHorLine(104,84, 11);
        LCD_DrawHorLine(104,108, 11);

        LCD_SetFont(&Gulim10);		// 폰트 : 굴림 10
        LCD_SetBackColor(RGB_WHITE);	// 글자배경색 : 흰색
        LCD_DisplayChar(3,18,'C');

        LCD_SetPenColor(RGB_GREEN);
        LCD_DrawRectangle(137,49,20,20);  //C 사각형  

     

}

/* GPIO (GPIOG(LED), GPIOH(Switch), GPIOF(Buzzer)) 초기 설정	*/
void _GPIO_Init(void)
{
	// LED (GPIO G) 설정
    	RCC->AHB1ENR	|=  0x00000040;	// RCC_AHB1ENR : GPIOG(bit#6) Enable							
	GPIOG->MODER 	|=  0x00005555;	// GPIOG 0~7 : Output mode (0b01)						
	GPIOG->OTYPER	&= ~0x00FF;	// GPIOG 0~7 : Push-pull  (GP8~15:reset state)	
 	GPIOG->OSPEEDR 	|=  0x00005555;	// GPIOG 0~7 : Output speed 25MHZ Medium speed 
   
	// SW (GPIO H) 설정 
	RCC->AHB1ENR    |=  0x00000080;	// RCC_AHB1ENR : GPIOH(bit#7) Enable							
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH 8~15 : Input mode (reset state)				
	GPIOH->PUPDR 	&= ~0xFFFF0000;	// GPIOH 8~15 : Floating input (No Pull-up, pull-down) :reset state

	// Buzzer (GPIO F) 설정 
    	RCC->AHB1ENR	|=  0x00000020; // RCC_AHB1ENR : GPIOF(bit#5) Enable							
	GPIOF->MODER 	|=  0x00040000;	// GPIOF 9 : Output mode (0b01)						
	GPIOF->OTYPER 	&= ~0x0200;	// GPIOF 9 : Push-pull  	
 	GPIOF->OSPEEDR 	|=  0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 
        
        //Joy Stick SW(PORT I) 설정
	RCC->AHB1ENR	|= 0x00000100;	// RCC_AHB1ENR GPIOI 8번 비트 Enable
	GPIOI->MODER	&=~0x000FFC00;	// GPIOI 5~9 : Input mode (reset state)
        GPIOI->PUPDR	&= ~0x000FFC00;	// GPIOI 5~9 : Floating input (No Pull-up, pull-down) (reset state)
}	

/* EXTI (EXTI8(GPIOH.8, SW0), EXTI9(GPIOH.9, SW1)) 초기 설정  */
void _EXTI_Init(void)
{
    	RCC->AHB1ENR 	|= 0x00000080;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= 0x00004000;	// Enable System Configuration Controller Clock
	
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH PIN8~PIN15 Input mode (reset state)			
        	      	
// EXTI9, 14 설정     , PH9, PH14
	SYSCFG->EXTICR[2] |= 0x0070; 	// EXTI9에 대한 소스 입력은 GPIOH로 설정
	SYSCFG->EXTICR[3] |= 0x0700; 	// EXTI14에 대한 소스 입력은 GPIOH로 설정

	EXTI->FTSR |= 0x004200;		// EXTI9,14: Falling Trigger Enable
 	EXTI->IMR  |= 0x004200;  	// EXTI9,14 인터럽트 mask (Interrupt Enable) 설정
		
	NVIC->ISER[0] |= (1 << 23);	   // Enable 'Global Interrupt EXTI9'
        NVIC->ISER[1] |= (1 << (40-32));	 // Enable 'Global Interrupt EXTI14'
     
    //조이스틱  업  PI6,  라이트  PI8        , EXTI6, EXTI8
      
        RCC->AHB1ENR 	|= 0x00000100;	  // RCC_AHB1ENR GPIOI Enable
	RCC->APB2ENR 	|= 0x00004000;	  // Enable System Configuration Controller Clock
			
	GPIOI->MODER 	&= ~ 0x00033000;	// GPIOI PIN6, PIN8 Input mode (reset state)		 
        SYSCFG->EXTICR[1] |= 0x0800;	       //EXTI6에 대한
        SYSCFG->EXTICR[2] |= 0x0008;        //EXTI8에 대한
        
        EXTI->FTSR |=  0x00000140 ;// EXTI6,8: Falling Trigger Enable 

	EXTI->IMR   |= 0x00000140 ;	//EXTI6,8 : 인터럽트 mask (Interrupt Enable) 설정
	NVIC->ISER[0] |= ( 1<<23  );     // Enable 'Global Interrupt EXTI6, EXTI8'
        
        NVIC->IP[23]= 0xF0;  // EXTI9_5 ,  Ligh Priority     EXTI6번에 대한   
	NVIC->IP[40]= 0xE0;  // EXTI15_10    How Priority    EXTI14번에 대한 , 더 높은 우선순위
       
}

/* EXTI5~9 ISR */
void EXTI9_5_IRQHandler(void)		
{
      if(EXTI->PR & 0x0100)                   // EXTI8 Interrupt Pending(발생) 여부?  ,PI8
	{
		EXTI->PR |= 0x0100; 		// Pending bit Clear (clear를 안하면 인터럽트 수행후 다시 인터럽트 발생)
		
                BEEP();
                LCD_SetFont(&Gulim10);		// 폰트 : 굴림 10
                LCD_SetTextColor(RGB_BLACK);      // 글자배경색 :검정
                LCD_SetBackColor(GET_RGB(255,192,203));	// 글자배경색 :분홍색
                if(Operator=='+')
                {
                      Operator='-';
                      LCD_DisplayChar(2,10,Operator); 
                      Fram_Write(530,Operator);         //연산자 저장
                }
                 else if(Operator=='-')
                {
                      Operator='x';
                      LCD_DisplayChar(2,10,Operator);    
                      Fram_Write(530,Operator);         //연산자 저장
                }
		else if(Operator=='x')
                {
                      Operator='&';
                      LCD_DisplayChar(2,10,Operator);
                      Fram_Write(530,Operator);         //연산자 저장
                }
                else if(Operator=='&')
                {
                      Operator='|';
                      LCD_DisplayChar(2,10,Operator);
                      Fram_Write(530,Operator);         //연산자 저장
                }
                else if(Operator=='|')
                {
                      Operator='^';
                      LCD_DisplayChar(2,10,Operator);
                      Fram_Write(530,Operator);         //연산자 저장
                }
                else if(Operator=='^')
                {
                      Operator='+';
                      LCD_DisplayChar(2,10,Operator);
                      Fram_Write(530,Operator);         //연산자 저장
                }
                EXTI->PR |= 0x0100; 		// Pending bit Clear
	}
        if(EXTI->PR & 0x0200)                   // EXTI9 Interrupt Pending(발생) 여부?  , PH9, SW1 
	{
		EXTI->PR |= 0x0200; 
                BEEP();      //부저 1회
                if(Operator=='+')   //연산자가 + 인 경우
                {
                      LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                      LCD_SetTextColor(RGB_BLACK);      // 글자배경색 :검정
                      LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C쪽  부호 사각형
                      
                      Cresult = AopndSUM + BopndSUM;   //덧셈 : 최솟값:0 ,최댓값:6
                      Fram_Write(531,Cresult);         //결과값 저장
                      switch (Cresult)
                      {
                       case 0:    //결과 = 0 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //결과 = 1 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //결과 = 2 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //결과 = 3 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 4:    //결과 = 4 이면
                              Cre1=0;    Cre2=1;   Cre3=0;    Cre4=0;     //0100
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 5:    //결과 = 5 이면
                              Cre1=0;    Cre2=1;   Cre3=0;    Cre4=1;     //0101
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 6:    //결과 = 6 이면
                              Cre1=0;    Cre2=1;   Cre3=1;    Cre4=0;     //0110
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }
                else if(Operator=='-')   //연산자가 -인 경우
                {
                      LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                      LCD_SetTextColor(RGB_BLACK);      // 글자배경색 :검정
                      LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
            
                         if(AopndSUM >= BopndSUM)         //A가 더 클 경우 , A=B 인 경우
                          {
                              LCD_DisplayChar(0,15,'+');
                              LCD_SetPenColor(RGB_BLACK);
                              LCD_DrawRectangle(115,0,17,17);  //C쪽  부호 사각형
                              
                              Cresult = AopndSUM - BopndSUM;   // : 최솟값:0 ,최댓값:3
                              Fram_Write(531,Cresult);         //결과값 저장
                              switch (Cresult)
                              {
                               case 0:    //결과 = 0 이면
                                      Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 1:    //결과 = 1 이면
                                      Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 2:    //결과 = 2 이면
                                      Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 3:    //결과 = 3 이면
                                      Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                                }
                          }
                         else if(AopndSUM < BopndSUM)         //B가 더 클 경우
                          {
                              LCD_DisplayChar(0,15,'-');
                              LCD_SetPenColor(RGB_BLACK);
                              LCD_DrawRectangle(115,0,17,17);  //C쪽  부호 사각형
                              
                              Cresult = BopndSUM - AopndSUM;   // : 최솟값:0 ,최댓값:3
                              Fram_Write(531,Cresult);         //결과값 저장
                              switch (Cresult)
                              {
                               case 0:    //결과 = 0 이면
                                      Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 1:    //결과 = 1 이면
                                      Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 2:    //결과 = 2 이면
                                      Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                               case 3:    //결과 = 3 이면
                                      Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                                      LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                      LCD_DisplayChar(4,15,Cre2+0x30);
                                      LCD_DisplayChar(6,15,Cre3+0x30);
                                      LCD_DisplayChar(8,15,Cre4+0x30);
                                   break;
                                }
                          }
                }
                else if(Operator=='x')   //연산자가 x 인 경우
                {
                      LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                      LCD_SetTextColor(RGB_BLACK);      // 글자배경색 :검정
                      LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C쪽  부호 사각형
                      
                      Cresult = AopndSUM * BopndSUM;   //  최솟값:0 ,최댓값:9
                      Fram_Write(531,Cresult);         //결과값 저장
                      switch (Cresult)
                      {
                       case 0:    //결과 = 0 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //결과 = 1 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //결과 = 2 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //결과 = 3 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 4:    //결과 = 4 이면
                              Cre1=0;    Cre2=1;   Cre3=0;    Cre4=0;     //0100
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 5:    //결과 = 5 이면
                              Cre1=0;    Cre2=1;   Cre3=0;    Cre4=1;     //0101
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 6:    //결과 = 6 이면
                              Cre1=0;    Cre2=1;   Cre3=1;    Cre4=0;     //0110
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 7:    //결과 = 7 이면
                              Cre1=0;    Cre2=1;   Cre3=1;    Cre4=1;     //0111
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 8:    //결과 = 8 이면
                              Cre1=1;    Cre2=0;   Cre3=0;    Cre4=0;     //1000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 9:    //결과 = 9 이면
                              Cre1=1;    Cre2=0;   Cre3=0;    Cre4=1;     //1001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }
                 else if(Operator=='&')   //연산자가 & 인 경우
                {
                      LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                      LCD_SetTextColor(RGB_BLACK);      // 글자배경색 :검정
                      LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C쪽  부호 사각형
                      
                      Cresult = AopndSUM & BopndSUM;   //  최솟값:0 ,최댓값:3
                      Fram_Write(531,Cresult);         //결과값 저장
                      switch (Cresult)
                      {
                       case 0:    //결과 = 0 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //결과 = 1 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //결과 = 2 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //결과 = 3 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }
               else if(Operator=='|')   //연산자가 | 인 경우
                {
                      LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                      LCD_SetTextColor(RGB_BLACK);      // 글자배경색 :검정
                      LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C쪽  부호 사각형
                      
                      Cresult = AopndSUM | BopndSUM;   //  최솟값:0 ,최댓값:3
                      Fram_Write(531,Cresult);         //결과값 저장
                      switch (Cresult)
                      {
                       case 0:    //결과 = 0 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //결과 = 1 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //결과 = 2 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //결과 = 3 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }
                else if(Operator=='^')   //연산자가 ^ 인 경우
                {
                      LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색  
                      LCD_SetTextColor(RGB_BLACK);      // 글자배경색 :검정
                      LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
                      LCD_DisplayChar(0,15,'+');
                      LCD_SetPenColor(RGB_BLACK);
                      LCD_DrawRectangle(115,0,17,17);  //C쪽  부호 사각형
                      
                      Cresult = AopndSUM ^ BopndSUM;   //  최솟값:0 ,최댓값:3
                      Fram_Write(531,Cresult);         //결과값 저장
                      switch (Cresult)
                      {
                       case 0:    //결과 = 0 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 1:    //결과 = 1 이면
                              Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 2:    //결과 = 2 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                       case 3:    //결과 = 3 이면
                              Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                              LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                              LCD_DisplayChar(4,15,Cre2+0x30);
                              LCD_DisplayChar(6,15,Cre3+0x30);
                              LCD_DisplayChar(8,15,Cre4+0x30);
                           break;
                      }
                }EXTI->PR |= 0x0200; 	// Pending bit Clear
        }
        if(EXTI->PR & 0x0040)                   // EXTI6 Interrupt Pending(발생) 여부?  ,PI6: 연속증가모드
	 {
              EXTI->PR |= 0x0040; 		// Pending bit Clear (clear를 안하면 인터럽트 수행후 다시 인터럽트 발생)
              LCD_SetTextColor(RGB_BLACK);       //글자색 :검정
              LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
              LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : 노란색
              LCD_DisplayText(7,9,"+1");             //+1표시
              BEEP();                //부저 1회
              GPIOG->ODR |= 0x0080;         //LED7 on
              
              ExoFlag=0;
              while(ExoFlag== 0)
              {
                  Cresult++;     // 결과 1씩 증가
                  
                  if(Cresult==16)      //결과=16 이면 0으로
                  {
                    Cresult=0;
                  }
                  Fram_Write(531,Cresult);         //결과값 저장
                     switch (Cresult)
                        {
                         case 0:    //결과 = 0 이면
                                Cre1=0;    Cre2=0;   Cre3=0;    Cre4=0;       //0000
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 1:    //결과 = 1 이면
                                Cre1=0;    Cre2=0;   Cre3=0;    Cre4=1;     //0001
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 2:    //결과 = 2 이면
                                Cre1=0;    Cre2=0;   Cre3=1;    Cre4=0;     //0010
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 3:    //결과 = 3 이면
                                Cre1=0;    Cre2=0;   Cre3=1;    Cre4=1;     //0011
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 4:    //결과 = 4 이면
                                Cre1=0;    Cre2=1;   Cre3=0;    Cre4=0;     //0100
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 5:    //결과 = 5 이면
                                Cre1=0;    Cre2=1;   Cre3=0;    Cre4=1;     //0101
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 6:    //결과 = 6 이면
                                Cre1=0;    Cre2=1;   Cre3=1;    Cre4=0;     //0110
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 7:    //결과 = 7 이면
                                Cre1=0;    Cre2=1;   Cre3=1;    Cre4=1;     //0111
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 8:    //결과 = 8 이면
                                Cre1=1;    Cre2=0;   Cre3=0;    Cre4=0;     //1000
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 9:    //결과 = 9 이면
                                Cre1=1;    Cre2=0;   Cre3=0;    Cre4=1;     //1001
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                        case 10:    //결과 = 10 이면
                                Cre1=1;    Cre2=0;   Cre3=1;    Cre4=0;     //1010
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 11:    //결과 = 11 이면
                                Cre1=1;    Cre2=0;   Cre3=1;    Cre4=1;     //1011
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                         case 12:    //결과 = 12 이면
                                Cre1=1;    Cre2=1;   Cre3=0;    Cre4=0;     //1100
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 13:    //결과 = 13 이면
                                Cre1=1;    Cre2=1;   Cre3=0;    Cre4=1;     //1101
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 14:    //결과 = 14 이면
                                Cre1=1;    Cre2=1;   Cre3=1;    Cre4=0;     //1110
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                             break;
                          case 15:    //결과 = 15 이면
                                Cre1=1;    Cre2=1;   Cre3=1;    Cre4=1;     //1111
                                LCD_DisplayChar(2,15,Cre1+0x30);                 //LCD에 결과값 표시  Cre1 : MSB ,  Cre4 : LSB
                                LCD_DisplayChar(4,15,Cre2+0x30);
                                LCD_DisplayChar(6,15,Cre3+0x30);
                                LCD_DisplayChar(8,15,Cre4+0x30);
                          break;
                        }
                  DelayMS(500);
              } 
              //while문 탈출
              LCD_DisplayText(7,9,"+0");             //+0표시
              BEEP();                //부저 3회
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
	if(EXTI->PR & 0x4000)                   // EXTI14 Interrupt Pending(발생) 여부?  ,연속 증가 모드 탈출
	{
            	
            BEEP();                //부저1회
            DelayMS(1000);     //1초후
            ExoFlag=1;  	   // 탈출 flag 변경
            EXTI->PR |= 0x4000;       // Pending bit Clear
	}
}
                
/* Switch가 입력되었는지 여부와 어떤 switch가 입력되었는지의 정보를 return하는 함수  */ 
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
