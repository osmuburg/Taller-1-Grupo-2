#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 8000000

// Pines de comunicación con el PIC16F887
#define RD5_PIN   PC5
#define START_PIC PC0

// Arreglos para mensajes
unsigned char signo[8] = {0x0, 0x04, 0x02, 0x01, 0xB1, 0x0A, 0x04, 0x0}; // Mensaje "?"
unsigned char PERDER[8] = {0x81, 0xC3, 0x66, 0x18, 0x18, 0x66, 0xC3, 0x81}; // "X"
unsigned char EMPECEMOS[80] = { // Mensaje "EMPECEMOS"
    0x0, 0x7E, 0x7E, 0x5A, 0x5A, 0x5A, 0x5A, 0x0, // E
    0x0, 0x7E, 0x04, 0x08, 0x08, 0x04, 0x7E, 0x0, // M
    0x0, 0x7E, 0x7E, 0x12, 0x12, 0x1E, 0x1E, 0x0, // P
    0x0, 0x7E, 0x7E, 0x5A, 0x5A, 0x5A, 0x5A, 0x0, // E
    0x0, 0x7E, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x0, // C
    0x0, 0x7E, 0x7E, 0x5A, 0x5A, 0x5A, 0x5A, 0x0, // E
    0x0, 0x7E, 0x04, 0x08, 0x08, 0x04, 0x7E, 0x0, // M
    0x0, 0x7E, 0x7E, 0x66, 0x66, 0x7E, 0x7E, 0x0, // O
    0x0, 0x4E, 0x4E, 0x5A, 0x5A, 0x72, 0x72, 0x0, // s
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0        // ESPACIO
};

// Arreglo para fijar filas
unsigned char PORT[8] = {1, 2, 4, 8, 16, 32, 64, 128};

// Arreglos para numéros
unsigned char uno[8] = {0x0, 0x0, 0x81, 0xFF, 0xFF, 0x80, 0x0, 0x0};
unsigned char dos[8] = {0x0, 0xC2, 0xE3, 0xB1, 0x99, 0x8F, 0x86, 0x0};
unsigned char tres[8] = {0x0, 0xC3, 0x99, 0x99, 0x99, 0xFF, 0xFF, 0x0};
unsigned char cuatro[8] = {0x0, 0x1F, 0x1F, 0x18, 0x18, 0xFF, 0xFF, 0x0};
unsigned char cinco[8] = {0x0, 0x8F, 0x8F, 0x89, 0x89, 0xF9, 0xF9, 0x0};
unsigned char seis[8] = {0x0, 0xFF, 0xFF, 0x91, 0x91, 0xF1, 0xF3, 0x0};
unsigned char siete[8] = {0x0, 0x03, 0xE1, 0xF1, 0x19, 0x0D, 0x07, 0x0};
unsigned char ocho[8] = {0x0, 0xF7, 0xFF, 0x99, 0x99, 0xFF, 0xF7, 0x0};
unsigned char nueve[8] = {0x0, 0x0F, 0x0F, 0x09, 0x09, 0xFF, 0xFF, 0x0};

// Función para mostrar una imagen en la matriz de LEDs
void mostrarMatriz(int side, const unsigned char* imagen) {
    if (side > 8) {
        for (int i = 0; i < side - 8; i++) {
            for (int k = 0; k < 50; k++) {
                for (int j = 0; j < 8; j++) {
                    PORTD = PORT[j]; // Fijar fila
                    PORTB = ~imagen[i + j]; // Mostrar columna
                    _delay_ms(0.05);
                }
            }
        }
    } 
    if (side == 8) {
        for (int k = 0; k < 50; k++) {
            for (int j = 0; j < 8; j++) {
                PORTD = PORT[j]; // Fijar fila
                PORTB = ~imagen[j]; // Mostrar columna
                _delay_ms(0.05);
            }
        }
    }
}

// Función para convertir de binario a decimal (pines PC1 a PC4)
unsigned char binarioADecimal() {
    return ((PINC & 0x1E) >> 1);  // Convertir PC1-PC4 a decimal
}

int main(void) {
    // Configuración de pines
    DDRB = 0xFF;  // Configurar PORTB como salida (columnas de la matriz LED)
    DDRD = 0xFF;  // Configurar PORTD como salida (filas de la matriz LED)
    DDRC = 0x01;  // Configurar PORTC como entrada menos PC0 (comunicación con el PIC16F887)
    PORTC = 0x00; // Desactivar pull-ups en PORTC

    // Mostrar mensaje "EMPECEMOS" y "?"
    mostrarMatriz(80, EMPECEMOS);
    mostrarMatriz(8, signo);
    
    // Inicializamos el PIC con START_PIC (PC0) con el valor de 1
    PORTC |= (1 << START_PIC);

    while (1) {
        // Si RD5_PIN es 1, mostrar número y cara feliz
        unsigned char numero = binarioADecimal();

        // Leer el valor de RD5_PIN (PC5) del PIC
        if (PINC & (1 << RD5_PIN)) {
            switch (numero) {
                case 1:
                    mostrarMatriz(8, uno); // Mostrar el 1
                    break;
                
                case 2:
                    mostrarMatriz(8, dos); // Mostrar el 2
                    break;

                case 3:
                    mostrarMatriz(8, tres); // Mostrar el 3
                    break;
                
                case 4:
                    mostrarMatriz(8, cuatro); // Mostrar el 4
                    break;
                    
                case 5:
                    mostrarMatriz(8, cinco); // Mostrar el 5
                    break;

                case 6:
                    mostrarMatriz(8, seis); // Mostrar el 6
                    break;

                case 7:
                    mostrarMatriz(8, siete); // Mostrar el 7
                    break;
                
                case 8:
                    mostrarMatriz(8, ocho); // Mostrar el 8
                    break;

                case 9:
                    mostrarMatriz(8, nueve); // Mostrar el 9
                    break;
                
                default:
                    PORTB = 0xFF;  // Apagar todas las columnas
                    PORTD = 0x00;  // Apagar todas las filas
                    break;
            }
        }
        else if (!(PINC & (1 << RD5_PIN)) && numero != 0) mostrarMatriz(8, PERDER);
        else mostrarMatriz(8, signo); // Si RD5_PIN es 0, mantener el signo "?"
    }
    return 0;
}