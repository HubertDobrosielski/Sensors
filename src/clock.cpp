#include "clock.h"

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

void CClock::paint() {
  CFramedWindow::paint();

  CPoint center(geom.topleft.x + geom.size.x / 2,
                geom.topleft.y + geom.size.y / 2);

  // Dial
  const int smaller_dimension = geom.size.x < geom.size.y ? geom.size.x : geom.size.y;
  const int dial_radius = smaller_dimension / 2 - 20;
  const int horizontal_shift = 12;
  const int vertical_shift = 4;
  for (int h = 1; h <= 12; ++h) {
    double hour_angle = M_PI / 2 - h * (M_PI / 6);
    CPoint hour_pos(center.x + dial_radius * cos(hour_angle) - horizontal_shift,
                    center.y - dial_radius * sin(hour_angle) - vertical_shift);
    char hour_text[3];
    sprintf(hour_text, "%2d", h);

    gfx_textout(hour_pos.x, hour_pos.y, hour_text, BLACK);
  }

  // Hands of the clock
  gfx_filledCircle(center.x, center.y, 10, BLACK);

  time_t timer;
  time(&timer);
  struct tm *pT = localtime(&timer);

  double sec_hand_angle = M_PI / 2 - pT->tm_sec * (M_PI / 30);
  double min_hand_angle =
      M_PI / 2 - (pT->tm_min + pT->tm_sec / 60.0) * (M_PI / 30);
  double hour_hand_angle =
      M_PI / 2 - (pT->tm_hour + pT->tm_min / 60.0) * (M_PI / 6);

  int sec_hand_length = 0.9 * dial_radius;
  int min_hand_length = 0.7 * dial_radius;
  int hour_hand_length = 0.5 * dial_radius;;

  CPoint sec_hand_end(center.x + sec_hand_length * cos(sec_hand_angle),
                      center.y - sec_hand_length * sin(sec_hand_angle));
  CPoint min_hand_end(center.x + min_hand_length * cos(min_hand_angle),
                      center.y - min_hand_length * sin(min_hand_angle));
  CPoint hour_hand_end(center.x + hour_hand_length * cos(hour_hand_angle),
                       center.y - hour_hand_length * sin(hour_hand_angle));

  gfx_line(center.x, center.y, sec_hand_end.x, sec_hand_end.y, BLACK);
  gfx_wline(center.x, center.y, min_hand_end.x, min_hand_end.y, BLACK, 3);
  gfx_wline(center.x, center.y, hour_hand_end.x, hour_hand_end.y, BLACK, 6);
}
