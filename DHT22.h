#define DHT_DDR DDRD
#define DHT_PORT PORTD
#define DHT_PIN	PIND
#define PIN 2

void DHT22_init(void)
{
	DHT_DDR  |= (1<<PIN);		//output pin
	DHT_PORT |= (1<<PIN);		//pin com estado lógico alto
}

/*		  18ms	   40us	    80us	80us
	¯¯¯¯|_______|¯¯¯¯¯¯¯¯|_______|¯¯¯¯¯¯¯|....dados....
	
	----------UC---------|--------dht22-------------|      
*/					

uint8_t DHT22_read(float *dht_temperatura, float *dht_humidade)
{
	uint8_t bits[5];
	uint8_t i,j=0;
	uint8_t contador = 0;
	
	//1: enviar um sinal low-voltage durante 18ms 
	
	DHT_PORT &= ~(1<<PIN);		//Nivel baixo
	_delay_ms(18);
	DHT_PORT |= (1<<PIN);		//Nivel alto
	DHT_DDR &= ~(1<<PIN);		//input pin
	
	//2: esperamos 20 a 40us até que o dht22 envie 0
	contador = 0;
	while(DHT_PIN & (1<<PIN))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 60)
		{
			DHT_DDR |= (1<<PIN);	//output pin
			DHT_PORT |= (1<<PIN);	//Nivel alto
			return 0;
		}	
	}	
	
	//3: esperamos 80us até que o dht22 envie 1
	contador = 0;
	while(!(DHT_PIN & (1<<PIN)))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 100)
		{
			DHT_DDR |= (1<<PIN);	//output pin
			DHT_PORT |= (1<<PIN);	//Nivel alto
			return 0;
		}	
	}
	
	//4: esperamos 80us até que o dht22 envie 0
	contador = 0;
	while(DHT_PIN & (1<<PIN))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 100)
		{
			DHT_DDR |= (1<<PIN);	//output pin
			DHT_PORT |= (1<<PIN);	//Nivel alto
			return 0;
		}
	}
	
	//5: ler 40 bits do dht22 como 5bytes
	for (j=0; j<5; j++)
	{
	uint8_t result=0;
		for (i=0; i<8; i++)
		{
			while (!(DHT_PIN & (1<<PIN)));
				_delay_us(35);

			if (DHT_PIN & (1<<PIN))
				result |= (1<<(7-i));
					
			while(DHT_PIN & (1<<PIN));
		}
		bits[j] = result;
	}

	DHT_DDR |= (1<<PIN);	//output pin
	DHT_PORT |= (1<<PIN);	//Nivel alto
	
	//6: Conversão da temperatura e da humidade		
	
	if ((uint8_t) (bits[0] + bits[1] + bits[2] +bits[3]) == bits[4])
	{
		uint16_t int_humidade = bits[0]<<8 | bits[1];
		uint16_t int_temperatura = bits[2] <<8 | bits[3];
	
		
		if (int_temperatura & 0x8000)  //Conversor para temperaturas negativas
		{
			*dht_temperatura = (float)((int_temperatura & 0x7fff) / 10.0)* -1.0;
		}else{
			*dht_temperatura = (float)(int_temperatura)/10.0; //Conversor para temperaturas positivas (int->dec)
		}

		*dht_humidade = (float)(int_humidade)/10.0; //Conversor para humidade (int->dec)

		return 1;
	}
	return 1;
}