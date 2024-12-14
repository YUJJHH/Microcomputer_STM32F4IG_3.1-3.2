//////////////////////////////////////////////////////////////////////////
// HW4: ���ӵ���(SPI)�� �̿��� Ball game
// ������: 2019130027 ����ȯ
// ������: 2023-12-02
// ����: SPI ����� �̿��Ͽ� ���ӵ������κ��� ���ӵ���(X,Y,Z)�� �о ǥ���ϰ�, 
// ŰƮ�� ����ӿ� ���� ���ϴ� ���ӵ����� �ݿ��Ͽ� ���� �̵��ϵ��� ǥ��
// SPI ����� �̿��� ���ӵ����� ����
//  NSS pin:  PA8 (PA4(SPI1_CS) ��ſ� ���)
//  SCK pin:  PA5 (SPI1_SCK)
//  MOSI pin: PA7 (SPI1_MOSI)
//  MISO pin: PA6 (SPI1_MISO)
// SPI mode: MASTER
// ���ӵ�����(LIS2HH12, Slave mode) X,Y,Z ���� TIM10 CC���ͷ�Ʈ 200ms���� ������ LCD�� ǥ�� 
//  ���� �� ���ο� ������ �̵� ����(Reset�� ����� �߾ӿ��� �����)
//////////////////////////////////////////////////////////////////////////
                 

#include "stm32f4xx.h"
#include "GLCD.h"
#include "ACC.h"

void DisplayTitle(void);
void _GPIO_Init(void);
void SPI1_Init(void);

void Display_Process(int16 *pBuf);
void TIMER5_Init(void);


void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);

//// void SPI1_Process(UINT16 *pBuf);  // ACC.c (ACC.h) 
//// void ACC_Init(void) // ACC.c (ACC.h)
//// void LCD_Init(void); // GLCD.c (GLCD.h)

UINT8 bControl;

uint8 x=56 ,y=63;  //��ġ �ʱⰪ



UINT8 x;


void TIMER4_Init(void);



int main(void)
{
	 
        
	LCD_Init();		// LCD ���� �Լ�
	DelayMS(100);		// LCD���� ������
	DisplayTitle();		// LCD �ʱ�ȭ�鱸�� �Լ�
     
	_GPIO_Init();		// LED, SW �ʱ�ȭ
	
	ACC_Init();		// ���ӵ����� �ʱ�ȭ
	
   
	
        TIMER5_Init();
    TIMER4_Init();
      
         while(1)
	{
		  GPIOG->ODR ^= 0x0008;
                  DelayMS(500);
                   
	}
}


void TIMER5_Init(void)  //ch2  ph11
{  

	RCC->AHB1ENR	|= (1<<7);	// 0x08, GPIOD Enable
	RCC->APB1ENR 	|= (1<<3);	// 0x04, TIMER4 Enable 



// PD13�� ��¼����ϰ� Alternate function(TIM4_CH2)���� ��� ����
	GPIOH->MODER 	|= (2<<22);	// 0x08000000(MODER.(27,26)=0b10), GPIOD PIN13 Output Alternate function mode 					
	GPIOH->OSPEEDR 	|= (2<<22);	// 0x08000000(OSPEEDER.(27,26)=0b11), GPIOD PIN13 Output speed (50MHz High speed)
	GPIOH->PUPDR	&= ~(3<<22); 	// GPIOD PIN13 NO Pull-up
  					// PD13 ==> TIM4_CH2
	GPIOH->AFR[1]	|= (2<<12);	// 0x00200000(AFR[1].(23~20)=0b0010): Connect TIM4 pins(PD13) to AF2(TIM3..5)

// Time base Mode
	// Setting CR1 : 0x0000 
	TIM5->CR1 &= ~(1<<4);	// DIR=0(Up counter)(reset state)
	TIM5->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled) 
	TIM5->CR1 &= ~(1<<2);	// URS=0(Update event source Selection)
	TIM5->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM5->CR1 |=  (1<<7);	// ARPE=1(ARR Preload Enable)
	TIM5->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
	//TIM5->CR1 |= (1<<5); 	// CMS(Center-aligned mode Sel)=00 (Edge-aligned mode) (reset state)
				// Center-aligned mode: The counter counts Up and DOWN alternatively
        // CR2 
	TIM5->CR2 &= ~(1<<7);	// TI1S=0(TIM4_CH1 pin is connected to TI1 input)   ,ũ�ν����� ����
 
	// PSC, ARR
	TIM5->PSC = 1-1;	// Prescaler=1
	TIM5->ARR = 4;	// Auto reload  :  ���ڴ� count�� ����: 0~10000  ,          �Ʊ�� 9999����

	// Update(Clear) the Counter
	TIM5->EGR |= (1<<0);    // UG: Update generation  , REG's Update (CNT clear) 
       
// Output Compare Mode
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1 or Ch2
	
	TIM5->CCMR1 |= (1<<8); 	// CC2S(CC2 channel) = '0b01' : Input 
	TIM5->CCMR1 &= ~(15<<12); // IC2F='0b0000': No Input Filter 

	// CCER(Capture/Compare Enable Register) : Ch1 & Ch2 Polarity : Noninverting / Rising Edge
	// : TI1FP1 NonInverting / Rising Edge
	
	// : TI2FP2 NonInverting / Rising Edge  
	TIM5->CCER &= ~(1<<5);	// CC2P=0
	TIM5->CCER &= ~(1<<7);	// CC2NP=0

 	// SMCR(Slave Mode Control Reg.) : Encoder Mode 1 
	TIM5->SMCR |= (7<<0);	// SMS(Slave Mode Selection)=0b001 
				// : Encoder 1 mode (TI1FP1 ������� TI2FP2 �޽��� ���� counting)
TIM5->SMCR |= (6<<4);
	TIM5->CR1 |= (1<<0);	// CEN: Enable the Tim4 Counter  	
 }


void TIMER4_Init(void)
{
	RCC->APB1ENR |= 0x03;	// RCC_APB1ENR TIMER3 Enable

	// Setting CR1 : 0x0000 
//	TIM4->CR1 |= (1<<4);  // DIR=0(Up counter)(reset state)
	TIM4->CR1 &= ~(1<<1);	// UDIS=0(Update event Enabled): By one of following events
                            //  Counter Overflow/Underflow, 
                            //  Setting the UG bit Set,
                            //  Update Generation through the slave mode controller 
                            // UDIS=1 : Only Update event Enabled by  Counter Overflow/Underflow,
	TIM4->CR1 &= ~(1<<2);	// URS=0(Update Request Source  Selection):  By one of following events
                            //	Counter Overflow/Underflow, 
                            // Setting the UG bit Set,
                            //	Update Generation through the slave mode controller 
                            // URS=1 : Only Update Interrupt generated  By  Counter Overflow/Underflow,
	TIM4->CR1 &= ~(1<<3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM4->CR1 &= ~(1<<7);	// ARPE=0(ARR is NOT buffered) (reset state)
	TIM4->CR1 &= ~(3<<8); 	// CKD(Clock division)=00(reset state)
//	TIM4->CR1 &= ~(3<<5); 	// CMS(Center-aligned mode Sel)=00 (Edge-aligned mode) (reset state)
                            // Center-aligned mode: The counter counts UP and DOWN alternatively

        TIM4->CR1 |= (3<<5); 	
        

    // Deciding the Period
	TIM4->PSC = 8400-1;	// Prescaler 84,000,000Hz/8400 = 10,000 Hz (0.1ms)  (1~65536)
	TIM4->ARR = 1000-1;	// Auto reload  0.1ms * 10 = 1ms

   	// Clear the Counter
	TIM4->EGR |= (1<<0);	// UG(Update generation)=1 
                        // Re-initialize the counter(CNT=0) & generates an update of registers   

	// Setting an UI(UEV) Interrupt 
	NVIC->ISER[0] |= (1<<30); // Enable Timer3 global Interrupt
 	TIM4->DIER |= (1<<0);	// Enable the Tim3 Update interrupt

	TIM4->CR1 |= (1<<0);	// Enable the Tim3 Counter (clock enable)   
}

void TIM4_IRQHandler(void)  	// 1ms Interrupt
{
	
    
	TIM4->SR &= ~(1<<0);	// Interrupt flag Clear
        
      GPIOG->ODR ^= 0x0080;
		x=TIM5->CNT;	
                
                
 		LCD_DisplayChar(3,9,x+0x30);	//Z AXIS
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
	LCD_SetFont(&Gulim8);		//��Ʈ  7
	LCD_SetBackColor(RGB_WHITE);    //���ڹ��� ���
	LCD_SetTextColor(RGB_BLACK);    //���ڻ�
	LCD_DisplayText(0,0,"Ball game: YJH 2019130027");  // Title
		
//	LCD_DisplayText(1,19,"Ax:-1.1");	//X AXIS
	LCD_DisplayText(2,0,"Steop motor");	//Y AXIS
	LCD_DisplayText(3,0,"Position");	//Z AXIS
        
  
         LCD_SetBrushColor(RGB_RED);        //�귯�û� 
         LCD_DrawRectangle(56,63,6,6);           //�� �ʱ���ġ
         
         
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
