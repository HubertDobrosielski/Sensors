#ifndef __CHART_H__
#define __CHART_H__

#include "renderer.h"
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



struct Title
{
  std::string text;
  CPoint pos;
  int fontSize;
  color Color;
};

struct Axis
{
  std::vector<CPoint> line;  // zwykle 2 punkty
  std::vector<CPoint> ticks; // kreski
};


struct ChartArea {
  CRect rect;
  color bg;
};

struct Grid {
  std::vector<std::pair<CPoint,CPoint>> lines;
};


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

  Title title;
  Axis xAxis;
  Axis yAxis;
  Grid grid;
  ChartArea area;

  // void layout();

  // TYMCZASOWE
  void calculate();
  bool checkGeom();
  void drawAxisX();
  void drawAxisY();
  void drawChartArea();
  void ChartUpdate();
  void drawGrid();
  void calcAxisX();
  void calcAxisY();
  bool needCalculate;
  bool IsEnoughSpaceForChart();
  bool needDraw; // czy jest potrzeba rysowania?

protected:
  uint numOfSensorPaint; // numer czujnika do rysowania
  CRect chart;           // geometria pola wykresu
  CRect geomCAL;
  list<uint> X;      // lista z współrzędnymi x
  list<uint> Ylabel; // lista z współrzędnymi y etykiet osi y

  CRect setLabelRect(cfloat min, cfloat max);
  uint boardSet(cCSensor *sensor);
  void paintName(string name, color color);
  void labelPaint(cfloat min, cfloat max, color c, cuint space);

public:
  CChart(cuint numXpoint, CRect r, color wc, color fc, color txt);
  // void paint(Renderer &r);
  void paint(Renderer &r);
  void update();

  vector<float> data; // view data (dane widoku)
};

class CSensorChart : public CChart
{
public:
  CSensorChart(cCSensor *sensor, cuint numXpoint, CRect r,
               color wc, color fc, color txt);
  void paint(Renderer &r);

private:
  void updateData(); // pull (pobranie)
  void paintData();  // draw data (rysuj dane)

  cCSensor *sensor;
  vector<float> buffer; // view buffer
  color sensorColor;
};

class CBaseChart : public CChart
{
public:
  CBaseChart(CBase *base, cuint numXpoint, CRect r, color wc, color fc, color txt);
  void paint(Renderer &r);
  bool handleEvent(int key);

private:
  void updateData();
  void paintData();

  CBase *base;
  vector<vector<float>> buffers; // buffer per sensor
  list<color> sensorsColors;
  list<uint> labelShift;
  int hu = 0;
};

#endif