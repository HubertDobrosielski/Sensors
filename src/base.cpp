#include "sensor.h"
#include "base.h"
#include "exception.h"

CBase::CBase(const uint size_of_array) //konstruktor przyjmujący rozmiar tablicy
{
  Sensors.reserve(size_of_array); //Rezerwowanie miejsca w tablicy/wektorze
}
CBase::CBase(const CBase &base) //konstruktor kopiujący
{
  this->Sensors.reserve(base.Sensors.capacity()); //Rezerwownie miejsca w nowym wektorze
  for (uint i = 0; i < base.Sensors.size(); i++)
    this->Sensors.push_back(base.Sensors[i]); //kopiowanie poszczególnych elementów talic/wektorów
}

void CBase::setSensor(const CSensor *pSensor) //Metoda wpisująca czujnik do tablicy czujników
{
  Sensors.push_back(pSensor); //dodawanie czujników do tablicy/wektora
}

CBase &CBase::operator=(const CBase &base) //operator przypisania
{
  if (this != &base)        //sprawdzanie czy nie przypisujemy obiektu do samego siebie
    Sensors = base.Sensors; //przypisanie wektorów przechowywującuch adresy czujników
  return *this;             //zwracanie przerobionego obiektu
}
CBase &CBase::operator+=(const CSensor &sensor) //operator dodawania czyjnika do bazy
{
  setSensor(&sensor); //wywoływanie metody dodającej czujnik
  return *this;       //zwracanie przerobionej bazy
}
CBase &CBase::operator-=(const CSensor &sensor) //operator usuwania z bazy pierwszego wystąpienia czujnika
{
  vector<const CSensor *>::iterator it;                 //tworzenie iteratora
  for (it = Sensors.begin(); it != Sensors.end(); it++) //pętla z użyciem iteratora
  {
    if (*it == &sensor) //szukanie elementu równego argumentowi podanemu do funkcji
    {
      Sensors.erase(it); //usuwanie elementu wskazanego przez iterator
      return *this;      //zwracanie przerobionego obiektu
    }
  }
  throw CException("sensor not found"); //zrzucanie wyjątku
}
ostream &operator<<(ostream &o, CBase &base) //operator strumienia dla klasy CBase
{
  o << endl;                      //przerwa przed wyświetleniem bazy
  o << "Base:" << endl; //wyświetlenie nagłówka bazy
  if (!base.Sensors.empty())      //sprawdzanie czy wektor z czujnikami nie jest pusty
  {
    for (uint i = 0; i < base.Sensors.size(); i++)
    {
      const CSensor *pSensor = base.Sensors.at(i); //Wybieranie kolejnych czujników z tablicy
      o << "  " << *pSensor;                       //korzystanie operatora strumienia klasy CSensor
    }
  }
  else
    o << "  no sensors" << endl; //komunikat w przypadku pustego wektora z czujnikami
  o << endl;                                                               //przerwa po wyświetleniu bazy
  return o;                                                                //zwrócenie strumienia
}
