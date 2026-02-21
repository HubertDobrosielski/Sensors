#include "clock.h"

#include "clock.h"
#include <ctime>   // time, localtime, struct tm
#include <cmath>   // M_PI (jeśli potrzebne)
#include "renderer.h"
#include "ui_draw.h"
#include "cpoint.h"

static Uint32 timer_callback(Uint32 interval, void *param) {
  SDL_Event event;
  SDL_UserEvent userevent;

  /* In this example, the callback pushes an SDL_USEREVENT event
  into the queue and causes the callback to be called again at the
  same interval: */

  userevent.type = SDL_USEREVENT;
  userevent.code = 0;
  userevent.data1 = NULL;
  userevent.data2 = NULL;

  event.type = SDL_USEREVENT;
  event.user = userevent;

  SDL_PushEvent(&event);

  return interval;
}

CClock::CClock(CRect r, color wc /*=DWC*/, color fc /*=DFC*/)
    : CFramedWindow(r, wc, fc) {
  const Uint32 delay = 1000; /* miliseconds */
  timer_id = SDL_AddTimer(delay, timer_callback, nullptr);
}

CClock::~CClock()
{
  SDL_RemoveTimer(timer_id);
}

void CClock::paint(Renderer& r)
{
  // tło + ramka
  CFramedWindow::paint(r);

  CPoint center(
    geom.topleft.x + geom.size.x / 2,
    geom.topleft.y + geom.size.y / 2
  );

  // Dial (tarcza)
  int smaller = geom.size.x < geom.size.y ? geom.size.x : geom.size.y;
  int dial_radius = smaller / 2 - 20;

  const int hshift = 12;
  const int vshift = 4;

  for (int h = 1; h <= 12; ++h) {
    double angle = M_PI / 2 - h * (M_PI / 6);
    CPoint pos(
      center.x + dial_radius * cos(angle) - hshift,
      center.y - dial_radius * sin(angle) - vshift
    );

    char buf[3];
    sprintf(buf, "%2d", h);
    r.text(pos, buf, BLACK);
  }

  // środek zegara
  r.filledCircle(center, 10, BLACK);

  // czas
  time_t timer;
  time(&timer);
  tm* t = localtime(&timer);

  double secA  = M_PI / 2 - t->tm_sec * (M_PI / 30);
  double minA  = M_PI / 2 - (t->tm_min + t->tm_sec / 60.0) * (M_PI / 30);
  double hourA = M_PI / 2 - (t->tm_hour + t->tm_min / 60.0) * (M_PI / 6);

  int secL  = 0.9 * dial_radius;
  int minL  = 0.7 * dial_radius;
  int hourL = 0.5 * dial_radius;

  CPoint secEnd(
    center.x + secL * cos(secA),
    center.y - secL * sin(secA)
  );
  CPoint minEnd(
    center.x + minL * cos(minA),
    center.y - minL * sin(minA)
  );
  CPoint hourEnd(
    center.x + hourL * cos(hourA),
    center.y - hourL * sin(hourA)
  );

  r.line(center, secEnd, BLACK);
  r.wideLine(center, minEnd, 3, BLACK);
  r.wideLine(center, hourEnd, 6, BLACK);
}