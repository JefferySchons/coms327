#include <stdio.h>

int main(int argc, char *argv[])
{
  const int i = 0;

  *(int *) &i = 1;

  printf("%d\n", i);

  return 0;
}
