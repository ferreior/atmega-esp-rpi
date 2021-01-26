#include <stdlib.h>	//include integers
#include <stdint.h>	//include uint

#include "portos_ATMEGA168P.h"

//Configuração dos pinos de Data do LCD D4-D7 (4bit-mode)

#define D4			PORTDbits.PD5_          // pin 11 lcd
#define D5          PORTDbits.PD6_          // pin 12 lcd
#define D6          PORTDbits.PD7_          // pin 13 lcd
#define D7          PORTBbits.PB0_          // pin 14 lcd

#define DDR_D4      DDRDbits.DDRD5			//D4 Out
#define DDR_D5      DDRDbits.DDRD6			//D5 Out
#define DDR_D6      DDRDbits.DDRD7			//D6 Out
#define DDR_D7      DDRBbits.DDRB0			//D7 Out

//Pinos de controlo para o LCD

#define RS          PORTDbits.PD3_          //Pino 4 LCD
#define EN          PORTDbits.PD4_          //Pino 6 LCD
#define DDR_RS		DDRDbits.DDRD3          //RS Out
#define DDR_EN		DDRDbits.DDRD4          //EN Out

#define ON  1
#define OFF 0

#define LCD_CLEAR       0X01    //Clear LCD
#define LCD_CURSOR_ON   0X0F    //Cursor ON
#define LCD_CURSOR_OFF  0X0C    //Cursor OFF
#define LCD_LINHA1      0X02    //Primera linha

void LCD_init(void);                    	//Inicializa LCD
void LCD_write_nible(unsigned char nible);
void LCD_cmd(unsigned char);                //Envia comando
void LCD_dato(char);						//Envia dato
void LCD_write(unsigned char dato);         //Função para enviar dados
void LCD_printf(char*);						//Imprime strings de carateres
void LCD_segunda_linha(void);
void LCD_clear(void);

//Inicializa o LCD em 4-bit mode

void LCD_init(void)
{   
	 EN = 0;                     //Clear dos pinos de controlo
	 RS = 0;
	 D4 = 0;                     //Clear dos pinos de dados
	 D5 = 0;
	 D6 = 0;
	 D7 = 0;

	 DDR_EN = 1;                 //Pinos de controlo como saída
	 DDR_RS = 1;
	 DDR_D4 = 1;                 //Pinos de dados como saída
	 DDR_D5 = 1;
	 DDR_D6 = 1;
	 DDR_D7 = 1;

	 _delay_ms(20);              

	 LCD_write_nible(0x03);
	 _delay_ms(5);
	 LCD_write_nible(0x03);
	 _delay_us(100);
	 LCD_write_nible(0x03);
	 _delay_us(100);
	 LCD_write_nible(0x02);        //Initial function set to change interface,
	 _delay_us(100);               //Configurar modo 4 bits

	 LCD_cmd(LCD_CURSOR_OFF);      //Cursor apagado
	 LCD_cmd(LCD_CLEAR);

}

//Função que permite enviar dados e comandos

void LCD_write_nible(unsigned char nible)
{  
    if (nible & (1<<0)) D4 = 1; else D4 = 0;             
    if (nible & (1<<1)) D5 = 1; else D5 = 0;
    if (nible & (1<<2)) D6 = 1; else D6 = 0;
    if (nible & (1<<3)) D7 = 1; else D7 = 0;
    
    EN = 1;     
    _delay_us(1);
    EN = 0; 
}

//Envia comandos para o lCD

void LCD_cmd(uint8_t dato)
{
    RS = 0; 
	asm("nop");
    LCD_write_nible(dato>>4);
    LCD_write_nible(dato & 0x0F);
	if (dato == LCD_CLEAR || dato == LCD_LINHA1)
		_delay_ms(2);
	else
		_delay_us(50);
}

//Envia dados para o LCD

void LCD_dato(char dato)
{
	RS = 1;
	asm("nop");	
    LCD_write_nible(dato>>4);
    LCD_write_nible(dato & 0x0F);
	_delay_us(50);
}

//Imprime uma string no LCD

void LCD_printf(char *dato) {
    while (*dato){
        LCD_dato(*dato);    // Envia dados ao LCD
        dato++;             // Incremeta o buffer de dados
    }
}

//Mete o LCD no primeiro cursor da segunda linha

void LCD_segunda_linha(){
     LCD_cmd(0xC0);
}

//Clear LCD

void LCD_clear(void) {
     LCD_cmd(0x01);
     _delay_ms(3);
}