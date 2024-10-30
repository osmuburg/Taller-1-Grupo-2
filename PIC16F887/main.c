// Configuración y juego de adivinanza para el PIC16F887 en MikroC for PIC

unsigned short numSelec = 0;

void Tone1() {
  Sound_Play(659, 250);   // Frecuencia = 659Hz, duración = 250ms
}

void Tone2() {
  Sound_Play(698, 250);   // Frecuencia = 698Hz, duración = 250ms
}

void Tone3() {
  Sound_Play(784, 250);   // Frecuencia = 784Hz, duración = 250ms
}

void Melody_Win() {       // Melodía de victoria: "We Are the Champions"
  Tone1(); Tone2(); Tone3(); Tone3();
  Tone1(); Tone2(); Tone3(); Tone3();
  Tone1(); Tone2(); Tone3();
  Tone1(); Tone2(); Tone3(); Tone3();
  Tone1(); Tone2(); Tone3();
  Tone3(); Tone3(); Tone2(); Tone2(); Tone1();
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
  TRISD  = 0x60;               // RD como salida (menos RD5 y RD6) para comunicación con ATmega328P

  Sound_Init(&PORTC, 3);       // Inicializa sonido en RC3
  
  // Espera hasta que se presione el botón de inicio en RB7
  while (Button(&PORTB, 7, 1, 1) == 0) {
    PORTD = 0x00;              // En espera, envía 0 por RD
  }
  
  // Inicia el juego, envía 1 al ATmega328P para indicar inicio
  PORTD = 0x01;

  while(1){
    // Detectar botón de selección de número
    if (Button(&PORTA, 0, 1, 1))
       numSelec = 1;
    while(RA0_bit);

    if (Button(&PORTA, 1, 1, 1))
         numSelec = 2;
    while(RA1_bit);

    if (Button(&PORTB, 0, 1, 1))
         numSelec = 3;
    while(RB0_bit);

    if (Button(&PORTB, 1, 1, 1))
         numSelec = 4;
    while(RB1_bit);

    if (Button(&PORTB, 2, 1, 1))
       numSelec = 5;
    while(RB2_bit);

    if (Button(&PORTB, 3, 1, 1))
       numSelec = 6;
    while(RB3_bit);

    if (Button(&PORTB, 4, 1, 1))
       numSelec = 7;
    while(RB4_bit);

    if (Button(&PORTB, 5, 1, 1))
       numSelec = 8;
    while(RB5_bit);

    if (Button(&PORTB, 6, 1, 1))
       numSelec = 9;
    while(RB6_bit);

    // Enviar el número seleccionado por RD2-RD5 sin afectar RD0 y RD1
    PORTD = (PORTD & 0x01) | (numSelec << 1);
    
    if (RD5_bit && RD6_bit)
       Melody_Win();               // Reproducir melodía de victoria
    if ((RD5_bit == 0) && RD6_bit)
       Melody_Lose();              // Reproducir melodía de derrota
  }
}