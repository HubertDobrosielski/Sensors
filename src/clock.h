#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "winsys.h"

class CClock : public CFramedWindow
{
private:
  SDL_TimerID timer_id;

protected:
  color fc;

public:
  CClock(CRect r, color wc = DWC, color fc = DFC);
  ~CClock();

  void paint();
};

#endif /*__CLOCK_H__*/
