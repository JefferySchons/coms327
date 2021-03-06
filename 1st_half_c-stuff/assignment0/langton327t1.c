#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

const char *params = "PATTERN I\nOUTPUT langton.mpg\nBASE_FILE_FORMAT PPM\n"
                     "INPUT_CONVERT *\nGOP_SIZE 650\nSLICES_PER_FRAME 1\n"
                     "INPUT_DIR .\nINPUT\n"
                     ".langton_data/*.pbm [0000000000-%d+%d]\n"
static int _x_size, _y_size, _skip, _sequence_number;

int start_encode(int x_size, int y_size, int skip)
{
  if ((x_size * y_size) % 8) {
    fprintf(stderr, "Error: x_size * y_size must be divisible by 8.\n");
    return 1;
                     "END_INPUT\nPIXEL HALF\nRANGE 10\n"
                     "PSEARCH_ALG LOGARITHMIC\nBSEARCH_ALG CROSS2\nIQSCALE 8\n"
                     "PQSCALE 10\nBQSCALE 25\nREFERENCE_FRAME ORIGINAL\n";

#define NAME_TEMPLATE ".langton_data/%010u.pbm"
  }

  if (x_size * y_size > 65536) {
    fprintf(stderr, "Error: Let's not be ridiculous.\n");
    return 1;
  }

  if (skip < 1) {
    fprintf(stderr, "Error: skip must be positive.\n");
  }

  system("rm -f ppmtompeg_params");
  system("rm -rf .langton_data");

  if (mkdir(".langton_data", 0700)) {
    perror(".langton_data");
    return 1;
  }

  _x_size = x_size;
  _y_size = y_size;
  _skip = skip;
  _sequence_number = 0;

  return 0;
}

int next_frame(char *data)
{
  char name[80];
  FILE *file;
  unsigned char byte;
  unsigned i;

  if (_sequence_number % _skip) {
    _sequence_number++;

    return 0;
  }

  sprintf(name, NAME_TEMPLATE, _sequence_number++);
  file = fopen(name, "w");

  fprintf(file, "P4\n%d %d\n", _x_size, _y_size);

  for (i = 0, byte = 0; i < _x_size * _y_size; i++) {
    if (i && !(i % 8)) {
      fwrite(&byte, 1, 1, file);
      byte = 0;
    }
    if (data[i]) {
      byte |= (1 << (7 - (i % 8)));
    }
  }
  fwrite(&byte, 1, 1, file);

  fclose(file);

  return 0;
}

int finish_encode(void)
{
  char name[80];
  char encoder_params[400];
  FILE *f;

  strcpy(name, "ppmtompeg_params");
  sprintf(encoder_params, params, _sequence_number - 1, _skip);

  f = fopen(name, "w");
  fwrite(encoder_params, strlen(encoder_params), 1, f);
  fclose(f);

  system("ppmtompeg ppmtompeg_params");
  system("rm -f ppmtompeg_params");
  system("rm -rf .langton_data");

  return 0;
}

char buffer[128][128];

int main(int argc, char *argv[])
{
  int i;

  start_encode(128, 128, 5);
  
  int j;
  int direction;
  direction = 1; 
  //direction: up=1, right = 2 down = 3, left = 4,
  
  //place ant initially with direction
  i = 60; //x axes
  j = 70; //y axes
  //also set color of first cell; 1 = black,  0 = white
  buffer[i][j] = 1;
  
  //check if ant is in i and j boundries
  
  //check what current cell is
  while ( 0<i && i<128 && 0<j && j<128)
  {
      if (buffer[i][j]==0)
      {
      	// cell is white
      	direction = direction + 1;
      	if (direction>4)
      	{
      		direction =1;
      	}
      	buffer[i][j]=1;
      	
      	//direction: up=1, right = 2 down = 3, left = 4,
      	//move forword one
      	if (direction == 1)
      	{
      		j=j+1;
      	}
      	else if (direction == 2)
      	{
      		i=i+1;
      	}
      	else if (direction == 3)
      	{
      		j=j-1;
      	}
      	else if (direction == 4)
      	{
      		i=i-1;
      	}
      }
      else
      {
      	//cell is black
      	direction = direction - 1;
      	if (direction<1)
      	{
      		direction =4;
      	}
      	buffer[i][j]=0;
      	
      	//direction: up=1, right = 2 down = 3, left = 4,
      	//move forword one
      	if (direction == 1)
      	{
      		j=j+1;
      	}
      	else if (direction == 2)
      	{
      		i=i+1;
      	}
      	else if (direction == 3)
      	{
      		j=j-1;
      	}
      	else if (direction == 4)
      	{
      		i=i-1;
      	}
      }
      
      next_frame((char *) buffer);
  }
  /*
  //do the two differnent commands baced on output
  //save direction ant is facing
  for (i = 0; i < 128; i++) {
	//ant is at i,i
    buffer[i][i] = 1;
    next_frame((char *) buffer);
  }
  */
  finish_encode();

  return 0;
}
