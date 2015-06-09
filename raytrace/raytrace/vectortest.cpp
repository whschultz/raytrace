#include "vector.hh"
#include <iostream.h>

int main(void)
{
  vector3 i(1,0,0);
  vector3 j(0,1,0);
  vector3 k(0,0,1);
  i.setrtp();
  j.setrtp();
  k.setrtp();

  cout << "i+j+k=" << i+j+k << endl;

  return 1;
}
