#include <stdio.h>

#define min(x, y) x < y ? x : y

#define max(x, y) ({ \
  typeof (x) _x = x; \
  typeof (y) _y = y; \
  _x < _y ? _y : _x; \
})

#define concatenate(x, y) x ## y

# define has_characteristic(character, bit)             \
  ((character)->npc->characteristics & NPC_##bit)

#define to_string(s) #s

#define compare(s) { #s, s }
struct {
  char *name;
  int (*comparitor)(void *, void *);
} comparitors[] = {
  compare(cmp_int),
  compare(strpcmp)
};

int main(int argc, char *argv[])
{
  int a = 5;
  int b = 7;

  printf("%d\n", max(sqrt(a), sqrt(b)));

  concatenate(1, 2);

  if (has_characteristic(c, SMART)) {
  }

  to_string(COM S 327);

  comparitors[i].comparitor(x, y);

  return 0;
}


