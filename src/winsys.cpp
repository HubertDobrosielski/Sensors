#include "cpoint.h"
#include "winsys.h"

#include <ctype.h>
#include <list>
#include <stdarg.h>
#include <string>
using namespace std;

//----------------------------------------------------------------------
// CView methods

bool CView::handleEvent(int key) {
  switch (key) {
  case SDLK_UP:
    move(CPoint(0, -10));
    return true;
  case SDLK_DOWN:
    move(CPoint(0, 10));
    return true;
  case SDLK_RIGHT:
    move(CPoint(10, 0));
    return true;
  case SDLK_LEFT:
    move(CPoint(-10, 0));
    return true;
  };
  return false;
}


//----------------------------------------------------------------------
// CWindow methods

void CWindow::paint() {
  gfx_filledRect(geom.topleft.x, geom.topleft.y,
                 geom.topleft.x + geom.size.x - 1,
                 geom.topleft.y + geom.size.y - 1, wc);
}

bool CWindow::handleEvent(int key) {
  if (CView::handleEvent(key))
    return true;

  switch (key) {
  case SDLK_EQUALS:
    geom.size += CPoint(10, 10);
    return true;
  case SDLK_MINUS:
    geom.size -= CPoint(10, 10);
    return true;
  };

  return false;
}

void CWindow::move(const CPoint &delta) { geom.topleft += delta; }

//----------------------------------------------------------------------
// CFramedWindow methods

void CFramedWindow::paint() {
  CWindow::paint();
  gfx_rect(geom.topleft.x, geom.topleft.y, geom.topleft.x + geom.size.x - 1,
           geom.topleft.y + geom.size.y - 1, fc);
}

//----------------------------------------------------------------------
// CInputLine methods

void CInputLine::paint() {
  CFramedWindow::paint();
  gfx_textout(geom.topleft.x, geom.topleft.y, text.c_str(), RED);
}

bool CInputLine::handleEvent(int c) {
  if (CFramedWindow::handleEvent(c))
    return true;

  if (c == SDLK_BACKSPACE) {
    if (text.length() > 0) {
      text.erase(text.length() - 1);
      return true;
    };
  }

  if ((c > 255) || (c < 0))
    return false;
  if (!isalnum(c) && (c != ' '))
    return false;
  text.push_back(c);
  return true;
}

//----------------------------------------------------------------------
// CGroup methods

void CGroup::paint() {
  for (list<CView *>::iterator i = children.begin(); i != children.end(); i++)
    (*i)->paint();
}

bool CGroup::handleEvent(int key) {
  if (CView::handleEvent(key))
    return true;

  if (!children.empty() && children.back()->handleEvent(key))
    return true;

  if (key == SDLK_PAGEUP) {
    if (!children.empty()) {
      children.push_front(children.back());
      children.pop_back();
    }
    return true;
  }
  else if (key == SDLK_PAGEDOWN) {
    if (!children.empty()) {
      children.push_back(children.front());
      children.pop_front();
    }
    return true;
  }

  return false;
}

void CGroup::move(const CPoint &delta) {

  for (list<CView *>::iterator i = children.begin(); i != children.end(); i++)
    (*i)->move(delta);
}

void CGroup::insert(CView *v) { children.push_back(v); }

CGroup::~CGroup() {
  for (list<CView *>::iterator i = children.begin(); i != children.end(); i++)
    delete (*i);
}

//----------------------------------------------------------------------
// CDesktop methods

void CDesktop::paint() {
  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, DBC);

  CGroup::paint();
}

bool CDesktop::handleEvent(int key) {
  if (!children.empty() && children.back()->handleEvent(key))
    return true;

  if (key == SDLK_TAB) {
    if (!children.empty()) {
      children.push_front(children.back());
      children.pop_back();
    };
    return true;
  }
  return false;
}

SDL_Event CDesktop::getEvent() {
  return gfx_getEvent();
}

void CDesktop::run() {
  paint();
  gfx_updateScreen();

  while (1) {
    bool updateNeeded = false;
    SDL_Event event = getEvent();

    if(event.type == SDL_USEREVENT)
      updateNeeded = true;
    else if(event.type == SDL_KEYDOWN)
    {
      int c = event.key.keysym.sym;
      if (c == 'q')
        break;
      updateNeeded = handleEvent(c);
    }

    if(updateNeeded) {
      paint();
      gfx_updateScreen();
    }
  }
}
