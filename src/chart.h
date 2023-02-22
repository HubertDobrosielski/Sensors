#ifndef __CHART_H__
#define __CHART_H__

#include "winsys.h"
#include "sensor.h"
#include "base.h"
#include <list>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#define TEXT_HIGHT 8     // wysokość tekstu
#define TEXT_SPACE 5     // przerwa pomiędzy tekstami
#define TEXT_FACTOR 12   // "współczynnik" szerokości tekstu
#define LEN_LINE_X 6     // długość linni podziałki osi x
#define LABEL_X_SHIFT 10 // przerwa pomiędzy etykietami osi y (według osi x)
#define LABEL_Y_SPACE 20 // przerwa pomiędzy etykietami osi y (według osi y)
#define DEF_BOARD_SIZE 5 // podstawowa wielkość obramowania

typedef const uint cuint;
typedef const float cfloat;
typedef const CBase cCBase;
typedef const CSensor cCSensor;

class CChart : public CFramedWindow
{
private:
  uint numYlabel;   // ilość wyświetlanych etykiet osi y
  uint rightBoard;  // prawa część obramowania
  uint topBoard;    // górna część obramowania
  uint leftBoard;   // lewa część obramowania
  uint bottomBoard; // dolna część obramowania
  color txt;        // kolor napisów, osi itp
  cuint numXpoint;  // ilość punktów na osi x

  // TYMCZASOWE
  void calculate();
  void checkGeom();
  bool needCalculate;
  bool needDraw; // czy jest potrzeba rysowania?

protected:
  uint numOfSensorPaint; // numer czujnika do rysowania
  CRect chart;           // geometria pola wykresu
  CRect geomCAL;
  list<uint> X;      // lista z współrzędnymi x
  list<uint> Ylabel; // lista z współrzędnymi y etykiet osi y

  CRect setLabelRect(cfloat min, cfloat max);
  void boardSet(cCSensor *sensor);
  void paintName(string name, color color);
  void labelPaint(cfloat min, cfloat max, color c, cuint space);

public:
  CChart(cuint numXpoint, CRect r, color wc, color fc, color txt);
  void paint();
};

class CSensorChart : public CChart
{
public:
  CSensorChart(cCSensor *sensor, cuint numXpoint, CRect r, color wc, color fc, color txt);
  void paint();

private:
  cCSensor *sensor;
  color sensorColor;
};

class CBaseChart : public CChart
{
public:
  CBaseChart(CBase *base, cuint numXpoint, CRect r, color wc, color fc, color txt);
  void paint();
  bool handleEvent(int key);

private:
  CBase *base;
  list<color> sensorsColors; // lista kolorów dostępnych dla czujników bazy
  list<uint> labelShift;
  int hu = 0;
};

#endif