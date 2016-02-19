#pragma once

#include "LedControl.h"

#define HAD_ROZMER (8)

#define DATA_IN 12
#define CLK 11
#define CS 10

extern LedControl led;

#define MAPA_PRAZDNE (0)
#define MAPA_HAD (1)
#define MAPA_JABLKO (2)

#define HAD_NEMAZAT (255)

#define SMER_DOPRAVA (0)
#define SMER_DOLEVA (1)
#define SMER_DOLU (2)
#define SMER_NAHORU (3)

void led_vysvit();
void had_reset();
void had_aktualizuj_mapu();
void had_vyresetuj_mapu();

struct Casovac
{
public:
  Casovac(unsigned long cas_oznameni = 1000);

  void aktualizuj(unsigned long cas_delta);
  bool je_pripraven();
  bool reset_pokud_pripraven();
  void reset();
  
private:
  unsigned long cas_oznameni;
  unsigned long cas_akumulator;  
};

struct Pozice
{
  Pozice(byte x = 0, byte y = 0);

  Pozice operator+(const Pozice& jina);

  bool je_validni();
  
  byte x;
  byte y;
};

struct Had
{
public:
    Had(Pozice zacatek, byte smer = 0);

    void aktualizuj_pozici();

    byte pocet_kusu;
    byte smer;
    Pozice souradnice_kusu[64];
    Pozice souradnice_smazana;
};

extern byte had_mapa[HAD_ROZMER][HAD_ROZMER];
extern Had had;
extern Pozice had_smery[4];
extern Pozice jablka[32];
extern byte jablka_pocet;

