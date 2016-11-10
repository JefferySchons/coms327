#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(int *a, int n)
{
  int i, j, t;

  for (i = 1; i < n; i++) {
    for (t = a[i], j = i - 1; j > -1 && a[j] > t; j--) {
      a[j + 1] = a[j];
    }
    a[j + 1] = t;
  }
}

void generic_insertion_sort(void *data, int n, size_t s,
                            int (*compare)(void *, void *))
{
  void *t;
  int i, j;
  char *a;

  a = data;
  t = malloc(s);

  for (i = 1; i < n; i++) {
    for (memcpy(t, a + (s * i), s), j = i - 1;
         j > -1 && compare(a + (s * j), t) > 0;
         j--) {
      memcpy(a + (s * (j + 1)), a + (s * j), s);
    }
    memcpy(a + (s * (j + 1)), t, s);
  }

  free(t);
}

int cmp_int(int *p1, int *p2)
{
  return *p1 - *p2;
}

int strpcmp(void *v1, void *v2)
{
  return strcmp(*(char **) v1, *(char **) v2);
}

int main(int argc, char *argv[])
{
  int a[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  int i;
  
  for (i = 0; i < 10; i++) {
    printf("%d\n", a[i]);
  }

  putchar('\n');

  //  insertion_sort(a, sizeof (a) / sizeof (a[0]));
  generic_insertion_sort(a, sizeof (a) / sizeof (a[0]),
                         sizeof (a[0]), (void *) cmp_int);

  for (i = 0; i < 10; i++) {
    printf("%d\n", a[i]);
  }

  
putchar('\n');

  char *s[] = {
    "zero",    "one",    "two",    "three",    "four",
    "five",    "six",    "seven",  "eight",    "nine"
  };

  generic_insertion_sort(s, sizeof (s) / sizeof (s[0]),
                         sizeof (s[0]), strpcmp);

  for (i = 0; i < 10; i++) {
    printf(s[i]);
    putchar('\n');
  }

  return 0;
}
