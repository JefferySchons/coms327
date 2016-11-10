#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dungeon_header.h"

int print_hardness_map(int (*dungion_hardness_map_input)[80][21]);
int print_map(char dungion_map_input[80][21]);
int load_map(char (*dungion_map_input)[80][21], int (*dungion_hardness_map_input)[80][21], room_t (*room_array_input)[6]);
int save_map(char (*dungion_map_input)[80][21], int (*dungion_hardness_map_input)[80][21], room_t (*room_array_input)[6]);


int main(int argc, char *argv[])
{
	int j;
	int k;
	//int i;
	
	//initalize char dungion_map[map_x_size][map_y_size];
	//initalize hardness map
	//initalize struct array
	int map_x_size = 80; //horizontal
	int map_y_size = 21; // veritical
	int number_of_rooms = 6;
	
	char **dungion_map[map_x_size][map_y_size];
	
	struct room *room_array[number_of_rooms];
	
	int **dungion_hardness_map[map_x_size][map_y_size];
	
	//preset information
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			
			**dungion_map[k][j]='+';
			//note 255 is max hardness
			**dungion_hardness_map[k][j]=255;
	  }
	}
	

	if(argv[1]!=NULL)
	{
		/*
		base
		-generate
		-disply
		-exit
		*/
		if(strcmp(argv[1], "--save") == 0)
		{
			/*
			-generate
			-display
			-save
			-exit
			*/
			printf("%c", 'h');
		}
		if(strcmp(argv[1], "--load") == 0)
		{
			/*
			load from disk
			disply
			exit
			*/
			printf("%c", 'h');
		}
	}
 
 
	srand(time(NULL));
	k=make_dungion_map(&dungion_map, &dungion_hardness_map, &room_array);
	//k=load_map(&dungion_map, &dungion_hardness_map, &room_array);
	
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			if(**dungion_map[k][j]=='+')
			{
				printf("%c", ' ');
			}
			else 
			{
				printf("%c", **dungion_map[k][j]);
			}
	  }
	  printf("\n");
	}
	
	//k=print_map(dungion_map);
	//k=print_hardness_map(&dungion_hardness_map);
	
	return 0;
}

int print_map(char dungion_map_input[80][21])
{
	int j;
	int k;	
	int map_x_size = 80; //horizontal
	int map_y_size = 21; // veritical
	//char *print_char=dungion_map;
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			if(dungion_map_input[j][k]=='+')
			{
				printf("%c", ' ');
			}
			else
			{
				printf("%c", dungion_map_input[j][k]);
			}
			//print_char++;
			
	  }
	  printf("\n");
	}
	return 0;
}

int print_hardness_map(int (*dungion_hardness_map_input)[80][21])
{
	int j;
	int k;	
	int map_x_size = 80; //horizontal
	int map_y_size = 21; // veritical
	//int *print_char=dungion_hardness_map;
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			printf("%d", (*dungion_hardness_map_input)[j][k]);
			//print_char++;
	  }
	  printf("\n");
	}
		return 0;
}

int load_map(char (*dungion_map_input)[80][21], int (*dungion_hardness_map_input)[80][21], room_t (*room_array_input)[6])
{
		return 0;
}

int save_map(char (*dungion_map_input)[80][21], int (*dungion_hardness_map_input)[80][21], room_t (*room_array_input)[6])
{
		return 0;
}
