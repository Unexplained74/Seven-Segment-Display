#include "stm32f10x.h"                  // Device header
#include <stdbool.h>

// External Variable Declerations
extern uint16_t buttonState1;
extern uint16_t buttonState2;
extern uint16_t buttonState3;
extern int16_t number;
extern uint32_t timer1;
extern bool flag1;
extern bool flag2;
extern uint16_t speed;
extern int16_t thenumber;

// Variables
uint16_t buttonState1 = 0;
uint16_t buttonState2 = 0;
uint16_t buttonState3 = 0;
int16_t number = 0;
uint32_t timer1 = 0;
bool flag1 = true;
bool flag2 = true;
uint16_t speed = 10;
int16_t thenumber = 0;


enum displayPin {
	//DPP -> Display Pin
	DPP_A = GPIO_Pin_8,
	DPP_B = GPIO_Pin_10,
	DPP_C = GPIO_Pin_5,
	DPP_D = GPIO_Pin_12,
	DPP_E = GPIO_Pin_11,
	DPP_F = GPIO_Pin_9,
	DPP_G = GPIO_Pin_7,
	DPP_Dp = GPIO_Pin_6,
};

enum digitPin {
	D1 = GPIO_Pin_3,
	D2 = GPIO_Pin_2,
	D3 = GPIO_Pin_1,
	D4 = GPIO_Pin_0,
};

enum sign {
	//A B C D E F G Dp	// 1 is on 0 is off for the led
	//---------------------
	//- -> 00000010 -> 0x02
	//0 -> 11111100 -> 0xFC
	//1 -> 01100000 -> 0x60
	//2 -> 11011010 -> 0xDA
	//3 -> 11110010 -> 0xF2
	//4 -> 01100110 -> 0x66
	//5 -> 10110110 -> 0xB6
	//6 -> 10111110 -> 0xBE
	//7 -> 11100000 -> 0xE0
	//8 -> 11111110 -> 0xFE
	//9 -> 11110110 -> 0xF6
	//A -> 11101110 -> 0xEE
	//B -> 00111110 -> 0x3E
	//C -> 10011100 -> 0x9C
	//D -> 01111010 -> 0x7A
	//E -> 10011110 -> 0x9E
	//F -> 10001110 -> 0x8E
	//---------------------
	negative = 0x02,
	zero = 0xFC,
	one = 0x60,
	two = 0xDA,
	three = 0xF2,
	four = 0x66,
	five = 0xB6,
	six = 0xBE,
	seven = 0xE0,
	eight = 0xFE,
	nine = 0xF6,
	A = 0xEE,
	B = 0x3E,
	C = 0x9C,
	D = 0x7A,
	E = 0x9E,
	F = 0x8E,
	on = 0xFF,
	off = 0x00,
};

// Function Declerations
int16_t absShort(int16_t);
void gpioConfig(void);
void delay(uint32_t);
void printToDigit(enum sign x, enum digitPin d);
void clearDigit(enum digitPin d);
void clearAllDigits(void);
void printToDisplay(enum sign s1, enum sign s2, enum sign s3, enum sign s4);
void countAll(uint32_t);
void printNumber(int16_t);
int16_t countNumber(bool, uint32_t);

// Return absolute value of a short int
int16_t absShort(int16_t intgr){
	if(intgr >= 0){return intgr;}
	else{return (-1*intgr);}
}

// Configuration Function
void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// Outputs
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
															 GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
															 GPIO_Pin_12 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStructure);
	
	// Inputs
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIOInitStructure);	
}

void delay(uint32_t time){for(;time > 0; time--);}	// Delay Function

// Print any symbol from sign data type to desired digit of the display
void printToDigit (enum sign x, enum digitPin d){
	GPIO_SetBits(GPIOA, d);
	const enum displayPin pinArray[8] = {DPP_Dp, DPP_G, DPP_F, DPP_E, DPP_D, DPP_C, DPP_B, DPP_A};
	for(int i=0; i<8; i++){
		if((x >> i) & 0x01){GPIO_ResetBits(GPIOA, pinArray[i]);}
		else{GPIO_SetBits(GPIOA, pinArray[i]);}
	}
}

// Clears desired digit
void clearDigit(enum digitPin d){
	GPIO_ResetBits(GPIOA, d);
}

// Clears all digits
void clearAllDigits(){
	GPIO_ResetBits(GPIOA, D1);
	GPIO_ResetBits(GPIOA, D2);
	GPIO_ResetBits(GPIOA, D3);
	GPIO_ResetBits(GPIOA, D4);
}
// Prints inputed sign to according digit of the display
void printToDisplay(enum sign s1, enum sign s2, enum sign s3, enum sign s4){
			uint32_t framespeed = 1000;
			printToDigit(s1, D1);
			delay(framespeed);
			clearDigit(D1);
			printToDigit(s2, D2);
			delay(framespeed);
			clearDigit(D2);
			printToDigit(s3, D3);
			delay(framespeed);
			clearDigit(D3);
			printToDigit(s4, D4);
			delay(framespeed);
			clearDigit(D4);
}
// counts 0 to 16 in hexadecimal and prints the same number to all digits
void countAll(uint32_t delayTime){
	const enum sign numberArray[16] = {zero, one, two, three, four, five, six, seven, eight, nine, A, B, C, D, E, F};
	for(int i=0; i<16; i++){
		printToDisplay(numberArray[i], numberArray[i], numberArray[i], numberArray[i]);
		delay(delayTime);
	}
}
// Prints any integer between -999 and 999 to the display
void printNumber(int16_t numb){
	int16_t first = absShort(numb % 10);
	int16_t second = absShort((numb / 10) % 10);
	int16_t third = absShort((numb / 100) % 10);
	enum sign numberArr[10] = {zero, one, two, three, four, five, six, seven, eight, nine};
	if(numb < 0){
		printToDisplay(negative, numberArr[third], numberArr[second], numberArr[first]);
	}
	else{
		printToDisplay(off, numberArr[third], numberArr[second], numberArr[first]);
	}
}
// Counts numbers between -999 and 999.
// Counting speed and counting up or down can be arranged.
// Returns the number at that time
int16_t countNumber(bool updown, uint32_t cdelay){
	printNumber(number);
		// timer
		if (timer1 >= cdelay){
			if(updown){number++;}
			else{number--;}
			timer1 = 0;
		}
		timer1++;
		if(number >= 1000){
			number = -999;
		}
		if(number <= -1000){
			number = 999;
		}
	return number;
}

int main(){
	gpioConfig();
	
	while(1){	
		buttonState1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
		buttonState2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
		buttonState3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
		
		if(buttonState1){
			if(flag1){flag1 = false;}
			else{flag1 = true;}
			delay(720000);
		}
		if(buttonState2){
			speed += 10;
			if(speed > 100){speed = 10;}
			delay(720000);
		}
		if(buttonState3){
			if(flag2){flag2 = false;}
			else{flag2 = true;}
			delay(720000);
		}
		if(flag2){thenumber = countNumber(flag1, speed);}
		else{printNumber(thenumber);}
		
	}
}
