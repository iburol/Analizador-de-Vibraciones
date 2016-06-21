/*
 *
 * Author: Ismael Burgos
 *
 * Blog: https://electroprogramacionblog.wordpress.com
 *
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>

#define BUAD 9600
#define BRC ((F_CPU/16/BUAD) - 1)

void ADC_Init (void);
void USART_Init (void);
void TIMER1_Init (void);

#define TX_Buffer 5
int TX_i = 0;

uint8_t dato;
char valor [TX_Buffer];

void main (void)
{
	USART_Init();
	ADC_Init();
	TIMER1_Init();
	
	sei();
	
	while(1)
	{
	
	}
}

void ADC_Init(void)
{
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADCSRB |= (1 << ADTS2) | (1 << ADTS0);
	DIDR0 |= (1 << ADC0D);
}

void USART_Init (void)
{
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void TIMER1_Init (void)
{
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
	OCR1A = 1499;
	TIMSK1 = (1 << OCIE1B);
}
 
ISR(ADC_vect)
{
	dato = ADCH;
	valor [0] = '0' + (dato / 100);
	int xdato  = dato % 100;
	valor[1] = '0' + (xdato / 10);
	valor[2] = '0' + (xdato % 10);
	valor[3] = 13;
	valor[4] = 10;
	UCSR0B |= (1 << UDRIE0);
}

ISR(USART_UDRE_vect)
{
	 if (TX_i < TX_Buffer)
	 {
		 UDR0 = valor[TX_i];
		 TX_i++;
	 }
	 else
	 {
		 TX_i = 0;
		 UCSR0B &= ~(1 << UDRIE0);
	 }
}
