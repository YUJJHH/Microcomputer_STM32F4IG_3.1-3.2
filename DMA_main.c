///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HW3: USART 통신을 이용한 센서 모니터링
// 제출자: 2019130027 유정환
// 제출일:2023-11-20
//동작개요: 세가지 아날로그 신호(전압값) SQR에 순서대로 등록(CH1우선)하고, 
//              타이머 주기마다 AD변환, SCAN mode로 단일 ADC하여 
//              DMA를 통하여 메모리(ADC_value[3])에 저장하고 전압값변환하여 LCD에 표시 및 USART를 이용해 PC에 업로딩
// 주요 내용 및 구현 방법
// 온도센서 종류:  ADC1_CH1 (PA1) : 키트가변저항,   ADC1_CH8 (PB0) : 배포된 가변저항,  ADC1_CH16 : MCU 내부 온도센서 
// Memory: ADC_value[3]; [0] 키트가변저항, [1] 배포된 가변저항 , [2] MCP 내부온도센서
// ADC설정: ADC1, SCAN mode, CONT mode, 3 ch 변환, DMA mode, 12 bit RES, 
// DMA 설정: DMA2 Stream0 channel0(ADC1), Peripheral-to-memory (P=>M), DMA_BufferSize = 3 (ADC_Value[3])
//               Circular mode, Priority level - High, FIFO_direct mode, 
//               Peripheral address pointer is fixed, Memory address pointer is incremented after each data transferd 
// USART1: TX pin: PA9, RX pin: PA10 
// TX: Polling 방식, RX: Interrupt 방식
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "stm32f4xx.h"
#include "GLCD.h"

#define SW0_PUSH        0xFE00  //PH8
#define SW1_PUSH        0xFD00  //PH9
#define SW2_PUSH        0xFB00  //PH10
#define SW3_PUSH        0xF700  //PH11
#define SW4_PUSH        0xEF00  //PH12
#define SW5_PUSH        0xDF00  //PH13
#define SW6_PUSH        0xBF00  //PH14
#define SW7_PUSH        0x7F00  //PH15

void _GPIO_Init(void);
uint16_t KEY_Scan(void);
void USART1_Init(void);
void USART_BRR_Configuration(uint32_t USART_BaudRate);

void _EXTI_Init(void);

void SerialSendChar(uint8_t c);
void SerialSendString(char *s);

void RunMenu(void);
void _ADC_Init(void);
void DMAInit(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint16_t ADC_value[3];

char str1[20];          //온도센서1 측정 문자열
char str2[20];          //온도센서2 측정 문자열
char str3[20];          //온도센서3 측정 문자열

void TIMER1_OC_Init(void);            // Timer 1 (Output Compare mode)

uint16_t Vsense, Voltage,T;

int main(void)
{
 _EXTI_Init();

        USART1_Init();
	_ADC_Init();
	DMAInit();
        _GPIO_Init();
        
	LCD_Init();		// LCD 구동 함수
	DelayMS(500);		// LCD구동 딜레이
          
	//LCD 초기화면구동 함수
	RunMenu();
        TIMER1_OC_Init();      // Timer 1 (Output Compare mode)
	
 	while(1)
	{
		
        switch(KEY_Scan())
		{
			case SW0_PUSH : 		//SW0
				GPIOG->ODR |= 0x01;	// LED0 On
				
                               SerialSendChar('.');  // 한 문자 보낼 떄
                                 SerialSendChar(5+0x30); // 한 문자 보낼 떄
                                  SerialSendChar(10);  // 한 문자 보낼 떄
                                 SerialSendChar(' '); // 한 문자 보낼 떄
			break;
                        
                        case SW7_PUSH : 		//SW7  overrun 발생
                        
                        break;
                        case SW6_PUSH : 		//SW6  overrun 삭제, 오버런 발생하면 kit 죽음
                      
                    
                        break;
                }    
        }
        
        
}

void ADC_IRQHandler(void)
{
          ADC1->SR &= ~(1<<1);		// EOC flag clear
  
          uint16_t x, y, z;      //막대그래프를 나타내기 위한 변수
    
          
              //온도센서1, 키트 가변저항
             LCD_SetTextColor(RGB_RED);     //글자색 빨강
	     sprintf(str1,"%2.1f",(3.3*10/4095)*ADC_value[0]);  //전압 표시 0 ~ 3.3V
	     LCD_DisplayText(1,14,str1);
            
             sprintf(str1,"%2.0f",3.5*((3.3/4095)*ADC_value[0])*((3.3/4095)*ADC_value[0])+1);  //온도 표시
             LCD_DisplayText(1,10,str1);
           
             x=atoi(str1); // 문자열  str1을 정수형으로 변환하여 x에 저장
        
             if(x>=1 && x<=23)      //1~23은 막대 4pixel 만큼 증가, 24~39는 3pixel 만큼 증가
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+4*x,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,4*x,8);     //4*23=92
             }
             else if(x==24)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+95,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,95,8);
             }
             else if(x==25)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+98,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,98,8);
             }
             else if(x==26)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+101,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,101,8);
             }
             else if(x==27)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+104,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,104,8);
             }
             else if(x==28)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+107,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,107,8);
             }
             else if(x==29)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+110,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,110,8);
             }
             else if(x==30)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+113,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,113,8);
             }
             else if(x==31)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+116,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,116,8);
             }
             else if(x==32)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+119,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,119,8);
            }
             else if(x==33)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+122,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,122,8);
             }
             else if(x==34)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+125,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,125,8);
             }
             else if(x==35)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+128,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,128,8);
             }
             else if(x==36)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+131,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,131,8);
            }
             else if(x==37)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+134,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,134,8);
             }
             else if(x==38)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+137,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,137,8);
             }
             else if(x==39)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+140,27,137,8);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(9,27,140,8);
             }
             
             //온도센서2, 배포된 가변저항
             LCD_SetTextColor(RGB_RED);     //글자색 빨강
	     sprintf(str2,"%2.1f",(3.3/4095)*ADC_value[1]);  //전압 표시 0 ~ 3.3V
       	     LCD_DisplayText(3,14,str2);
            
             sprintf(str2,"%2.0f",3.5*((3.3/4095)*ADC_value[1])*((3.3/4095)*ADC_value[1])+1);  //온도 표시
             LCD_DisplayText(3,10,str2); 
             
                   
             y=atoi(str2); // 문자열  str2을 정수형으로 변환하여 y에 저장
        
             if(y>=1 && y<=23)      //1~23은 막대 4pixel 만큼 증가, 24~39는 3pixel 만큼 증가
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+4*y,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색  초록   
                 LCD_DrawFillRect(9,54,4*y,8);     //4*23=92
             }
             else if(y==24)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+95,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,95,8);
             }
             else if(y==25)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+98,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,98,8);
             }
             else if(y==26)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+101,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,101,8);
             }
              else if(y==27)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+104,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,104,8);
            }
             else if(y==28)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+107,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,107,8);
             }
             else if(y==29)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+110,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,110,8);
             }
             else if(y==30)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+113,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,113,8);
             }
             else if(y==31)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+116,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,116,8);
             }
             else if(y==32)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+119,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,119,8);
             }
             else if(y==33)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+122,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,122,8);
             }
             else if(y==34)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+125,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,125,8);
             }
             else if(y==35)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+128,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,128,8);
             }
             else if(y==36)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+131,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,131,8);
             }
             else if(y==37)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+134,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,134,8);
             }
             else if(y==38)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+137,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,137,8);
             }
             else if(y==39)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+140,54,137,8);    
             
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색 초록   
                 LCD_DrawFillRect(9,54,140,8);
             }
             
       //      Voltage = ADC_Value * (3.3 * 100) / 4095;   // 3.3 : 4095 =  Volatge : ADC_Value 
             //온도센서3, 내부온도센서
              LCD_SetTextColor(RGB_RED);     //글자색 빨강
	      sprintf(str3,"%2.1f",(3.3/4095)*ADC_value[2]);  //내부 전압 표시 0 ~ 3.3V
	      LCD_DisplayText(5,14,str3);
              
        
              
              sprintf(str3,"%2.0f",(((3.3/4095)*ADC_value[2]-0.76)/2.5)+25);  //온도 표시
              LCD_DisplayText(5,10,str3);
              
              z=atoi(str3); // 문자열  str3을 정수형으로 변환하여 z에 저장
                            
             if(z>=1 && z<=23)      //1~23은 막대 4pixel 만큼 증가, 24~39는 3pixel 만큼 증가
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+4*z,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑  
                 LCD_DrawFillRect(9,80,4*z,8);     //4*23=92
                 
             }
             else if(z==24)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+95,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,95,8);
                 
             }
             else if(z==25)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+98,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,98,8);
                 
             }
             else if(z==26)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+101,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,101,8);
                 
             }
             else if(z==27)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+104,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,104,8);
                 
             }
             else if(z==28)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+107,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,107,8);
                 
             }
             else if(z==29)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색              
                 LCD_DrawFillRect(9+110,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,110,8);
                 
             }
             else if(z==30)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+113,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,113,8);
                 
             }
             else if(z==31)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+116,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,116,8);
                 
             }
             else if(z==32)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+119,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,119,8);
                 
             }
             else if(z==33)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+122,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,122,8);
                 
             }
             else if(z==34)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+125,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,125,8);
                 
             }
             else if(z==35)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+128,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,128,8);
                 
             }
             else if(z==36)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+131,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,131,8);
                 
             }
             else if(z==37)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+134,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,134,8);
                 
             }
             else if(z==38)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+137,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,137,8);
                 
             }
             else if(z==39)    
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(9+140,80,137,8);    
             
                 LCD_SetBrushColor(RGB_BLUE);        //브러시색 파랑   
                 LCD_DrawFillRect(9,80,140,8);
                 
             }
             
}


void _ADC_Init(void)
{
	/* 1st Analog signal */
	RCC->AHB1ENR |= 0x01;  	// RCC_AHB1ENR GPIOA Enable
	GPIOA->MODER |= (3<<2);	// GPIOA PIN1(PA1) 키트의 가변저항(온도센서1) : Analog mode
  
	/* 2nd Analog signal */
         RCC->AHB1ENR |= 0x02;  	// RCC_AHB1ENR GPIOB Enable
	GPIOB->MODER |= (3<<0);	// GPIOB PIN0(PB0) 온도센서2: Analog mode

	/* ADC Common Init **********************************************************/
	RCC->APB2ENR |= (1<<9);	// RCC_APB2ENR ADC1 Enable
  
	ADC->CCR &= ~0X0000001F;// ADC_Mode_Independent
	ADC->CCR |= 0x00010000;	// ADC_Prescaler_Div4 (ADC MAX Clock 36Mhz, 84Mhz(APB2)/4 = 21Mhz
        
        ADC->CCR |= (1<<23);	//Temperature sensor enable


	/* ADC2 Init ****************************************************************/
	ADC2->CR1 &= ~(3<<24);	// RES[1:0]=0b00 : 12bit Resolution
	ADC2->CR1 |= 0x00000100;	// ADC_ScanCovMode Enable (SCAN=1)
	ADC2->CR1 |=  (1<<5);		// EOCIE=1: Interrupt enable for EOC
        
        ADC2->CR2 &= ~0x00000002;	// ADC_ContinuousConvMode Disable (CONT=0)            
	ADC2->CR2 |= (3<<28);	// EXTEN[1:0]=0b01: ADC_ExternalTrigConvEdge_Enable (Both Edge)
        
        ADC2->CR2 |= (1<<24);	// EXTSEL:TIM1_CC2
       //   ADC2->CR2 |= (15<<24);	// EXTSEL: EXTI11          이거로 하니까  sw3 누를때마다 실행
        
	ADC2->CR2 &= ~(1<<11);	// ALIGN=0: ADC_DataAlign_Right
        
	ADC2->SQR1 |= (2<<20); // ADC Regular channel sequece length = 3 conversion

	/* ADC_RegularChannelConfig *********************************************/
   
	ADC2->SMPR2 |= 0x07 << (3*1);	//ADC1_CH1 Sample Time_480Cycles (3*Channel_1)  
	ADC2->SQR3 |= (1<< (5*(1-1)));  //ADC1_CH1 << (5*(Rank-1)), Rank = 1 (1순위로 변환: 키트 가변저항(온도센서1))

        ADC2->SMPR2 |= 0x07 << (3*8);	// ADC1_CH8 Sample TIme_480Cycles (3*Channel_1)  
	ADC2->SQR3 |= 8 << (5*(2-1));	// ADC1_CH8 << (5 * (Rank - 1)),  Rank = 2 (2순위로 변환: 가변저항(온도센서2))
 
        ADC2->SMPR1 |= 0x07 << (18);	// ADC1_CH16 Sample TIme_480Cycles (3*Channel_1)  
	ADC2->SQR3 |= 16 << (5*(3-1));	// ADC1_CH16 << (5 * (Rank - 1)),  Rank = 3 (3순위로 변환: 내부 온도센서(온도센서3))
        
	/* Enable DMA request after last transfer (Single-ADC mode) */
        ADC2->CR2 |= (1<<9); // DMA requests are issued	as long as data are converted and DMA=1	
				// for single ADC mode
        
	/* Enable ADC1 DMA */
	ADC2->CR2 |= (1<<8);	// DMA mode enabled  (DMA=1)
	ADC2->CR2 |= 0x00000001;	// Enable ADC1:  ADON=1
  
        NVIC->ISER[0] |= (1<<18);	// Enable ADC global Interrupt
        
	/* Start ADC Software Conversion */ 
	ADC2->CR2 |= 0x40000000;   // SWSTART=1
}

/*
void _ADC_Init(void)
{
//	 1st Analog signal 
	RCC->AHB1ENR |= 0x01;  	// RCC_AHB1ENR GPIOA Enable
	GPIOA->MODER |= (3<<2);	// GPIOA PIN1(PA1) 키트의 가변저항(온도센서1) : Analog mode
  
//	 2nd Analog signal 
         RCC->AHB1ENR |= 0x02;  	// RCC_AHB1ENR GPIOB Enable
	GPIOB->MODER |= (3<<0);	// GPIOB PIN0(PB0) 온도센서2: Analog mode

//	 ADC Common Init **********************************************************
	RCC->APB2ENR |= (1<<8);	// RCC_APB2ENR ADC1 Enable
  
	ADC->CCR &= ~0X0000001F;// ADC_Mode_Independent
	ADC->CCR |= 0x00010000;	// ADC_Prescaler_Div4 (ADC MAX Clock 36Mhz, 84Mhz(APB2)/4 = 21Mhz
        
        ADC->CCR |= (1<<23);	//Temperature sensor enable


//	 ADC1 Init ****************************************************************
	ADC1->CR1 &= ~(3<<24);	// RES[1:0]=0b00 : 12bit Resolution
	ADC1->CR1 |= 0x00000100;	// ADC_ScanCovMode Enable (SCAN=1)
	ADC1->CR1 |=  (1<<5);		// EOCIE=1: Interrupt enable for EOC
        
        ADC1->CR2 &= ~0x00000002;	// ADC_ContinuousConvMode Disable (CONT=0)            
	ADC1->CR2 |= (3<<28);	// EXTEN[1:0]=0b01: ADC_ExternalTrigConvEdge_Enable (Both Edge)
        
        ADC1->CR2 |= (1<<24);	// EXTSEL:TIM1_CC2
         // ADC1->CR2 |= (15<<24);	// EXTSEL: EXTI11          이거로 하니까  sw3 누를때마다 실행
        
	ADC1->CR2 &= ~(1<<11);	// ALIGN=0: ADC_DataAlign_Right
        
	ADC1->SQR1 |= (2<<20); // ADC Regular channel sequece length = 3 conversion

//	 ADC_RegularChannelConfig *********************************************
   
	ADC1->SMPR2 |= 0x07 << (3*1);	//ADC1_CH1 Sample Time_480Cycles (3*Channel_1)  
	ADC1->SQR3 |= (1<< (5*(1-1)));  //ADC1_CH1 << (5*(Rank-1)), Rank = 1 (1순위로 변환: 키트 가변저항(온도센서1))

        ADC1->SMPR2 |= 0x07 << (3*8);	// ADC1_CH8 Sample TIme_480Cycles (3*Channel_1)  
	ADC1->SQR3 |= 8 << (5*(2-1));	// ADC1_CH8 << (5 * (Rank - 1)),  Rank = 2 (2순위로 변환: 가변저항(온도센서2))
 
        ADC1->SMPR1 |= 0x07 << (18);	// ADC1_CH16 Sample TIme_480Cycles (3*Channel_1)  
	ADC1->SQR3 |= 16 << (5*(3-1));	// ADC1_CH16 << (5 * (Rank - 1)),  Rank = 3 (3순위로 변환: 내부 온도센서(온도센서3))
        
//	 Enable DMA request after last transfer (Single-ADC mode) *
        ADC1->CR2 |= (1<<9); // DMA requests are issued	as long as data are converted and DMA=1	
				// for single ADC mode
        
//	 Enable ADC1 DMA *
	ADC1->CR2 |= (1<<8);	// DMA mode enabled  (DMA=1)
	ADC1->CR2 |= 0x00000001;	// Enable ADC1:  ADON=1
  
        NVIC->ISER[0] |= (1<<18);	// Enable ADC global Interrupt
        
	// Start ADC Software Conversion  
	ADC1->CR2 |= 0x40000000;   // SWSTART=1
}*/


void TIMER1_OC_Init(void) //TIM1_CH2  주기= 400ms
{
	// PE11: TIM1_CH2   
	// PE11을 출력설정하고 Alternate function(TIM1_CH2)으로 사용 선언
	RCC->AHB1ENR   |= (1<<4);   // RCC_AHB1ENR GPIOE Enable
	GPIOE->MODER   |= (2<<2*11);   // GPIOE PIN11 Output Alternate function mode               
	GPIOE->OSPEEDR |= (3<<2*11);   // GPIOE PIN11 Output speed (100MHz High speed)
	GPIOE->OTYPER  = 0x00000000;   // GPIOE PIN11 Output type push-pull (reset state)
	GPIOE->PUPDR   |= (1<<2*11);   // GPIOE PIN11 Pull-up
	GPIOE->AFR[1]  |= (1<<12); // Connect TIM1 pins(PE11) to AF1(TIM1/2)

	
	// Timerbase Mode
	RCC->APB2ENR   |= 0x01;     // RCC_APB2ENR TIMER1 Enable

	TIM1->PSC = 8400-1;   // Prescaler 168,000,000Hz/8400= 20000Hz (0.05ms)

	
	TIM1->ARR = 8000-1;    // Auto reload  : 0.05ms * 8000 = 400ms(period) 
                                                                    
	TIM1->CR1 &= ~(1<<4);   // DIR: Countermode = Upcounter (reset state)
	TIM1->CR1 &= ~(3<<8);   // CKD: Clock division = 0 (reset state)
	TIM1->CR1 &= ~(3<<5);    // CMS(Center-aligned mode Sel): No(reset state)

	TIM1->EGR |= (1<<0);    // UG: Update generation 
    
	// Output/Compare Mode
	TIM1->CCER |= (1<<4);   // CC2E=1: CC2 channel Output Enable 
      
        TIM1->CCER &= ~(1<<5);  // CC2P=0: OCPolarity_Active High 

	TIM1->CCR2 = 100;   // TIM1_Pulse

	TIM1->BDTR |= (1<<15);  // main output enable  
   
	TIM1->CCMR1 &= ~(3<<8); // CC2S(CC1channel): Output 
	TIM1->CCMR1 &= ~(1<<11); // OC2PE: Output Compare 2 preload disable
	TIM1->CCMR1 |= (3<<12);   // OC2M: Output Compare 2 Mode : toggle

	TIM1->CR1 &= ~(1<<7);   // ARPE: Auto reload preload disable     

      
	TIM1->CR1 |= (1<<0);   // CEN: Enable the Tim1 Counter 
}

/*
void DMAInit(void)  //ADC1 은 stream0 ,4 둘다 가능
{
 	// DMA2 Stream0 channel0 configuration *************************************
	RCC->AHB1ENR |= (1<<22);		//DMA2 clock enable
	DMA2_Stream2->CR &= ~(7<<25);	        //DMA2 Stream0 channel 0 selected

	// ADC1->DR(Peripheral) ==> ADC_vlaue(Memory)
	DMA2_Stream4->PAR |= (uint32_t)&ADC1->DR;	   //Peripheral address - ADC1->DR(Regular data) Address
	DMA2_Stream4->M0AR |= (uint32_t)&ADC_value;      //Memory address - ADC_Value address 
	DMA2_Stream4->CR &= ~(3<<6);		         //Data transfer direction : Peripheral-to-memory (P=>M)
	DMA2_Stream4->NDTR = 3;			         //DMA_BufferSize = 3 (ADC_Value[3])

	DMA2_Stream4->CR &= ~(1<<9); 	//Peripheral increment mode  - Peripheral address pointer is fixed
	DMA2_Stream4->CR |= (1<<10);	//Memory increment mode - Memory address pointer is incremented after each data transferd 
	DMA2_Stream4->CR |= (1<<11);	//Peripheral data size - halfword(16bit)
	DMA2_Stream4->CR |= (1<<13);	//Memory data size - halfword(16bit)   
	DMA2_Stream4->CR |= (1<<8);	//Circular mode enabled   
	DMA2_Stream4->CR |= (2<<16);	//Priority level - High

	DMA2_Stream4->FCR &= ~(1<<2);	//DMA_FIFO_direct mode enabled
	DMA2_Stream4->FCR |= (1<<0);	//DMA_FIFO Threshold_HalfFull , Not used in direct mode

	DMA2_Stream4->CR &= ~(3<<23);	//Memory burst transfer configuration - single transfer
	DMA2_Stream4->CR &= ~(3<<21);	//Peripheral burst transfer configuration - single transfer  
	DMA2_Stream4->CR |= (1<<0);	//DMA2_Stream0 enabled
}*/


void DMAInit(void)  //ADC2 DMA2 stream 2 ch1 로 변경
{
 	// DMA2 Stream0 channel0 configuration *************************************
	RCC->AHB1ENR |= (1<<22);		//DMA2 clock enable
	DMA2_Stream2->CR |= (1<<25);	        //DMA2 Stream4 channel 1 selected

	// ADC1->DR(Peripheral) ==> ADC_vlaue(Memory)
	DMA2_Stream2->PAR |= (uint32_t)&ADC2->DR;	   //Peripheral address - ADC1->DR(Regular data) Address
	DMA2_Stream2->M0AR |= (uint32_t)&ADC_value;      //Memory address - ADC_Value address 
	DMA2_Stream2->CR &= ~(3<<6);		         //Data transfer direction : Peripheral-to-memory (P=>M)
	DMA2_Stream2->NDTR = 3;			         //DMA_BufferSize = 3 (ADC_Value[3])

	DMA2_Stream2->CR &= ~(1<<9); 	//Peripheral increment mode  - Peripheral address pointer is fixed
	DMA2_Stream2->CR |= (1<<10);	//Memory increment mode - Memory address pointer is incremented after each data transferd 
	DMA2_Stream2->CR |= (1<<11);	//Peripheral data size - halfword(16bit)
	DMA2_Stream2->CR |= (1<<13);	//Memory data size - halfword(16bit)   
	DMA2_Stream2->CR |= (1<<8);	//Circular mode enabled   
	DMA2_Stream2->CR |= (2<<16);	//Priority level - High
        
       

	DMA2_Stream2->FCR &= ~(1<<2);	//DMA_FIFO_direct mode enabled
	DMA2_Stream2->FCR |= (1<<0);	//DMA_FIFO Threshold_HalfFull , Not used in direct mode

	DMA2_Stream2->CR &= ~(3<<23);	//Memory burst transfer configuration - single transfer
	DMA2_Stream2->CR &= ~(3<<21);	//Peripheral burst transfer configuration - single transfer  
	DMA2_Stream2->CR |= (1<<0);	//DMA2_Stream0 enabled
}







void USART1_Init(void)
{
	// USART1 : TX(PA9)
	RCC->AHB1ENR	|= (1<<0);	// RCC_AHB1ENR GPIOA Enable
	GPIOA->MODER	|= (2<<2*9);	// GPIOA PIN9 Output Alternate function mode					
	GPIOA->OSPEEDR	|= (3<<2*9);	// GPIOA PIN9 Output speed (100MHz Very High speed)
	GPIOA->AFR[1]	|= (7<<4);	// Connect GPIOA pin9 to AF7(USART1)
    
	// USART1 : RX(PA10)
	GPIOA->MODER 	|= (2<<2*10);	// GPIOA PIN10 Output Alternate function mode
	GPIOA->OSPEEDR	|= (3<<2*10);	// GPIOA PIN10 Output speed (100MHz Very High speed
	GPIOA->AFR[1]	|= (7<<8);	// Connect GPIOA pin10 to AF7(USART1)

	RCC->APB2ENR	|= (1<<4);	// RCC_APB2ENR USART1 Enable
    
	USART_BRR_Configuration(9600); // USART Baud rate Configuration
    
	USART1->CR1	|= (1<<12);	// USART_WordLength 9 Data bit
	USART1->CR1	|= (1<<10);	// YES USART_Parity
        USART1->CR1	|= (1<<9);	// USART_Parity Odd

	USART1->CR1	|= (1<<2);	// 0x0004, USART_Mode_RX Enable
	USART1->CR1	|= (1<<3);	// 0x0008, USART_Mode_Tx Enable

	USART1->CR2	&= ~(3<<12);	// 0b00, USART_StopBits_1
	USART1->CR3	= 0x0000;	// No HardwareFlowControl, No DMA
    
	USART1->CR1 	|= (1<<5);	// 0x0020, RXNE interrupt Enable
	NVIC->ISER[1]	|= (1<<(37-32));// Enable Interrupt USART1 (NVIC 37번)

	USART1->CR1 	|= (1<<13);	//  0x2000, USART1 Enable
}

void USART1_IRQHandler(void)	
{       
	if ( (USART1->SR & USART_SR_RXNE) ) // USART_SR_RXNE= 1? RX Buffer Full?
	// #define  USART_SR_RXNE ((uint16_t)0x0020)    //  Read Data Register Not Empty     
	{
		char ch;
            
		
                ch = (uint16_t)(USART1->DR & (uint16_t)0x01FF);	// 수신된 문자 저장
                //SerialSendChar(ch); 		// PC에 수신된 문자를 Echo
                
                LCD_SetTextColor(RGB_RED);     //글자색 빨강
                LCD_DisplayChar(0,17,ch); 	// 수신된 문자를 LCD에 display
                
            
                if(ch=='1')  //1을 수신 받으면
                {   
                      if(str1[0]==' ')   //온도의 십의자리가 공백이면 ( 일의자리만 있으면)
                      {
                    //  SerialSendChar(str1[0]);   //십의자리 출력 안함
                        SerialSendChar(str1[1]);   //일의자리 PC로 송신
                        SerialSendChar(' ');        //공백 PC로 송신
                      }
                      else           // 온도가 두 자리 수이면
                      {
                        SerialSendChar(str1[0]);     //십의자리  PC로 송신
                        SerialSendChar(str1[1]);    //  일의자리  PC로 송신
                        SerialSendChar(' ');          //공백 PC로 송신
                      }
                }
                if(ch=='2')  //2를 수신 받으면
                {
                     if(str2[0]==' ')    //온도의 십의자리가 공백이면 (일의자리만 있으면)
                     { 
                    //  SerialSendChar(str2[0]);  //십의자리 출력 안함
                        SerialSendChar(str2[1]);
                        SerialSendChar(' ');
                     }
                     else                     // 온도가 두 자리 수이면
                     {
                       SerialSendChar(str2[0]);     //십의자리  PC로 송신
                       SerialSendChar(str2[1]);     //  일의자리  PC로 송신
                       SerialSendChar(' ');         //공백 PC로 송신
                     }
                }
                if(ch=='3')  //3을 수신 받으면
                {
                    if(str3[0]==' ')    //온도의 십의자리가 공백이면 ( 일의자리만 있으면)
                     { 
                    //  SerialSendChar(str3[0]);  //십의자리 출력 안함
                        SerialSendChar(str3[1]);
                        SerialSendChar(' ');
                     }
                     else                     // 온도가 두 자리 수이면
                     {
                       SerialSendChar(str3[0]);     //십의자리
                       SerialSendChar(str3[1]);     //  일의자리
                       SerialSendChar(' ');         //공백
                     }
                
                }
	} 
	// DR 을 읽으면 SR.RXNE bit(flag bit)는 clear 된다. 즉 clear 할 필요없음 
}

void SerialSendChar(uint8_t Ch) // 1문자 보내기 함수
{
	// USART_SR_TXE(1<<7)=0?, TX Buffer NOT Empty? 
	// TX buffer Empty되지 않으면 계속 대기(송신 가능한 상태까지 대기)
        while((USART1->SR & USART_SR_TXE) == RESET); 
	USART1->DR = (Ch & 0x01FF);	// 전송 (최대 9bit 이므로 0x01FF과 masking)
}

void SerialSendString(char *str) // 여러문자 보내기 함수
{
	while (*str != '\0') // 종결문자가 나오기 전까지 구동, 종결문자가 나온후에도 구동시 메모리 오류 발생가능성 있음.
	{
		SerialSendChar(*str);	// 포인터가 가르키는 곳의 데이터를 송신
		str++; 			// 포인터 수치 증가
	}
}

// Baud rate 설정
void USART_BRR_Configuration(uint32_t USART_BaudRate)
{ 
	uint32_t tmpreg = 0x00;
	uint32_t APB2clock = 84000000;	//PCLK2_Frequency
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;

	// Find the integer part 
	if ((USART1->CR1 & USART_CR1_OVER8) != 0) // USART_CR1_OVER8=(1<<15)
	//  #define  USART_CR1_OVER8 ((uint16_t)0x8000) // USART Oversampling by 8 enable   
	{       // USART1->CR1.OVER8 = 1 (8 oversampling)
		// Computing 'Integer part' when the oversampling mode is 8 Samples 
		integerdivider = ((25 * APB2clock) / (2 * USART_BaudRate));  // 공식에 100을 곱한 곳임(소수점 두번째자리까지 유지하기 위함)  
	}
	else  // USART1->CR1.OVER8 = 0 (16 oversampling)
	{	// Computing 'Integer part' when the oversampling mode is 16 Samples 
		integerdivider = ((25 * APB2clock) / (4 * USART_BaudRate));  // 공식에 100을 곱한 곳임(소수점 두번째자리까지 유지하기 위함)    
	}								     // 100*(f_CK) / (8*2*Buadrate) = (25*f_CK)/(4*Buadrate)	
	tmpreg = (integerdivider / 100) << 4;
  
	// Find the fractional part 
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

	// Implement the fractional part in the register 
	if ((USART1->CR1 & USART_CR1_OVER8) != 0)	
	{	// 8 oversampling
		tmpreg |= (((fractionaldivider * 8) + 50) / 100) & (0x07);
	}
	else	// 16 oversampling
	{
		tmpreg |= (((fractionaldivider * 16) + 50) / 100) & (0x0F);
	}

	// Write to USART BRR register
	USART1->BRR = (uint16_t)tmpreg;
}

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
}

void RunMenu(void)         
{
	LCD_Clear(RGB_WHITE);
	LCD_SetFont(&Gulim8);
	LCD_SetBackColor(RGB_WHITE);	//글자배경색
	LCD_SetTextColor(RGB_BLACK);     //글자색 검정
 	LCD_DisplayText(0,1,"YJH 2019130027");
        LCD_SetTextColor(RGB_BLUE);     //글자색 파랑
        LCD_DisplayText(1,1,"EXT1 TMP:24C(2.6V)");
         LCD_DisplayText(3,1,"EXT2 TMP:39C(3.3V)");

        LCD_DisplayText(5,1,"INT  TMP:21C(2.4V)");
        
        LCD_SetTextColor(RGB_RED);     //글자색 레드
	LCD_DisplayText(0,17,"0");
	

	

}
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
void DelayMS(unsigned short wMS)
{
	register unsigned short i;
	for (i=0; i<wMS; i++)
		DelayUS(1000);         	// 1000us => 1ms
}

void DelayUS(unsigned short wUS)
{
	volatile int Dly = (int)wUS*17;
	for(; Dly; Dly--);
}
void _EXTI_Init(void)    //EXTI11(PH11,SW3)           인터럽트 발생 x , 이벤트만   03.21.00  녹  ,exti 트리거만 필요하기 때문
{
    	RCC->AHB1ENR    |= (1<<7); 	// 0x80, RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= (1<<14);	// 0x4000, Enable System Configuration Controller Clock
	
	SYSCFG->EXTICR[2] |= (7<<12);	// 0x7000, EXTI11에 대한 소스 입력은 GPIOH로 설정 (EXTICR3) (reset value: 0x0000)	
	
//	EXTI->FTSR |= (1<<11);		// 0x000800, Falling Trigger Enable  (EXTI11:PH11)
        
        	EXTI->RTSR |= (1<<11);		// 0x000800, Rising Trigger Enable  (EXTI11:PH11)
}
