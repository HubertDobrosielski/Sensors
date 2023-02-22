#include "sensor.h"
#include "base.h"
#include "rand.h"

CSensor::CSensor(const float min, const float max) //konstruktor przyjmujący zakres czujnika
{
  this->min = min;
  this->max = max;
}

float CSensor::makeMeasurement() const { return getRandomNumber(min, max); }
float CSensor::getMeasurement() const { return makeMeasurement(); }
float CSensor::getMin() const { return min; }
float CSensor::getMax() const { return max; }
void CSensor::checkLimits(const float min, const float max, const float minLimit,
                          const float maxLimit, const float minDef, const float maxDef) //funkcja sprawdzająca poprawność podanych zakresów czujników, w razie potrzeby zamienia na poprawne
{
  if (min < minLimit || max > maxLimit) //sprawdzanie czy podane zakresy są prawidłowe
  {
    this->min = minDef; //przypisywanie prawidłowej minimalnej wartości zakresu
    this->max = maxDef; //przypisywanie prawidłowej maksymalnej wartości zakresu
  }
}

//

CTemperatureSensor::CTemperatureSensor(const float min, const float max) : CSensor(min, max)
{
  //checkLimits(min, max, -20, 50, -10, 40);
}
CHumiditySensor::CHumiditySensor(const float min, const float max) : CSensor(min, max)
{
 // checkLimits(min, max, 0, 100, 0, 100);
}
CPressureSensor::CPressureSensor(const float min, const float max) : CSensor(min, max)
{
 // checkLimits(min, max, 800, 1200, 90, 1150);
}
CWindSensor::CWindSensor(const float min, const float max) : CSensor(min, max)
{
  //checkLimits(min, max, 0, 50, 0, 30);
}
CInsolationSensor::CInsolationSensor(const float min, const float max) : CSensor(min, max)
{
 // checkLimits(min, max, 800, 1100, 900, 1050);
}

string CTemperatureSensor::getName() const { return "temperature"; }
string CHumiditySensor::getName() const { return "humidity"; }
string CPressureSensor::getName() const { return "atmospheric pressure"; }
string CWindSensor::getName() const { return "wind speed"; }
string CInsolationSensor::getName() const { return "insolation"; }

string CTemperatureSensor::getUnit() const { return "°C"; }
string CHumiditySensor::getUnit() const { return "%"; }
string CPressureSensor::getUnit() const { return "hPa"; }
string CWindSensor::getUnit() const { return "m/s"; }
string CInsolationSensor::getUnit() const { return "W/m^2"; }

ostream &operator<<(ostream &o, const CSensor &sensor)
{
  o << "Current " << sensor.getName() << ": " << sensor.getMeasurement() << " " << sensor.getUnit() << endl;
  return o;
}
