#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int x;
int tmp = 0;
int PIR = 0;
int CO = 0;
int count = 0;
int main(void)
{
	ADCSRA |= 1<<ADPS2;
	ADMUX |= 1<<REFS0 | 1<<REFS1|1<<8;
	ADCSRA |= 1<<ADIE;
	ADCSRA |= 1<<ADEN;
	ADCSRA |= 1<<8;
	DDRG=0xff;
	DDRB=0xff;
	DDRC=0xff;
	DDRD=0xff;
	DDRE=0xff;
	DDRA=0;
	sei();

	ADCSRA |= 1<<ADSC;

	while (1)
	{
		
	}
}

ISR(ADC_vect)
{
	uint8_t theLow = ADCL;
	uint16_t theTenBitResult = ADCH<<8 | theLow;
	uint8_t cmpare0=0x00;
	uint8_t cmpare1=0x00;
	uint8_t cmpare2=0x00;
	switch(ADMUX)
	{
		case 0xC0:{//temp
			PORTD = theLow;
			if(PORTD>=0x93) cmpare0|=0x01;
			else cmpare0&=0x00;
			ADMUX = 0xC1;
			PORTG =1;
			_delay_ms(100);
			break;
		}
		case 0xC1:{//CO2
			PORTC = theLow;
			if(PORTC<0x26) cmpare1|=0x05;
			else cmpare1&=0x00;
			ADMUX = 0xC2;
			PORTG =0;
			_delay_ms(100);
			break;
		}
		case 0xC2:{//PIR
			PORTB = theLow;
			if(PORTB>=0x40) cmpare2|=0x08;
			else cmpare2&=0x00;
			ADMUX = 0xC0;
			PORTG =2;
			_delay_ms(100);
			break;
		}
		default : break;
	}	
	PORTE=cmpare0|cmpare1|cmpare2;
	ADCSRA |= 1<<ADSC;
}
