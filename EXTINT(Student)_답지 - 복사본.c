///////////////////////////////////////////////////////////////////////////////////
//      과제명 :  TP1. Two Elevators
//      과제개요:  6층 건물에 두대의 엘리베이터(Left 엘리베이터(L-E), Right 엘리베이터(R-E)) 설치 하여
//                   출발층에서 목표층 스위치(1~6층)를 선택하면 엘리베이터가 이동하도록 프로그램 작성
//      사용한 하드웨어(기능) : GPIO, EXTI, GLCD 
//      제출일 :    2023.6.01     
//      제출자 클래스 :    목요일반
//                    학번:      2019130027 
//                    이름:       유정환
//////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"
#include "GLCD.h"
#include "FRAM.h"

void ELE_L_UP(void); //왼쪽 엘베 올라감  
void ELE_L_DOWN(void); //왼쪽 엘베 내려감 
void ELE_R_UP(void); //오른쪽 엘베 올라감  
void ELE_R_DOWN(void); //오른쪽 엘베 내려감  

void BEEP_3(void); // 부저 3번 함수


void _GPIO_Init(void);
void _EXTI_Init(void);

void DisplayInitScreen(void);
uint16_t KEY_Scan(void);
void BEEP(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint8_t	SW0_Flag;

int  START_FL=1;  //출발층 선택
int  DESTI_FL=1;  //목표층 선택

int y1=89;  //왼쪽 엘리베이터 좌표값
int y2=89;  //오른쪽 엘리베이터 좌표값

int Y1=1;  //왼쪽 엘리베이터 실제 층 수
int Y2=1;  //오른쪽 엘리베이터 실제 층 수




int main(void)
{
	LCD_Init();	// LCD 모듈 초기화
	DelayMS(10);
	_GPIO_Init();	// GPIO 초기화
	_EXTI_Init();	// EXTI 초기화

      	Fram_Init();                    // FRAM 초기화 H/W 초기화
	Fram_Status_Config();   // FRAM 초기화 S/W 초기화
        
	DisplayInitScreen();	// LCD 초기화면

	GPIOG->ODR &= ~0x00FF;	// 초기값: LED0~7 Off
	GPIOG->ODR |= 0x0080;	// 초기 LED7 on 상태
    
          Y1=Fram_Read(2023);       //왼쪽 엘리베이터  저장값 리드
        LCD_SetTextColor(RGB_RED);  
        LCD_SetPenColor(RGB_BLUE);
        LCD_SetBrushColor(RGB_BLUE);
        
        if(Y1==1)        //왼쪽 엘리베이터에 1층 저장
        {
            y1=89;
            LCD_DrawRectangle(20, y1, 10, 12);
            LCD_DrawFillRect(20, y1, 10, 12);
            START_FL=Y1;
            DESTI_FL=Y1;     
            LCD_DisplayChar(6,6,START_FL+0x30);
            LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==2)      //왼쪽 엘리베이터에  2층 저장
        {
            y1=77;
            LCD_DrawRectangle(20, y1, 10, 24);
            LCD_DrawFillRect(20, y1, 10, 24);
            START_FL=Y1;
            DESTI_FL=Y1;     
            LCD_DisplayChar(6,6,START_FL+0x30);
            LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==3)    //왼쪽 엘리베이터에 3층 저장
        { 
             y1=65;
             LCD_DrawRectangle(20, y1, 10, 36);
             LCD_DrawFillRect(20, y1, 10, 36);
             START_FL=Y1;
             DESTI_FL=Y1;     
             LCD_DisplayChar(6,6,START_FL+0x30);
             LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==4)      //왼쪽 엘리베이터에 4층 저장
        { 
           y1=53;
           LCD_DrawRectangle(20, y1, 10, 48);
           LCD_DrawFillRect(20, y1, 10, 48);
           START_FL=Y1;
           DESTI_FL=Y1;     
           LCD_DisplayChar(6,6,START_FL+0x30);
           LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==5)     //왼쪽 엘리베이터에 5층 저장
        { 
           y1=41;
           LCD_DrawRectangle(20, y1, 10, 60);
           LCD_DrawFillRect(20, y1, 10, 60);
           START_FL=Y1;
           DESTI_FL=Y1;     
           LCD_DisplayChar(6,6,START_FL+0x30);
           LCD_DisplayChar(6,10,START_FL+0x30);
        }
        else if(Y1==6)        //왼쪽 엘리베이터에 6층 저장
        { 
            y1=29;
            LCD_DrawRectangle(20, y1, 10, 72);
            LCD_DrawFillRect(20, y1, 10, 72);
            START_FL=Y1;        
            DESTI_FL=Y1;     
            LCD_DisplayChar(6,6,START_FL+0x30);
             LCD_DisplayChar(6,10,START_FL+0x30);
         }
        
         Y2=Fram_Read(2024);           //오른쪽 엘리베이터 저장값 리드
         LCD_SetPenColor(RGB_GREEN); 
         LCD_SetBrushColor(RGB_GREEN);
         
        if(Y2==1)          //오른쪽 엘리베이터에  1층 저장
        {
              y2=89;
              LCD_DrawRectangle(110, y2, 10, 12);
               LCD_DrawFillRect(110, y2, 10, 12);
        }
        else if(Y2==2)        //오른쪽 엘리베이터에 2층 저장
        {
             y2=77;
             LCD_DrawRectangle(110, y2, 10, 24);
             LCD_DrawFillRect(110, y2, 10, 24);
        }
        else if(Y2==3) //오른쪽 엘리베이터에  3층 저장
        { 
               y2=65;
               LCD_DrawRectangle(110, y2, 10, 36);
                LCD_DrawFillRect(110, y2, 10, 36);
        }
        else if(Y2==4) //오른쪽 엘리베이터에  4층 저장
        { 
             y2=53;
             LCD_DrawRectangle(110, y2, 10, 48);
             LCD_DrawFillRect(110, y2, 10, 48);
        }
        else if(Y2==5) //오른쪽 엘리베이터에  5층 저장
        { 
              y2=41;
               LCD_DrawRectangle(110, y2, 10, 60);
               LCD_DrawFillRect(110, y2, 10, 60);
        }
        else if(Y2==6) //오른쪽 엘리베이터에 6층 저장
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
                             BEEP();                                         //부저 1회
                             START_FL++;                                  //출발층 증가
                          
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
                            BEEP();                                             //부저 1회
                            DESTI_FL++;                                       //목표층 증가
                             
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

/* GLCD 초기화면 설정 함수 */
void DisplayInitScreen(void)
{
	LCD_Clear(RGB_WHITE);		// 화면 클리어
	LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
	LCD_SetBackColor(RGB_WHITE);	//글자배경색 : 흰색
	LCD_SetTextColor(RGB_BLACK);	// 글자색 : Black
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
        
      //엘베모양(사각형)
        LCD_SetPenColor(RGB_BLUE);
	LCD_DrawRectangle(20, y1, 10, 12);
	LCD_SetBrushColor(RGB_BLUE);
	LCD_DrawFillRect(20, y1, 10, 12);
        
        LCD_SetPenColor(RGB_GREEN);
	LCD_DrawRectangle(110,  y2, 10, 12);
	LCD_SetBrushColor(RGB_GREEN);
	LCD_DrawFillRect(110,  y2, 10, 12);
       
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
	
	SYSCFG->EXTICR[3] |= 0x7000;	// EXTI15(PH15)에 대한 소스 입력은 GPIOH로 설정
        SYSCFG->EXTICR[2] |= 0x0007;	// EXTI8(PH8)에 대한 소스 입력은 GPIOH로 설정
					
	EXTI->FTSR |= 0x00008000;	// EXTI15: Falling Trigger Enable 
        EXTI->FTSR |= 0x00000100;	// EXTI8: Falling Trigger Enable

	 
         EXTI->IMR   |= 0x00000100;	// EXTI8 인터럽트 mask (Interrupt Enable) 설정
		
	NVIC->ISER[1] |= ( 1<<(40-32)  );	// Enable 'Global Interrupt EXTI15'  
					
        NVIC->ISER[0] |= ( 1<<23 );	// Enable 'Global Interrupt EXTI8'
        
        NVIC->IP[23]= 0xF0;  // EXTI9_5 ,  Low Priority     EXTI8번에 대한   
       
        NVIC->IP[40]= 0xE0;  // EXTI15_10    High Priority    EXTI15번이 더 높은 우선순위
        
}

void EXTI9_5_IRQHandler(void)
{		
	 
      if(EXTI->PR & 0x0100)			// EXTI8 Interrupt Pending(발생) 여부?
      {
          EXTI->IMR   |= 0x00008000;	// EXTI15- 인터럽트 mask (Interrupt Enable) 설정: 실행모드에서 enable
          EXTI->PR |= 0x0100;		// Pending bit Clear (clear를 안하면 인터럽트 수행후 다시 인터럽트 발생)	
        
          LCD_SetTextColor(RGB_RED);     
          LCD_DisplayText(2,7,"EX");        //실행모드 EX 표시
          LCD_DisplayText(5,8,"U");         //UP
        
         BEEP();  //부저 1회
         GPIOG->ODR ^= 0x01;		// LED0 Toggle
         GPIOG->ODR &=~ 0xFE;		// LED7~1 T모두 OFF
                
                if(abs(Y1-START_FL) <= abs(Y2-START_FL))  //출발층과 가장 가까운 엘리베이터가 왼쪽 엘리베이터 일때  &거리 차이가 같으면 왼쪽 우선 
                {  
                      LCD_SetTextColor(RGB_BLUE);
                      LCD_DisplayText(4,7,"L-E");                       //왼쪽 엘리베이터 동작 표시
                      DelayMS(500);
                          if(Y1<START_FL)                                 //엘리베이터가 출발층 밑에 위치
                          {
                                  if(START_FL <DESTI_FL)             //현재층<출발층 < 목표층  
                                  {
                                  
                                            while(START_FL!=Y1)          //출발층에 도착하면 while문 탈출
                                              {
                                                  y1=y1-12;                     //좌표값 -12
                                                  Y1=Y1+1;                      //층 +1      
                                                  ELE_L_UP();                //왼쪽 엘베 상승
                                                  DelayMS(500);   
                                              }      
                                        //출발층에 도착하면
                                                LCD_SetTextColor(RGB_RED);     
                                                LCD_DisplayText(5,8,"S");           // STOP
                                                DelayMS(500);                            //0.5초후 
                                                LCD_DisplayText(5,8,"U");              // UP
                                                DelayMS(500);                             //0.5초후 
                                                 
                                          while(DESTI_FL!=Y1)     //도착층에 도착하면 while문 탈출
                                              {
                                                      y1=y1-12;               //좌표값 -12
                                                      Y1=Y1+1;               //층 +1
                                                      ELE_L_UP();         //왼쪽 엘베 상승
                                                     DelayMS(500); 
                                              }  
                                       //도착층에 도착하면
                                             Fram_Write(2023,Y1);                              //2023번지에 도착층 저장
                                             LCD_SetTextColor(RGB_RED);       
                                             LCD_DisplayText(5,8,"S");                   //S 표시
                                             DelayMS(500);                                   //0.5초후
                                             LCD_DisplayText(2,7,"FL");                  //FL 모드
                                             BEEP_3();                                          //부저 3번
                                             GPIOG->ODR |= 0x80;		            // LED7 ON
                                             GPIOG->ODR &=~ 0x7F;	            // LED6~0 모두 OFF
                                      
                                  }
                                 else if(START_FL > DESTI_FL)     //현재층 < 출발층 > 도착층  
                                  {
                                             while(START_FL!=Y1)     //출발층에 도착하면 while문 탈출
                                              {
                                                    y1=y1-12;             //좌표값 -12
                                                    Y1=Y1+1;              //층 +1      
                                                    ELE_L_UP();         //왼쪽 엘베 상승
                                                    DelayMS(500);   
                                              }
                                     //출발층에 도착하면
                                               LCD_SetTextColor(RGB_RED);     
                                               LCD_DisplayText(5,8,"S");             // STOP
                                               DelayMS(500);                            //0.5초후 
                                               LCD_DisplayText(5,8,"U");              // UP
                                               DelayMS(500);                             //0.5초후 
                                                 
                                             while(DESTI_FL!=Y1)     //목표층에 도착하면 while문 탈출
                                              {
                                                     ELE_L_DOWN();    //왼쪽 엘베 하강
                                                     y1=y1+12;           //좌표값 +12
                                                     Y1=Y1-1;           //층 -1
                                                     DelayMS(500); 
                                              }  
                                      //도착층에 도착하면
                                             Fram_Write(2023,Y1);                           //2023번지에 도착층 저장
                                             LCD_SetTextColor(RGB_RED);       
                                             LCD_DisplayText(5,8,"S");                    //S 표시
                                             DelayMS(500);                                   //0.5초후
                                             LCD_DisplayText(2,7,"FL");                 //FL 모드
                                             BEEP_3();                                        //부저 3번
                                             GPIOG->ODR |= 0x80;		           // LED7 ON
                                             GPIOG->ODR &=~ 0x7F;		   // LED6~0 모두 OFF
                                      

                                      
                                  }
                                  else if(START_FL == DESTI_FL)     //현재층 <출발층 = 목표층  : 엘리베이터가 현재층으로 이동
                                  {
                                             while(START_FL!=Y1)               //출발층에 도착하면 while문 탈출
                                              {
                                                     y1=y1-12;                      //좌표값 -12
                                                     Y1=Y1+1;                       //층 +1
                                                     ELE_L_UP();                   //왼쪽 엘베 상승
                                                     DelayMS(500);                //0.5초후 
                                              
                                              }   
                                          //출발층에 도착하면 1초후 도착처리
                                              DelayMS(500);    
                                              Fram_Write(2023,Y1);                              //2023번지에 도착층 저장
                                              LCD_SetTextColor(RGB_RED);       
                                              LCD_DisplayText(5,8,"S");                        //S 표시
                                              DelayMS(500);                                       //0.5초후
                                              LCD_DisplayText(2,7,"FL");                      //FL 모드
                                              BEEP_3();                                             //부저 3번
                                              GPIOG->ODR |= 0x80;		                 // LED7 ON
                                              GPIOG->ODR &=~ 0x7F;		         // LED6~0 모두 OFF
                                   }
                         }
                         else if(Y1>START_FL)                              //엘리베이터가 출발층 위에 위치
                         {
                                 if(START_FL <DESTI_FL)                 //현재층 > 출발층 < 도착층  
                                 {
                                           while(START_FL!=Y1)           //출발층에 도착하면 while문 탈출
                                              {
                                                  ELE_L_DOWN();           //왼쪽 엘베 하강
                                                  y1=y1+12;                  //좌표값 +12
                                                  Y1=Y1-1;                   //층 -1
                                                  DelayMS(500);   
                                              }     
                                           //출발층에 도착하면
                                                 LCD_SetTextColor(RGB_RED);     
                                                 LCD_DisplayText(5,8,"S");             // STOP
                                                 DelayMS(500);                             //0.5초후 
                                                 LCD_DisplayText(5,8,"U");              // UP
                                                 DelayMS(500);                             //0.5초후 

                                           while(DESTI_FL!=Y1)     //목표층에 도착하면 while문 탈출
                                              {
                                                     y1=y1-12;            //좌표값 -12
                                                     Y1=Y1+1;              //층 +1
                                                     ELE_L_UP();         //왼쪽 엘베 상승
                                                     DelayMS(500); 
                                              }  
                                          //도착층에 도착하면
                                             Fram_Write(2023,Y1);                              //2023번지에 도착층 저장
                                             LCD_SetTextColor(RGB_RED);       
                                             LCD_DisplayText(5,8,"S");                  //S 표시
                                             DelayMS(500);                                 //0.5초후
                                             LCD_DisplayText(2,7,"FL");                //FL 모드
                                             BEEP_3();                                       //부저 3번
                                             GPIOG->ODR |= 0x80;		          // LED7 ON
                                             GPIOG->ODR &=~ 0x7F;	          // LED6~0 모두 OFF

                                 }
                                else  if(START_FL >DESTI_FL)     //현재층 > 출발층 > 도착층  
                                 {
                                          while(START_FL!=Y1)            //출발층에 도착하면 while문 탈출
                                              {
                                                      ELE_L_DOWN();         //왼쪽 엘베 하강
                                                      y1=y1+12;               //좌표값 +12
                                                      Y1=Y1-1;                //층 -1
                                                      DelayMS(500);   
                                              }
                                         //출발층에 도착하면
                                                  LCD_SetTextColor(RGB_RED);    
                                                  LCD_DisplayText(5,8,"S");               // STOP
                                                  DelayMS(500);                               //0.5초후           
                                                  LCD_DisplayText(5,8,"U");               //0.5초후 UP
                                                  DelayMS(500);                               //0.5초후 
                                                   
                                            while(DESTI_FL!=Y1)              //목표층에 도착하면 while문 탈출
                                                {
                                                      ELE_L_DOWN();             //왼쪽 엘베 하강
                                                      y1=y1+12;                   //좌표값 +12
                                                      Y1=Y1-1;                     //층 -1
                                                      DelayMS(500);
                                                }
                                                 //목표층에 도착하면
                                                Fram_Write(2023,Y1);                              //2023번지에 목표층 저장
                                                LCD_SetTextColor(RGB_RED);       
                                                LCD_DisplayText(5,8,"S");                       //S 표시
                                                DelayMS(500);                                       //0.5초후
                                                LCD_DisplayText(2,7,"FL");                      //FL 모드
                                                BEEP_3();                                             //부저 3번
                                                GPIOG->ODR |= 0x80;		           // LED7 ON
                                                GPIOG->ODR &=~ 0x7F;		           // LED6~0 모두 OFF
                                        
                                 }
                                 else  if(START_FL == DESTI_FL)     //현재층 > 출발층 = 목표층  
                                 { 
                                           while(START_FL!=Y1)              //출발층에 도착하면 while문 탈출
                                                {     
                                                      ELE_L_DOWN();           //왼쪽 엘베 하강
                                                      y1=y1+12;                 //좌표값 +12
                                                      Y1=Y1-1;                  //층 -1
                                                      DelayMS(500);  
                                                }
                                            //출발층에 도착하면  1초후 도착 처리 
                                               Fram_Write(2023,Y1);                               //2023번지에 목표층 저장
                                               DelayMS(500);                                         //0.5초후 
                                               LCD_SetTextColor(RGB_RED);       
                                               LCD_DisplayText(5,8,"S");                        //S 표시
                                               DelayMS(500);
                                               LCD_DisplayText(2,7,"FL");                      //0.5초후 FL 모드
                                               BEEP_3();                                             //부저 3번
                                               GPIOG->ODR |= 0x80;		          // LED7 ON
                                               GPIOG->ODR &=~ 0x7F;		          // LED6~0 모두 OFF
                                  }
                        }
                        else if(Y1==START_FL)                      //  출발층 = 현재층
                        {
                                      
                                   if(START_FL <DESTI_FL)                    // 출발층 = 현재충  < 목표층  
                                   {
                                             DelayMS(500);                       //위342줄 0.5초  + 지금 0.5초 =1초 후 이동 목표층 이동
                                            while(DESTI_FL!=Y1)              //목표층에 도착하면 while문 탈출
                                             {
                                                    y1=y1-12;                     //좌표값 -12
                                                    Y1=Y1+1;                      //층 +1
                                                    ELE_L_UP();                 //왼쪽 엘베 상승
                                                    DelayMS(500); 
                                             }
                                        //목표층에 도착하면
                                                 Fram_Write(2023,Y1);                //2023번지에 목표층 저장
                                                 LCD_SetTextColor(RGB_RED);       
                                                 LCD_DisplayText(5,8,"S");         //S 표시
                                                 DelayMS(500);
                                                 LCD_DisplayText(2,7,"FL");        //0.5초후 FL 모드
                                                 BEEP_3();                                //부저 3번
                                                 GPIOG->ODR |= 0x80;		// LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;		// LED6~0T모두 OFF

                                   }
                                   else if(START_FL >DESTI_FL)     //왼쪽 엘베 출발층 = 현재층  >  목표층
                                   {
                                             DelayMS(500);                       //위342줄 0.5초  + 지금 0.5초 =1초 후 이동 목표층 이동
                                              while(DESTI_FL!=Y1)                 //목표층에 도착하면 while문 탈출
                                              {
                                                     ELE_L_DOWN();                  //왼쪽 엘베 하강
                                                     y1=y1+12;                         //좌표값 +12
                                                     Y1=Y1-1;                          //층 -1
                                                     DelayMS(500);  
                                              }
                                                 //목표층에 도착하면
                                                  Fram_Write(2023,Y1);                   //2023번지에 목표층 저장
                                                  LCD_SetTextColor(RGB_RED);       
                                                  LCD_DisplayText(5,8,"S");            //S 표시
                                                  DelayMS(500);
                                                  LCD_DisplayText(2,7,"FL");           //0.5초후 FL 모드
                                                  BEEP_3();                                 //부저 3번
                                                  GPIOG->ODR |= 0x80;		// LED7 ON
                                                  GPIOG->ODR &=~ 0x7F;		// LED6~0T모두 OFF
                                   }
                                   else if(START_FL ==DESTI_FL)     //왼쪽 엘베 출발층 = 현재충  = 목표층  : 1초후 도착 처리
                                   {
                                            
                                        DelayMS(500);                                //위342줄 0.5초  + 지금 0.5초 =1초 후
                                        Fram_Write(2023,Y1);                         //2023번지에 목표층 저장
                                                   
                                        LCD_SetTextColor(RGB_RED);        
                                        LCD_DisplayText(5,8,"S");                  //S 표시
                                        DelayMS(500);
                                        LCD_DisplayText(2,7,"FL");                //0.5초후 FL 모드
                                        BEEP_3();                                     //부저 3번
                                        GPIOG->ODR |= 0x80;		         // LED7 ON
                                        GPIOG->ODR &=~ 0x7F;	  	 // LED6~0T모두 OFF
                                     }
                        }
                }
                else  if(abs(Y1-START_FL) > abs(Y2-START_FL))            //출발층과 가장 가까운 엘리베이터가 오른쪽 엘리베이터 라면
                {
                      LCD_SetTextColor(RGB_GREEN);
                      LCD_DisplayText(4,7,"R-E");                                        //오른쪽 엘리베이터 동작 표시  
                      DelayMS(500);
                          if(Y2<START_FL)                                                   //엘리베이터가 출발층 밑에 위치 : 현재층 < 출발층 
                          {
                                    if(START_FL <DESTI_FL)                               //현재층 < 출발층 < 목표층  
                                    {
                                               while(START_FL!=Y2)                         //출발층에 도착하면 while문 탈출
                                               { 
                                                        y2=y2-12;                             //좌표값 -12
                                                        Y2=Y2+1;                             //층 +1
                                                        ELE_R_UP();                         //오른쪽 엘베 상승
                                                        DelayMS(500);                       //0.5초후 
                                                } 
                                           //출발층에 도착하면
                                                      LCD_SetTextColor(RGB_RED);    
                                                      LCD_DisplayText(5,8,"S");           // STOP 표시
                                                      DelayMS(500);                           //0.5초후 
                                                      LCD_DisplayText(5,8,"U");            //0.5초후 UP
                                                      DelayMS(500);                          //0.5초후 
                                              while(DESTI_FL != Y2)                          //목표층에 도착하면 while문 탈출
                                                 {   
                                                         y2=y2-12;                                //좌표값 -12
                                                         Y2=Y2+1;                                //층 +1
                                                         ELE_R_UP();                           //오른쪽 엘베 상승
                                                         DelayMS(500);                          //0.5초후 
                                                 }
                                            //목표층에 도착하면
                                                  Fram_Write(2024,Y2);                    //2024번지에 목표층 저장
                                                 LCD_SetTextColor(RGB_RED);    
                                                 LCD_DisplayText(5,8,"S");                //Stop 표시
                                                 DelayMS(500);                         
                                                 LCD_DisplayText(2,7,"FL");              //0.5초후 FL 모드
                                                 BEEP_3();                                  //부저 3번
                                                 GPIOG->ODR |= 0x80;		        // LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;		// LED6~0T모두 OFF
                                   }  
                                   else if(START_FL > DESTI_FL)                    //  현재층 < 출발층 > 목표층  
                                   {
                                              while(START_FL!=Y2)                     //출발층에 도착하면 while문 탈출
                                               { 
                                                      y2=y2-12;                         //좌표값 -12
                                                      Y2=Y2+1;                           //층 +1
                                                      ELE_R_UP();                     //오른쪽 엘베 상승
                                                      DelayMS(500);                   //0.5초후                             
                                               }     
                                           //출발층에 도착하면
                                                       LCD_SetTextColor(RGB_RED);    
                                                       LCD_DisplayText(5,8,"S");                    // STOP 표시
                                                       DelayMS(500);                                 //0.5초후
                                                       LCD_DisplayText(5,8,"U");                  // UP
                                                       DelayMS(500);                                 //0.5초후
                                              while(DESTI_FL != Y2)                                       //목표층에 도착하면 while문 탈출
                                               {
                                                        ELE_R_DOWN();                                     //오른쪽 엘베 하강
                                                        DelayMS(500);                                       //0.5초후 
                                                        y2=y2+12;                                          //좌표값 +12
                                                        Y2=Y2-1;                                            //층 -1
                                                }
                                           //목표층에 도착하면
                                                 Fram_Write(2024,Y2);                //2024번지에 목표층 저장
                                                 LCD_SetTextColor(RGB_RED);    
                                                 LCD_DisplayText(5,8,"S");               //Stop 표시
                                                 DelayMS(500);                         
                                                 LCD_DisplayText(2,7,"FL");              //0.5초후 FL 모드
                                                 BEEP_3();                                   //부저 3번
                                                 GPIOG->ODR |= 0x80;		        // LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;		// LED6~0T모두 OFF
                                  }
                                  else if(START_FL == DESTI_FL)                    //현재층 < 출발층 = 목표층  
                                  {
                                            while(START_FL!=Y2)                       //출발층에 도착하면 while문 탈출
                                              {
                                                    y2=y2-12;                                    //좌표값 -12
                                                    Y2=Y2+1;                                    //층 +1
                                                    ELE_R_UP();                               //오른쪽 엘베 상승
                                                    DelayMS(500);                           //0.5초후 
                                              
                                              }
                                        //출발층에 도착하면  1초후 도착 처리 
                                              Fram_Write(2024,Y2);                      //2024번지에 목표층 저장
                                              DelayMS(500);                                     //0.5초후 
                                              LCD_SetTextColor(RGB_RED);       
                                              LCD_DisplayText(5,8,"S");                      //Stop 표시
                                              DelayMS(500);
                                              LCD_DisplayText(2,7,"FL");                    //0.5초후 FL 모드
                                              BEEP_3();                                         //부저 3번
                                              GPIOG->ODR |= 0x80;		           // LED7 ON
                                              GPIOG->ODR &=~ 0x7F;		           // LED6~0T모두 OFF
                                   }
                      }
                      else if(Y2>START_FL)                                //현재층 > 출발층
                      {
                                  if(START_FL <DESTI_FL)                    //현재층 > 출발층 < 목표층  
                                   {
                                           while(START_FL!=Y2)                //출발층에 도착하면 while문 탈출
                                              {
                                                       ELE_R_DOWN();           //오른쪽 엘베 하강
                                                       y2=y2+12;                  //좌표값 +12
                                                       Y2=Y2-1;                   //층 -1
                                                       DelayMS(500);   
                                                      
                                              }
                                           //출발층에 도착하면
                                                LCD_SetTextColor(RGB_RED);    
                                                LCD_DisplayText(5,8,"S");                // STOP 표시
                                                DelayMS(500);                               //0.5초후
                                                LCD_DisplayText(5,8,"U");                // UP
                                                DelayMS(500);                               //0.5초후
                                              
                                             while(DESTI_FL!=Y2)                             //목표층에 도착하면 while문 탈출
                                              {
                                                       y2=y2-12;                                //좌표값 -12
                                                       Y2=Y2+1;                                 //층 +1
                                                       ELE_R_UP();                             //오른쪽 엘베 상승
                                                       DelayMS(500);                             //0.5초후 
                                              }
                                        //목표층에 도착하면
                                             Fram_Write(2024,Y2);                          //2024번지에 목표층 저장
                                             LCD_SetTextColor(RGB_RED);        
                                             LCD_DisplayText(5,8,"S");                  // STOP 표시
                                             DelayMS(500);
                                             LCD_DisplayText(2,7,"FL");                //0.5초후 FL 모드
                                             BEEP_3();                                      //부저 3번
                                             GPIOG->ODR |= 0x80;		        // LED7 ON
                                             GPIOG->ODR &=~ 0x7F;	          	// LED6~0T모두 OFF
                                      
                                  }
                                  else  if(START_FL >DESTI_FL)                //현재층 > 출발층 > 목표층  
                                  {
                                            while(START_FL!=Y2)                 //출발층에 도착하면 while문 탈출
                                            {
                                                       ELE_R_DOWN();                  //오른쪽 엘베 하강
                                                       y2=y2+12;                         //좌표값 +12
                                                       Y2=Y2-1;                         //층 -1
                                                       DelayMS(500);   
                                                          
                                           }
                                    //출발층에 도착하면
                                              LCD_SetTextColor(RGB_RED);    
                                              LCD_DisplayText(5,8,"S");                 // STOP 표시
                                              DelayMS(500);                                //0.5초후
                                              LCD_DisplayText(5,8,"U");                 // UP
                                              DelayMS(500);                               //0.5초후    
                                                     
                                            while(DESTI_FL!=Y2)                       //목표층에 도착하면 while문 탈출
                                            {
                                                       ELE_R_DOWN();                   //오른쪽 엘베 하강
                                                       y2=y2+12;                         //좌표값 +12
                                                       Y2=Y2-1;                         //층 -1
                                                       DelayMS(500);
                                             }
                                       //목표층에 도착하면
                                                 Fram_Write(2024,Y2);                          //2024번지에 목표층 저장
                                                 LCD_SetTextColor(RGB_RED);        
                                                 LCD_DisplayText(5,8,"S");                  //STOP 표시
                                                 DelayMS(500);
                                                 LCD_DisplayText(2,7,"FL");                //0.5초후 FL 모드
                                                 BEEP_3();                                      //부저 3번
                                                 GPIOG->ODR |= 0x80;		           // LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;	           // LED6~0T모두 OFF
                                  
                                  }
                                  else  if(START_FL == DESTI_FL)     //현재층 > 출발층 = 목표층  
                                  { 
                                            while(START_FL!=Y2)                   //출발층에 도착하면 while문 탈출
                                            {
                                                   ELE_R_DOWN();                   //오른쪽 엘베 하강
                                                   y2=y2+12;                          //좌표값 +12
                                                   Y2=Y2-1;                           //층 -1
                                                   DelayMS(500);  
                                            }
                                        //출발층에 도착하면 1초후 도착처리
                                                  Fram_Write(2024,Y2);                      //2024번지에 목표층 저장
                                                  DelayMS(500);                               //0.5초후 
                                                   LCD_SetTextColor(RGB_RED);       
                                                   LCD_DisplayText(5,8,"S");            //STOP 표시
                                                   DelayMS(500);
                                                   LCD_DisplayText(2,7,"FL");           //0.5초후 FL 모드
                                                   BEEP_3();                                 //부저 3번
                                                   GPIOG->ODR |= 0x80;		// LED7 ON
                                                   GPIOG->ODR &=~ 0x7F;		// LED6~0T모두 OFF
                                  }
                        }
                         else if(Y2==START_FL)                      //  출발층=현재충 
                        {
                                  if(START_FL <DESTI_FL)             // 출발층=현재충  < 목표층  
                                  {
                                         DelayMS(500);                       //위342줄 0.5초  + 지금 0.5초 =1초 후 이동 목표층 이동
                                            while(DESTI_FL!=Y2)         //목표층에 도착하면 while문 탈출
                                            {
                                                     y2=y2-12;              //좌표값 -12
                                                     Y2=Y2+1;               //층 +1
                                                     ELE_R_UP();           //오른쪽 엘베 상승
                                                     DelayMS(500);          //0.5초후  
                                             }
                                       //목표층에 도착하면   
                                                   Fram_Write(2024,Y2);                       //2024번지에 목표층 저장
                                            
                                                   LCD_SetTextColor(RGB_RED);        
                                                   LCD_DisplayText(5,8,"S");               //STOP 표시
                                                   DelayMS(500);
                                                   LCD_DisplayText(2,7,"FL");              //0.5초후 FL 모드
                                                   BEEP_3();                                   //부저 3번
                                                   GPIOG->ODR |= 0x80;		  // LED7 ON
                                                   GPIOG->ODR &=~ 0x7F;		  // LED6~0T모두 OFF
  
                                  }
                                  else if(START_FL >DESTI_FL)     // 출발층=현재충  > 목표층  
                                  {
                                           DelayMS(500);                       //위342줄 0.5초  + 지금 0.5초 =1초 후 이동 목표층 이동 
                                              while(DESTI_FL!=Y2)
                                             {
                                                    ELE_R_DOWN();                   //오른쪽 엘베 하강
                                                    y2=y2+12;                          //좌표값 +12
                                                    Y2=Y2-1;                           //층 -1
                                                    DelayMS(500);  
                                              }
                                   //목표층에 도착하면          
                                                     Fram_Write(2024,Y2);                         //2024번지에 목표층 저장
                           
                                                      LCD_SetTextColor(RGB_RED);       
                                                      LCD_DisplayText(5,8,"S");                     //STOP 표시
                                                      DelayMS(500);
                                                      LCD_DisplayText(2,7,"FL");                  //0.5초후 FL 모드
                                                      BEEP_3();                                       //부저 3번
                                                      GPIOG->ODR |= 0x80;		          // LED7 ON
                                                      GPIOG->ODR &=~ 0x7F;		         // LED6~0T모두 OFF
                                   }
                                    else if(START_FL ==DESTI_FL)     // 출발층=현재충  = 목표층  : 1초후 도착 처리
                                   {
                                                DelayMS(500);                       //위342줄 0.5초  + 지금 0.5초 =1초 후 도착 처리
                                                Fram_Write(2024,Y2);               //2024번지에 목표층 저장
                                                
                                                 LCD_SetTextColor(RGB_RED);       
                                                 LCD_DisplayText(5,8,"S");          //STOP 표시
                                                 DelayMS(500);
                                                 LCD_DisplayText(2,7,"FL");         //0.5초후 FL 모드
                                                 BEEP_3();                              //부저 3번
                                                 GPIOG->ODR |= 0x80;		   // LED7 ON
                                                 GPIOG->ODR &=~ 0x7F;	   // LED6~0T모두 OFF
                                   }
                        }
            }//오른쪽 엘리베이터에 대한 if문  
              
               EXTI->PR |= 0x0100;		// Pending bit Clear 
               EXTI->IMR   &=~ 0x00008000;	// EXTI15- 인터럽트 disable , 실행모드 중에서만 중단모드를 실행하기 위함
	}
	
}

void EXTI15_10_IRQHandler(void)
{	
          
            if( EXTI->PR & 0x8000)			// EXTI15 Interrupt Pending(발생) 여부?
              {
         
               EXTI->PR |= 0x8000;		// Pending bit Clear (clear를 안하면 인터럽트 수행후 다시 인터럽트 발생)           
              LCD_SetTextColor(RGB_RED);     
              LCD_DisplayText(2,7,"HD");        //중단모드 HD 표시
               
         
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
                
                LCD_DisplayText(2,7,"EX");        //모드 EX로 변경
                        
                GPIOG->ODR &= ~0x00C0;	// LED7 , LED6  OFF
                GPIOG->ODR |= 0x0001;	// LED0  ON
                
                EXTI->PR |= 0x8000;        // Pending bit Clear
                       
                }
        
}


/* Switch가 입력되었는지 여부와 어떤 switch가 입력되었는지의 정보를 return하는 함수  */ 
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

void BEEP_3(void)  //부저 3회
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



void ELE_L_UP(void)        //왼쪽 엘베 올라감
{
       LCD_SetPenColor(RGB_BLUE);       
       LCD_DrawRectangle(20, y1, 10, 12);
       LCD_SetBrushColor(RGB_BLUE);
       LCD_DrawFillRect(20, y1, 10, 12);
}
void ELE_L_DOWN(void) //왼쪽 엘베 내려감
{
       LCD_SetPenColor(RGB_WHITE);         
       LCD_DrawRectangle(20, y1, 10, 12);
       LCD_SetBrushColor(RGB_WHITE);
       LCD_DrawFillRect(20, y1, 10, 12);
}
void ELE_R_UP(void) //오른쪽 엘베 올라감
{
      LCD_SetPenColor(RGB_GREEN);         
      LCD_DrawRectangle(110, y2, 10, 12);
      LCD_SetBrushColor(RGB_GREEN);
      LCD_DrawFillRect(110, y2, 10, 12);
}
void ELE_R_DOWN(void) //오른쪽 엘베 내려감
{
      LCD_SetPenColor(RGB_WHITE);         
      LCD_DrawRectangle(110, y2, 10, 12);
      LCD_SetBrushColor(RGB_WHITE);
      LCD_DrawFillRect(110, y2, 10, 12);
}