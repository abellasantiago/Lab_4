#include <avr/io.h>

extern void mostrar_voltaje(uint16_t centivoltios);

int main(void)
{
    // CONFIGURACIÓN DEL ADC
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    uint16_t resultado_adc;
    uint32_t suma;
    uint16_t promedio;
    uint16_t centivoltios = 0;

    while (1)
    {
        // PROMEDIAR 100 CONVERSIONES
        suma = 0;
        for (uint8_t i = 0; i < 100; i++)
        {
            ADCSRA |= (1 << ADSC);
            while (ADCSRA & (1 << ADSC));
            resultado_adc = ADC;
            suma += resultado_adc;
        }
        promedio = suma / 100;

        // CONVERTIR A CENTIVOLTIOS
        centivoltios = ((uint32_t)promedio * 500UL) / 1023UL;

        // MOSTRAR EN EL DISPLAY muchas veces para que se vea estable
        for (uint8_t r = 0; r < 50; r++)
        {
            mostrar_voltaje(centivoltios);
        }
    }
}