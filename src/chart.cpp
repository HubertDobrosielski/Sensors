#include "chart.h"
#include <iostream>
using namespace std;

CChart::CChart(cuint numXpoint, CRect r, color wc, color fc, color txt) : CFramedWindow(r, wc, fc), numXpoint(numXpoint)
{
  rightBoard = leftBoard = topBoard = bottomBoard = DEF_BOARD_SIZE;
  this->topBoard += TEXT_HIGHT; // początkowe dodanie górnej obramówki
  this->txt = txt;
  geomCAL = geom; // zapisanie geometri okna w celu późniejszego sprawdzania
  needCalculate = true;
  numOfSensorPaint = 0;
  this->ChartUpdate();
  this->paint();
}
CRect CChart::setLabelRect(cfloat min, cfloat max)
{
  uint minLen = to_string((int)min).length();                                 // długość dolnej granicy zakresu
  uint maxLen = to_string((int)max).length();                                 // długość górnej granicy zakresu
  uint LEN = (maxLen > minLen) ? TEXT_FACTOR * maxLen : TEXT_FACTOR * minLen; // powiększenie przestrzeni na etykiety osi y
  return CRect(CPoint(chart.topleft.x, chart.topleft.y), CPoint(chart.topleft.x + LEN, chart.topleft.y + chart.size.y));
}

uint CChart::boardSet(cCSensor *sensor)
{
  const uint shiftY = TEXT_HIGHT + TEXT_SPACE; // shift (przesunięcie)
  this->topBoard += shiftY;                    // side-effect (efekt uboczny)
  this->rightBoard += TEXT_FACTOR;
  return shiftY; // albo return (shiftY + shiftX) — decyzja design (projekt)
}

bool CChart::checkGeom()
{
  if (geomCAL == geom)
    return false;
  geomCAL = geom;
  return true;
}

void dispList(list<uint> l)
{
  for (list<uint>::iterator it = l.begin(); it != l.end(); it++)
    cout << *it << " ";
  cout << endl;
}

void CChart::ChartUpdate()
{
  this->chart = CRect(CPoint(geom.topleft.x + leftBoard, geom.topleft.y + topBoard), CPoint(geom.size.x - leftBoard - rightBoard, geom.size.y - topBoard - bottomBoard));
}

void CChart::paintName(string name, color c)
{
  gfx_textout(geom.topleft.x, geom.topleft.y + (TEXT_SPACE / 2) + ((TEXT_HIGHT + TEXT_SPACE) * numOfSensorPaint), name.c_str(), c);
}

void CChart::drawAxisX() // wyświetlanie osi x
{
  gfx_line(this->chart.topleft.x, this->chart.getBottomRight().y, this->chart.topleft.x + this->chart.size.x, this->chart.getBottomRight().y, BLACK);
  for (list<uint>::iterator it = this->X.begin(); it != this->X.end(); it++)
    gfx_line(*it, this->chart.getBottomRight().y + (LEN_LINE_X / 2), *it, this->chart.getBottomRight().y - (LEN_LINE_X / 2), BLACK);
}

void CChart::drawAxisY() // wyświetlanie osi y
{
  int axisYx = this->chart.topleft.x + this->chart.size.x; // współrzędna x osi y
  gfx_line(axisYx, this->chart.topleft.y, axisYx, this->chart.getBottomRight().y, BLACK);
  for (list<uint>::iterator it = this->Ylabel.begin(); it != this->Ylabel.end(); it++)
    gfx_line(axisYx + (LEN_LINE_X / 2), *it, axisYx - (LEN_LINE_X / 2), *it, BLACK);
}

void CChart::drawChartArea()
{
  gfx_filledRect(this->chart.topleft.x, this->chart.topleft.y, this->chart.getBottomRight().x, this->chart.getBottomRight().y, WHITE);
}

void CChart::drawGrid()
{
  // vertical grid (X)
  for (auto x : X)
    gfx_line(x, this->chart.topleft.y, x, this->chart.getBottomRight().y, BLACK);
  // horizontal grid (Y)
  for (auto y : Ylabel)
    gfx_line(this->chart.topleft.x, y, this->chart.getBottomRight().x, y, BLACK);
}

bool CChart::IsEnoughSpaceForChart()
{
  int space = TEXT_HIGHT + LABEL_Y_SPACE;
  return (this->chart.size.x > 10 || this->chart.size.y > space);
}

void CChart::calcAxisX()
{
  // obliczenia osi x
  float shiftX = (float)this->chart.size.x / (float)(numXpoint - 1); // przesunięcie pomiędzy kolejnymi punktami osi x
  X.clear();                                                         // czyszczenie listy przed wpisaniem nowych danych
  for (uint i = 0; i < numXpoint; i++)
  {
    X.push_back(this->chart.topleft.x + (i * shiftX)); // wpisywanie do listy kolejnych punktów osi x
  }
}
void CChart::calcAxisY()
{
  int space = TEXT_HIGHT + LABEL_Y_SPACE;
  // obliczenia osi y
  uint numYlabel = this->chart.size.y / space + 1;           // obliczenie ilości wyświetlanych podziałek/etykiet
  float shift = (float)this->chart.size.y / (numYlabel - 1); // obliczenie przesunięcia pomiędzy kolejnymi podziałkami osi y
  Ylabel.clear();                                            // czyszczenie listy przed wpisaniem nowych danych
  for (uint i = 0; i < numYlabel; i++)
  {
    Ylabel.push_back(this->chart.topleft.y + shift * i); // wpisywanie do listy kolejnych punktów osi y
  }
}

void CChart::paint()
{
  if (this->IsEnoughSpaceForChart())
  {
    if (this->checkGeom())
    {
      this->ChartUpdate();
      this->calcAxisX();
      this->calcAxisY();
    }

    CFramedWindow::paint();
    drawAxisX();
    drawAxisY();
    drawChartArea();
    drawGrid();
  }
}

void CChart::labelPaint(cfloat min, cfloat max, color c, cuint space = 0)
{
  float valueShift = (float)(max - min) / (float)(Ylabel.size() - 1); // obliczenie przesunięcia wartości osi y
  uint numRow = 0;                                                    // indeks kolumny
  for (list<uint>::iterator it = Ylabel.begin(); it != Ylabel.end(); it++)
  {
    int labelValue = max - (numRow++ * valueShift); // obliczenie wartości wyświetlanych na osi y
    gfx_textout(chart.getBottomRight().x + LABEL_X_SHIFT + space, *it - (TEXT_HIGHT / 2), to_string(labelValue).c_str(), c);
  }
}
// ------------------------------------------------------------------------------
// CSensorChart methods
// ------------------------------------------------------------------------------
CSensorChart::CSensorChart(cCSensor *sensor, cuint numXpoint, CRect r, color wc, color fc, color txt)
    : CChart(numXpoint, r, wc, fc, txt),
      sensor(sensor),
      sensorColor(BLUE) // domyślny kolor czujnika
{
  boardSet(sensor);
}
void CSensorChart::paint()
{
  CChart::paint();
  // CChart::update();
  paintName(sensor->getName(), sensorColor);
  labelPaint(sensor->getMin(), sensor->getMax(), sensorColor);

  updateData();
  paintData();
}

void CSensorChart::updateData()
{
  buffer.push_back(sensor->getMeasurement());
  if (buffer.size() > X.size())
    buffer.erase(buffer.begin()); // sliding window (okno)
}

void CSensorChart::paintData()
{
  if (buffer.size() < 2)
    return;

  float scale = chart.size.y / (sensor->getMax() - sensor->getMin());
  auto itX = X.begin();

  for (size_t i = 0; i < buffer.size() - 1; ++i)
  {
    uint x1 = *itX++;
    uint x2 = *itX;

    uint y1 = chart.getBottomRight().y -
              (buffer[i] - sensor->getMin()) * scale;
    uint y2 = chart.getBottomRight().y -
              (buffer[i + 1] - sensor->getMin()) * scale;

    gfx_line(x1, y1, x2, y2, sensorColor);
  }
}

// ------------------------------------------------------------------------------
// CBaseChart methods
// ------------------------------------------------------------------------------
CBaseChart::CBaseChart(CBase *base, cuint numXpoint, CRect r, color wc, color fc, color txt) : CChart(numXpoint, r, wc, fc, txt), base(base)
{
  cuint numColor = 8;
  color colors[numColor] = {BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE}; // lista dostępnych kolorów
  for (uint i = 0; i < numColor; i++)
    if (colors[i] != wc)                  // kolor taki sam jak kolor okna nie będzie dodawany
      sensorsColors.push_back(colors[i]); // dodawanie kolorów
  for (auto it = base->Sensors.begin(); it != base->Sensors.end(); it++)
  {
    labelShift.push_back(boardSet(*it)); // ustawienie przestrzeni na etykiety i zapisanie ich
  }
  labelShift.push_front(0); // zapchanie pierwszego elementu, aby przesunąć wszystkie wartości w prawo
}
void CBaseChart::paint()
{
  CChart::paint();
  // CChart::update();
  auto itL = labelShift.begin();
  auto itC1 = sensorsColors.begin();
  auto itC2 = sensorsColors.begin();
  for (vector<const CSensor *>::iterator it = base->Sensors.begin(); it != base->Sensors.end(); it++)
  {
    paintName((*it)->getName(), *itC1++);
    labelPaint((*it)->getMin(), (*it)->getMax(), *itC2++, *itL++);
    numOfSensorPaint++;
  }
  numOfSensorPaint = 0;
  updateData();
  paintData();
}

bool CBaseChart::handleEvent(int key)
{
  if (CFramedWindow::handleEvent(key))
    return true;

  switch (key)
  {
  case SDLK_n:
    *base += *(new CTemperatureSensor(1, 100));
    return true;
  case SDLK_u:
    if (!base->Sensors.empty())
      base->Sensors.erase(base->Sensors.begin());
    return true;
  };
  return false;
}

void CBaseChart::updateData()
{
  if (buffers.size() != base->Sensors.size())
    buffers.resize(base->Sensors.size());

  for (size_t i = 0; i < base->Sensors.size(); ++i)
  {
    buffers[i].push_back(base->Sensors[i]->getMeasurement());
    if (buffers[i].size() > X.size())
      buffers[i].erase(buffers[i].begin());
  }
}

void CBaseChart::paintData()
{
  auto itC = sensorsColors.begin();

  for (size_t s = 0; s < buffers.size(); ++s)
  {
    auto& buf = buffers[s];
    auto* sen = base->Sensors[s];
    if (buf.size() < 2) continue;

    float scale = chart.size.y / (sen->getMax() - sen->getMin());
    auto itX = X.begin();

    for (size_t i = 0; i < buf.size() - 1; ++i)
    {
      uint x1 = *itX++;
      uint x2 = *itX;

      uint y1 = chart.getBottomRight().y -
                (buf[i]   - sen->getMin()) * scale;
      uint y2 = chart.getBottomRight().y -
                (buf[i+1] - sen->getMin()) * scale;

      gfx_line(x1, y1, x2, y2, *itC);
    }
    ++itC;
  }
}