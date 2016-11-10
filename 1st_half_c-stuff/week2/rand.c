#include <stdio.h>  /* To print stuff */
#include <stdlib.h> /* For rand() and srand() */
#include <time.h>   /* For time() */

int main(int argc, char *argv[])
{
  int i;

  srand(time(NULL));

  for (i = 0; i < 10; i++) {
    printf("%d\n", rand() % 100);
  }

  return 0;
}
