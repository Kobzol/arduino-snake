#include "had.h"

LedControl led = LedControl(DATA_IN, CLK, CS, 1);

byte had_mapa[HAD_ROZMER][HAD_ROZMER] = { 0 };
Had had((Pozice(1, 1)));
Pozice had_smery[4] = {
  Pozice(0, 1),
  Pozice(0, -1),
  Pozice(1, 0),
  Pozice(-1, 0)
};

Pozice::Pozice(byte x, byte y) : x(x), y(y)
{

}

Pozice Pozice::operator+(const Pozice& jina)
{
  return Pozice(this->x + jina.x, this->y + jina.y);  
}

bool Pozice::je_validni()
{
  return  this->x < HAD_ROZMER &&
          this->y < HAD_ROZMER;
}

Had::Had(Pozice zacatek, byte smer): smer(smer)
{
  this->pocet_kusu = 1;
  this->souradnice_kusu[0] = zacatek;
}

void Had::aktualizuj_pozici()
{
  this->souradnice_smazana = this->souradnice_kusu[this->pocet_kusu - 1];

  Pozice pristi = this->souradnice_kusu[0] + had_smery[this->smer];

  if (!pristi.je_validni() || had_mapa[pristi.x][pristi.y] == MAPA_HAD)
  {
    had_reset();
    return;
  }

  for (byte x = this->pocet_kusu - 1; x > 0; x--)
  {
    this->souradnice_kusu[x] = this->souradnice_kusu[x - 1];
  }
  this->souradnice_kusu[0] = pristi;
  
  if (had_mapa[pristi.x][pristi.y] == MAPA_JABLKO)
  {
    this->pocet_kusu++;
    this->souradnice_kusu[this->pocet_kusu - 1] = this->souradnice_smazana;
    this->souradnice_smazana = Pozice(HAD_NEMAZAT, HAD_NEMAZAT);
  }
}

Casovac::Casovac(unsigned long cas_oznameni) : cas_oznameni(cas_oznameni), cas_akumulator(0)
{
  
}

void Casovac::aktualizuj(unsigned long cas_delta)
{
  this->cas_akumulator += cas_delta;  
}
bool Casovac::je_pripraven()
{
  return this->cas_akumulator >= this->cas_oznameni;  
}
bool Casovac::reset_pokud_pripraven()
{
  if (this->je_pripraven())
  {
    this->reset();
    return true;  
  }
  else return false;
}
void Casovac::reset()
{
  this->cas_akumulator = 0;  
}

void led_vysvit()
{
  for (byte i = 0; i < HAD_ROZMER; i++)
  {
    byte value = 0;
    for (byte j = 0; j < HAD_ROZMER; j++)
    {
      if (had_mapa[i][j] != 0)
      {
        value |= (1 << (HAD_ROZMER - j - 1));
      }
    }

    led.setRow(0, i, value);
  }  
}

void had_reset()
{
  had_vyresetuj_mapu();
  
  for (byte x = 0; x < HAD_ROZMER; x++)
  {
    for (byte y = 0; y < HAD_ROZMER; y++)
    {
      if (x == y)
      {
        had_mapa[x][y] = 1;
        had_mapa[x][HAD_ROZMER - y - 1] = 1;
      }
    }  
  }

  led_vysvit();
  delay(2000);

  had_vyresetuj_mapu();
  led.clearDisplay(0);
  
  had = Had(Pozice(1, 1));
}

void had_vyresetuj_mapu()
{
  memset(had_mapa, 0, sizeof(had_mapa));
}

void had_aktualizuj_mapu()
{
  for (byte i = 0; i < had.pocet_kusu; i++)
  {
    Pozice pozice = had.souradnice_kusu[i];
    had_mapa[pozice.x][pozice.y] = MAPA_HAD;
  }

  if (had.souradnice_smazana.x != HAD_NEMAZAT && had_mapa[had.souradnice_smazana.x][had.souradnice_smazana.y] == MAPA_HAD)
  {
    had_mapa[had.souradnice_smazana.x][had.souradnice_smazana.y] = MAPA_PRAZDNE;
  }
}
