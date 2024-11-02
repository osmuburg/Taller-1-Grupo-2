/* Juego de adivinanza para el PIC16F887 en MikroC for PIC */

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
    int duration_short = 200;
    int duration_standard = 400;
    int duration_long = 600;

    do_prime(duration_standard);  // C
    la_prime(duration_short);      // A
    sol(duration_short);           // G
    la_prime(duration_standard);    // A
    do_prime(duration_long);       // C
    sol(duration_short);           // G
    la_prime(duration_short);      // A
    do_prime(duration_standard);    // C
    re_prime(duration_short);      // D
    re_prime(duration_standard);    // D
    la_prime(duration_short);       // A
    sol(duration_short);           // G
    fa_prime(duration_long);       // F
    sol(duration_short);           // G
    do_prime(duration_standard);    // C
    la_prime(duration_short);      // A
    sol(duration_short);           // G
    la_prime(duration_standard);    // A
    do_prime(duration_long);       // C
    sol(duration_short);           // G
    la_prime(duration_short);      // A
    do_prime(duration_standard);    // C
    re_prime(duration_short);      // D
    re_prime(duration_standard);    // D
    la_prime(duration_short);       // A
    sol(duration_short);           // G
    fa_prime(duration_long);       // F
    sol(duration_short);           // G
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

void Melody_Lose() {
  unsigned short i;
  for (i = 9; i > 0; i--) {
    ToneA();
    ToneC();
    ToneE();
  }
}

void main() {
  // Configuraciones iniciales
  ANSEL  = 0;
  ANSELH = 0;
  C1ON_bit = 0;
  C2ON_bit = 0;

  TRISB  = 0xFF;
  TRISA  = 0xFF;
  TRISC  = 0x00;
  TRISD  = 0x00;

  Sound_Init(&PORTC, 3);
  srand(1);
  
  // Enviar 0 por RD1-RD4
  PORTD = (PORTD & 0x01) | (numSelec << 1);

  while (1) {
    // Detectar botón de selección de número
    if (Button(&PORTA, 0, 1, 1)) numSelec = 1;
    while(RA0_bit);
    if (Button(&PORTA, 1, 1, 1)) numSelec = 2;
    while(RA1_bit);
    if (Button(&PORTB, 0, 1, 1)) numSelec = 3;
    while(RB0_bit);
    if (Button(&PORTB, 1, 1, 1)) numSelec = 4;
    while(RB1_bit);
    if (Button(&PORTB, 2, 1, 1)) numSelec = 5;
    while(RB2_bit);
    if (Button(&PORTB, 3, 1, 1)) numSelec = 6;
    while(RB3_bit);
    if (Button(&PORTB, 4, 1, 1)) numSelec = 7;
    while(RB4_bit);
    if (Button(&PORTB, 5, 1, 1)) numSelec = 8;
    while(RB5_bit);
    if (Button(&PORTB, 6, 1, 1)) numSelec = 9;
    while(RB6_bit);
    
    // Enviar el número seleccionado por RD1-RD4
    PORTD = (PORTD & 0x01) | (numSelec << 1);
    
    // Generar el número aleatorio después de soltar el botón
    randomNumber = 1 + rand() % 9;

    // Mostrar el número aleatorio en binario en RC4-RC7
    PORTC = (PORTC & 0x0F) | (randomNumber << 4);

    // Comparación y reacción
    if (numSelec == randomNumber) {
       PORTD |= (1 << 5);
       Melody_Win(); // Reproducir melodía de victoria
    } 
    if (!(numSelec == randomNumber) && numSelec != 0) {
       PORTD &= ~(1 << 5);
       Melody_Lose(); // Reproducir melodía de derrota
    }
    numSelec = 0;
    Delay_ms(1000); // Esperar 1 segundo antes de la próxima ronda
  }
}