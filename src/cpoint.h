#ifndef __CPOINT_H__
#define __CPOINT_H__

#include <list>

using namespace std;

struct CPoint
{
  int x;
  int y;
  CPoint(int _x = 0, int _y = 0) : x(_x), y(_y){};
  CPoint &operator+=(const CPoint &delta)
  {
    x += delta.x;
    y += delta.y;
    return *this;
  }
  CPoint &operator-=(const CPoint &delta)
  {
    x -= delta.x;
    y -= delta.y;
    return *this;
  }
  bool operator==(const CPoint &p)
  {
    if ((x == p.x) && (y == p.y))
      return true;

    return false;
  }
  bool operator!=(const CPoint &p)
  {
    if ((x != p.x) || (y != p.y))
      return true;

    return false;
  }
  bool operator==(const list<CPoint> &l)
  {
    for (auto it = l.begin(); it != l.end(); ++it)
      if (*this == *it)
        return true;

    return false;
  }
  bool operator!=(const list<CPoint> &l)
  {
    for (auto it = l.begin(); it != l.end(); ++it)
      if (*this == *it)
        return false;

    return true;
  }

  // DOPISANE
  CPoint &operator=(const CPoint &p)
  {
    x = p.x;
    y = p.y;
    return *this;
  }
};

struct CRect
{
  CPoint topleft;
  CPoint size;
  CPoint bottomRight;
  CRect(CPoint t1 = CPoint(), CPoint s = CPoint()) : topleft(t1), size(s){};
  CPoint getBottomRight()
  {
    this->bottomRight.x = topleft.x + size.x;
    this->bottomRight.y = topleft.y + size.y;
    return bottomRight;
  }
  //  CRect(CPoint t1=CPoint(), CPoint t2=CPoint()):
  //    topleft(t1), size(t2.x-t1.x, t2.y-t1.y) {};

  // DOPIOSANE
  bool operator==(const CRect &r)
  {
    if ((topleft == r.topleft) && (size == r.size))
      return true;
    return false;
  }
  bool operator!=(const CRect &r)
  {
    if ((topleft != r.topleft) || (size != r.size))
      return true;
    return false;
  }
  CRect &operator=(const CRect &r)
  {
    if (this != &r)
    {
      size = r.size;
      topleft = r.topleft;
    }
    return *this;
  }
};

#endif
