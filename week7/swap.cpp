#include <iostream>

using namespace std;

void cswap(int *x, int *y)
{
  int tmp;

  tmp = *x;
  *x = *y;
  *y = tmp;
}

void cppswap(int &x, int &y)
{
  int tmp;

  tmp = x;
  x = y;
  y = tmp;
}

int main(int argc, char *argv[])
{
  int x = 5;
  int y = 7;

  cout << x << " " << y << endl;

  cswap(&x, &y);

  cout << x << " " << y << endl;

  cppswap(x, y);

  cout << x << " " << y << endl;

  return 0;
}
