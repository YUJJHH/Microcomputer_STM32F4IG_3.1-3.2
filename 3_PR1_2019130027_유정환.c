///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Term Project 1: Tracking a Car(자동차 추종시스템)
// 제출자: 2019130027 유정환
// 제출일:2023-12-14
// 동작개요:  전기자동차 2대가 길을 따라 운행하고, 선도차(Lead car)는 random하게 운행하고 추종차(Following car)는 선도차와의 거리와
// 인도와의 거리를 계속 측정하면서 속도와 방향을 조정하며 선도차를 따라가는 동작
// 주요 내용 및 구현 방법
// 거리센서 종류:  ADC3_CH1 (PA1) : 키트가변저항,   ADC3_CH9 (PF3) : 외부 가변저항
// 추종차 엔진: TIM4_CH2(PB7) : 외부 LED로 PWM 변화 확인 
// 추종차 핸들: TIM14_CH1(PF9) : 부저로 PWM 변화 확인
// EXTI12 (SW4) : MOVE-key , EXTI14 (SW6) : STOP-key
// PC 통신 : MOVE-key('M') , STOP-key('S')
// Memory: ADC_value[2]; [0] 키트가변저항, [1] 외부 가변저항 
// ADC설정: ADC3, SCAN mode,  2 ch 변환, DMA mode, 10 bit RES, 
// DMA 설정: DMA2 Stream0 channel 2(ADC3), Peripheral-to-memory (P=>M), DMA_BufferSize = 2 (ADC_Value[2])
//               Circular mode, Priority level - High, FIFO_direct mode, 
//               Peripheral address pointer is fixed, Memory address pointer is incremented after each data transferd 
// USART1: TX pin: PA9, RX pin: PA10 
// TX: Polling 방식, RX: Interrupt 방식
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FRAM.h"
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

uint16_t ADC_value[2];

char str1[20];          //거리센서1 측정 문자열
char str2[20];          //거리센서2 측정 문자열


void TIMER1_OC_Init(void);            // Timer 1 (Output Compare mode)


  int16_t x=0, y=1;      //막대그래프를 나타내기 위한 변수

 void TIMER14_PWM_Init(void);  // 추종차 방향 제어, 주기 400us
void TIMER4_PWM_Init(void);  // 추종차 속도 제어, 주기 5s
  
uint16_t MOVE_FLAG=0; //MOVE 상태 플래그



int main(void)
{

         

        _EXTI_Init();

        USART1_Init();
	_ADC_Init();          
        
        
	DMAInit();
        _GPIO_Init();
        
	LCD_Init();		// LCD 구동 함수
	DelayMS(500);		// LCD구동 딜레이
          
	
	RunMenu();
        TIMER1_OC_Init();      // Timer 1 (Output Compare mode)  , 400ms마다 AD 변환
	
       TIMER14_PWM_Init();  // 추종차 방향 제어, 주기 400us
       TIMER4_PWM_Init();    //추종차 속도제어 , 주기 5초
       
      
         
        Fram_Init();                    // FRAM 초기화 H/W 초기화
	Fram_Status_Config();   // FRAM 초기화 S/W 초기화
 
     
        //FRAM 저장값 리드
        
        MOVE_FLAG=Fram_Read(1201);  
      
        LCD_SetBackColor(RGB_WHITE);	//글자배경색 
        LCD_SetFont(&Gulim7);
        LCD_SetTextColor(RGB_BLUE);     //글자색 
        
        if(MOVE_FLAG==0)  //STOP 상태
        {
            MOVE_FLAG=0;        //MOVE 동작 x      
                
            LCD_DisplayText(1,24," S");  //STOP 표시
            LCD_DisplayText(2,24," 0"); //D1 거리 0
            LCD_DisplayText(3,24," 1"); // D2 거리 1
                       
            TIM4->CCR2=0;  //엔진 PWM  DR=0%
            TIM14->CCR1=0;  //핸들 PWM  DR=0%
                      
            //D1 막대 지움
            LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
            LCD_DrawFillRect(21,23,6*(19),9);    
            
            x=0;   //D1막대 0
            y=1;  // D2막대 1 표시
        
            LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
            LCD_DrawFillRect(21+38*y,34,38*(3-y),9);    
            
            LCD_SetBrushColor(RGB_GREEN);        //브러시색  초록   
            LCD_DrawFillRect(21,34,38*y,9);   
             
            LCD_DisplayText(4,7,"00");  //엔진 PWM DR=0%
            LCD_DisplayText(4,21,"F");  // 핸들 직진 F
                   
        
        }
        
        else if(MOVE_FLAG==1)  // MOVE 상태
        {
             MOVE_FLAG=1;   //MOVE 동작
             LCD_DisplayText(1,24," M");  //MOVE 표시
        }
  
       
       
       
 	while(1)
	{
          LCD_SetTextColor(RGB_BLACK);     //글자색 검정
          LCD_DisplayText(3,0,"D2:");
          LCD_DisplayText(4,0,"SP(DR):");
          LCD_DisplayText(4,9,"%   DIR(DR):");     
          LCD_DisplayText(4,22,"    ");
         
             if(MOVE_FLAG==1)   //MOVE 상태이면
             {       
                    
                   LCD_SetTextColor(RGB_BLUE);     //글자색   
             
                      if(y==0)                   //명령값 = 0 이면   (핸들 오른쪽 )
                      {
                          TIM14->CCR1 = 8*3;		// DR=30%
                          LCD_DisplayText(4,21,"R");
                      }
                      else if(y==1)             //명령값 = 1 이면      (핸들 직진 )
                      {
                          TIM14->CCR1 = 0;		// DR=0%
                           LCD_DisplayText(4,21,"F");
                      }
                      else if(y==2 || y==3)             //명령값 = 2~3 이면     (핸들 왼쪽 )
                      {
                          TIM14->CCR1 = 8*9;		// DR=90%
                           LCD_DisplayText(4,21,"L");
                      }
                      
                      
                      
                      if(x==3 || x==4)                   //측정거리 3~4m이면   
                      {
                          TIM4->CCR2 = 5000*1;		// DR=10%
                          LCD_DisplayText(4,7,"10");
                      }
                      else if(x==5 || x==6)               //측정거리 5~6m이면   
                      {
                           TIM4->CCR2 = 5000*2;		// DR=20%
                           LCD_DisplayText(4,7,"20");
                      }
                      else if(x==7 || x==8)             //측정거리 7~8m이면   
                      {
                           TIM4->CCR2 = 5000*3;		// DR=30%
                           LCD_DisplayText(4,7,"30");
                      }
                      else if(x==9 || x==10)              //측정거리 9~10m이면   
                      {
                           TIM4->CCR2 = 5000*4;		// DR=40%
                           LCD_DisplayText(4,7,"40");
                      }
                     else if(x==11 || x==12)              //측정거리 11~12m이면   
                      {
                           TIM4->CCR2 = 5000*5;		// DR=50%
                           LCD_DisplayText(4,7,"50");
                      }
                      else if(x==13 || x==14)              //측정거리 13~14m이면   
                      {
                           TIM4->CCR2 = 5000*6;		// DR=60%
                           LCD_DisplayText(4,7,"60");
                      }
                      else if(x==15 || x==16)             //측정거리 15~16m이면   
                      {
                           TIM4->CCR2 = 5000*7;		// DR=70%
                           LCD_DisplayText(4,7,"70");
                      }
                      else if(x==17 || x==18)             //측정거리 17~18m이면   
                      {
                          TIM4->CCR2 = 5000*8;		// DR=80%
                          LCD_DisplayText(4,7,"80");
                      }
                      else if(x==19)                       //측정거리 19m이면   
                      {
                           TIM4->CCR2 = 5000*9;		// DR=90%
                           LCD_DisplayText(4,7,"90");
                      }
                       else if(x==0)                       //측정거리 0m이면   
                      {
                          		
                           LCD_DisplayText(4,7,"00");
                      }


             }
            
        }
}

void ADC_IRQHandler(void)
{
       ADC3->SR &= ~(1<<1);		// EOC flag clear 
  
   
       LCD_SetTextColor(RGB_BLUE);     //글자색 블루
        
       if(MOVE_FLAG==1)   
       {    
     
            sprintf(str1,"%2.0f",floor(((3.3*ADC_value[0])*5)/1023+3));  //D1 거리 표시 3~19, floor: 소수점 내림
	     LCD_DisplayText(2,24,str1);
            
             x=atoi(str1); // 문자열  str1을 정수형으로 변환하여 x에 저장
        
             if(x>=0 && x<=19)     
             {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(21+6*x,23,6*(19-x),9);    
             
                 LCD_SetBrushColor(RGB_RED);        //브러시색 빨강   
                 LCD_DrawFillRect(21,23,6*x,9);     
             
             }
             
     
     
            y= ((3.3*ADC_value[1])/1023);           //D2 거리 표시 0~3
            LCD_DisplayChar(3,25,y+0x30);
            
            if((y>=0) && (y<=3))      
            {
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(19,32,116,12);  
                 
                 LCD_SetBrushColor(RGB_GREEN);        //브러시색  초록   
                 LCD_DrawFillRect(21,34,38*y,9);   
            }
                  
             
       }
           
              
}


void _ADC_Init(void)
{
	/* 1st Analog signal */
	RCC->AHB1ENR |= 0x01;  	// RCC_AHB1ENR GPIOA Enable
	GPIOA->MODER |= (3<<2);	// GPIOA PIN1(PA1)        키트의 가변저항(거리센서1) : Analog mode
  
	/* 2nd Analog signal */
         RCC->AHB1ENR |= (1<<5);  	// RCC_AHB1ENR GPIOF Enable
	GPIOF->MODER |= (3<<6);	// GPIOF PIN3(PF3)         거리센서2: Analog mode

	/* ADC Common Init **********************************************************/
	RCC->APB2ENR |= (1<<10);	// RCC_APB2ENR ADC3 Enable
  
	ADC->CCR &= ~0X0000001F;// ADC_Mode_Independent
	ADC->CCR |= 0x00010000;	// ADC_Prescaler_Div4 (ADC MAX Clock 36Mhz, 84Mhz(APB2)/4 = 21Mhz
        
      

	/* ADC3 Init ****************************************************************/
	ADC3->CR1 |= (1<<24);	// RES[1:0]=0b00 : 10bit Resolution
	ADC3->CR1 |= (1<<8);	// ADC_ScanCovMode Enable (SCAN=1)
	ADC3->CR1 |=  (1<<5);		// EOCIE=1: Interrupt enable for EOC
        
        ADC3->CR2 &= ~(1<<1);	// ADC_ContinuousConvMode Disable (CONT=0)            
	ADC3->CR2 |= (3<<28);	// EXTEN[1:0]=0b01: ADC_ExternalTrigConvEdge_Enable (Both Edge)
        
        ADC3->CR2 |= (2<<24);	// EXTSEL:TIM1_CC3
      
        ADC3->CR2 &= ~(1<<10);	//  EOCS =0:Endof conversion selection
	ADC3->CR2 &= ~(1<<11);	// ALIGN=0: ADC_DataAlign_Right
        
	ADC3->SQR1 |= (1<<20); // ADC Regular channel sequece length = 2 conversion

	/* ADC_RegularChannelConfig *********************************************/
   
	ADC3->SMPR2 |= 0x07 << (3*1);	//ADC3_CH1 Sample Time_480Cycles (3*Channel_1)  
	ADC3->SQR3 |= (1<< (5*(1-1)));  //ADC3_CH1 << (5*(Rank-1)), Rank = 1 (1순위로 변환: 키트 가변저항(거리센서1))

       
        ADC3->SMPR2 |= 0x07 << (3*9);	// ADC3_CH9 Sample TIme_480Cycles (3*Channel_1)  
	ADC3->SQR3 |= 9 << (5*(2-1));	// ADC3_CH9 << (5 * (Rank - 1)),  Rank = 2 (2순위로 변환: 가변저항(거리센서2))
 
             
        NVIC->ISER[0] |= (1<<18);	// Enable ADC global Interrupt

/* Enable DMA request after last transfer (Single-ADC mode) */
        ADC3->CR2 |= (1<<9); // DMA requests are issued	as long as data are converted and DMA=1	
				// for single ADC mode
        
	/* Enable ADC1 DMA */
	ADC3->CR2 |= (1<<8);	// DMA mode enabled  (DMA=1)
	ADC3->CR2 |= 0x00000001;	// Enable ADC3:  ADON=1
  	
}

void TIMER1_OC_Init(void) //TIM1_CH3  주기= 400ms
{
// PE13: TIM1_CH3
	// PE13을 출력설정하고 Alternate function(TIM1_CH3)으로 사용 선언
	RCC->AHB1ENR   |= (1<<4);   // RCC_AHB1ENR GPIOE Enable
	GPIOE->MODER   |= (2<<2*13);   // GPIOE PIN13 Output Alternate function mode               
	GPIOE->OSPEEDR |= (3<<2*13);   // GPIOE PIN13 Output speed (100MHz High speed)
	GPIOE->OTYPER  = 0x00000000;   // GPIOE PIN13 Output type push-pull (reset state)
	GPIOE->PUPDR   |= (1<<2*13);   // GPIOE PIN13 Pull-up
	GPIOE->AFR[1]  |= (1<<4*(13-8)); // Connect TIM1 pins(PE13) to AF1(TIM1/2)

	// Timerbase Mode
	RCC->APB2ENR   |= 0x01;// RCC_APB1ENR TIMER1 Enable

	TIM1->PSC = 8400-1;   // Prescaler 168,000,000Hz/8400= 20000Hz (0.05ms)

	
	TIM1->ARR = 8000-1;    // Auto reload  : 0.05ms * 8000 = 400ms(period) 
	
	TIM1->CR1 &= ~(1<<4);   // DIR: Countermode = Upcounter (reset state)
	TIM1->CR1 &= ~(3<<8);   // CKD: Clock division = 0 (reset state)
	TIM1->CR1 &= ~(3<<5);    // CMS(Center-aligned mode Sel): No(reset state)

	TIM1->EGR |= (1<<0);    // UG: Update generation 
    
	// Output/Compare Mode
	TIM1->CCER |= (1<<8);     // CC3E=1: CC3 channel Output Enable 
	                   
	TIM1->CCER &= ~(1<<(4*(3-1)+1));  // CC3P: OCPolarity_Active High

	TIM1->CCR3 = 100;   // TIM1_Pulse

	TIM1->BDTR |= (1<<15);  // main output enable  고급타이머
   
	TIM1->CCMR2 &= ~(3<<8*0); // CC3S(CC1channel): Output 
	TIM1->CCMR2 &= ~(1<<3); // OC3PE: Output Compare 3 preload disable
	TIM1->CCMR2 |= (3<<4);   // OC3M: Output Compare 3 Mode : toggle

	TIM1->CR1 &= ~(1<<7);   // ARPE: Auto reload preload disable      

        
	TIM1->DIER |= (1<<3);   // CC3IE: Enable the Tim1 CC3 interrupt
   
	NVIC->ISER[0] |= (1<<27); // TIM1_CC
	TIM1->CR1 |= (1<<0);   // CEN: Disable the Tim1 Counter 
}


void TIM1_CC_IRQHandler(void)	
{ 
            TIM1->SR &= ~(1<<3);	// CC Interrupt Claer

 // GPIOG->ODR ^= 0x0001; 	// GPIOG->ODR.0 'H'(LED01 O
                     
              if(MOVE_FLAG==1)  //move 상태
              {
                      if(str1[0]==' ')   //거리센서1의 측정값의 십의자리가 공백이면 ( 일의자리만 있으면)
                      {
                        SerialSendChar(str1[1]);   //일의자리 PC로 송신
                        SerialSendChar('m');        // m  PC로 송신
                        SerialSendChar(' ');        //공백 PC로 송신
                      }
                      else           // 거리센서1의 측정값이 두 자리 수이면
                      {
                        SerialSendChar(str1[0]);     //십의자리  PC로 송신
                        SerialSendChar(str1[1]);    //  일의자리  PC로 송신
                        SerialSendChar('m');        // m  PC로 송신
                        SerialSendChar(' ');          //공백 PC로 송신
                      }
              }
              else  //stop 상태 
              {
              SerialSendChar('0');        // '0m '  PC로 송신
              SerialSendChar('m');        
              SerialSendChar(' ');         
              }
              
}




void TIMER14_PWM_Init(void)  // 추종차 방향 제어, 주기 400us,    부저로 표시
{
//PF9/T14_CH1

// Clock Enable : GPIOF & TIMER14
	RCC->AHB1ENR	|= (1<<5);	// GPIOF Enable
	RCC->APB1ENR 	|= (1<<8);	// TIMER14 Enable 
    						
// PF9을 출력설정하고 Alternate function(TIM14_CH1)으로 사용 선언 : PWM 출력
	GPIOF->MODER 	|= (2<<18);	// PF9 Output Alternate function mode					
	GPIOF->OSPEEDR 	|= (3<<18);	// PF9 Output speed (100MHz High speed)
	GPIOF->OTYPER	&= ~(1<<9);	// PF9 Output type push-pull (reset state)
	GPIOF->AFR[1]	|= (9<<4); 	//  Connect TIM14 pins(PF9) to AF9(CAN1/CAN2, TIM12.. TIM14)
					// PF9==> TIM14_CH1
    

        
// TIM14 Channel 1 : PWM 1 mode
	// Assign 'PWM Pulse Period'
	TIM14->PSC	= 420-1;	// Prescaler 84,000,000Hz/420 = 200kHz(0.005ms)  (1~65536)
	TIM14->ARR	= 80-1;	// Auto reload  (0.005ms * 80 = 400us : PWM Period)
    
	// Define the corresponding pin by 'Output'  
	// CCER(Capture/Compare Enable Register) : Enable "Channel 1" 
	TIM14->CCER	|= (1<<0);	// CC1E=1: OC1(TIM14_CH1) Active(Capture/Compare 1 output enable)
					// 해당핀(27번)을 통해 신호출력
	TIM14->CCER	&= ~(1<<1);	// CC1P=0: CC1 output Polarity High (OC1으로 반전없이 출력)

	// Duty Ratio 
	TIM14->CCR1	= 80;		// CCR1 value   , 듀티비=100%

	// 'Mode' Selection : Output mode, PWM 1
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1
	TIM14->CCMR1 	&= ~(3<<0); 	// CC1S(CC1 channel)='0b00' : Output 
	TIM14->CCMR1 	|= (1<<3); 	// OC1PE=1: Output Compare 1 preload Enable

	TIM14->CCMR1	|= (6<<4);	// OC1M: Output compare 1 mode: PWM 1 mode
	TIM14->CCMR1	|= (1<<7);	// OC1CE: Output compare 1 Clear enable

	// CR1 : Up counting & Counter TIM14 enable
	TIM14->CR1 	&= ~(1<<4);	// DIR: Countermode = Upcounter (reset state)
	TIM14->CR1 	&= ~(3<<8);	// CKD: Clock division = 0 (reset state)
	TIM14->CR1 	&= ~(3<<5); 	// CMS(Center-aligned mode Sel): No(reset state)
	TIM14->CR1	&= ~(1<<7);	// ARPE: Auto-reload preload disable
	TIM14->CR1	|= (1<<0);	// CEN: Counter TIM14 enable
}



void TIMER4_PWM_Init(void)  // PWM 1 모드, 주기 5초,  추종차 속도 제어  , LED로 표시
{   
// TIM4_CH2 : PB7 (165번 핀)
// Clock Enable : GPIOB & TIMER4
	RCC->AHB1ENR	|= (1<<1);	// GPIOB CLOCK Enable
	RCC->APB1ENR 	|= (1<<2);	// TIMER4 CLOCK Enable 
    						
// PB7을 출력설정하고 Alternate function(TIM4_CH2)으로 사용 선언 : PWM 출력
	GPIOB->MODER 	|= (2<<14);	//  PB7 Output Alternate function mode					
	GPIOB->OSPEEDR 	|= (3<<14);	// PB7 Output speed (100MHz High speed)
	GPIOB->OTYPER	&= ~(1<<7);	// PB7 Output type push-pull (reset state)
	GPIOB->PUPDR	|= (1<<14);	//  PB7 Pull-up
 	GPIOB->AFR[0]	|= (2<<28);	// (AFR[0]): Connect TIM4 pins(PB7) to AF2(TIM3..5)
    
// TIM4 Channel 2 : PWM 1 mode
	// Assign 'PWM Pulse Period'
	TIM4->PSC	= 8400-1;	// Prescaler 84,000,000Hz/8400 = 10,000 Hz(0.1ms)  (1~65536)
	TIM4->ARR	= 50000-1;	// Auto reload  (0.1ms *50000  = 5s : PWM Period)

	// Setting CR1 : 0x0000 (Up counting)
	TIM4->CR1 &= ~(1<<4);	// DIR=0(Up counter)(reset state)
	TIM4->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled)
	TIM4->CR1 &= ~(1<<2);	// URS=0(Update event source Selection)g events
	TIM4->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM4->CR1 &= ~(1<<7);	// ARPE=0(ARR is buffered): ARR Preload Disable 
	TIM4->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM4->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=00 : Edge-aligned mode(reset state)          
        
 				
	// Define the corresponding pin by 'Output'  
	// CCER(Capture/Compare Enable Register) : Enable "Channel 2" 
	TIM4->CCER	|= (1<<4);	// CC2E=1: OC2(TIM4_CH2) Active(Capture/Compare 2 output enable)
					// 해당핀(165번)을 통해 신호출력
	
        TIM4->CCER	|= (1<<5);	// CC2P=1: CC2 Output Polarity (OCPolarity_LOW : 반전해서 출력)
      
	// Duty Ratio 
	TIM4->CCR2	= 5000;		// CCR2 value 듀티비=10%

	// 'Mode' Selection : Output mode, PWM 1
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1 or Ch2
	TIM4->CCMR1 &= ~(3<<8); // CC2S(CC2 channel)= '0b00' : Output 
	TIM4->CCMR1 &= ~(1<<11); 	// OC2PE=0: Output Compare 2 preload disable

	TIM4->CCMR1	|= (6<<12);	// OC2M=0b110: Output compare 2 mode: PWM 1 mode
     
	TIM4->CCMR1	|= (1<<15);	// OC2CE=1: Output compare 2 Clear enable
	
	//Counter TIM4 enable
	TIM4->CR1	|= (1<<0);	// CEN: Counter TIM4 enable
}

void DMAInit(void)  
{
 	// DMA2 Stream0 channel0 configuration *************************************
	RCC->AHB1ENR |= (1<<22);		//DMA2 clock enable
	DMA2_Stream0->CR |= (2<<25);	        //DMA2 Stream0 channel 2 selected

	// ADC1->DR(Peripheral) ==> ADC_vlaue(Memory)
	DMA2_Stream0->PAR |= (uint32_t)&ADC3->DR;	   //Peripheral address - ADC3->DR(Regular data) Address
	DMA2_Stream0->M0AR |= (uint32_t)&ADC_value;      //Memory address - ADC_Value address 
	DMA2_Stream0->CR &= ~(3<<6);		         //Data transfer direction : Peripheral-to-memory (P=>M)
	DMA2_Stream0->NDTR = 2;			         //DMA_BufferSize = 2 (ADC_Value[2])

	DMA2_Stream0->CR &= ~(1<<9); 	//Peripheral increment mode  - Peripheral address pointer is fixed
	DMA2_Stream0->CR |= (1<<10);	//Memory increment mode - Memory address pointer is incremented after each data transferd 
	DMA2_Stream0->CR |= (1<<11);	//Peripheral data size - halfword(16bit)
	DMA2_Stream0->CR |= (1<<13);	//Memory data size - halfword(16bit)   
	DMA2_Stream0->CR |= (1<<8);	//Circular mode enabled   
	DMA2_Stream0->CR |= (2<<16);	//Priority level - High
        
        DMA2_Stream0->FCR &= ~(1<<2);	//DMA_FIFO_direct mode enabled
	DMA2_Stream0->FCR |= (1<<0);	//DMA_FIFO Threshold_HalfFull , Not used in direct mode

	DMA2_Stream0->CR &= ~(3<<23);	//Memory burst transfer configuration - single transfer
	DMA2_Stream0->CR &= ~(3<<21);	//Peripheral burst transfer configuration - single transfer  
	DMA2_Stream0->CR |= (1<<0);	//DMA2_Stream0 enabled
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
    
	USART_BRR_Configuration(19200); // USART Baud rate Configuration
    
	USART1->CR1	&= ~(1<<12);	// USART_WordLength 8 Data bit
	USART1->CR1	&= ~(1<<10);	// NO USART_Parity
      

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
       
                LCD_SetFont(&Gulim7);
                LCD_SetBackColor(RGB_WHITE);	//글자배경색      
                LCD_SetTextColor(RGB_BLACK);     //글자색 
                LCD_DisplayText(4,0,"SP(DR):");
                LCD_DisplayText(4,9,"%   DIR(DR):     ");
                LCD_SetTextColor(RGB_BLUE);     //글자색 
                
                if(ch=='M')  //M을 수신 받으면
                {   
                      
                    MOVE_FLAG=1;   //MOVE 동작
                    LCD_DisplayText(1,24," M");  //MOVE 표시
                     
                    Fram_Write(1201,MOVE_FLAG);         //MOVE 상태 저장
                    
                }
                if(ch=='S')  //S를 수신 받으면
                {
                    MOVE_FLAG=0;        //MOVE 동작 x      
                        
                    LCD_DisplayText(1,24," S");  //STOP 표시
                    LCD_DisplayText(2,24," 0"); //D1 거리 0
                    LCD_DisplayText(3,24," 1"); // D2 거리 1
                               
                              
                    TIM4->CCR2=0;  //엔진 PWM  DR=0%
                    TIM14->CCR1=0;  //핸들 PWM  DR=0%
                              
                    //D1 막대 지움
                    LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                    LCD_DrawFillRect(21,23,6*(19),9);    
                    
                    x=0;   //D1막대 0
                    y=1;  // D2막대 1 표시
                    LCD_SetBrushColor(RGB_WHITE);     //막대 지우고 생성
                    LCD_DrawFillRect(21+38*y,34,38*(3-y),9);    
                     
                    LCD_SetBrushColor(RGB_GREEN);           
                    LCD_DrawFillRect(21,34,38*y,9);   
                     
                    LCD_DisplayText(4,7,"00");  //엔진 PWM DR=0%
                    LCD_DisplayText(4,21,"F");  // 핸들 직진, F
                           
                    Fram_Write(1201,MOVE_FLAG);         //STOP 상태 저장 
                }    
                 
	} 
	// DR 을 읽으면 SR.RXNE bit(flag bit)는 clear 된다. 즉 clear 할 필요없음 
}


void EXTI15_10_IRQHandler(void)		
{
	LCD_SetTextColor(RGB_BLACK);     //글자색 검정
        LCD_DisplayText(3,0,"D2:");
        LCD_DisplayText(4,0,"SP(DR):");
        LCD_DisplayText(4,9,"%   DIR(DR):");;      
               
             
        if(EXTI->PR & (1<<12))                   // EXTI12 Interrupt Pending(발생) 여부?  
	{
                  EXTI->PR |= (1<<12);       // Pending bit Clear      
                  MOVE_FLAG=1;   //MOVE 동작
                  
                  LCD_SetTextColor(RGB_BLUE);     //글자색 
                  LCD_DisplayText(1,24," M");  //MOVE 표시
                   
                 Fram_Write(1201,MOVE_FLAG);         //MOVE 상태 저장
                 
             
	}
       
        if(EXTI->PR & (1<<14))                   // EXTI14 Interrupt Pending(발생) 여부?  
	{
                EXTI->PR |= (1<<14);       // Pending bit Clear    	
                MOVE_FLAG=0;        //stop 동작      
                
                 LCD_SetTextColor(RGB_BLUE);     //글자색         
                 LCD_DisplayText(1,24," S");  //STOP 표시
                 LCD_DisplayText(2,24," 0"); //D1 거리 0
                 LCD_DisplayText(3,24," 1"); // D2 거리 1
                               
                              
                 TIM4->CCR2=0;  //엔진 PWM  DR=0%
                 TIM14->CCR1=0;  //핸들 PWM  DR=0%
                              
                 //D1 막대 지움
                 LCD_SetBrushColor(RGB_WHITE);     //브러시색 흰색
                 LCD_DrawFillRect(21,23,6*(19),9);    
                  
                 x=0;  //D1막대 0표시
                 y=1;  // D2막대 1 표시
                 LCD_SetBrushColor(RGB_WHITE);     //막대 지우고 생성
                 LCD_DrawFillRect(21+38*y,34,38*(3-y),9);    
                     
                 LCD_SetBrushColor(RGB_GREEN);           
                 LCD_DrawFillRect(21,34,38*y,9);   
                     
                  LCD_SetTextColor(RGB_BLUE);     //글자색   
                
                 LCD_DisplayText(4,7,"00");  //엔진 PWM DR=0%
                 LCD_DisplayText(4,21,"F");  // 핸들 직진, F
                           
                 Fram_Write(1201,MOVE_FLAG);         //STOP 상태 저장 
        
           
	}
}
   


void _EXTI_Init(void)  
{
	RCC->AHB1ENR 	|= 0x00000080;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= 0x00004000;	// Enable System Configuration Controller Clock
			// 0b 0000 0000 0000 0000 0100 0000 0000 0000
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH PIN8~PIN15 Input mode (reset state)				 
	
	SYSCFG->EXTICR[3] |= 0x0007;	// EXTI12에 대한 소스 입력은 GPIOH로 설정
        SYSCFG->EXTICR[3] |= 0x0700; 	// EXTI14에 대한 소스 입력은 GPIOH로 설정

	EXTI->FTSR |= (1<<12);	// EXTI12: Falling Trigger Enable 
	EXTI->IMR   |=  (1<<12);	// EXTI12 인터럽트 mask (Interrupt Enable) 설정

        EXTI->FTSR |= (1<<14);	// EXTI14: Falling Trigger Enable 
	EXTI->IMR   |=  (1<<14);	// EXTI14 인터럽트 mask (Interrupt Enable) 설정

	NVIC->ISER[1] |= (1<< (40-32));	// Enable 'Global Interrupt EXTI12,14'
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
//	RCC->AHB1ENR	|=  0x00000020; // RCC_AHB1ENR : GPIOF(bit#5) Enable							
//	GPIOF->MODER 	|=  0x00040000;	// GPIOF 9 : Output mode (0b01)						
//	GPIOF->OTYPER 	&= ~0x0200;	// GPIOF 9 : Push-pull  	
//	GPIOF->OSPEEDR 	|=  0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 
}

void RunMenu(void)         
{
	LCD_Clear(RGB_WHITE);
	LCD_SetFont(&Gulim7);
	LCD_SetBackColor(RGB_WHITE);	//글자배경색
	LCD_SetTextColor(RGB_BLACK);     //글자색 검정
 	LCD_DisplayText(0,0,"YJH 2019130027");
     
        LCD_DisplayText(1,0,"Tracking Car");
        LCD_DisplayText(2,0,"D1:");
         LCD_DisplayText(3,0,"D2:");
         LCD_DisplayText(4,0,"SP(DR):");
         LCD_DisplayText(4,9,"%   DIR(DR):");
         
       
  	
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


