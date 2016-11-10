#include <cstring>
#include <cstdlib>

#include "string327.h"

string327::string327()
{
  str = malloc(1);
  str[0] = 0;
}

string327::string327(const char *s)
{
  str = strdup(s);
}


string327::~string327()
{
  free(str);
}

string327 string327::operator+(const string327 &s)
{
  string327 out;

  out.str = malloc(strlen(str) + s.length() + 1);
  strcpy(out.str, this->str);
  strcat(out.str, s.str);

  return out;
}

string327 &string327::operator+=(const string327 &s)
{
  str = realloc(str, length() + strlen(s.str) + 1);
  strcat(str, s.str);

  return *this;
}

string327 &string327::operator=(const char *s)
{
  free(str);
  str = strdup(s);

  return *this;
}

string327 &string327::operator=(const string327 &s)
{
  free(str);
  str = strdup(s.str);

  return *this;
}

std::istream &operator>>(std::istream &i, const string327 &str)
{
  //Broken!  Why?
  return i >> str.str;
}

std::ostream &operator<<(std::ostream &o, const string327 &str)
{
  return o << str.c_str();
}
