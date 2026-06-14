 
 // Program to blink the inbuilt LED of ATmega328P Xplained mini
 // #define F_CPU 16000000UL // defino 16mhz la velocidad del micro
 #include <avr/io.h>
 #include <avr/delay.h>
 #define LED_ON  PORTB = 0b11101111
 #define LED_OFF PORTB = 0b11111111
// Defino la rutina de assembler que voy a llamar
 extern void delay_joel ();
 // Definiendo acceso a los registros
 unsigned char *pr18 = 0x12;
 unsigned char *pr19 = 0x13;
 unsigned char *pr20 = 0x14;
 int main(void)
 {
	 DDRB = 0b00111100;	// Output
	 PORTB = 0xFF;	// Clear the PORTB
	 DDRC = 0X00;

	 while (1)
	 {
		 
		 *pr18=0xFF;// cargo valor para delay0
		 *pr19=0xFF; //cargo valor para delay1
		 *pr20=0x20; //cargo valor para delay2
		 delay_joel();
		 LED_ON;
		 *pr18=0xFF;// cargo valor para delay0
		 *pr19=0xFF; //cargo valor para delay1
		 *pr20=0x20; //cargo valor para delay2
		 delay_joel();
		 LED_OFF;
	 }
 }
 
 
