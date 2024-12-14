/////////////////////////////////////////////////////////////
// HW1: 세계 디지털 시계 제작
// 제출자: 2019130027 유정환
// 주요 내용 및 구현 방법
// -세계 주요도시의 시계를 표시
// -각 도시를 변경하는 기능 
// -지역 시간: TIM2의 Down-Counting mode(100msec) 이용,
//   TIM6: Up Counting mode 이용 ,  TIM9:Output Compare mode 이용
// -지역 시간 reset : 시간 reset 해도 도시별 시차를 유지
// -지역 시간 증가 속도 변경:  100ms <-> 10ms 변경 기능
//////////////////////////////////////////////////////////////
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
void _EXTI_Init(void);
uint16_t KEY_Scan(void);

void TIMER2_Init(void);	// General-purpose Timer 2   (Down Counting mode)
void TIMER6_Init(void);	// Basic-purpose Timer 6       (Up Counting mode)
void TIMER9_OC_Init(void);	// General-purpose Timer 9   (Output Compare mode)

void DisplayInitScreen(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);
void BEEP(void);

int8_t min1, min10, hour,  hour_10, hour_1;  //signed char    인천 시간 변수

uint8_t ORDmin1, ORDmin10, ORDhour, ORDhour_10, ORDhour_1;  //unsigned char    시카고 시간 변수

uint8_t LHRmin1, LHRmin10, LHRhour, LHRhour_10, LHRhour_1;  //unsigned char    런던 시간 변수

uint8_t	SW0_Flag, SW1_Flag, SW2_Flag,SW6_Flag, SW7_Flag;

int click; //처음에 버튼을 눌림을 확인

uint8_t city_name1, city_name2, city_name3;  //도시 변수

int main(void)
{
	_GPIO_Init();
	_EXTI_Init();
	LCD_Init();	
	DelayMS(10);
	BEEP();
   	DisplayInitScreen();	// LCD 초기화면
	GPIOG->ODR &= 0xFF00;// 초기값: LED0~7 Off

	TIMER2_Init();	// TIM2 Init  
       TIMER6_Init();	// TIM6 Init   
       TIMER9_OC_Init();	// TIM9 Init   
 
        
       min1 = 0;     //인천 시간 초기값
       min10 = 5;
       hour = 23;
      
       ORDmin1 = min1;      //시카고 시차 적용
       ORDmin10 = min10;
       ORDhour = hour-14;
             
       LHRmin1= min1;         //런던 시차 적용
       LHRmin10 = min10;   
       LHRhour= hour-8;
            
      
	while(1)              
	{
		
		switch(KEY_Scan())
		{
			case SW0_PUSH  : 	//SW0
                		SW0_Flag=1;      //SW0만 플래그 발생
                                SW1_Flag=0;
                                SW2_Flag=0;       
                                click=0;             //초기에 인천 시간 동작
         		  break;

			case SW1_PUSH  : 	//SW1
                               SW1_Flag=1;      //SW1만 플래그 발생
                               SW0_Flag=0;    
                               SW2_Flag=0;
                                click=1;            //버튼을 누르면 인천 시간에서 탈출
	   		  break;

            		case SW2_PUSH  : 	//SW2
                              SW2_Flag=1;         //SW2만 플래그 발생
                              SW0_Flag=0;
                              SW1_Flag=0;       
                               click=1;               //버튼을 누르면 인천 시간에서 탈출
          	     	break;	
        	}
               
	}
}

void TIMER2_Init(void)
{
	RCC->APB1ENR |= 0x01;	//  RCC_APB1ENR TIMER2 Enable

	TIM2->CR1 |= (1<<4);	// DIR=1(down counter)
	TIM2->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled): By one of following events
                            //  Counter Overflow/Underflow, 
                            //  Setting the UG bit Set,
                            //  Update Generation through the slave mode controller 
                            // UDIS=1 : Only Update event Enabled by  Counter Overflow/Underflow,
	TIM2->CR1 &= ~(1<<2);	// URS=0(Update event source Selection): one of following events
                            //	Counter Overflow/Underflow, 
                            // Setting the UG bit Set,
                            //	Update Generation through the slave mode controller 
                            // URS=1 : Only Update Interrupt generated  By  Counter Overflow/Underflow,
	TIM2->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM2->CR1 |= (1<<7);	// ARPE=1 
	TIM2->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM2->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=00 : Edge-aligned mode(reset state)

	// Setting the Period
	TIM2->PSC = 42-1;	// Prescaler=42, 84MHz/42 = 2MHz (0.5us)
	TIM2->ARR = 200000;	// Auto reload  : 0.5us * 200K = 100ms(period) : 인터럽트주기나 출력신호의 주기 결정

	// Update(Clear) the Counter
	TIM2->EGR |= 0x01;    // UG: Update generation    
               
	NVIC->ISER[0] 	|= (1<<28);	// Enable Timer2 global Interrupt on NVIC
        TIM2->DIER |= (1<<0);	// Enable the Tim2 Update interrupt
	TIM2->CR1 |= (1<<0);	// CEN: Enable the Tim2 Counter  					
}




void TIMER6_Init(void)
{
	RCC->APB1ENR |= 0x10;	// RCC_APB1ENR TIMER6 Enable

	TIM6->CR1 &= ~(1<<4);  // DIR=0(Up counter)(reset state)
	TIM6->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled): By one of following events
                            //  Counter Overflow/Underflow, 
                            //  Setting the UG bit Set,
                            //  Update Generation through the slave mode controller 
                            // UDIS=1 : Only Update event Enabled by  Counter Overflow/Underflow,
	TIM6->CR1 &= ~(1<<2);	// URS=0(Update Request Source  Selection):  By one of following events
                            //	Counter Overflow/Underflow, 
                            // Setting the UG bit Set,
                            //	Update Generation through the slave mode controller 
                            // URS=1 : Only Update Interrupt generated  By  Counter Overflow/Underflow,
	TIM6->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM6->CR1 |= (1<<7);	// ARPE=1
	TIM6->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM6->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=00 (Edge-aligned mode) (reset state)

    // Deciding the Period
	TIM6->PSC = 1680-1;	// Prescaler=1680, 84,000,000Hz/1680 = 50k Hz (0.02ms)  (1~65536)
	TIM6->ARR = 5000-1;	// Auto reload  5000= 100ms   *  50k Hz 

   	// Clear the Counter
	TIM6->EGR |= (1<<0);	// UG(Update generation)=1 
                        // Re-initialize the counter(CNT=0) & generates an update of registers   

	// Setting an UI(UEV) Interrupt 
	NVIC->ISER[1] |= (1<<(54-32)); // Enable Timer6 global Interrupt
 	TIM6->DIER |= (1<<0);	// Enable the Tim6 Update interrupt

	TIM6->CR1 |= (1<<0);	// Enable the Tim6 Counter (clock enable)   

}

void TIMER9_OC_Init(void)        
{
 	RCC->APB2ENR |= 0x00010000;	//  RCC_APB2ENR TIMER9 Enable

	TIM9->CR1 &= ~(1<<4);	// DIR=0(Up counter)(reset state)
	TIM9->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled): By one of following events
                            //  Counter Overflow/Underflow, 
                            //  Setting the UG bit Set,
                            //  Update Generation through the slave mode controller 
                            // UDIS=1 : Only Update event Enabled by  Counter Overflow/Underflow,
	TIM9->CR1 &= ~(1<<2);	// URS=0(Update event source Selection): one of following events
                            //	Counter Overflow/Underflow, 
                            // Setting the UG bit Set,
                            //	Update Generation through the slave mode controller 
                            // URS=1 : Only Update Interrupt generated  By  Counter Overflow/Underflow,
	TIM9->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM9->CR1 |= (1<<7);	// ARPE=1(ARR is buffered): ARR Preload Enable 
	TIM9->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM9->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=00 : Edge-aligned mode(reset state)

	// Setting the Period
	TIM9->PSC = 16800-1;	// Prescaler=16800, 168MHz/16800 = 10kHz (0.1ms)
	TIM9->ARR = 1000-1;	// Auto reload  : 0.1ms * 1K = 100ms(period) : 100ms의 인터럽트주기나 출력신호의 주기 결정
                                                                  
	// Update(Clear) the Counter
	TIM9->EGR |= (1<<0);    // UG: Update generation    

        
// Output Compare 설정
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1 or Ch2
	TIM9->CCMR1 &= ~(3<<0); // CC1S(CC1 channel) = '0b00' : Output 
	TIM9->CCMR1 &= ~(1<<2); // OC1FE=0: Output Compare 1 Fast disable 
	TIM9->CCMR1 |= (1<<3); // OC1PE=1: Output Compare 1 preload Enable 
	TIM9->CCMR1 |= (3<<4);	// OC1M=0b011 (Output Compare 1 Mode : toggle)
				// OC1REF toggles when CNT = CCR1
				
	// CCER(Capture/Compare Enable Register) : Enable "Channel 1" 
      TIM9->CCER |= (1<<0);	// CC1E=1: CC1 channel Output Enable

        
        // OC1(TIM4_CH1) Active: 해당핀(100번)을 통해 신호출력
	TIM9->CCER &= ~(1<<1);	// CC1P=0: CC1 channel Output Polarity (OCPolarity_High : OC1으로 반전없이 출력)  

	// CC1I(CC 인터럽트) 인터럽트 발생시각 또는 신호변화(토글)시기 결정: 신호의 위상(phase) 결정
	
	TIM9->CCR1 = 100;	// TIM9 CCR1 TIM9_Pulse   

	TIM9->DIER |= (1<<0);	// UIE: Enable Tim9 Update interrupt
	TIM9->DIER |= (1<<1);	// CC1IE: Enable the Tim9 CC1 interrupt

	NVIC->ISER[0] |= (1<<24);	// Enable Timer9 global Interrupt on NVIC

	TIM9->CR1 |= (1<<0);	// CEN: Enable the Tim9 Counter  					
}


void TIM2_IRQHandler(void)      
{
      TIM2->SR &= ~0x01;	// Interrupt flag Clear
     
   
       min1++;
        
       if(min1==10)        //시간 증가
       {
                  min1=0;
                  min10++;
                  if(min10==6)
                  {
                        min10=0;
                        hour++;
                        if(hour==24)
                            {
                                hour=0;
                      
                            }
                  }
       }
                
       
                
       if((SW0_Flag==1) || ( click==0 )) //sw0를 누르거나 초기화면 일 때
       {
         city_name1='I';
         city_name2='C';
         city_name3='N';
         LCD_DisplayChar(1,0,city_name1); 
         LCD_DisplayChar(1,1,city_name2); 
         LCD_DisplayChar(1,2,city_name3); 
         
         hour_10 = hour/10;     // 시간 십의자리
         hour_1 = hour%10;     // 시간 일의자리
         LCD_DisplayChar(1,8,min1+0x30);
         LCD_DisplayChar(1,7,min10+0x30);
         LCD_DisplayChar(1,6,':');
         LCD_DisplayChar(1,5,hour_1+0x30);
         LCD_DisplayChar(1,4,hour_10+0x30);
      }
}


void TIM6_DAC_IRQHandler(void)      
{
      TIM6->SR &= ~(1<<0);	// Interrupt flag Clear
      
      ORDmin1++;
        
      if(ORDmin1==10)       //시간 증가
     {
             ORDmin1=0;
             ORDmin10++;
             if(ORDmin10==6)
             {
                   ORDmin10=0;
                   ORDhour++;
                         if(ORDhour==24)
                         {
                          ORDhour=0;
                       
                   
                          }
             }
      }
      
      
     if(SW1_Flag==1)    //sw1을 눌렀을 때
     {
              
      city_name1='O';
      city_name2='R';
      city_name3='D';
      LCD_DisplayChar(1,0,city_name1); 
      LCD_DisplayChar(1,1,city_name2); 
      LCD_DisplayChar(1,2,city_name3); 
                
      ORDhour_10 = ORDhour/10;     // 시간 십의자리
      ORDhour_1 = ORDhour%10;     // 시간 일의자리
      LCD_DisplayChar(1,8,ORDmin1+0x30);
      LCD_DisplayChar(1,7,ORDmin10+0x30);
      LCD_DisplayChar(1,6,':');
      LCD_DisplayChar(1,5,ORDhour_1+0x30);
      LCD_DisplayChar(1,4,ORDhour_10+0x30);
      }
}

void TIM1_BRK_TIM9_IRQHandler(void)      
{
      
       if ((TIM9->SR & 0x01) != RESET)	// Update interrupt flag 
	{
		TIM9->SR &= ~(1<<0);	// Update Interrupt Claer             
	}

      
  	if((TIM9->SR & 0x02) != RESET)	// Capture/Compare 1 interrupt flag                 
	{
		TIM9->SR &= ~(1<<1);	// CC 1 Interrupt Claer
	
                LHRmin1++;
            
                if(LHRmin1==10)         //시간 증가
               {
                      LHRmin1=0;
                      LHRmin10++;
                      if(LHRmin10==6)
                      {
                            LHRmin10=0;
                            LHRhour++;
                                  if(LHRhour==24)
                                  {
                                      LHRhour=0;
                           
                                  }
                      }
               }
               
               
               if(SW2_Flag==1)        //sw2을 눌렀을 때
               {
                city_name1='L';
                city_name2='H';
                city_name3='R';
                LCD_DisplayChar(1,0,city_name1); 
                LCD_DisplayChar(1,1,city_name2); 
                LCD_DisplayChar(1,2,city_name3); 
                  
               LHRhour_10 = LHRhour/10;     // 시간 십의자리
               LHRhour_1 = LHRhour%10;     // 시간 일의자리
                LCD_DisplayChar(1,8,LHRmin1+0x30);
                LCD_DisplayChar(1,7,LHRmin10+0x30);
                LCD_DisplayChar(1,6,':');
                LCD_DisplayChar(1,5,LHRhour_1+0x30);
                LCD_DisplayChar(1,4,LHRhour_10+0x30);
               }
        }
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

void _EXTI_Init(void)
{

        RCC->AHB1ENR 	|= 0x0080;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= 0x4000;	// Enable System Configuration Controller Clock
	
	GPIOH->MODER 	&= 0x0000FFFF;	// GPIOH PIN8~PIN15 Input mode (reset state)				 
	
	SYSCFG->EXTICR[3] |= 0x7000;	// EXTI15(PH15)에 대한 소스 입력은 GPIOH로 설정
	SYSCFG->EXTICR[3] |= 0x0700; 	// EXTI14에 대한 소스 입력은 GPIOH로 설정
        
        EXTI->FTSR |= 0x004000;		// EXTI14: Falling Trigger Enable
 	EXTI->IMR  |= 0x004000;  	// EXTI14 인터럽트 mask (Interrupt Enable) 설정
        
        
	EXTI->FTSR |= 0x00008000;	// EXTI15: Falling Trigger Enable 
   
	EXTI->IMR   |= 0x00008000;	// EXTI15 인터럽트 mask (Interrupt Enable) 설정
 
      
	NVIC->ISER[1] |= ( 1<<(40-32)  );	// Enable 'Global Interrupt EXTI15 ,  EXTI14'
					// Vector table Position 참조
      
}

void EXTI15_10_IRQHandler(void)    // EXTI 10~15 인터럽트 핸들러
{
        if(EXTI->PR & 0x8000)                    // EXTI15 nterrupt Pending?
        {
          EXTI->PR |= 0x8000;                  // Pending bit Clear
       
              if(city_name1=='I'  && city_name2=='C' && city_name3=='N' )  //인천화면 일 때만 가능
              {
                          
                  TIM2->CR1 &= ~0x01;               // CEN: disable
                  GPIOG->ODR ^= 0x0080;	// LED7 Toggle
                  BEEP();          //부저 1회
                  DelayMS(3000);   //3초
                  GPIOG->ODR ^= 0x0080;	// LED7 Toggle
                  TIM2->CNT &= ~0xFFFFFFFF;
                
                  min1 = 0;     //인천 시간 초기값
                  min10 = 0;
                  hour = 0;
                
                  ORDmin1 = min1;
                  ORDmin10 = min10;
                  ORDhour = hour+10;  // 시카고 시차 적용
                
                   LHRmin1= min1;
                   LHRmin10 = min10;
                   LHRhour= hour+16;    //런던 시차 적용
                
                
                  hour_10 = hour/10;                     // 시간 십의자리
                  hour_1 = hour%10;                     // 시간 일의자리
                  LHRhour_10 = LHRhour/10;     // 시간 십의자리
                  LHRhour_1 = LHRhour%10;     // 시간 일의자리
                  ORDhour_10 = ORDhour/10;     // 시간 십의자리
                  ORDhour_1 = ORDhour%10;     // 시간 일의자리
      
                 TIM2->CR1 |= 0x01;                    // CEN: Enable 
               
                  EXTI->PR |= 0x8000;                  // Pending bit Clear
                 }
        }
        
        
       if(EXTI->PR & 0x4000)          // EXTI14 nterrupt Pending?
        {
          EXTI->PR |= 0x4000;                 // Pending bit Clear
          
              if(city_name1=='I'  && city_name2=='C' && city_name3=='N' )    //인천화면 일 때만 가능
              {
              min1++;
        
                   if(min1==10)        //시간 증가
                   {
                              min1=0;
                              min10++;
                              if(min10==6)
                              {
                                    min10=0;
                                    hour++;
                                    if(hour==24)
                                        {
                                            hour=0;
                                  
                                        }
                              }
                   } 
              
              ORDmin1 = min1;
              ORDmin10 = min10;
              LHRmin1= min1;
              LHRmin10 = min10;
                  
             if(hour>=0 && hour <= 13)          //0~13에서 +10을 해도 24를 넘지 않기 위해서
               {
                   ORDhour = hour+10;  // 시카고 시차 적용
               }
             else if(hour>=14 && hour <= 23)       //14~23에서 -14을 해도 음수가 되는 것을 막기 위해서
               {
                   ORDhour = hour-14;  // 시카고 시차 적용
               }
          
             if(hour>=0 && hour <= 7)          //0~7에서 +16을 해도 24를 넘지 않기 위해서
               { 
                   LHRhour= hour+16;    //런던 시차 적용
               }  
             else  if(hour>=8 && hour <= 23)      //8~23에서 -8을 해도 음수가 되는 것을 막기 위해서
               { 
                   LHRhour= hour-8;    //런던 시차 적용
               }  
                    
               
              if((TIM2->ARR == 200000 )&&(TIM6->ARR == 5000-1) && (TIM9->ARR == 1000-1 ))  //인터럽트 주기가 100ms 일 때
                      {
                                TIM2->CR1 &= ~0x01;          //   CEN: disable
                                 TIM6->CR1 &= ~0x01;         // CEN: disable
                                 TIM9->CR1 &= ~0x01;            // CEN: disable
                            
                                 TIM2->ARR = 20000 ;            // 10ms로 변경
                                 TIM6->ARR = 500-1; 
                                 TIM9->ARR = 100-1 ;
                                
                                 TIM2->CNT &= ~0xFFFFFFFF;
                                TIM6->CNT &= ~0xFFFFFFFF;
                                TIM9->CNT &= ~0xFFFFFFFF;
                                  
                       
                                 TIM2->CR1 |= 0x01;               // CEN: Enable 
                                 TIM6->CR1 |= 0x01;               // CEN: Enable 
                                 TIM9->CR1 |= 0x01;                 // CEN: Enable 
                                 LCD_DisplayText(2,11,"10 ");     //LCD 10 ms 표시
                       }
                       else  if((TIM2->ARR == 20000 )&&(TIM6->ARR == 500-1) && (TIM9->ARR == 100-1 ))  //인터럽트 주기가 10ms 일 때
                       {
                                TIM2->CR1 &= ~0x01;
                                 TIM6->CR1 &= ~0x01;
                                 TIM9->CR1 &= ~0x01;
                                 
                                 TIM2->ARR = 200000 ;                // 100ms로 변경
                                 TIM6->ARR = 5000-1; 
                                 TIM9->ARR = 1000-1 ;
                     
                                 TIM2->CNT &= ~0xFFFFFFFF;
                                TIM6->CNT &= ~0xFFFFFFFF;
                                TIM9->CNT &= ~0xFFFFFFFF;
                         
                                 TIM2->CR1 |= 0x01;
                                 TIM6->CR1 |= 0x01;
                                 TIM9->CR1 |= 0x01;
                                 LCD_DisplayText(2,11,"100");   //LCD 100 ms 표시
                       }
              }
              

        }
 
 
}
void BEEP(void)			// Beep for 20 ms 
{ 	GPIOF->ODR |= 0x0200;	// PF9 'H' Buzzer on
	DelayMS(20);		// Delay 20 ms
	GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
}

void DelayMS(unsigned short wMS)
{
	register unsigned short i;
	for (i=0; i<wMS; i++)
		DelayUS(1000);   // 1000us => 1ms
}

void DelayUS(unsigned short wUS)
{
	volatile int Dly = (int)wUS*17;
	for(; Dly; Dly--);
}

void DisplayInitScreen(void)
{
	LCD_Clear(RGB_WHITE);		// 화면 클리어
	LCD_SetFont(&Gulim8);		// 폰트 : 굴림 8
	LCD_SetBackColor(RGB_YELLOW);	// 글자배경색 : Yellow
	LCD_SetTextColor(RGB_BLUE);	// 글자색 : 파랑
        LCD_SetBrushColor(RGB_YELLOW);
        LCD_DrawFillRect(0,0,140, 40);                //노란 배경
	LCD_DisplayText(0,0,"YJH 2019130027"); 
        LCD_DisplayText(2,0,"Int period 100ms");  

        LCD_SetTextColor(RGB_BLACK);	// 글자색 : 검정
        LCD_DisplayText(1,4,"23:50"); 
	LCD_DisplayText(2,11,"100"); 
        
        
        city_name1='I';
        city_name2='C';
        city_name3='N';
          LCD_DisplayChar(1,0,city_name1); 
            LCD_DisplayChar(1,1,city_name2); 
              LCD_DisplayChar(1,2,city_name3); 
              
       
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
		{   	key_flag = 1;		
			DelayMS(10);
			return key;
		}
	}
}
