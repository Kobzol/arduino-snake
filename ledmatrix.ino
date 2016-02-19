#include "had.h"

unsigned long cas_predtim = millis();
Casovac casovac_akt = Casovac(1000);
Casovac casovac_jablko = Casovac(3000);

void setup() {
  randomSeed(analogRead(A0));
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  led.shutdown(0, false);
  led.setIntensity(0, 2);
  led.clearDisplay(0);

  Serial.begin(9600);

  interrupts();
}

void loop() {
  unsigned long cas_ted = millis();
  unsigned long delta = cas_ted - cas_predtim;
  cas_predtim = cas_ted;
  casovac_akt.aktualizuj(delta);
  casovac_jablko.aktualizuj(delta);

  if (casovac_jablko.reset_pokud_pripraven())
  {
    while (true)
    {
      byte x = random(HAD_ROZMER);
      byte y = random(HAD_ROZMER);

      if (had_mapa[x][y] != MAPA_PRAZDNE)
      {
        continue;
      }
      
      had_mapa[x][y] = MAPA_JABLKO;
      break;
    }  
  }

  if (casovac_akt.reset_pokud_pripraven())
  {
    had.aktualizuj_pozici();
    had_aktualizuj_mapu();
    led_vysvit();
  }
  
  if (Serial.available() > 0)
  {
    char smer = Serial.read();
    switch (smer)
    {
      case 'w':
        had.smer = SMER_NAHORU;
        break;
      case 's':
        had.smer = SMER_DOLU;
        break;
      case 'a':
        had.smer = SMER_DOLEVA;
        break;
      case 'd':
        had.smer = SMER_DOPRAVA;
        break;  
    }
  }
  delay(100);
}

