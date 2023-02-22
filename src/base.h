#ifndef __BASE_H__
#define __BASE_H__

#include <vector>
using namespace std;

class CBase
{
private:
  void setSensor(const CSensor *pSensor);

public:
  vector<const CSensor *> Sensors;
  CBase(const uint size_of_array);
  CBase(const CBase &base);

  CBase &operator=(const CBase &base);
  CBase &operator+=(const CSensor &sensor);
  CBase &operator-=(const CSensor &sensor);
  friend ostream &operator<<(ostream &o, CBase &base);
};

#endif