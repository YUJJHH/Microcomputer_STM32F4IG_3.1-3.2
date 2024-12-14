//////////////////////////////////////////////////////////////////////////
// HW4: 가속도값(SPI)를 이용한 Ball game
// 제출자: 2019130027 유정환
// 제출일: 2023-12-02
// 개요: SPI 통신을 이용하여 가속도센서로부터 가속도값(X,Y,Z)을 읽어서 표시하고, 
// 키트를 기울임에 따라 변하는 가속도값을 반영하여 공이 이동하도록 표시
// SPI 통신을 이용한 가속도센서 측정
//  NSS pin:  PA8 (PA4(SPI1_CS) 대신에 사용)
//  SCK pin:  PA5 (SPI1_SCK)
//  MOSI pin: PA7 (SPI1_MOSI)
//  MISO pin: PA6 (SPI1_MISO)
// SPI mode: MASTER
// 가속도센서(LIS2HH12, Slave mode) X,Y,Z 값을 TIM10 CC인터럽트 200ms마다 읽은후 LCD에 표시 
//  공이 벽 라인에 만나면 이동 멈춤(Reset시 경기장 중앙에서 재시작)
//////////////////////////////////////////////////////////////////////////
                 

#include "stm32f4xx.h"
#include "GLCD.h"
#include "ACC.h"

void DisplayTitle(void);
void _GPIO_Init(void);
void SPI1_Init(void);
void TIMER10_OC_Init(void);               //oc mode
void Display_Process(int16 *pBuf);

void TIMER11_Init(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

//// void SPI1_Process(UINT16 *pBuf);  // ACC.c (ACC.h) 
//// void ACC_Init(void) // ACC.c (ACC.h)
//// void LCD_Init(void); // GLCD.c (GLCD.h)

UINT8 bControl;

uint8 x=56 ,y=63;  //위치 초기값

int main(void)
{
	int16 buffer[3];
    
      
        
	LCD_Init();		// LCD 구동 함수
	DelayMS(10);		// LCD구동 딜레이
	DisplayTitle();		// LCD 초기화면구동 함수
     
	_GPIO_Init();		// LED, SW 초기화
	SPI1_Init();        	// SPI1 초기화
	ACC_Init();		// 가속도센서 초기화
	
   
	  TIMER10_OC_Init();		// 가속도센서 스캔 주기 생성
        
         while(1)
	{
		if(bControl)
		{
			bControl = FALSE;     
			SPI1_Process(&buffer[0]);	// SPI통신을 이용하여 가속도센서 측정
			Display_Process(&buffer[0]);	// 측정값을 LCD에 표시
		}
	}
}

///////////////////////////////////////////////////////
// Master mode, Full-duplex, 8bit frame(MSB first), 
// fPCLK/32 Baud rate, Software slave management EN
void SPI1_Init(void)
{
	/*!< Clock Enable  *********************************************************/
	RCC->APB2ENR 	|= (1<<12);	// 0x1000, SPI1 Clock EN
	RCC->AHB1ENR 	|= (1<<0);	// 0x0001, GPIOA Clock EN		
  
	/*!< SPI1 pins configuration ************************************************/
	
	/*!< SPI1 NSS pin(PA8) configuration : GPIO 핀  */
	GPIOA->MODER 	|= (1<<(2*8));	// 0x00010000, PA8 Output mode
	GPIOA->OTYPER 	&= ~(1<<8); 	// 0x0100, push-pull(reset state)
	GPIOA->OSPEEDR 	|= (3<<(2*8));	// 0x00030000, PA8 Output speed (100MHZ) 
	GPIOA->PUPDR 	&= ~(3<<(2*8));	// 0x00030000, NO Pullup Pulldown(reset state)
    
	/*!< SPI1 SCK pin(PA5) configuration : SPI1_SCK */
	GPIOA->MODER 	|= (2<<(2*5)); 	// 0x00000800, PA5 Alternate function mode
	GPIOA->OTYPER 	&= ~(1<<5); 	// 0020, PA5 Output type push-pull (reset state)
	GPIOA->OSPEEDR 	|= (3<<(2*5));	// 0x00000C00, PA5 Output speed (100MHz)
	GPIOA->PUPDR 	|= (2<<(2*5)); 	// 0x00000800, PA5 Pull-down
	GPIOA->AFR[0] 	|= (5<<(4*5));	// 0x00500000, Connect PA5 to AF5(SPI1)
    
	/*!< SPI1 MOSI pin(PA7) configuration : SPI1_MOSI */    
	GPIOA->MODER 	|= (2<<(2*7));	// 0x00008000, PA7 Alternate function mode
	GPIOA->OTYPER	&= ~(1<<7);	// 0x0080, PA7 Output type push-pull (reset state)
	GPIOA->OSPEEDR 	|= (3<<(2*7));	// 0x0000C000, PA7 Output speed (100MHz)
	GPIOA->PUPDR 	|= (2<<(2*7)); 	// 0x00008000, PA7 Pull-down
	GPIOA->AFR[0] 	|= (5<<(4*7));	// 0x50000000, Connect PA7 to AF5(SPI1)
    
	/*!< SPI1 MISO pin(PA6) configuration : SPI1_MISO */
	GPIOA->MODER 	|= (2<<(2*6));	// 0x00002000, PA6 Alternate function mode
	GPIOA->OTYPER 	&= ~(1<<6);	// 0x0040, PA6 Output type push-pull (reset state)
	GPIOA->OSPEEDR 	|= (3<<(2*6));	// 0x00003000, PA6 Output speed (100MHz)
	GPIOA->PUPDR 	|= (2<<(2*6));	// 0x00002000, PA6 Pull-down
	GPIOA->AFR[0] 	|= (5<<(4*6));	// 0x05000000, Connect PA6 to AF5(SPI1)

	// Init SPI1 Registers 
	SPI1->CR1 |= (1<<2);	// MSTR(Master selection)=1, Master mode        
	SPI1->CR1 &= ~(1<<15);	// SPI_Direction_2 Lines_FullDuplex          
	SPI1->CR1 &= ~(1<<11);	// SPI_DataSize_8bit                         
	SPI1->CR1 |= (1<<9);  	// SSM(Software slave management)=1,           
				// NSS 핀 상태가 코딩에 의해 결정
	SPI1->CR1 |= (1<<8);	// SSI(Internal_slave_select)=1,               
				// 현재 MCU가 Master이므로 NSS 상태는 'High'            
	SPI1->CR1 &= ~(1<<7);	// LSBFirst=0, MSB transmitted first                  
	SPI1->CR1 |= (4<<3);	// BR(BaudRate)=0b100, fPCLK/32 (84MHz/32 = 2.625MHz)        
	SPI1->CR1 |= (1<<1);	// CPOL(Clock polarity)=1, CK is 'High' when idle
	SPI1->CR1 |= (1<<0);	// CPHA(Clock phase)=1, 두 번째 edge 에서 데이터가 샘플링
 
	SPI1->CR1 |= (1<<6);	// SPE=1, SPI1 Enable
}




void TIMER10_OC_Init(void)
{
// PB8: TIM10_CH1

// PB8을 출력설정하고 Alternate function(TIM10_CH1)으로 사용 선언
	RCC->AHB1ENR	|= (1<<1);	// RCC_AHB1ENR GPIOB Enable 

	GPIOB->MODER    |= (2<<16);	// GPIOB PIN8 Output Alternate function mode 					
	GPIOB->OSPEEDR 	|= (3<<16);	// GPIOB PIN8 Output speed (100MHz High speed)
	GPIOB->OTYPER	&= ~(1<<8);	// ~0x1000, GPIOB PIN8 Output type push-pull (reset state)
	GPIOB->PUPDR    |= (1<<16); 	// 0x01000000, GPIOB PIN8 Pull-up
  					
	GPIOB->AFR[1]	|= (3<<0);  //  Connect TIM10 pins(PB8) to AF3(TIM8..11)
 
// Time base 설정
	RCC->APB2ENR |= (1<<17);	//  RCC_APB2ENR TIMER10 Enable

	TIM10->CR1 &= ~(1<<4);	// DIR=0(Up counter)(reset state)
	TIM10->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled): By one of following events
                            //  Counter Overflow/Underflow, 
                            //  Setting the UG bit Set,
                            //  Update Generation through the slave mode controller 
                            // UDIS=1 : Only Update event Enabled by  Counter Overflow/Underflow,
	TIM10->CR1 &= ~(1<<2);	// URS=0(Update event source Selection): one of following events
                            //	Counter Overflow/Underflow, 
                            // Setting the UG bit Set,
                            //	Update Generation through the slave mode controller 
                            // URS=1 : Only Update Interrupt generated  By  Counter Overflow/Underflow,
	TIM10->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM10->CR1 &= ~(1<<7);	// ARPE=0(ARR is buffered): ARR Preload disable
	TIM10->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM10->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=00 : Edge-aligned mode(reset state)

	// Setting the Period
	TIM10->PSC = 8400-1;	// Prescaler=16800, 168MHz/8400 = 20kHz (0.05ms)
	TIM10->ARR = 4000-1;	// Auto reload  : 0.05ms * 4000 = 200ms(period) 

	// Update(Clear) the Counter
	TIM10->EGR |= (1<<0);    // UG: Update generation    

// Output Compare 설정
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1 or Ch2
	TIM10->CCMR1 &= ~(3<<0); // CC1S(CC1 channel) = '0b00' : Output 
	TIM10->CCMR1 &= ~(1<<2); // OC1FE=0: Output Compare 1 Fast disable 
	TIM10->CCMR1 &= ~(1<<3); // OC1PE=0: Output Compare 1 preload disable(CCR1에 언제든지 새로운 값을 loading 가능) 
	TIM10->CCMR1 |= (3<<4);	// OC1M=0b011 (Output Compare 1 Mode : toggle)
				// OC1REF toggles when CNT = CCR1
				
	// CCER(Capture/Compare Enable Register) : Enable "Channel 1" 
	TIM10->CCER |= (1<<0);	// CC1E=1: CC1 channel Output Enable
	TIM10->CCER &= ~(1<<1);	// CC1P=0: CC1 channel Output Polarity (OCPolarity_High : OC1으로 반전없이 출력)  

	TIM10->CCR1 = 100;	// TIM10 CCR1 

	TIM10->DIER |= (1<<1);	// CC1IE: Enable the Tim10 CC1 interrupt

	NVIC->ISER[0] 	|= (1<<25);	// Enable Timer10 global Interrupt on NVIC

	TIM10->CR1 |= (1<<0);	// CEN: Enable the Tim10 Counter  					
}


void TIM1_UP_TIM10_IRQHandler(void)	// 200ms int
{
	if((TIM10->SR & 0x02) != RESET)	// Capture/Compare 1 interrupt flag
	{
            TIM10->SR &= ~(1<<1);	//Interrupt flag Clear

            bControl = TRUE;	// 200ms마다 센서 측정
       }
}

void Display_Process(int16 *pBuf)
{
	UINT16 G_VALUE;
        
        LCD_SetFont(&Gulim7);		//폰트 7
        LCD_SetTextColor(RGB_RED);    //글자색 빨강
        
	// X 축 가속도 표시		
	if (pBuf[0] < 0)  //음수
	{
		G_VALUE = abs(pBuf[0]);

		LCD_DisplayChar(1,22,'-'); // g 부호 표시
           
	}
	else if(pBuf[0] > 0)	// 양수
	{
		G_VALUE = pBuf[0];

		LCD_DisplayChar(1,22,'+'); // g 부호 표시
                
   
	}
	
        G_VALUE = 10 * G_VALUE / 0x4009; // 가속도 --> g 변환
      
        LCD_DisplayChar(1,23, G_VALUE/10 +0x30);
	LCD_DisplayChar(1,24,'.');
	LCD_DisplayChar(1,25, G_VALUE%10/1 +0x30);
       
        
        if(x>3 && x<112-6-1)  // 공의 x축 이동 범위
        {  
              if(y>13 && y<125-6-1)  // 공의 y축 이동 범위
              {  
                        if (pBuf[0] > 0)  //x축 값 양수 , 공 왼쪽 이동
                        {
                                x=x-G_VALUE;
                            
                                LCD_SetBrushColor(RGB_WHITE);        //먼저 흰색으로 지우고 공 생성
                                LCD_DrawFillRect(x+G_VALUE,y,8,7); 
                                LCD_SetPenColor(RGB_RED);        
                                LCD_DrawRectangle(x,y,6,6);      //공
                        }        
                        else if (pBuf[0] < 0)  //x축 값 음수  , 공 오른쪽 이동
                        {
                                x=x+G_VALUE;
                               
                                LCD_SetBrushColor(RGB_WHITE);        //먼저 흰색으로 지우고 공 생성
                                LCD_DrawFillRect(x-G_VALUE,y,8,7);   
                                LCD_SetPenColor(RGB_RED);       
                                LCD_DrawRectangle(x,y,6,6);         //공
                        }
               }
        }
   
     
     
     
 	// Y 축 가속도 표시	
	if (pBuf[1] < 0)  //음수
	{
		G_VALUE = abs(pBuf[1]);
		
		LCD_DisplayChar(2,22,'-'); // g 부호 표시
	}
	else		 // 양수
	{
		G_VALUE = pBuf[1];
	
		LCD_DisplayChar(2,22,'+'); // g 부호 표시
	}
      
        
	G_VALUE = 10 * G_VALUE / 0x4009; 
	LCD_DisplayChar(2,23, G_VALUE/10 +0x30);
	LCD_DisplayChar(2,24,'.');
	LCD_DisplayChar(2,25, G_VALUE%10/1 +0x30);


       if(x>3 && x<112-6-1)     // 공의 x축 이동 범위
       {   
              if(y>13 && y<125-6-1)  // 공의 y축 이동 범위
              {  
      
                     if (pBuf[1] > 0)  //y축 값 양수, 공 위로 이동
                     {
                            y=y-G_VALUE;
               
                            LCD_SetBrushColor(RGB_WHITE);        //먼저 흰색으로 지우고 공 생성
                            LCD_DrawFillRect(x,y+G_VALUE,7,8);
                            LCD_SetPenColor(RGB_RED);       
                            LCD_DrawRectangle(x,y,6,6);           //공
                     }
                     else if (pBuf[1] < 0)  //y축 값 음수,  공 아래로 이동
                     {
                            y=y+G_VALUE;
              
                            LCD_SetBrushColor(RGB_WHITE);        //먼저 흰색으로 지우고 공 생성
                            LCD_DrawFillRect(x,y-G_VALUE,7,8);   
                            LCD_SetPenColor(RGB_RED);      
                            LCD_DrawRectangle(x,y,6,6);       //공
                     }
              }
        }  
     
       
       
	// Z 축 가속도 표시	
	if (pBuf[2] < 0)  //음수
	{
		G_VALUE = abs(pBuf[2]);
		LCD_DisplayChar(3,22,'-'); // g 부호 표시
	}
	else				// 양수
	{
		G_VALUE = pBuf[2];
		LCD_DisplayChar(3,22,'+'); // g 부호 표시
	}
	G_VALUE = 10 * G_VALUE / 0x4009; 
	LCD_DisplayChar(3,23, G_VALUE/10 +0x30);
	LCD_DisplayChar(3,24,'.');
	LCD_DisplayChar(3,25, G_VALUE%10/1 +0x30);

        
        
        //공이랑 외벽이랑 닿으면
        
        if(x+6>112 && x<130)       //공이 오른쪽 벽을 넘어가면
       {
       
             LCD_SetBrushColor(RGB_WHITE);        //브러시색 흰색
             LCD_DrawFillRect(107,y,15,15);   //넘어간 부분 지움
             
             LCD_SetPenColor(RGB_BLUE);         // 펜색            
             LCD_DrawRectangle(2,12,110,113);   //사각 외벽
              
             LCD_SetPenColor(RGB_RED);        
             LCD_DrawRectangle(112-6-1,y,6,6);  //공
       }
       else if(x<=2 || x>130)       //공이 왼쪽 벽을 넘어가면
       {
       
             LCD_SetBrushColor(RGB_WHITE);        //브러시색 흰색
             LCD_DrawFillRect(0,y,15,15);   //넘어간 부분 지움
             
             LCD_SetPenColor(RGB_BLUE);         // 펜색            
             LCD_DrawRectangle(2,12,110,113);   //사각 외벽
              
             LCD_SetPenColor(RGB_RED);        
             LCD_DrawRectangle(3,y,6,6);  //공
       }
       
       
       
       if(y<=12)       //공이 위쪽 벽을 넘어가면
       {
       
             LCD_SetBrushColor(RGB_WHITE);        //브러시색 흰색
             LCD_DrawFillRect(x,4,15,15);      //넘어간 부분 지움
             
             LCD_SetPenColor(RGB_BLUE);         // 펜색            
             LCD_DrawRectangle(2,12,110,113);   //사각 외벽
              
             LCD_SetPenColor(RGB_RED);        
             LCD_DrawRectangle(x,13,6,6);  //공
       }
       else if(y+6>=125)       //공이 아래쪽 벽을 넘어가면
       {
       
             LCD_SetBrushColor(RGB_WHITE);        //브러시색 흰색
             LCD_DrawFillRect(x,118,15,15);   //넘어간 부분 지움
             
             LCD_SetPenColor(RGB_BLUE);         // 펜색            
             LCD_DrawRectangle(2,12,110,113);   //사각 외벽
             
             LCD_SetPenColor(RGB_RED);        
             LCD_DrawRectangle(x,125-6-1,6,6);  //공
       }
     
        // 사라진 부분 재생성
        LCD_SetTextColor(RGB_BLACK);    //글자색
	LCD_DisplayText(0,0,"Ball game: YJH 2019130027");  // Title
		
	LCD_DisplayText(1,19,"Ax:");	//X AXIS
	LCD_DisplayText(2,19,"Ay:");	//Y AXIS
	LCD_DisplayText(3,19,"Az:");	//Z AXIS
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

	GPIOG->ODR &= 0x00;	// LED0~7 Off 
}

void DelayMS(unsigned short wMS)
{
	register unsigned short i;

	for (i=0; i<wMS; i++)
		DelayUS(1000);		//1000us => 1ms
}

void DelayUS(unsigned short wUS)
{
        volatile int Dly = (int)wUS*17;
         for(; Dly; Dly--);
}

void DisplayTitle(void)
{
	LCD_Clear(RGB_WHITE);
	LCD_SetFont(&Gulim7);		//폰트  7
	LCD_SetBackColor(RGB_WHITE);    //글자배경색 흰색
	LCD_SetTextColor(RGB_BLACK);    //글자색
	LCD_DisplayText(0,0,"Ball game: YJH 2019130027");  // Title
		
	LCD_DisplayText(1,19,"Ax:-1.1");	//X AXIS
	LCD_DisplayText(2,19,"Ay:+0.7");	//Y AXIS
	LCD_DisplayText(3,19,"Az:+1.2");	//Z AXIS
        
  
         LCD_SetBrushColor(RGB_RED);        //브러시색 
         LCD_DrawRectangle(56,63,6,6);           //공 초기위치
         
         
         LCD_SetPenColor(RGB_BLUE);  // 펜색            
         LCD_DrawRectangle(2,12,110,113);   //x길이 110, y길이 113 
}


