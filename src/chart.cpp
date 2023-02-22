#include "chart.h"

CChart::CChart(cuint numXpoint, CRect r, color wc, color fc, color txt) : CFramedWindow(r, wc, fc), numXpoint(numXpoint)
{
  rightBoard = leftBoard = topBoard = bottomBoard = DEF_BOARD_SIZE;
  this->topBoard += TEXT_HIGHT; // początkowe dodanie górnej obramówki
  this->txt = txt;
  geomCAL = geom; // zapisanie geometri okna w celu późniejszego sprawdzania
  needCalculate = true;
  numOfSensorPaint = 0;
}
CRect CChart::setLabelRect(cfloat min, cfloat max)
{
  uint minLen = to_string((int)min).length();                    // długość dolnej granicy zakresu
  uint maxLen = to_string((int)max).length();                    // długość górnej granicy zakresu
  uint LEN = (maxLen > minLen) ? TEXT_FACTOR * maxLen : TEXT_FACTOR * minLen; // powiększenie przestrzeni na etykiety osi y
  return CRect(CPoint(chart.topleft.x, chart.topleft.y), CPoint(chart.topleft.x + LEN, chart.topleft.y + chart.size.y))
}
CRect CChart::boardSet(cCSensor *sensor)
{
  rightBoard +=
  topBoard += (TEXT_HIGHT + TEXT_SPACE); // miejsce na nazwy czujników
}
void CChart::paintName(string name, color c)
{
  gfx_textout(chart.topleft.x, geom.topleft.y + (TEXT_SPACE / 2) + ((TEXT_HIGHT + TEXT_SPACE) * numOfSensorPaint), name.c_str(), c);
}
void CChart::checkGeom()
{
  if (geomCAL != geom)
  {
    needCalculate = true;
    geomCAL = geom;
  }
}
void CChart::calculate()
{
  checkGeom();
  if (needCalculate)
  {
    chart = CRect(CPoint(geom.topleft.x + leftBoard, geom.topleft.y + topBoard), CPoint(geom.size.x - leftBoard - rightBoard, geom.size.y - topBoard - bottomBoard));

    int space = TEXT_HIGHT + LABEL_Y_SPACE;       // minimalne odstępy pomiędzy podziałkami osi y
    if (chart.size.x > 0 || chart.size.y > space) // zabezpieczenie przed obliczaniem przy zbyt małych wymiarach okna
    {
      // obliczenia osi x
      float shiftX = (float)chart.size.x / (float)(numXpoint - 1); // przesunięcie pomiędzy kolejnymi punktami osi x
      X.clear();                                                   // czyszczenie listy przed wpisaniem nowych danych
      for (uint i = 0; i < numXpoint; i++)
        X.push_back(chart.topleft.x + (i * shiftX)); // wpisywanie do listy kolejnych punktów osi x

      // obliczenia osi y
      uint numYlabel = chart.size.y / space + 1;           // obliczenie ilości wyświetlanych podziałek/etykiet
      float shift = (float)chart.size.y / (numYlabel - 1); // obliczenie przesunięcia pomiędzy kolejnymi podziałkami osi y
      Ylabel.clear();                                      // czyszczenie listy przed wpisaniem nowych danych
      for (uint i = 0; i < numYlabel; i++)
        Ylabel.push_back(chart.topleft.y + shift * i); // wpisywanie do listy kolejnych punktów osi y

      needDraw = true;
    }
    else
      needDraw = false;
    needCalculate = false; // już obliczono
  }
}
void CChart::paint()
{
  CFramedWindow::paint();

  calculate();
  if (needDraw)
  {
    // wyświetlanie osi x
    gfx_line(chart.topleft.x, chart.getBottomRight().y, chart.topleft.x + chart.size.x, chart.getBottomRight().y, BLACK);
    for (list<uint>::iterator it = X.begin(); it != X.end(); it++)
      gfx_line(*it, chart.getBottomRight().y + (LEN_LINE_X / 2), *it, chart.getBottomRight().y - (LEN_LINE_X / 2), BLACK);

    // wyświetlanie osi y
    gfx_line(chart.topleft.x + chart.size.x, chart.topleft.y, chart.topleft.x + chart.size.x, chart.getBottomRight().y, BLACK);
    for (list<uint>::iterator it = Ylabel.begin(); it != Ylabel.end(); it++)
      gfx_line(chart.topleft.x + chart.size.x + (LEN_LINE_X / 2), *it, chart.topleft.x + chart.size.x - (LEN_LINE_X / 2), *it, BLACK);
  }
  //gfx_filledRect(chart.topleft.x, chart.topleft.y, chart.topleft.x + chart.size.x, chart.getBottomRight().y, BLACK);
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

CSensorChart::CSensorChart(cCSensor *sensor, cuint numXpoint, CRect r, color wc, color fc, color txt)
    : CChart(numXpoint, r, wc, fc, txt),
      sensor(sensor)
{
  boardSet(sensor);
  this->sensorColor = BLUE;
}
void CSensorChart::paint()
{
  CChart::paint();
  paintName(sensor->getName(), sensorColor);
  labelPaint(sensor->getMin(), sensor->getMax(), sensorColor);
}

CBaseChart::CBaseChart(CBase *base, cuint numXpoint, CRect r, color wc, color fc, color txt) : CChart(numXpoint, r, wc, fc, txt), base(base)
{
  cuint numColor = 8;
  color colors[numColor] = {BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE}; // lista dostępnych kolorów
  for (uint i = 0; i < numColor; i++)
    if (colors[i] != wc)                  // kolor taki sam jak kolor okna nie będzie dodawany
      sensorsColors.push_back(colors[i]); // dodawanie kolorów
  for (auto it = base->Sensors.begin(); it != base->Sensors.end(); it++)
    labelShift.push_back(boardSet(*it)); // ustawienie przestrzeni na etykiety i zapisanie ich
  labelShift.push_front(0);              // zapchanie pierwszego elementu, aby przesunąć wszystkie wartości w prawo
}
void CBaseChart::paint()
{
  CChart::paint();
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

// gfx_textout(geom.topleft.x + 4, geom.topleft.y +4+(12*i), "asd", txt);
