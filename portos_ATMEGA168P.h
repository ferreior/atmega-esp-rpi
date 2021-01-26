#define DDRBbits (*(DDBbits)_SFR_MEM_ADDR(DDRB))
#define PORTBbits (*(PBbits)_SFR_MEM_ADDR(PORTB))
#define PINBbits (*(PIBbits)_SFR_MEM_ADDR(PINB))

#define DDRCbits (*(DDCbits)_SFR_MEM_ADDR(DDRC))
#define PORTCbits (*(PCbits)_SFR_MEM_ADDR(PORTC))
#define PINCbits (*(PICbits)_SFR_MEM_ADDR(PINC))

#define DDRDbits (*(DDDbits)_SFR_MEM_ADDR(DDRD))
#define PORTDbits (*(PDbits)_SFR_MEM_ADDR(PORTD))
#define PINDbits (*(PIDbits)_SFR_MEM_ADDR(PIND))

typedef struct DDBbits_t
{
	unsigned char DDRB0  :1;
	unsigned char DDRB1  :1;
	unsigned char DDRB2  :1;
	unsigned char DDRB3  :1;
	unsigned char DDRB4  :1;
	unsigned char DDRB5  :1;
	unsigned char DDRB6  :1;
	unsigned char DDRB7  :1;
	
}volatile *DDBbits;


typedef struct PBbits_t
{
	unsigned char PB0_  :1;
	unsigned char PB1_  :1;
	unsigned char PB2_  :1;
	unsigned char PB3_  :1;
	unsigned char PB4_  :1;
	unsigned char PB5_  :1;
	unsigned char PB6_  :1;
	unsigned char PB7_  :1;
	
}volatile *PBbits;

typedef struct PINBbits_t
{
	unsigned char PINB0_  :1;		
	unsigned char PINB1_  :1;		
	unsigned char PINB2_  :1;
	unsigned char PINB3_  :1;		
	unsigned char PINB4_  :1;		
	unsigned char PINB5_  :1;
	unsigned char PINB6_  :1;
	unsigned char PINB7_  :1;	
	
}volatile *PIBbits;

//**********************************

typedef struct DDCbits_t
{
	unsigned char DDRC0  :1;
	unsigned char DDRC1  :1;
	unsigned char DDRC2  :1;
	unsigned char DDRC3  :1;
	unsigned char DDRC4  :1;
	unsigned char DDRC5  :1;
	unsigned char DDRC6  :1;
	unsigned char DDRC7  :1;
	
}volatile *DDCbits;

typedef struct PCbits_t
{
	unsigned char PC0_  :1;
	unsigned char PC1_  :1;
	unsigned char PC2_  :1;
	unsigned char PC3_  :1;
	unsigned char PC4_  :1;
	unsigned char PC5_  :1;
	unsigned char PC6_  :1;
	unsigned char PC7_  :1;
	
}volatile *PCbits;

typedef struct PINCbits_t
{
	unsigned char PINC0_  :1;
	unsigned char PINC1_  :1;
	unsigned char PINC2_  :1;
	unsigned char PINC3_  :1;
	unsigned char PINC4_  :1;
	unsigned char PINC5_  :1;
	unsigned char PINC6_  :1;
	unsigned char PINC7_  :1;
	
}volatile *PICbits;

//*********************************************

typedef struct DDDbits_t
{
	unsigned char DDRD0  :1;
	unsigned char DDRD1  :1;
	unsigned char DDRD2  :1;
	unsigned char DDRD3  :1;
	unsigned char DDRD4  :1;
	unsigned char DDRD5  :1;
	unsigned char DDRD6  :1;
	unsigned char DDRD7  :1;
	
}volatile *DDDbits;

typedef struct PDbits_t
{
	unsigned char PD0_  :1;
	unsigned char PD1_  :1;
	unsigned char PD2_  :1;
	unsigned char PD3_  :1;
	unsigned char PD4_  :1;
	unsigned char PD5_  :1;
	unsigned char PD6_  :1;
	unsigned char PD7_  :1;
	
}volatile *PDbits;

typedef struct PINDbits_t
{
	unsigned char PIND0_  :1;
	unsigned char PIND1_  :1;
	unsigned char PIND2_  :1;
	unsigned char PIND3_  :1;
	unsigned char PIND4_  :1;
	unsigned char PIND5_  :1;
	unsigned char PIND6_  :1;
	unsigned char PIND7_  :1;
	
}volatile *PIDbits;

