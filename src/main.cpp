#include <stdlib.h>
#include <unistd.h>

#include "winsys.h"
#include "clock.h"

#include "chart.h"
#include "sensor.h"

int main(int argc, char *argv[])
{
  if (gfx_init())
    exit(3);

  CWindow *pW1 = new CWindow(CRect(CPoint(100, 100), CPoint(200, 300)));
  CFramedWindow *pW2 = new CFramedWindow(CRect(CPoint(150, 150), CPoint(250, 250)));

  CFramedWindow *pW3 = new CFramedWindow(CRect(CPoint(140, 200), CPoint(300, 300)), YELLOW, BLACK);
  CInputLine *pW4 = new CInputLine(CRect(CPoint(200, 170), CPoint(350, 350)), WHITE, RED);

  CGroup *pG = new CGroup;
  pG->insert(pW3);
  pG->insert(pW4);

  CClock *pClock = new CClock(CRect(CPoint(350, 50), CPoint(300, 300)), GREEN, BLACK);

  CTemperatureSensor temperatureSensor(1, 100);
  CHumiditySensor humiditySensor(1, 100);
  CPressureSensor pressureSensor(1, 100);
  CWindSensor windSensor(1, 100);
  CInsolationSensor insolationSensor(1, 10000);
  CTemperatureSensor temperatureSensor2(1, 100);

  CBase base(8);

  base += temperatureSensor;
  base += humiditySensor;
  base += pressureSensor;
  base += windSensor;
  base += insolationSensor;
  base += temperatureSensor2;

  CSensorChart *pChartBase = new CSensorChart(&humiditySensor, 10, CRect(CPoint(350, 50), CPoint(300, 300)), GREEN, BLACK, RED);
  CBaseChart *pChartSensor = new CBaseChart(&base, 10, CRect(CPoint(350, 50), CPoint(300, 300)), GREEN, BLACK, RED);


  CDesktop d;
  d.insert(pW1);
  d.insert(pW2);
  d.insert(pG);

  d.insert(pClock);

  d.insert(pChartSensor);
  d.insert(pChartBase);

  d.run();

  return 0;
}
