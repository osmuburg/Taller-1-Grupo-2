// Configuración y juego de adivinanza para el PIC16F887 en MikroC for PIC

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
  TRISC  = 0x00;               // RC como salida para comunicación y melodías
  TRISD  = 0x00;               // RD como salida para comunicación con ATmega328P

  Sound_Init(&PORTC, 3);       // Inicializa sonido en RC3
  int numero_seleccionado = 0;
  int numero_aleatorio = 0;

  // Espera hasta que se presione el botón de inicio en RB7
  while (Button(&PORTB, 7, 1, 1) == 0) {
    PORTD = 0x00;              // En espera, envía 0 por RD
  }
  
  // Inicia el juego, envía 1 al ATmega328P
  PORTD = 0x01;
  while (Button(&PORTB, 7, 1, 1) == 1); // Espera a que se suelte el botón

  // Detectar botón de selección de número
  if (Button(&PORTA, 0, 1, 1)) numero_seleccionado = 1;
  else if (Button(&PORTA, 1, 1, 1)) numero_seleccionado = 2;
  else if (Button(&PORTB, 0, 1, 1)) numero_seleccionado = 3;
  else if (Button(&PORTB, 1, 1, 1)) numero_seleccionado = 4;
  else if (Button(&PORTB, 2, 1, 1)) numero_seleccionado = 5;
  else if (Button(&PORTB, 3, 1, 1)) numero_seleccionado = 6;
  else if (Button(&PORTB, 4, 1, 1)) numero_seleccionado = 7;
  else if (Button(&PORTB, 5, 1, 1)) numero_seleccionado = 8;
  else if (Button(&PORTB, 6, 1, 1)) numero_seleccionado = 9;
  
  // Espera a que se suelte el botón de selección
  while (PORTA.F0 == 0 || PORTA.F1 == 0 || PORTB.F0 == 0 || PORTB.F1 == 0 ||
         PORTB.F2 == 0 || PORTB.F3 == 0 || PORTB.F4 == 0 || PORTB.F5 == 0 || PORTB.F6 == 0);

  // Genera número aleatorio entre 1 y 9
  numero_aleatorio = (rand() % 9) + 1;

  // Comparación de números y reacción
  if (numero_seleccionado == numero_aleatorio) {
    // Jugador ganó
    PORTC.F0 = 1;               // Enviar valor 2 (RC0=1 y RC1=0)
    PORTC.F1 = 0;
    Melody_Win();               // Reproducir melodía de victoria
  } else {
    // Jugador perdió
    PORTC.F0 = 1;               // Enviar valor 3 (RC0=1 y RC1=1)
    PORTC.F1 = 1;
    Melody_Lose();              // Reproducir melodía de derrota
  }

  // Enviar el número seleccionado por RC2-RC5
  PORTC = (PORTC & 0x03) | (numero_seleccionado << 2);
}

