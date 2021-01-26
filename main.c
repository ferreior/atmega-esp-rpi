#define F_CPU 16000000UL
#define BAUD_RATE 115200                                     //BAUD RATE ESP8266-01
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1) //Gets ignored as the value for UBRR0 at 115200 BAUD RATE is floating point and the error rate was too high

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "LCD.h"
#include "DHT22.h"

volatile unsigned int contador = 500;

void init(void){
	 DDRB   = 0b00001000; //PB3 como entrada 
	 PORTB  = 0;
	 TCCR2A = 0b00000010; // Modo Ctc
	 TCCR2B = 0b00000101; //Prescaler 128
	 TIMSK2 |= (1<<OCIE2A); //Inicializa a interrupção no Timer2 por Compare Match
	 OCR2A  = 255; //Para dar 1s
	 sei();
}

ISR(TIMER2_COMPA_vect){
	
	contador--;
	if(contador == 0) {
		PORTB ^= (1<<PORTB3);
		contador = 500; //Para fazer um 1 segundo
	}
}

void ADC_Init()
{
	DDRC=0x0;		                //Configuração ADC como entrada
	ADCSRA = 0x87;		            //Enable ADC, fr=128
}

int ADC_Read() {
	
	ADMUX = 0x40;		             // Vref: Avcc, ADC channel: 0
	ADCSRA |= (1<<ADSC);	         // start conversion
	while ((ADCSRA &(1<<ADIF))==0);	 // monitor end of conversion interrupt flag
	ADCSRA |=(1<<ADIF);	             // set the ADIF bit of ADCSRA register
	return(ADCW);		             // return the ADCW = ADCH + ADCL
}

void usart_init (void) {
	
	UBRR0 = 8;                       //Manually setting as the formula give a float value that has too much error.
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); //Dá enable a Rx e a Tx para haver comunicação USART
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00); //tamanho do char = 8 bits
}

void USART_send( unsigned char data) {
	
	while (!(UCSR0A & (1 << UDRE0))); //wait until UDR0 is empty
	UDR0 = data;                      //transmit ch
	
}

void USART_putstring(char* StringPtr) {
	
	while (*StringPtr != 0x00) { //espera pela string
		USART_send(*StringPtr); //manda a string
		StringPtr++;
	}
}

int main() {
	
	char thingSpeakUpdate[512]; //tamanho preciso para mandar os dados
	char printbuff[8]; //precisamos de dois printbuffs para converter os dados em strings
	char printbuff2[8];
	float temperatura; //variavel determinada pela função DHT_read
	float humidade;//variavel determinada pela função DHT_read
	
	uint8_t contador = 200;
	
	init();
	LCD_init();
	DHT22_init();
	ADC_Init();
	usart_init();
	
    while (1)
    {	
		float adc_value = ADC_Read();
		ADCSRA |= (1<<ADSC);            //start conversion
		while((ADCSRA&(1<<ADIF))==0); //wait for conversion to finish
		ADCSRA |= (1<<ADIF);
		int chuva = ADCL;
		chuva = chuva | (ADCH<<8);
		chuva = 100 - (adc_value*100.00)/1023.00; //finish properly formatting tempC value
		contador++;
		if(contador >= 200) {			            //ler dht22 a cada 200x10ms = 2000ms
			contador=0;
			uint8_t status = DHT22_read(&temperatura, &humidade); //le os valores
			if (status) { //quando lê os valores faz este if
				
				LCD_clear();
				LCD_printf("Temp. = ");
				dtostrf(temperatura, 2, 2, printbuff); //Converte um float para uma string de carateres (variavel float, tamanho da string, nº casas decimais, vetor que guarda o resultado)
				LCD_printf(printbuff);
				LCD_printf(" C");
				
				LCD_segunda_linha();
				LCD_printf("Humi. = ");
				dtostrf(humidade, 2, 2, printbuff2); //Converte um float para uma string de carateres (variavel float, tamanho da string, nº casas decimais, vetor que guarda o resultado)
				LCD_printf(printbuff2);
				LCD_printf(" %");
				
				char setMUX[] = "AT+CIPMUX=0\r\n"; //configura para um unico ip
				USART_putstring(setMUX);
				_delay_ms(1000);
				
				char thingSpeakStart[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"; //configura para o site do Thingspeak
				USART_putstring(thingSpeakStart);
				_delay_ms(1000);
				
				char thingSpeakSend[] = "AT+CIPSEND=512\r\n"; //envia os dados
				USART_putstring(thingSpeakSend);
				_delay_ms(1000);
				snprintf(thingSpeakUpdate, sizeof(thingSpeakUpdate), "GET https://api.thingspeak.com/update?api_key=Y5I5VT23VMQG1TRZ&field1=%s&field2=%s&field3=%d\r\n", printbuff, printbuff2, chuva); //sprint com tamanho n = sizeof(thingspeakupdate)
				USART_putstring(thingSpeakUpdate);
				_delay_ms(1000);
				char thingSpeakClose[] = "AT+CIPCLOSE\r\n"; // e fecha
				USART_putstring(thingSpeakClose);
				_delay_ms(1000);
			} 
			
			else {
				LCD_clear();
				LCD_printf("Erro");
				printf("ERRO\n\r");
				}
			{
			    _delay_ms(10);
			}		
		}
	}
}