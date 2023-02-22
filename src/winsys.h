#ifndef __WINSYS_H__
#define __WINSYS_H__

#include "../lib/primlib.h"
#include "cpoint.h"
#include <list>

#include <string>
using namespace std;

#define DBC WHITE /* Default background color*/
#define DWC BLACK /* Default window color*/
#define DFC GREEN /* Default frame color*/

class CView
{
public:
  CView() {}
  virtual ~CView() {}

  virtual void paint () = 0;
  virtual bool handleEvent (int key);
  virtual void move (const CPoint & delta) = 0;
};

class CWindow : public CView
{
protected:
  CRect geom;   // geometry
  color wc;     // window color

public:
  CWindow(CRect r, color wc = DWC) : geom(r), wc(wc) {}

  void paint();
  bool handleEvent(int key);
  void move (const CPoint & delta);
};

class CFramedWindow : public CWindow
{
protected:
  color fc;

public:
  CFramedWindow(CRect r, color wc = DWC, color fc = DFC) : CWindow(r, wc), fc(fc) {}

  void paint();
};

class CInputLine : public CFramedWindow
{
  string text;
public:
  CInputLine(CRect r, color wc = DWC, color fc = DFC) : CFramedWindow(r, wc, fc) {}

  void paint();
  bool handleEvent(int c);
};

class CGroup : public CView
{
protected:
  list<CView *> children;

public:
  CGroup() {}
  ~CGroup();

  void paint();
  bool handleEvent(int key);
  void move (const CPoint & delta);

  void insert(CView * v);
};

class CDesktop : public CGroup
{
public:
  CDesktop() {};
  ~CDesktop() {};

  void paint();
  bool handleEvent(int key);

  SDL_Event getEvent();
  void run();
};

#endif
