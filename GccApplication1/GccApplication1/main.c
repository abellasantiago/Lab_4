 
 #include <avr/io.h>
 #include <avr/delay.h>

 // Por ahora no necesitamos el extern ni los punteros a registros
 // Eso lo agregamos en la Parte 2 cuando mezclemos con Assembly

 int main(void)
 {
	 // =========================================================
	 // ÍTEM i) — CONFIGURACIÓN DEL ADC
	 // =========================================================

	 // ADMUX: le decimos al ADC QUÉ canal leer y con qué referencia de voltaje
	 //
	 // (1 << REFS0) ? pone REFS0=1, REFS1=0 ? referencia = AVCC = 5V
	 //                es el voltaje de la placa, el mismo máximo del potenciómetro
	 //
	 // El resto del registro queda en 0 ? MUX=0000 ? canal ADC0
	 // ADC0 es el pin PC0, donde está conectado el potenciómetro en la placa
	 ADMUX = (1 << REFS0);

	 // ADCSRA: le decimos al ADC CÓMO funcionar
	 //
	 // (1 << ADEN)  ? habilita el ADC. Sin esto el conversor está apagado
	 //
	 // (1 << ADPS2) |
	 // (1 << ADPS1) | ? prescaler = 128
	 // (1 << ADPS0)   El ADC necesita un clock entre 50kHz y 200kHz
	 //                El micro va a 16MHz ? 16.000.000 / 128 = 125.000 Hz ?
	 ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	 // =========================================================
	 // VARIABLES
	 // =========================================================

	 uint16_t resultado_adc;   // guarda una conversión individual (0 a 1023)
	 uint32_t suma;            // suma de 100 conversiones
	 // usamos 32 bits porque 1023 × 100 = 102.300
	 // no cabe en 16 bits (máximo 65.535)
	 uint16_t promedio;        // promedio de las 100 conversiones (0 a 1023)
	 uint16_t centivoltios;    // voltaje en centivoltios (0 a 500 = 0.00V a 5.00V)

	 while (1)
	 {
		 // =========================================================
		 // ÍTEM ii) — PROMEDIAR 100 CONVERSIONES
		 // =========================================================

		 // ¿Por qué promediar?
		 // Los bits menos significativos del ADC tienen ruido eléctrico.
		 // Si leés una sola vez podés obtener 512 o 514 o 511 aunque el
		 // potenciómetro no se movió. Promediando 100 lecturas ese ruido
		 // se cancela solo y obtenés un valor estable.

		 suma = 0;  // reiniciamos la suma antes de cada ciclo de 100 lecturas

		 for (uint8_t i = 0; i < 100; i++)
		 {
			 // Arrancar una conversión:
			 // Ponemos ADSC=1 ? el ADC empieza a convertir
			 ADCSRA |= (1 << ADSC);

			 // Esperar que termine:
			 // El hardware pone ADSC=0 automáticamente cuando termina.
			 // Este while queda loopeando hasta que eso pase.
			 while (ADCSRA & (1 << ADSC));

			 // Leer el resultado:
			 // El registro ADC combina ADCL y ADCH y nos da el valor entre 0 y 1023
			 resultado_adc = ADC;

			 // Acumulamos en la suma
			 suma += resultado_adc;
		 }

		 // Dividimos la suma entre 100 para obtener el promedio
		 promedio = suma / 100;

		 // =========================================================
		 // ÍTEM iii) — CONVERTIR A VOLTAJE (en centivoltios)
		 // =========================================================

		 // ¿Por qué centivoltios y no float?
		 // El ATmega328P no tiene unidad de punto flotante.
		 // Usar float obliga al compilador a generar muchísimo código extra.
		 // En cambio con enteros es rápido y liviano.
		 //
		 // Centivoltios significa que guardamos el voltaje × 100:
		 //   2.54V ? 254 centivoltios
		 //   5.00V ? 500 centivoltios
		 //   0.00V ? 0 centivoltios
		 //
		 // La fórmula viene de despejar VIN de la fórmula del ADC:
		 //   ADC = (VIN × 1024) / VRef
		 //   VIN = (ADC × VRef) / 1024
		 //
		 // Como VRef = 5V y queremos centivoltios (×100):
		 //   centivoltios = (promedio × 500) / 1023
		 //
		 // Usamos 1023 porque ese es el valor máximo real del ADC de 10 bits
		 // (va de 0 a 1023, no de 0 a 1024)

		 centivoltios = ((uint32_t)promedio * 500) / 1023;
		 // Nota: el cast a uint32_t es porque promedio × 500 puede dar hasta
		 // 1023 × 500 = 511.500, que no cabe en uint16_t (máximo 65.535) ?
		 // Sí cabe, pero es buena práctica para evitar overflow en otros casos

	 } // fin while
 }
 
 
 
 
 
 
 
 
 
 /*
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
 
*/
 