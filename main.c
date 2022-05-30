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

void ADC_init() {

	ADMUX |= (1<<(REFS0));// ustawianie napiďż˝cia referencyjnego AVCC


	ADCSRA |= (1<<(ADPS0));// ustawianie podzielnika czďż˝stotliwoďż˝ci na 2
	ADCSRA |= (1<<(ADEN));  // uruchomineie ukďż˝adu przetwornika
}

int convertToVoltage(uint16_t current) {
	return (5.00 * current / 1023.00) * 100.00;
}

uint16_t ADC_10bit() {
	ADCSRA |= (1<<(ADSC));
	loop_until_bit_is_set(ADCSRA, ADSC);
	return ADC;
}

int main() {
	LCD_HD44780::init();
	LCD_HD44780::home();
	char text[20]; // zmienna pomocnicza
	int votage;
	ADC_init(); // inicjacja przetwornika A
	DDRC |= (1<<(3));

	LCD_HD44780::goTo(0, 0);
	while (1) {
		votage = convertToVoltage(ADC_10bit());
		LCD_HD44780::clear();  // czysci wyswietlacz


		LCD_HD44780::goTo(0, 0); // przechodzi do pierwszej linijki
		sprintf(text, "Napiecie: %d", votage); // zapisuje nowy ciďż˝g znakďż˝w do wyďż˝wietlenia
		LCD_HD44780::writeText(text);  // wyswietla zadany ciďż˝g znakďż˝w

		LCD_HD44780::goTo(0, 1);  // przechodzi do pierwszej linijki

		if (votage > 10) {
			sprintf(text, "On");
			sbi(PORTC, 3);
		} else {
			sprintf(text, "Off");
			cbi(PORTC, 3);
		}
		LCD_HD44780::writeText(text); // wyswietla zadany ciďż˝g znakďż˝w

		_delay_ms(1000);
	}

}
