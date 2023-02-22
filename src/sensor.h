#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <string>
#include <iostream>
using namespace std;

class CSensor
{
private:
  float min;
  float max;
  float makeMeasurement() const;

public:
  CSensor(const float min, const float max);
  float getMeasurement() const;
  virtual string getName() const = 0;
  virtual string getUnit() const = 0;

  friend ostream &operator<<(ostream &o, const CSensor &sensor);

  void checkLimits(const float min, const float max,
                   const float minLimit, const float maxLimit,
                   const float minDef, const float maxDef);

  float getMin() const;
  float getMax() const;
};

class CTemperatureSensor : public CSensor
{
public:
  CTemperatureSensor(const float min, const float max);

private:
  string getName() const;
  string getUnit() const;
};

class CHumiditySensor : public CSensor
{
public:
  CHumiditySensor(const float min, const float max);

private:
  string getName() const;
  string getUnit() const;
};

class CPressureSensor : public CSensor
{
public:
  CPressureSensor(const float min, const float max);

private:
  string getName() const;
  string getUnit() const;
};

class CWindSensor : public CSensor
{
public:
  CWindSensor(const float min, const float max);

private:
  string getName() const;
  string getUnit() const;
};

class CInsolationSensor : public CSensor
{
public:
  CInsolationSensor(const float min, const float max);

private:
  string getName() const;
  string getUnit() const;
};

#endif