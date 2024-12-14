//////////////////////////////////////////////////////////////////////
// DMA processing between ADC and memory (peripheral to memory)
// Peripheral: ADC1_CH1 (PA1) : 가변저항
//                ADC1_CH0 (PA0) : 거리센서
// Memory: ADC_value[2]; [0] 가변저항, [1] 거리센서
// 동작개요: 두 아날로그 신호(전압값) SQR에 순서대로 등록(CH1우선)하고, 
//              SCAN mode로 연속 ADC하여 
//              DMA를 통하여 메모리(ADC_value[2])에 저장하고 전압값변환하여 LCD에 표시
// ADC설정: ADC1, SCAN mode, CONT mode, 2 ch 변환, DMA mode, 12 bit RES, 
// DMA 설정: DMA2 Stream0 channel0(ADC1), Peripheral-to-memory (P=>M), DMA_BufferSize = 2 (ADC_Value[2])
//               Circular mode, Priority level - High, FIFO_direct mode, 
//               Peripheral address pointer is fixed, Memory address pointer is incremented after each data transferd 
//////////////////////////////////////////////////////////////////////
#include "stm32f4xx.h"
#include "GLCD.h"

void RunMenu(void);
void _ADC_Init(void);
void DMAInit(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

uint16_t ADC_value[2], V25, Avg_Slope, Vsense;
char str[20];


void  TIMER1_OC_Init(void);      // Timer 1 (Output Compare mode)  , 400ms마다 AD 변환
  int16_t x=0, y=1;      //막대그래프를 나타내기 위한 변수
int main(void)
{
	
	DMAInit();
    
	LCD_Init();		// LCD 구동 함수
	DelayMS(500);		// LCD구동 딜레이
          
	//LCD 초기화면구동 함수
	RunMenu();
   
          TIMER1_OC_Init();      // Timer 1 (Output Compare mode)  , 400ms마다 AD 변환
       
        _ADC_Init();
        
 	while(1)
	{
    /*
		LCD_SetTextColor(RGB_BLACK);    
  
		///// ADC1_CH1 가변저항 전압값 표시
		sprintf(str,"Value : %4d",ADC_value[0]);
		LCD_DisplayText(3,0,str);     
		sprintf(str,"Voltage : %2.2f[V]",(3.3/4095)*ADC_value[0]);  
		LCD_DisplayText(4,0,str);

		///// ADC1_CH0 거리센서 전압값 표시
		sprintf(str,"Value : %4d",ADC_value[1]);
		LCD_DisplayText(6,0,str);     
		sprintf(str,"Voltage : %2.2f[V]",(3.3/4095)*ADC_value[1]); //ADC1_CH0 거리센서 전압값 표시
		LCD_DisplayText(7,0,str);



  
//PB0 가변저항  채널 8
           
                    sprintf(str,"Value : %4d",ADC_value[1]);
		LCD_DisplayText(6,0,str);     
		sprintf(str,"Voltage : %2.2f[V]",(3.3/255)*ADC_value[1]);  
		LCD_DisplayText(7,0,str);
                
             */  
           
	}
}


void ADC_IRQHandler(void)
{
       ADC3->SR &= ~(1<<1);		// EOC flag clear 
  
   
       LCD_SetTextColor(RGB_BLUE);     //글자색 블루
        
        
     
          /*  sprintf(str1,"%2.0f",floor(((3.3*ADC_value[0])*5)/1023+3));  //D1 거리 표시 3~19, floor: 소수점 내림
	     LCD_DisplayText(2,24,str1);
            
             x=atoi(str1); // 문자열  str1을 정수형으로 변환하여 x에 저장
        */
       
       x=floor(((3.3*ADC_value[0])*5)/1023+3);
       
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
                  
        /*  sprintf(str1,"%2.1f",floor(((3.3*ADC_value[0])*5)/1023+3));  //D1 거리 표시 3~19, floor: 소수점 내림
	     LCD_DisplayText(7,20,str1);  
             
              sprintf(str1,"%2d",3.3*ADC_value[0]);  //D1 거리 표시 3~19, floor: 소수점 내림
	//     LCD_DisplayText(8,23,str1);  
             
             sprintf(str1,"%4d",ADC_value[0]);  //D1 거리 표시 3~19, floor: 소수점 내림
	     LCD_DisplayText(9,20,str1); */
       
           
          
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



/*
void ADC_CH8(void)
{

ADC1->SMPR2 |= 0x07 << (24);	// ADC1_CH8 Sample TIme_480Cycles (3*Channel_1)
	ADC1->SQR3 |= 8 << (5*1);	// ADC1_CH8 << (5 * (Rank - 1)),  Rank = 2                  PB0  CH8
        ADC1->CR1 |= (1<<24);	// RES[1:0]=0b00 : 12bit Resolution
}

void ADC_CH0(void)
{

	ADC1->SMPR2 |= 0x07 << (3*1);	// ADC1_CH1 Sample TIme_480Cycles (3*Channel_1)
	ADC1->SQR3 |= 0x01 << (5*(1-1));	// ADC1_CH1 << (5 * (Rank - 1)),  Rank = 1 (1순위로 변환: 가변저항) PA1 CH1
ADC1->CR1 &= ~(3<<24);	// RES[1:0]=0b00 : 12bit Resolution

}*/


void DMAInit(void)
{
 	// DMA2 Stream0 channel0 configuration *************************************
	RCC->AHB1ENR |= (1<<22);		//DMA2 clock enable
	DMA2_Stream0->CR |= ~(2<<25);	//DMA2 Stream0 channel 0 selected

	// ADC1->DR(Peripheral) ==> ADC_vlaue(Memory)
	DMA2_Stream0->PAR |= (uint32_t)&ADC3->DR;	   //Peripheral address - ADC1->DR(Regular data) Address
	DMA2_Stream0->M0AR |= (uint32_t)&ADC_value; //Memory address - ADC_Value address 
	DMA2_Stream0->CR &= ~(3<<6);		  //Data transfer direction : Peripheral-to-memory (P=>M)
	DMA2_Stream0->NDTR = 2;			  //DMA_BufferSize = 2 (ADC_Value[2])

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

void RunMenu(void)
{
	LCD_Clear(RGB_WHITE);
	LCD_SetFont(&Gulim7);
	LCD_SetBackColor(RGB_GREEN);
	LCD_SetTextColor(RGB_BLACK);
 	LCD_DisplayText(0,0,"ADC");

	LCD_SetBackColor(RGB_WHITE);	//글자배경색
	LCD_SetTextColor(RGB_RED);

	LCD_DisplayText(1,0,"ADC 12bit");
	LCD_DisplayText(2,0,"REF 3.3V");

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

