/*Juego de adivinanza para el
  PIC16F887 en MikroC for PIC*/

unsigned short numSelec = 0;
unsigned short randomNumber = 1;

// Funciones para cada nota
void sol(int duration) {
    Sound_Play(392.00, duration);
}

void la_sharp(int duration) {
    Sound_Play(466.16, duration);
}

void do_prime(int duration) {
    Sound_Play(523.25, duration);
}

void re_prime(int duration) {
    Sound_Play(587.33, duration);
}

void re_sharp_prime(int duration) {
    Sound_Play(622.25, duration);
}

void fa_prime(int duration) {
    Sound_Play(698.46, duration);
}

void sol_prime(int duration) {
    Sound_Play(783.99, duration);
}

void la_prime(int duration) {
    Sound_Play(880.00, duration);
}

// Función para reproducir la melodía completa
void Melody_Win() {
    // Duraciones de cada nota en milisegundos
    int duration_short = 200;   // 200 ms para notas cortas
    int duration_standard = 400; // 400 ms para notas estándar
    int duration_long = 600;     // 600 ms para notas largas

    // Primera sección
    do_prime(duration_standard);  // C
    la_prime(duration_short);      // A
    sol(duration_short);           // G
    la_prime(duration_standard);    // A
    do_prime(duration_long);       // C
    sol(duration_short);           // G
    la_prime(duration_short);      // A
    do_prime(duration_standard);    // C

    // Segunda sección
    re_prime(duration_short);      // D
    re_prime(duration_standard);    // D
    la_prime(duration_short);       // A
    sol(duration_short);           // G
    fa_prime(duration_long);       // F
    sol(duration_short);           // G

    // Repetición de la sección
    do_prime(duration_standard);    // C
    la_prime(duration_short);      // A
    sol(duration_short);           // G
    la_prime(duration_standard);    // A
    do_prime(duration_long);       // C
    sol(duration_short);           // G
    la_prime(duration_short);      // A
    do_prime(duration_standard);    // C

    // Tercera sección
    re_prime(duration_short);      // D
    re_prime(duration_standard);    // D
    la_prime(duration_short);       // A
    sol(duration_short);           // G
    fa_prime(duration_long);       // F
    sol(duration_short);           // G

    // Finalizar
    do_prime(duration_long);       // C
}

void ToneA() {
  Sound_Play(880, 50);
}

void ToneC() {
  Sound_Play(1046, 50);
}

void ToneE() {
  Sound_Play(1318, 50);
}

void Melody_Lose() {       // Melodía de derrota
  unsigned short i;
  for (i = 9; i > 0; i--) {
    ToneA(); ToneC(); ToneE();
  }
}

void main() {
  // Configuraciones iniciales
  ANSEL  = 0;                  // Configura AN pines como digitales
  ANSELH = 0;
  C1ON_bit = 0;                // Desactiva comparadores
  C2ON_bit = 0;

  TRISB  = 0xFF;               // Configura RB como entrada para botones
  TRISA  = 0xFF;               // Configura RA como entrada para botones
  TRISC  = 0x00;               // RC como salida para melodías
  TRISD  = 0x00;               // RD como salida para comunicación con ATmega328P

  Sound_Init(&PORTC, 3);       // Inicializa sonido en RC3
  srand(1);  // Semilla usando el valor de 1

  while(1){
    // Genera un número aleatorio entre 1 y 9
    randomNumber = 1 + rand() % 9;

    // Detectar botón de selección de número
    if (Button(&PORTA, 0, 1, 1)) numSelec = 1;
    else if (Button(&PORTA, 1, 1, 1)) numSelec = 2;
    else if (Button(&PORTB, 0, 1, 1)) numSelec = 3;
    else if (Button(&PORTB, 1, 1, 1)) numSelec = 4;
    else if (Button(&PORTB, 2, 1, 1)) numSelec = 5;
    else if (Button(&PORTB, 3, 1, 1)) numSelec = 6;
    else if (Button(&PORTB, 4, 1, 1)) numSelec = 7;
    else if (Button(&PORTB, 5, 1, 1)) numSelec = 8;
    else if (Button(&PORTB, 6, 1, 1)) numSelec = 9;
    else numSelec = 0;
    
    // Enviar el número seleccionado por RD2-RD5 sin afectar RD0 y RD1
    PORTD = (PORTD & 0x01) | (numSelec << 1);

    // Mostrar el número aleatorio en binario en RC4-RC7
    PORTC = (PORTC & 0x0F) | (randomNumber << 4);

    if (numSelec == randomNumber && RD5_bit == 0) {
       PORTD |= (1 << 5);
       numSelec = 0;
       Melody_Win();               // Reproducir melodía de victoria
    } if (!(numSelec == randomNumber)) {
       //Melody_Lose();              // Reproducir melodía de derrota
       PORTD &= ~(1 << 5);
    }
    Delay_ms(1000);
  }
}