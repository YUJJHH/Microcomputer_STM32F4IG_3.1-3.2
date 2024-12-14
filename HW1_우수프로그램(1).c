/////////////////////////////////////////////////////////////
// HW1: 세계 디지털 시계 제작
// 제출자: XXXXXXXXXXXXXXXXX
// 주요 내용 및 구현 방법
// -세계 주요도시의 시계를 표시
// -각 도시를 변경하는 기능, Int period 변경, reset 기능 등등
// -지역 시간: TIM2의 Down-Counting mode(100msec) 이용 (ARR 업데이트로 10ms도 가능)
// TIM6의 Up-Counting mode (100msec) 이용(ARR 업데이트로 10ms도 가능)
// TIM9의 Up-Counting mode (OC Inturrupt 100ms) 이용(ARR 업데이트로 10ms도 가능)
// -지역 시간 reset : SW7 입력 -> EXTI15 인터럽트 발생 -> Int. Flag reset 등등
// -지역 시간 증가 속도 변경: SW6을 입력하면
//  EXTI14 인터럽트 발생하여, 인터럽트주기 변경 등등
/////////////////////////////////////////////////////////////
#include "stm32f4xx.h"
#include "GLCD.h"
#include <stdio.h> //sprintf를 위한 헤더 파일
#include <stdlib.h> //sprintf를 위한 헤더 파일

#define SW0_PUSH        0xFE00  //PH8
#define SW1_PUSH        0xFD00  //PH9
#define SW2_PUSH        0xFB00  //PH10
#define SW3_PUSH        0xF700  //PH11
#define SW4_PUSH        0xEF00  //PH12
#define SW5_PUSH        0xDF00  //PH13
#define SW6_PUSH        0xBF00  //PH14
#define SW7_PUSH        0x7F00  //PH15
#define ICN 1 
#define ORD 2 
#define LHR 3 // 지역 코드

void _RCC_Init(void);
void _GPIO_Init(void);
void _EXTI_Init(void);
uint16_t KEY_Scan(void);
void TIMER2_Init(void);  //TIMER2 초기화
void TIMER6_Init(void); //TIMER6 초기화
void TIMER9_OC_Init(void); // TIMER 9 초기화

void DisplayInitScreen(void);

void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);
void BEEP(void);
void DisplayParameters(uint8_t  city, uint8_t hour, uint8_t min, uint8_t period);
// LCD에 지역, 시간, 주기를 보여주는 함수

uint8_t	SW0_Flag, SW1_Flag, SW2_Flag;
uint8_t	SW6_Flag, SW7_Flag; //각 SW가 눌렸음을 보여주는 플래그 변수들
uint8_t  ICN_hour = 23, ICN_min = 50 - 1;
uint8_t  ORD_hour = 9, ORD_min = 50 - 1;
uint8_t LHR_hour = 15, LHR_min = 50 - 1; //각 도시의 시간 변수 및 초기 설정
// -1은 50분부터 시작하도록 하기 위함; 디스플레이 컬리브레이션
uint8_t period; // 인터럽트 주기를 설정하기 위한 변수


int main(void)
{
	_GPIO_Init();  		// GPIO 초기화
	_EXTI_Init();		// 외부인터럽트 초기화
	LCD_Init();		// GLCD 초기화
	DelayMS(10);
	BEEP();			// Beep 한번 

	GPIOG->ODR &= 0xFF00;	// 초기값: LED0~7 Off
	DisplayInitScreen();	// LCD 초기화면

	SW0_Flag = 1; //초기에 ICN을 표시하기 위함.
	period = 100; //초기 주기 100ms
	TIMER2_Init();
	TIMER6_Init();
	TIMER9_OC_Init(); // 범용타이머들 초기화

	GPIOG->ODR |= 0x0001; // LED0 ON


	while (1)
	{
		switch (KEY_Scan())
		{
		case SW0_PUSH: 	//SW0
			if (SW0_Flag == 0) {
				GPIOG->ODR &= ~0x0004;
				SW2_Flag = 0;
				GPIOG->ODR &= ~0x0002;
				SW1_Flag = 0;
				//다른 도시들의 LED OFF & 플래그 변수 초기화
				GPIOG->ODR |= 0x0001;
				SW0_Flag = 1;
				//ICN 표시 모드 ON
			}
			break;
		case SW1_PUSH: //SW1
			if (SW1_Flag == 0) {
				GPIOG->ODR &= ~0x0004;
				SW2_Flag = 0;
				GPIOG->ODR &= ~0x0001;
				SW0_Flag = 0;
				//다른 도시들의 LED OFF & 플래그 변수 초기화
				GPIOG->ODR |= 0x0002; // LED7 ON
				SW1_Flag = 1;
				//ORD 표시 모드 ON
			}
			break;
		case SW2_PUSH: //SW2
			if (SW2_Flag == 0) {
				GPIOG->ODR &= ~0x0002;
				SW1_Flag = 0;
				GPIOG->ODR &= ~0x0001;
				SW0_Flag = 0;
				//다른 도시들의 LED OFF & 플래그 변수 초기화
				GPIOG->ODR |= 0x0004; // LED7 ON
				SW2_Flag = 1;
				//LHR 표시 모드 ON
			}
			break;
		}  // switch(KEY_Scan())

		if (SW7_Flag == 1) // 도시 시간 reset 루틴
		{
			TIM2->CR1 &= ~0x0001;
			TIM6->CR1 &= ~0x0001;
			TIM9->CR1 &= ~0x0001; //CR1.CEN = 0

			GPIOG->ODR ^= 0x0080; // LED7 ON
			BEEP(); //부저 1회
			DelayMS(3000); //delay 3초
			GPIOG->ODR ^= 0x0080; // LED7 OFF

			TIM2->CNT = 0;
			TIM6->CNT = 0;
			TIM9->CNT = 0; // CNT = 0

			ICN_hour = 0;
			ICN_min = 0;
			ORD_hour = 10;
			ORD_min = 0;
			LHR_hour = 16;
			LHR_min = 0; //각 지역 별 시간 초기화

			DisplayParameters(ICN, ICN_hour, ICN_min, 100); // ICN 정보 표시

			TIM2->CR1 |= 0x0001;
			TIM6->CR1 |= 0x0001;
			TIM9->CR1 |= 0x0001;// CR1.CEN = 1
			SW7_Flag = 0; //SW7 플래그 초기화
		}
		else if (SW6_Flag == 1) //최소 시간 간격 변경 루틴
		{
			TIM2->CR1 &= ~0x0001;
			TIM6->CR1 &= ~0x0001;
			TIM9->CR1 &= ~0x0001; //CR1.CEN = 0

			if (period == 100) //인터럽트 주기가 100ms일 때
			{
				TIM2->ARR &= ~0xFFFFFFFF;
				TIM6->ARR &= ~0xFFFFFFFF;
				TIM9->ARR &= ~0xFFFFFFFF; //기존 ARR 데이터 초기화

				TIM2->ARR |= 20000;
				TIM6->ARR |= 500 - 1;
				TIM9->ARR |= 100 - 1; // 인터럽트 주기10ms 위한 ARR 수정
				period = 10; // period 데이터 업데이트
			}
			else if (period == 10) //인터럽트 주기가 10ms일 때
			{
				TIM2->ARR &= ~0xFFFFFFFF;
				TIM6->ARR &= ~0xFFFFFFFF;
				TIM9->ARR &= ~0xFFFFFFFF; //기존 ARR 데이터 초기화

				TIM2->ARR |= 200000;
				TIM6->ARR |= 5000 - 1;
				TIM9->ARR |= 1000 - 1; // 인터럽트 주기 100ms 위한 ARR 수정
				period = 100; // period 데이터 업데이트
			}
			ORD_min = ICN_min;
			LHR_min = ICN_min; //주기 변경으로 인한 min오차를 컬리브레이션


			TIM2->CNT = 0;
			TIM6->CNT = 0;
			TIM9->CNT = 0; //CNT = 0

			TIM2->CR1 |= 0x0001;
			TIM6->CR1 |= 0x0001;
			TIM9->CR1 |= 0x0001; // CR1.CEN = 1
			SW6_Flag = 0; // SW6 플래그 초기화
		}
	}
}

void TIMER2_Init(void) //TIMER2 초기화 루틴
{
	RCC->APB1ENR |= 0x01;	// RCC_APB1ENR TIMER2 Enable

	// Setting CR1 : 0x0000 
	TIM2->CR1 |= (1 << 4);  // DIR=1(Down counter)(reset state)


	TIM2->CR1 &= ~(1 << 1);	// UDIS=0(Update event Enabled): By one of following events
	//  Counter Overflow/Underflow, 
	//  Setting the UG bit Set,
	//  Update Generation through the slave mode controller 
	// UDIS=1 : Only Update event Enabled by  Counter Overflow/Underflow,
	TIM2->CR1 &= ~(1 << 2);	// URS=0(Update Request Source  Selection):  By one of following events
	//	Counter Overflow/Underflow, 
	// Setting the UG bit Set,
	//	Update Generation through the slave mode controller 
	// URS=1 : Only Update Interrupt generated  By  Counter Overflow/Underflow,
	TIM2->CR1 &= ~(1 << 3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM2->CR1 &= ~(1 << 7);	// ARPE=0(ARR is NOT buffered) (reset state)
	TIM2->CR1 &= ~(3 << 8); 	// CKD(Clock division)=00(reset state)
	TIM2->CR1 &= ~(3 << 5); 	// CMS(Center-aligned mode Sel)=00 (Edge-aligned mode) (reset state)
	// Center-aligned mode: The counter counts UP and DOWN alternatively


// Deciding the Period
	TIM2->PSC = 42 - 1;	// Prescaler 84,000,000Hz/42= 2000000 Hz (1/2000000 ms)  (1~65536)
	TIM2->ARR = 200000;	// Auto reload  (1/2000000) ms * 200000= 100ms

	// Clear the Counter
	TIM2->EGR |= (1 << 0);	// UG(Update event generation)=1 
	// Re-initialize the counter(CNT=0) & generates an update of registers   

// Setting an UI(UEV) Interrupt 
	TIM2->DIER |= (1 << 0);	// Enable the Tim2 Update interrupt

	NVIC->ISER[0] |= (1 << (28)); // Enable Timer2 global Interrupt

	TIM2->CR1 |= (1 << 0);	// Enable the Tim2 Counter (clock enable)   
}


void TIM2_IRQHandler(void)  //TIMER2 핸들러
{
	TIM2->SR &= ~(1 << 0);	// Interrupt flag Clear

	ICN_min++; //(개념적) 1분 추가
	if (ICN_min != 60) //60분이 안되었을 때
	{
		if (SW0_Flag == 1) DisplayParameters(ICN, ICN_hour, ICN_min, period);
		//ICN일 경우 Display
		return; // 60분이 안되었으면 return
	}
	ICN_min = 0; // 60분이 되었을 경우 분 초기화
	ICN_hour++; // 60분이 되었을 경우 시 업데이트

	if (ICN_hour != 24) //24시간이 안되었을 때
	{
		if (SW0_Flag == 1) DisplayParameters(ICN, ICN_hour, ICN_min, period);
		//ICN인 경우 Display
		return; //24시간이 안되었으면 return
	}
	ICN_hour = 0; // 24시간이 되었으면 시 초기화
	if (SW0_Flag == 1) DisplayParameters(ICN, ICN_hour, ICN_min, period);
	//24시간이 되었을 경우, 00:00표시
}

void TIMER6_Init(void) //TIMER6초기화
{
	RCC->APB1ENR |= 0x10;	// RCC_APB1ENR TIMER6 Enable

	// Setting CR1 : 0x0000 
	TIM6->CR1 &= ~(1 << 4);  // DIR=0(Up counter)(reset state)


	TIM6->CR1 &= ~(1 << 1);	// UDIS=0(Update event Enabled): By one of following events
	//  Counter Overflow/Underflow, 
	//  Setting the UG bit Set,
	//  Update Generation through the slave mode controller 
	// UDIS=1 : Only Update event Enabled by  Counter Overflow/Underflow,
	TIM6->CR1 &= ~(1 << 2);	// URS=0(Update Request Source  Selection):  By one of following events
	//	Counter Overflow/Underflow, 
	// Setting the UG bit Set,
	//	Update Generation through the slave mode controller 
	// URS=1 : Only Update Interrupt generated  By  Counter Overflow/Underflow,
	TIM6->CR1 &= ~(1 << 3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM6->CR1 &= ~(1 << 7);	// ARPE=0(ARR is NOT buffered) (reset state)
	TIM6->CR1 &= ~(3 << 8); 	// CKD(Clock division)=00(reset state)
	TIM6->CR1 &= ~(3 << 5); 	// CMS(Center-aligned mode Sel)=00 (Edge-aligned mode) (reset state)
	// Center-aligned mode: The counter counts UP and DOWN alternatively


// Deciding the Period
	TIM6->PSC = 1680 - 1;	// Prescaler 84,000,000Hz/1680=  50000Hz (1/50000ms)  (1~65536)
	TIM6->ARR = 5000 - 1;	// Auto reload  1/50000ms * 5000= 100ms

	// Clear the Counter
	TIM6->EGR |= (1 << 0);	// UG(Update event generation)=1 
	// Re-initialize the counter(CNT=0) & generates an update of registers   

// Setting an UI(UEV) Interrupt 


	NVIC->ISER[1] |= (1 << (54 - 32)); // Enable Timer6 global Interrupt

	TIM6->DIER |= (1 << 0);	// Enable the Tim6 Update interrupt

	TIM6->CR1 |= (1 << 0);	// Enable the Tim6 Counter (clock enable)   
}


void TIM6_DAC_IRQHandler(void)  // TIM6 Inturrupt 핸들러
{
	TIM6->SR &= ~(1 << 0);	// Interrupt flag Clear

	ORD_min++; //(개념적) 1분 추가

	if (ORD_min != 60) //60분이 안되었을 때
	{
		if (SW1_Flag == 1) DisplayParameters(ORD, ORD_hour, ORD_min, period);
		//ORD인 경우 Display
		return; //60분이 안되었을 때 리턴
	}
	ORD_min = 0;// 60분이 되었을 경우 분 초기화
	ORD_hour++; // 60분이 되었을 경우 시 업데이트

	if (ORD_hour != 24)//24시간이 안되었을 때
	{
		if (SW1_Flag == 1) DisplayParameters(ORD, ORD_hour, ORD_min, period);
		//ORD인 경우 Display
		return; //24시간이 안되었을 때 리턴
	}
	ORD_hour = 0; // 24시간이 되었으면 시 초기화
	if (SW1_Flag == 1) DisplayParameters(ORD, ORD_hour, ORD_min, period);
	//24시간이 되었을 경우, 00:00표시
}

void TIMER9_OC_Init(void)
{
	RCC->AHB1ENR |= (1 << 4);	// 0x08, RCC_AHB1ENR GPIOE Enable : AHB1ENR.4

	GPIOE->MODER |= (2 << 10);	// GPIOE PIN5 Output Alternate function mode 					
	GPIOE->OSPEEDR |= (3 << 10);	// GPIOD PIN5 Output speed (100MHz High speed)
	GPIOE->OTYPER &= ~(1 << 5);	// GPIOE PIN5 Output type push-pull (reset state)
	GPIOE->PUPDR |= (1 << 10); 	// GPIOD PIN5 Pull-up
	// PE5 ==> TIM9_CH1
	GPIOE->AFR[0] |= 0x00300000;  // Connect TIM9 pins(PE5) to AF3(TIM8..11)

	// Time base 설정
	RCC->APB2ENR |= (1 << 16);	// RCC_APB1ENR TIMER9 Enable

	// Setting CR1 : 0x0000 
	TIM9->CR1 &= ~(1 << 4);	// DIR=0(Up counter)(reset state)
	TIM9->CR1 &= ~(1 << 1);	// UDIS=0(Update event Enabled): By one of following events
	//  Counter Overflow/Underflow, 
	//  Setting the UG bit Set,
	//  Update Generation through the slave mode controller 
	// UDIS=1 : Only Update event Enabled by  Counter Overflow/Underflow,
	TIM9->CR1 &= ~(1 << 2);	// URS=0(Update event source Selection): one of following events
	//	Counter Overflow/Underflow, 
	// Setting the UG bit Set, 
	//	Update Generation through the slave mode controller 
	// URS=1 : Only Update Interrupt generated  By  Counter Overflow/Underflow,
	TIM9->CR1 &= ~(1 << 3);	// OPM=0(The counter is NOT stopped at update event) (reset state)
	TIM9->CR1 |= (1 << 7);	// ARPE=1(ARR is buffered): ARR Preload Enalbe 
	TIM9->CR1 &= ~(3 << 8); 	// CKD(Clock division)=00(reset state)
	TIM9->CR1 &= ~(3 << 5); 	// CMS(Center-aligned mode Sel)=00 : Edge-aligned mode(reset state)

	// Setting the Period
	TIM9->PSC = 16800 - 1;	// Prescaler=168MHz, 168MHz/16800 = 10000Hz (1/10000s)
	TIM9->ARR = 1000 - 1;	// Auto reload  : 1/10000s * 1000 = 100ms(period) : 인터럽트주기나 출력신호의 주기 결정
	// Update(Clear) the Counter
	TIM9->EGR |= (1 << 0);    // UG: Update generation    

	// Output Compare 설정
	// CCMR1(Capture/Compare Mode Register 1) : Setting the MODE of Ch1 or Ch2
	TIM9->CCMR1 &= ~(3 << 0); // CC1S(CC1 channel) = '0b00' : Output 
	TIM9->CCMR1 &= ~(1 << 2); // OC1FE=0: Output Compare 1 Fast disable 
	TIM9->CCMR1 &= ~(1 << 3); // OC1PE=0: Output Compare 1 preload disable(CCR1에 언제든지 새로운 값을 loading 가능) 
	TIM9->CCMR1 |= (3 << 4);	// OC1M=0b011 (Output Compare 1 Mode : toggle)
	// OC1REF toggles when CNT = CCR1

// CCER(Capture/Compare Enable Register) : Enable "Channel 1" 
	TIM9->CCER |= (1 << 0);	// CC1E=1: CC1 channel Output Enable
	TIM9->CCER &= ~(1 << 1);	// CC1P=0: CC1 channel Output Polarity (OCPolarity_High : OC1으로 반전없이 출력)  

	// CC1I(CC 인터럽트) 인터럽트 발생시각 또는 신호변화(토글)시기 결정: 신호의 위상(phase) 결정
	// 인터럽트 발생시간(1000 펄스)의 10%(100) 시각에서 compare match 발생
	TIM9->CCR1 = 100;	// TIM9 CCR1 TIM9_Pulse

	//	TIM9->DIER |= (1<<0);	// UIE: Enable Tim4 Update interrupt
			//업데이트 인터럽트 안쓸 때는 주석 처리 필수
	TIM9->DIER |= (1 << 1);	// CC1IE: Enable the Tim4 CC1 interrupt

	NVIC->ISER[0] |= (1 << 24);	// Enable Timer9 global Interrupt on NVIC

	TIM9->CR1 |= (1 << 0);	// CEN: Enable the Tim9 Counter  					

}

void TIM1_BRK_TIM9_IRQHandler(void)      //TIM9를 위한 핸들러, RESET: 0
{
	if ((TIM9->SR & 0x02) != RESET)	// Capture/Compare 1 interrupt flag
	{
		TIM9->SR &= ~(1 << 1);	// CC 1 Interrupt Claer
		LHR_min++; //(개념적) 1분 추가

		if (LHR_min != 60) //60분이 안되었을 때
		{
			if (SW2_Flag == 1) DisplayParameters(LHR, LHR_hour, LHR_min, period);
			//LHR인 경우 Display
			return;//60분이 안되었을 때 리턴
		}
		LHR_min = 0; // 60분이 되었을 때 min 초기화
		LHR_hour++; // 60분이 디었을 때 시간 업데이트

		if (LHR_hour != 24) //24시간이 안되었을 때
		{
			if (SW2_Flag == 1) DisplayParameters(LHR, LHR_hour, LHR_min, period);
			//LHR인 경우 Display
			return; //24시간이 안되었을 때 리턴
		}
		LHR_hour = 0; // 24시간이 되었을 때 초기화
		if (SW2_Flag == 1) DisplayParameters(LHR, LHR_hour, LHR_min, period);
		// 00:00인 경우 Display

	}
}
void DisplayParameters(uint8_t  city, uint8_t hour, uint8_t min, uint8_t period)
// 지역, 시간, 주기를 보여주는 함수
{
	char str[20]; //문자열을 임시 저장할 배열 변수
	LCD_SetTextColor(RGB_BLACK);// 글자색 : Black

	if (city == ICN) LCD_DisplayText(1, 0, "ICN");
	else if (city == ORD) LCD_DisplayText(1, 0, "ORD");
	else if (city == LHR) LCD_DisplayText(1, 0, "LHR");
	//각 지역 별로 Display
	sprintf(str, "%d", hour);// 숫자를 문자열로 변환하여 str에 저장
	if (hour < 10) //시간이 10미만일 경우 앞자리에 0표시
	{
		LCD_DisplayText(1, 4, "0");
		LCD_DisplayText(1, 5, str);
	}
	else // 시간이 10이상일 경우 두 자리 표시
		LCD_DisplayText(1, 4, str);

	sprintf(str, "%d", min); //숫자를 문자열로 변환하여 str에 저장
	if (min < 10) //분이 10미만일 경우 앞자리에 0표시
	{
		LCD_DisplayText(1, 7, "0");
		LCD_DisplayText(1, 8, str);
	}
	else //시간이 10이상일 경우 두 자리 표시
		LCD_DisplayText(1, 7, str);

	if (period == 100) //인터럽트 주기가 100ms일 때 Display
		LCD_DisplayText(2, 11, "100");
	else if (period == 10) //인터럽트 주기가 10ms일 때 Display
		LCD_DisplayText(2, 11, " 10");

}


void _GPIO_Init(void)
{
	// LED (GPIO G) 설정
	RCC->AHB1ENR |= 0x00000040;	// RCC_AHB1ENR : GPIOG(bit#6) Enable							
	GPIOG->MODER |= 0x00005555;	// GPIOG 0~7 : Output mode (0b01)						
	GPIOG->OTYPER &= ~0x00FF;	    // GPIOG 0~7 : Push-pull  (GP8~15:reset state)	
	GPIOG->OSPEEDR |= 0x00005555;	// GPIOG 0~7 : Output speed 25MHZ Medium speed 

	// SW (GPIO H) 설정 
	RCC->AHB1ENR |= 0x00000080;	// RCC_AHB1ENR : GPIOH(bit#7) Enable							
	GPIOH->MODER &= ~0xFFFF0000;	// GPIOH 8~15 : Input mode (reset state)				
	GPIOH->PUPDR &= ~0xFFFF0000;	// GPIOH 8~15 : Floating input (No Pull-up, pull-down) :reset state

	// Buzzer (GPIO F) 설정 
	RCC->AHB1ENR |= 0x00000020;     // RCC_AHB1ENR : GPIOF(bit#5) Enable							
	GPIOF->MODER |= 0x00040000;	// GPIOF 9 : Output mode (0b01)						
	GPIOF->OTYPER &= ~0x0200;	    // GPIOF 9 : Push-pull  	
	GPIOF->OSPEEDR |= 0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 
}

void _EXTI_Init(void)
{
	RCC->AHB1ENR |= 0x0080;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR |= 0x4000;	// Enable System Configuration Controller Clock

	GPIOH->MODER &= 0x0000FFFF;	// GPIOH PIN8~PIN15 Input mode (reset state)				 

	// EXTI14,15에 대한 소스 입력은 GPIOH로 설정 (EXTICR4) (reset value: 0x0000)	
	SYSCFG->EXTICR[3] |= 0x7700;

	EXTI->FTSR |= 0x00C000;	// Falling Trigger Enable  (EXTI14, 15:PH14, 15) 
	EXTI->IMR |= 0x00C000;  	// EXTI14,15 인터럽트 mask (Interrupt Enable)

	NVIC->ISER[1] |= (1 << (40 - 32));   // Enable Interrupt EXTI14,15 Vector table Position 참조
}

void EXTI15_10_IRQHandler(void)		// EXTI 10~15 인터럽트 핸들러
{

	if (SW0_Flag == 1 && EXTI->PR & 0x4000) 	// ICN이면서 EXTI14 Interrupt Pending?
	{
		EXTI->PR |= 0x4000; 	// Pending bit Clear
		SW6_Flag = 1;
	}

	else if (SW0_Flag == 1 && EXTI->PR & 0x8000) 	// ICN이면서 EXTI15 Interrupt Pending?
	{
		EXTI->PR |= 0x8000; 	// Pending bit Clear
		SW7_Flag = 1; //플래그 변수 업데이트; 이하 루틴은 main함수에서

	}
	else  if (EXTI->PR & 0x4000)
	{
		EXTI->PR |= 0x4000;
	}
	else  if (EXTI->PR & 0x8000)
	{
		EXTI->PR |= 0x8000;
	} //ICN이 아닌 경우 pending 초기화를 위한 루틴
}

void BEEP(void)			/* beep for 30 ms */
{
	GPIOF->ODR |= 0x0200;	// PF9 'H' Buzzer on
	DelayMS(30);		// Delay 30 ms
	GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
}

void DelayMS(unsigned short wMS)
{
	register unsigned short i;
	for (i = 0; i < wMS; i++)
		DelayUS(1000);   // 1000us => 1ms
}

void DelayUS(unsigned short wUS)
{
	volatile int Dly = (int)wUS * 17;
	for (; Dly; Dly--);
}

void DisplayInitScreen(void)
{
	LCD_Clear(RGB_YELLOW);	// 화면 클리어
	LCD_SetFont(&Gulim8);	// 폰트 : 굴림 8
	LCD_SetBackColor(RGB_YELLOW);// 글자배경색 : Yellow

	LCD_SetTextColor(RGB_BLUE);// 글자색 : Blue
	LCD_DisplayText(0, 0, "BJW 2019132018"); //이름, 학번
	LCD_DisplayText(2, 0, "Int period    ms"); //주기 표시

	LCD_SetTextColor(RGB_BLACK);// 글자색 : Black 
	LCD_DisplayText(1, 0, "ICN 23:50"); // 초기 시간 표시
	LCD_DisplayText(2, 11, "100"); // 초기 주기 표시
}

uint8_t key_flag = 0;
uint16_t KEY_Scan(void)	// input key SW0 - SW7 
{
	uint16_t key;
	key = GPIOH->IDR & 0xFF00;	// any key pressed ?
	if (key == 0xFF00)		// if no key, check key off
	{
		if (key_flag == 0)
			return key;
		else
		{
			DelayMS(10);
			key_flag = 0;
			return key;
		}
	}
	else				// if key input, check continuous key
	{
		if (key_flag != 0)	// if continuous key, treat as no key input
			return 0xFF00;
		else			// if new key,delay for debounce
		{
			key_flag = 1;
			DelayMS(10);
			return key;
		}
	}
}
