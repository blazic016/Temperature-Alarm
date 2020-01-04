
#ifndef F_CPU
#define F_CPU 1000000UL
#endif


#define A0 eS_PORTA0 // PIN SENZOR 1
#define A1 eS_PORTA1 // PIN SENZOR 2
//#define BUT3 eS_PORTA2
//#define BUT4 eS_PORTA3
//#define A4 eS_PORTA4
//#define A5 eS_PORTA5
//#define A6 eS_PORTA6
//#define A7 eS_PORTA7
#define BUT3 eS_PORTB0
#define BUT4 eS_PORTB1
///#define BUT3 eS_PORTB2
//#define B3 eS_PORTB3
//#define B4 eS_PORTB4
//#define B5 eS_PORTB5
//#define B6 eS_PORTB6
//#define B7 eS_PORTB7
#define BUT1 eS_PORTC0
#define BUT2 eS_PORTC1
//#define BUT4 eS_PORTC2
//#define BUT4 eS_PORTC3
#define C4 eS_PORTC4
#define C5 eS_PORTC5
#define RS eS_PORTC6 //LCD
#define EN eS_PORTC7 //LCD
//#define D0 eS_PORTD0
#define LED1 eS_PORTD1
#define LED2 eS_PORTD2
#define LED_TEST eS_PORTD3
#define D4 eS_PORTD4 //LCD
#define D5 eS_PORTD5 //LCD
#define D6 eS_PORTD6 //LCD
#define D7 eS_PORTD7 //LCD


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "pins.h"
#include "LCD.h"

#define get_bit(reg,bitnum) ((reg & (1<<bitnum))>>bitnum)

int getSenzor1();
int getSenzor2();
void monitoring(int x, int y, int x_limit, int y_limit);

int temp1;
int temp2;
int alarm_1 = 15;
int alarm_2 = 15;
char KARAKTER [2];



int main(void)
{

	//LM35 CONFIG
	//pinConfig(A0,1); //Input, DDRA&=~(1<<0);
	//pinConfig(A1,1);

	//LED CONFIG
	pinConfig(LED1,0);
	pinConfig(LED2,0);
	pinConfig(LED_TEST,0);
	
	//BUTTON CONFIG (Input)
	pinConfig(BUT1,1);
	pinConfig(BUT2,1);
	pinConfig(BUT3,1);
	pinConfig(BUT4,1);
	
	//LCD CONFIG
	pinConfig(D4,0);
	pinConfig(D5,0);
	pinConfig(D6,0);
	pinConfig(D7,0);
	pinConfig(RS,0);
	pinConfig(EN,0);
	Lcd4_Init();
	//_delay_ms(1000);
	Lcd4_Clear();
	
	while (1) {

		
		//Sensor State
		temp1 = getSenzor1();
		//temp1 = 20;
		if (temp1 >= alarm_1) {pinChange(LED1,1);} else {pinChange(LED1,0);}
		temp2 = getSenzor2();
		//temp2 = 20;
		if (temp2 >= alarm_2) {pinChange(LED2,1);} else {pinChange(LED2,0);}
		


		
		if (pinScan(BUT1) == 1) {
			alarm_1--;
			//while (pinScan(BUT1) == 1)  {}
		}
		if (pinScan(BUT2) == 1) {
			alarm_1++;
			//while (pinScan(BUT2) == 1)  {}
		}
		if (pinScan(BUT3) == 1) {
			alarm_2--;
			//while (pinScan(BUT3) == 1)  {}
		}
		if (pinScan(BUT4) == 1) {
			alarm_2++;
			//while (pinScan(BUT4) == 1)  {}
		}

		
		// Monitoring
		monitoring( temp1,  temp2,  alarm_1,  alarm_2);
		
	}
	
}


void monitoring(int sen1, int sen2, int alarm_1, int alarm_2) {
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("SEN1:");
	Lcd4_Set_Cursor(1,6);
	itoa(sen1,KARAKTER,10);
	Lcd4_Write_String(KARAKTER);

	Lcd4_Set_Cursor(2,1);
	Lcd4_Write_String("SEN2:");
	Lcd4_Set_Cursor(2,6);
	itoa(sen2,KARAKTER,10);
	Lcd4_Write_String(KARAKTER);
	
	Lcd4_Set_Cursor(1,12);
	itoa(alarm_1,KARAKTER,10);
	Lcd4_Write_String(KARAKTER);

	Lcd4_Set_Cursor(2,12);
	itoa(alarm_2,KARAKTER,10);
	Lcd4_Write_String(KARAKTER);
}



int getSenzor1() {
	ADMUX=0b11000000; //A0
	ADCSRA=0b10000111;
	ADCSRA|=(1<<6); //Set bit 6 n ADCSRA to start conversion
	while(get_bit(ADCSRA,6)==1) {}
	int ADCOut = ADCL|(ADCH<<8); // Save the ADC reading into an integer variable ADCOut. The ADCL must be read first as written here.
	return ADCOut/4; // Calculate Temperature
}

int getSenzor2() {
	ADMUX=0b11000001; //A1
	ADCSRA=0b10000111;
	ADCSRA|=(1<<6); //Set bit 6 n ADCSRA to start conversion
	while(get_bit(ADCSRA,6)==1) {}
	int ADCOut = ADCL|(ADCH<<8); // Save the ADC reading into an integer variable ADCOut. The ADCL must be read first as written here.
	return ADCOut/4; // Calculate Temperature
}

