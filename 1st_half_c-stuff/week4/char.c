#include <stdio.h>
#include <ctype.h>

char *words[] = {
  "animal",
  "balogni",
  "cat",
  "dog",
  "emacs",
  "fun",
  "ghetto",
  "hallway",
  "igloo",
  "judge",
  "kangaroo",
  "lemon",
  "meme",
  "november",
  "ostentatious",
  "photosysthesis",
  "query",
  "rambuntios",
  "soup",
  "telekenesis",
  "ukelele",
  "victory",
  "wumbo",
  "xenophobe",
  "yellow",
  "zylophone"
};

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Please supply a letter.\n");
    return 1;
  }

  printf("%s begins with %c\n", words[toupper(argv[1][0]) - 'A'], argv[1][0]);

  return 0;
}
