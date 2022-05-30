#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "LCD_HD44780.h"

#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif

#ifndef inb
#define	inb(addr)			(addr)
#endif

#ifndef outb
#define	outb(addr, data)	addr = (data)
#endif

#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

#ifndef tbi
#define tbi(reg,bit)		reg ^= (_BV(bit))
#endif

#define MAX_VOLTAGE = 5;
/*
 *  Gotowe zaimplementowane:
 #define 	bit_is_set(sfr, bit)   (_SFR_BYTE(sfr) & _BV(bit))
 #define 	bit_is_clear(sfr, bit)   (!(_SFR_BYTE(sfr) & _BV(bit)))
 #define 	loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
 #define 	loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))

 */

// MIN/MAX/ABS macros
#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))

void ADC_init() {

	sbi(ADMUX, REFS0); // ustawianie napi�cia referencyjnego AVCC

	sbi(ADCSRA, ADPS0); // ustawianie podzielnika cz�stotliwo�ci na 2
	sbi(ADCSRA, ADEN); // uruchomineie uk�adu przetwornika
}

int convertToVoltage(uint16_t current) {
	return (5.00 * current / 1023.00) * 100.00;

}

uint16_t ADC_10bit() {
	sbi(ADCSRA, ADSC);
	loop_until_bit_is_set(ADCSRA, ADSC);
	return ADC;
}

int main() {
	LCD_HD44780::init();
	LCD_HD44780::home();
	char text[20]; // zmienna pomocnicza
	int votage;
	ADC_init(); // inicjacja przetwornika A/D
	sbi(DDRC, 3);

	while (1) {
		votage = convertToVoltage(ADC_10bit());
		LCD_HD44780::clear();  // czysci wyswietlacz


		LCD_HD44780::goTo(0, 0); // przechodzi do pierwszej linijki
		sprintf(text, "Napiecie: %d", votage); // zapisuje nowy ci�g znak�w do wy�wietlenia
		LCD_HD44780::writeText(text);  // wyswietla zadany ci�g znak�w

		LCD_HD44780::goTo(0, 1);  // przechodzi do pierwszej linijki

		if (votage > 250) {
			sprintf(text, "On");
			sbi(PORTC, 3);
		} else {
			sprintf(text, "Off");
			cbi(PORTC, 3);
		}
		LCD_HD44780::writeText(text); // wyswietla zadany ci�g znak�w

		_delay_ms(1000);
	}

}
