#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL

void mostrar_numero_y_cara(uint8_t numero, char cara) {
    // Mostrar número en la matriz 8x8
    // Mostrar cara :) o :( según el valor de 'cara'
}

int main(void) {
    DDRB = 0xFF; // Columnas como salida
    DDRD = 0xFF; // Filas como salida
    DDRC = 0x00; // PC como entrada (para comunicación con PIC)

    while (1) {
        uint8_t mensaje = PINC & 0x3F; // Leer PC0-PC5
        
        if (mensaje == 0x00) {
            // No mostrar nada
        } else if (mensaje == 0x01) {
            // Mostrar "EMPEZAMOS" y luego "?"
        } else if (mensaje == 0x02) {
            // Leer PC2-PC5 para obtener el número
            uint8_t numero = (PINC >> 2) & 0x0F;
            mostrar_numero_y_cara(numero, ':)'); // Cara feliz
        } else if (mensaje == 0x03) {
            // Leer PC2-PC5 para obtener el número
            uint8_t numero = (PINC >> 2) & 0x0F;
            mostrar_numero_y_cara(numero, ':('); // Cara triste
        }
    }
}

