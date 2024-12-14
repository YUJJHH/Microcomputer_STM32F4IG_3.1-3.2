///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HW2: STEP Motor �� DC Moter ���� ����̺��ȣ �߻�
// ������: 2019130027 ����ȯ
// �ֿ� ���� �� ���� ���
// -STEP Motor�� ��ġ���� ��� : TIM5�� External Clock Mode 1,Timer 4 (up counting mode), Timer 1(Output Compare mode)
// -DC Motor�� ��ũ���� ��� : TIM8�� External Clock Mode 1, Timer 3 (up counting mode),  Timer 14 (PWM mode)
// -Ǫ����ư�� ���� ��ũ�� �����Ͽ� ���� ���
// -SW3�� ���� ��ġ�� �����Ͽ� �޽� �߻�
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

// Joystick Define
#define NAVI_PUSH	0x03C0  //PI5 0000 0011 1100 0000 
#define NAVI_UP		0x03A0  //PI6 0000 0011 1010 0000 
#define NAVI_DOWN	0x0360  //PI7 0000 0011 0110 0000 
#define NAVI_RIGHT	0x02E0  //PI8 0000 0010 1110 0000 
#define NAVI_LEFT	0x01E0  //PI9 0000 0001 1110 0000 

void _GPIO_Init(void);
uint16_t KEY_Scan(void);
uint16_t JOY_Scan(void);	// Joystick �Է� �Լ�
void DisplayInitScreen(void);

void TIMER1_OC_Init(void);	    // Timer 1 (Output Compare mode)
void TIMER14_PWM_Init(void);       // Timer 14 (PWM mode)

void TIMER3_UP_COUNTING_Init(void);      // Timer 3(up counting mode)
void TIMER4_UP_COUNTING_Init(void);      // Timer 4 (up counting mode)

void TIMER5_PULSE_COUNTER_Init(void);         //External Clock Mode 1
void TIMER8_PULSE_COUNTER_Init(void);          //External Clock Mode 1


void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);
void BEEP(void);




uint8_t pulse_num1_TIM5=0;   //TIM5->CNT ���� �д� ����
uint8_t pulse_num2_TIM8=0;   //TIM8->CNT ���� �д� ����


UINT16 INT_COUNT=0;  //���ͷ�Ʈ Ƚ�� ī��Ʈ

UINT16 count_old, count_new;  //��ġ ��ɰ��� �����ϱ� ���� ����
int main(void)
{
	_GPIO_Init();
	LCD_Init();		// LCD ���� �Լ�
	DelayMS(10);		// LCD���� ������
	DisplayInitScreen();	// LCD �ʱ�ȭ�鱸�� �Լ�
	BEEP();

	TIMER1_OC_Init();	// Timer 1 (Output Compare mode)
        
        TIMER3_UP_COUNTING_Init();                  // Timer 3(up counting mode)
        TIMER4_UP_COUNTING_Init();                  // Timer 4(up counting mode)
        
        TIMER5_PULSE_COUNTER_Init();                  //External Clock Mode 1
        TIMER8_PULSE_COUNTER_Init();                    //External Clock Mode 1
        
        TIMER14_PWM_Init();	// TIM14 Init (PWM mode)
   

	GPIOG->ODR &= ~0x00FF;	// �ʱⰪ: LED0~7 Off
        
       
     
     
	while(1)
	{
		
                 if(pulse_num2_TIM8==0)                   //��ɰ�(��ũ ��) = 0 �̸�
                  {
                      TIM14->CCR1 = (80-0);		// DR=0%
                  }
                  else if(pulse_num2_TIM8==1)           //��ɰ�(��ũ ��) = 1 �̸�
                  {
                      TIM14->CCR1 = (80-8);		// DR=10%
                  }
                  else if(pulse_num2_TIM8==2)            //��ɰ�(��ũ ��) = 2 �̸�
                  {
                      TIM14->CCR1 = (80-16);		// DR=20%
                  }
                  else if(pulse_num2_TIM8==3)           //��ɰ�(��ũ ��) = 3 �̸�
                  {
                      TIM14->CCR1 = (80-24);		// DR=30%
                  }
                  else if(pulse_num2_TIM8==4)          //��ɰ�(��ũ ��) = 4�̸�
                  {
                      TIM14->CCR1 = (80-32);		// DR=40%
                  }
                  else if(pulse_num2_TIM8==5)            //��ɰ�(��ũ ��) = 5 �̸�
                  {
                      TIM14->CCR1 = (80-40);		// DR=50%
                  }
                  else if(pulse_num2_TIM8==6)             //��ɰ�(��ũ ��) = 6 �̸�
                  {
                      TIM14->CCR1 = (80-48);		// DR=60%
                  }
                  else if(pulse_num2_TIM8==7)             //��ɰ�(��ũ ��) = 7 �̸�
                  {
                      TIM14->CCR1 = (80-56);		// DR=70%
                  }
        
           
	}
}


//����(1)
void TIMER1_OC_Init(void) //TIM1_CH3 CC 
{
	// PE13: TIM1_CH3
	// PE13�� ��¼����ϰ� Alternate function(TIM1_CH3)���� ��� ����
	RCC->AHB1ENR   |= (1<<4);   // RCC_AHB1ENR GPIOE Enable
	GPIOE->MODER   |= (2<<2*13);   // GPIOE PIN13 Output Alternate function mode               
	GPIOE->OSPEEDR |= (3<<2*13);   // GPIOE PIN13 Output speed (100MHz High speed)
	GPIOE->OTYPER  = 0x00000000;   // GPIOE PIN13 Output type push-pull (reset state)
	GPIOE->PUPDR   |= (1<<2*13);   // GPIOE PIN13 Pull-up
	GPIOE->AFR[1]  |= (1<<4*(13-8)); // Connect TIM1 pins(PE13) to AF1(TIM1/2)

	
	// Timerbase Mode
	RCC->APB2ENR   |= 0x01;     // RCC_APB2ENR TIMER1 Enable

	TIM1->PSC = 8400-1;   // Prescaler 168,000,000Hz/8400= 20000Hz (0.05ms)

	
	TIM1->ARR = 10000-1;    // Auto reload  : 0.05ms * 10000 = 0.5s(period) 
	
	TIM1->CR1 &= ~(1<<4);   // DIR: Countermode = Upcounter (reset state)
	TIM1->CR1 &= ~(3<<8);   // CKD: Clock division = 0 (reset state)
	TIM1->CR1 &= ~(3<<5);    // CMS(Center-aligned mode Sel): No(reset state)

	TIM1->EGR |= (1<<0);    // UG: Update generation 
    
	// Output/Compare Mode
	TIM1->CCER &= ~(1<<4*(3-1));   // CC3E: OC3  not Active 
      
        TIM1->CCER |= (1<<(4*(3-1)+1));  // CC3P: OCPolarity_Active Low 

	TIM1->CCR3 = 100;   // TIM1_Pulse

	TIM1->BDTR |= (1<<15);  // main output enable  
   
	TIM1->CCMR2 &= ~(3<<8*0); // CC3S(CC1channel): Output 
	TIM1->CCMR2 &= ~(1<<3); // OC3PE: Output Compare 3 preload disable
	TIM1->CCMR2 |= (3<<4);   // OC3M: Output Compare 3 Mode : toggle

	TIM1->CR1 &= ~(1<<7);   // ARPE: Auto reload preload disable     

        
	TIM1->DIER |= (1<<3);   // CC3IE: Enable the Tim1 CC3 interrupt
   
	NVIC->ISER[0] |= (1<<27); // TIM1_CC
	TIM1->CR1 &= ~(1<<0);   // CEN: Disable the Tim1 Counter 
}


void TIM1_CC_IRQHandler(void)      //0.5s Interrupt
{
	if ((TIM1->SR & 0x08) != RESET)	// CC3 interrupt flag 
	{
		TIM1->SR &= ~0x08;	       // CC3 Interrupt Claer
              
                 INT_COUNT++;                         //���ͷ�Ʈ Ƚ�� ī��Ʈ
                 if(INT_COUNT >= 4*pulse_num1_TIM5)      // ��� �޽��� ���� 
                 {
                      TIM1->CCER &= ~(1<<4*(3-1));     // CC3E Disable 
                      TIM1->CR1 &= ~(1<<0);                 // TIM1 Disable
                      INT_COUNT= 0;                               //Ƚ�� �ʱ�ȭ
               
                 }
              
	}
}


void TIMER5_PULSE_COUNTER_Init(void)
{  
//PH11/TIM5_CH2
// Clock Enable : GPIOH & TIMER5
	RCC->AHB1ENR	|= (1<<7);	// 0x0080, GPIOH Enable
	RCC->APB1ENR 	|= (1<<3);	// 0x0008, TIMER5 Enable 


// PH11�� �Է¼����ϰ� Alternate function(TIM5_CH2)���� ��� ����
	GPIOH->MODER 	|= (2<<22);	// GPIOH PIN11 intput Alternate function mode 					
	GPIOH->OSPEEDR 	|= (2<<22);	// GPIOH PIN11 Output speed (50MHz High speed)
	GPIOH->PUPDR	&= ~(3<<22); 	// GPIOH PIN11 NO Pull-up
  					// PH11 ==> TIM5_CH2
	GPIOH->AFR[1]	|= (2<<12);	// Connect TIM5 pins(PH11) to AF2(TIM3..5)
  
        
// Time base Mode
	TIM5->CR1 &= ~(1<<4);	// DIR=0(Up counter)(reset state)
	TIM5->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled)
	TIM5->CR1 &= ~(1<<2);	// URS=0(Update event source Selection)
	TIM5->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM5->CR1 |=  (1<<7);	// ARPE=1(ARR Preload Enable)
	TIM5->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM5->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=11 (center-aligned mode) 

	// PSC, ARR
	TIM5->PSC = 1-1;	// Prescaler=1
	TIM5->ARR = 4;	        // Auto reload  :  count�� ����: 0~4
        
	// Update(Clear) the Counter
	TIM5->EGR |= (1<<0);    // UG=1, REG's Update (CNT clear) 

// External Clock Mode 1
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1 or Ch2
	TIM5->CCMR1 |= (1<<8); 	// CC2S(CC2 channel) = '0b01' : Input 
	TIM5->CCMR1 &= ~(15<<12); // IC2F='0b0000: No Input Filter 
				
	// CCER(Capture/Compare Enable Register) : Enable "Channel 2" 
	TIM5->CCER &= ~(1<<4);	// CC2E=0: Capture Disable          
	// TI2FP2    inverting / falling Edge  
	TIM5->CCER |= (1<<5);	// CC2P=1 
	TIM5->CCER &= ~(1<<7);	// CC2NP=0   

	// SMCR(Slave Mode Control Reg.) : External Clock Enable
	TIM5->SMCR |= (6<<4);	// TS(Trigger Selection)=0b110 :TI2FP2(Filtered Timer Input 2 ��½�ȣ)
	TIM5->SMCR |= (7<<0);	// SMS(Slave Mode Selection)=0b111 : External Clock Mode 1

      
        
	TIM5->CR1 |= (1<<0);	// CEN: Enable the Tim4 Counter  	
}


void TIMER4_UP_COUNTING_Init(void)
{
	RCC->APB1ENR |= (1<<2);	// RCC_APB1ENR TIMER4 Enable
    
	// Setting CR1 : 0x0000 
	TIM4->CR1 &= ~(1<<4);	// DIR=0(Up counter)(reset state)
	TIM4->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled): By one of following events				
	TIM4->CR1 &= ~(1<<2);	// URS=0(Update event source Selection): one of following events				
	TIM4->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM4->CR1 &= ~(1<<7);	// ARPE=0(ARR is NOT buffered) (reset state)
	TIM4->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM4->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=00 (Edge-aligned mode) (reset state)
			
   
	// Deciding the Period
	TIM4->PSC = 8400-1;	// Prescaler 84,000,000Hz/8400 = 10,000 Hz (0.1ms)  (1~65536)
	TIM4->ARR = 1000-1;	// Auto reload  0.1ms * 1000 = 100ms

	// Clear the Counter
	TIM4->EGR |= (1<<0);	// UG(Update generation)=1 (Re-initialize the counter & generates an update of the registers)   

	// Setting an UI(UEV) Interrupt 
	NVIC->ISER[0] |= (1<<30); // Enable Timer4 global Interrupt
 	TIM4->DIER |= (1<<0);	// Enable the Tim4 Update interrupt

	TIM4->CR1 |= (1<<0);	// Enable the Tim4 Counter (clock enable)   
}


void TIM4_IRQHandler(void)  	// 100ms  Interrupt
{
	TIM4->SR &= ~(1<<0);	// Interrupt flag Clear
	

 
        pulse_num1_TIM5= TIM5->CNT;             //TIM5->CNT ����  ����

        LCD_DisplayChar(3,10,0x30+pulse_num1_TIM5);   // ���ܸ��� ��ġ�� ǥ��
            
        
        
        count_old=count_new;      
        count_new=TIM5->CNT;      //tim5�� ���� ����
        
        if(count_new>count_old)       //tim5�� ���� �����  
        {
           TIM1->CCER |= (1<<4*(3-1));                     // CC3E Enable
            TIM1->CR1 |= (1<<0);                    	// TIM1 Enable
        }
        
 

}



//����(2)
void TIMER3_UP_COUNTING_Init(void)
{
	RCC->APB1ENR |= 0x02;	// RCC_APB1ENR TIMER3 Enable
    
	// Setting CR1 : 0x0000 
	TIM3->CR1 &= ~(1<<4);	// DIR=0(Up counter)(reset state)
	TIM3->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled): By one of following events
	TIM3->CR1 &= ~(1<<2);	// URS=0(Update event source Selection): one of following events
	TIM3->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM3->CR1 &= ~(1<<7);	// ARPE=0(ARR is NOT buffered) (reset state)
	TIM3->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM3->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=00 (Edge-aligned mode) (reset state)
 
	// Deciding the Period
	TIM3->PSC = 8400-1;	// Prescaler 84,000,000Hz/8400 = 10,000 Hz (0.1ms)  (1~65536)
	TIM3->ARR = 200-1;	// Auto reload  0.1ms * 200 = 20ms

	// Clear the Counter
	TIM3->EGR |= (1<<0);	// UG: Update generation =1

	// Setting an UI(UEV) Interrupt 
	NVIC->ISER[0] |= (1<<29); // Enable Timer3 global Interrupt
 	TIM3->DIER |= (1<<0);	// Enable the Tim3 Update interrupt

	TIM3->CR1 |= (1<<0);	// Enable the Tim3 Counter (clock enable)   
}


void TIM3_IRQHandler(void)  	// 200ms  Interrupt
{
	TIM3->SR &= ~(1<<0);	// Interrupt flag Clear
	


        pulse_num2_TIM8= TIM8->CNT;           //TIM8->CNT ����  ����

        LCD_DisplayChar(5,8,0x30+pulse_num2_TIM8);   //DC����  ��ũ  LCD ǥ��
                              

}
void TIMER8_PULSE_COUNTER_Init(void)
{  
//PI5/TIM8_CH1
// Clock Enable : GPIOI & TIMER8
	RCC->AHB1ENR	|= (1<<8);	// 0x0100, GPIOI Enable
	RCC->APB2ENR 	|= (1<<1);	// 0x0002, TIMER8 Enable 


// PI5�� �Է¼����ϰ� Alternate function(TIM8_CH1)���� ��� ����
	GPIOI->MODER 	|= (2<<10);	// GPIOI PIN5 intput Alternate function mode 					
	GPIOI->OSPEEDR 	|= (2<<10);	// GPIOI PIN5 Output speed (50MHz High speed)
	GPIOI->PUPDR	&= ~(3<<10); 	// GPIOI PIN5 NO Pull-up
  					// PI5 ==> TIM8_CH1
	GPIOI->AFR[0]	|= (3<<20);	// Connect TIM8 pins(PI5) to AF3(TIM8..11)
  
        
// Time base Mode	
	TIM8->CR1 &= ~(1<<4);	// DIR=0(Up counter)(reset state)
	TIM8->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled)
	TIM8->CR1 &= ~(1<<2);	// URS=0(Update event source Selection)
	TIM8->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM8->CR1 |=  (1<<7);	// ARPE=1(ARR Preload Enable)
	TIM8->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	TIM8->CR1 |= (3<<5); 	// CMS(Center-aligned mode Sel)=11 (center-aligned mode) 

	// PSC, ARR
	TIM8->PSC = 1-1;	// Prescaler=1
	TIM8->ARR = 7;	// Auto reload  :  count�� ����: 0~7
        
	// Update(Clear) the Counter
	TIM8->EGR |= (1<<0);    // UG=1, REG's Update (CNT clear) 

// External Clock Mode 1
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1 or Ch2
	TIM8->CCMR1 |= (1<<0); 	// CC1S(CC1 channel) = '0b01' : Input 
	TIM8->CCMR1 &= ~(15<<4); // IC1F='0b0000: No Input Filter 
				
	// CCER(Capture/Compare Enable Register) : Enable "Channel 1" 
	TIM8->CCER &= ~(1<<0);	// CC1E=0: Capture Disable
	// TI1FP1 nonInverting / rising Edge  
	TIM8->CCER &= ~(1<<1);	// CC1P=0
	TIM8->CCER &= ~(1<<3);	// CC1NP=0   

	// SMCR(Slave Mode Control Reg.) : External Clock Enable
	TIM8->SMCR |= (5<<4);	// TS(Trigger Selection)=0b101 :TI1FP1(Filtered Timer Input 1 ��½�ȣ)
	TIM8->SMCR |= (7<<0);	// SMS(Slave Mode Selection)=0b111 : External Clock Mode 1

       TIM8->BDTR |=(1<<15);  //    main output enable
        
	TIM8->CR1 |= (1<<0);	// CEN: Enable the Tim4 Counter  	
}



void TIMER14_PWM_Init(void)
{
//PF9/T14_CH1
// �����޽�(PWM)��:PF9(T14 CH1)
// Clock Enable : GPIOF & TIMER14
	RCC->AHB1ENR	|= (1<<5);	// GPIOF Enable
	RCC->APB1ENR 	|= (1<<8);	// TIMER14 Enable 
    						
// PF9�� ��¼����ϰ� Alternate function(TIM14_CH1)���� ��� ���� : PWM ���
	GPIOF->MODER 	|= (2<<18);	// PF9 Output Alternate function mode					
	GPIOF->OSPEEDR 	|= (3<<18);	// PF9 Output speed (100MHz High speed)
	GPIOF->OTYPER	&= ~(1<<9);	// PF9 Output type push-pull (reset state)
	GPIOF->AFR[1]	|= (9<<4); 	//  Connect TIM14 pins(PF9) to AF9(CAN1/CAN2, TIM12.. TIM14)
					// PF9==> TIM14_CH1
    

        
// TIM14 Channel 1 : PWM 2 mode
	// Assign 'PWM Pulse Period'
	TIM14->PSC	= 420-1;	// Prescaler 84,000,000Hz/420 = 200kHz(0.005ms)  (1~65536)
	TIM14->ARR	= 80-1;	// Auto reload  (0.005ms * 80 = 400us : PWM Period)
    
	// Define the corresponding pin by 'Output'  
	// CCER(Capture/Compare Enable Register) : Enable "Channel 1" 
	TIM14->CCER	|= (1<<0);	// CC1E=1: OC1(TIM14_CH1) Active(Capture/Compare 1 output enable)
					// �ش���(27��)�� ���� ��ȣ���
	TIM14->CCER	&= ~(1<<1);	// CC1P=0: CC1 output Polarity High (OC1���� �������� ���)

	// Duty Ratio 
	TIM14->CCR1	= 80;		// CCR1 value   , ��Ƽ��=0%

	// 'Mode' Selection : Output mode, PWM 2
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1
	TIM14->CCMR1 	&= ~(3<<0); 	// CC1S(CC1 channel)='0b00' : Output 
	TIM14->CCMR1 	|= (1<<3); 	// OC1PE=1: Output Compare 1 preload Enable

	TIM14->CCMR1	|= (7<<4);	// OC1M: Output compare 1 mode: PWM 2 mode
	TIM14->CCMR1	|= (1<<7);	// OC1CE: Output compare 1 Clear enable

	// CR1 : Up counting & Counter TIM14 enable
	TIM14->CR1 	&= ~(1<<4);	// DIR: Countermode = Upcounter (reset state)
	TIM14->CR1 	&= ~(3<<8);	// CKD: Clock division = 0 (reset state)
	TIM14->CR1 	&= ~(3<<5); 	// CMS(Center-aligned mode Sel): No(reset state)
	TIM14->CR1	|= (1<<7);	// ARPE: Auto-reload preload enable
	TIM14->CR1	|= (1<<0);	// CEN: Counter TIM5 enable
}




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
//	RCC->AHB1ENR	|=  0x00000020; // RCC_AHB1ENR : GPIOF(bit#5) Enable							
//	GPIOF->MODER 	|=  0x00040000;	// GPIOF 9 : Output mode (0b01)						
//	GPIOF->OTYPER 	&= ~0x0200;	// GPIOF 9 : Push-pull  	
 //	GPIOF->OSPEEDR 	|=  0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 
	
	//Joy Stick SW(PORT I) ����
	RCC->AHB1ENR 	|= 0x00000100;	// RCC_AHB1ENR GPIOI Enable
	GPIOI->MODER 	&= ~0x000FFC00;	// GPIOI 5~9 : Input mode (reset state)
	GPIOI->PUPDR    &= ~0x000FFC00;	// GPIOI 5~9 : Floating input (No Pull-up, pull-down) (reset state)
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
	LCD_Clear(RGB_YELLOW);		// ȭ�� Ŭ����
	LCD_SetFont(&Gulim8);		// ��Ʈ : ���� 8
      LCD_SetBackColor(RGB_BLACK);	// ���ڹ��� : black

        LCD_SetTextColor(RGB_WHITE);	// ���ڻ� : white
        LCD_SetBrushColor(RGB_BLACK);
        LCD_DrawFillRect(0,0,140,25);              //�ش� ������ŭ �����簢��
	LCD_DisplayText(0,0,"Motor Control");  // Title
	LCD_DisplayText(1,0,"2019130027 YJH");

        LCD_SetBackColor(RGB_YELLOW);	// ���ڹ��� : Yellow
        LCD_SetTextColor(RGB_BLUE);	        // ���ڻ� : Blue
	LCD_DisplayText(2,0,"Step Motor");
        LCD_SetTextColor(RGB_BLACK);	// ���ڻ� : Black
        LCD_DisplayText(3,1,"Posotion: ");
        LCD_SetTextColor(RGB_RED);        	// ���ڻ� : red
        LCD_DisplayText(3,10,"0");                 //��ġ �ʱⰪ
        
        
        
        LCD_SetTextColor(RGB_BLUE);	        // ���ڻ� : Blue
	LCD_DisplayText(4,0,"DC Motor");
        LCD_SetTextColor(RGB_BLACK);	// ���ڻ� : Black
        LCD_DisplayText(5,1,"Torque:");
         LCD_SetTextColor(RGB_RED);	       // ���ڻ� : red
        LCD_DisplayText(5,8,"0");                  //��ũ �ʱⰪ
        
        
}

/* Joystick switch�� �ԷµǾ������� ���ο� � Joystick switch�� �ԷµǾ������� ������ return�ϴ� �Լ�  */ 
uint8_t joy_flag = 0;
uint16_t JOY_Scan(void)	// input joy stick NAVI_* 
{ 
	uint16_t key;
	key = GPIOI->IDR & 0x03E0;	// any key pressed ?
	if(key == 0x03E0)		// if no key, check key off
	{  	if(joy_flag == 0)
			return key;
		else
		{	DelayMS(10);
			joy_flag = 0;
			return key;
		}
	}
  	else				// if key input, check continuous key
	{	if(joy_flag != 0)	// if continuous key, treat as no key input
			return 0x03E0;
		else			// if new key,delay for debounce
		{	joy_flag = 1;
			DelayMS(10);
 			return key;
		}
	}
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

