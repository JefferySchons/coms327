#include <iostream>

#include "header.h"

using namespace std;

void cpp_function(const char *s)
{
  cout << s << endl;
}

ostream *return_cout(void)
{
  return &cout;
}

void use_cout(ostream *o)
{
  *o << "Using cout pointer that I got from C" << endl;
}
