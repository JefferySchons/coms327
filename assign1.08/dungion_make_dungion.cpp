#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <string>
#include "dungion.h"


struct room
{
	int x_pos; //uper left corrner
	int y_pos;
	int x_size;
	int y_size;
};

struct creature
{
//for both monster and PC
	int is_pc;//not needed becouse pc will always be the first [0] in the array of monsters
	char* type;//like skelliten or zombie or dragon
	char* name; //for specific names like this one is named bob or allice
	int specs; //technically 4 bit used, called charactersitics in assignment
	//assign intelligence to the least significant bit (on the right), telepathy next, tunneling third, 
	//and erratic behavior last (on the left),
	//erratic, tunneling, telepathy, intelligence
	char icon; //defult for monster is ?; will never use it for monster
	int move_time; //=100/creature_array[x].speed;
	int next_turn;//for what this creatures next turn is (prev turn +move_time)
	int x_pos;
	int y_pos;
	int speed;
	int alive;
};
	
int map_x_size;
int map_y_size;
int number_of_rooms;

int make_dungeon_map(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], room room_array_input[6],  int map_x_size, int map_y_size, int number_of_rooms);
int print_map(char dungion_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char monster_map_input[80][21]);
int print_hardness_map(int dungion_hardness_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char pathfinding_map_input[80][21]);
int save_dungion(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], room room_array_input[6],  int map_x_size, int map_y_size, int number_of_rooms);
int load_dungion(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], room room_array_input[6],  int map_x_size, int map_y_size, int number_of_rooms);
int pathfinding(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], char pathfinding_map_input[80][21], int map_x_size, int map_y_size, int pc_x, int pc_y);
int pathfinding_wall(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], char pathfinding_wall_map_input[80][21], int map_x_size, int map_y_size, int pc_x, int pc_y);
int print_path_through_wall_map(int dungion_hardness_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char pathfinding_wall_map_input[80][21]);

int place_monsters(char monster_map_input[80][21], char dungion_map_input[80][21], room room_array_input[6], int map_x_size, int map_y_size, int number_of_rooms, int num_monsters, creature creature_array[6]);
int run_game(char monster_map_input[80][21], char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], char pathfinding_map_input[80][21], room room_array_input[6], int map_x_size, int map_y_size, int number_of_rooms, int num_monsters, creature creature_array[6], int line_of_sight_map_input[80][21]);

int line_of_sight(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], int line_of_sight_map_input[80][21], int map_x_size, int map_y_size, int pc_x, int pc_y);

//functiom(dungion_map, dungion_hardness_map, pathfinding_map, room_array, map_x_size, map_y_size, number_of_rooms, num_monsters, creature_array);
		

int main(int argc, char *argv[]){
	int j;
	int k;
	int i;
	int x;
	
	//initalize char dungion_map[map_x_size][map_y_size];
	//initalize hardness map
	//initalize struct array
	map_x_size = 80; //horizontal
	map_y_size = 21; // veritical
	number_of_rooms = 6; //base number of rooms
	
	int num_monsters=6; //defult //average of 1 per room (minus pc room)
	//true defult is one less (pc takes a slot
	//int num_obejcts=15; [map_x_size][map_y_size] -> [80][21]
	
	char dungion_map[80][21];
	
	struct room room_array[6];
	
	int dungion_hardness_map[80][21];
	char pathfinding_map[80][21];
	int line_of_sight_map[80][21];
	
	//only used to display monster token
	//reset after every move
	char monster_map[80][21];
	
	int prompt_save=0;
	int prompt_load=0;
	
	//preset information
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			
			dungion_map[k][j]='+';
			//note 255 is max hardness
			dungion_hardness_map[k][j]=1;
			pathfinding_map[k][j]='.';
			monster_map[k][j]='#';
			line_of_sight_map[k][j]=100;//will assume sight can not reach that far
	  }
	}
	
	//change to looping through argv
	/*
	-generate (or load if inabled)
	-disply
	(save if inabled)
	-exit
	*/
	
  for (i = 0; i < argc; i++) 
  {
		if(argv[i]!=NULL)
		{
			if(strcmp(argv[i], "--save") == 0)
			{
				prompt_save=1;
			}
			else if(strcmp(argv[i], "--load") == 0)
			{
				prompt_load=1;
			}
			else if(strcmp(argv[i], "--nummon") == 0)
			{
				num_monsters=atoi(argv[i+1]);
				num_monsters++;//for player
				num_monsters=6;//10  //to avoid variable array lack of problem for c++1
			}
		}
	}
	k=prompt_save;
	k=prompt_load;
	srand(time(NULL));
	
	if(prompt_load==1)
	{
		//load from disk; 
		//k=load_dungion(dungion_map, dungion_hardness_map, room_array, map_x_size, map_y_size, number_of_rooms);
		//temp
		//k=make_dungeon_map(dungion_map, dungion_hardness_map, room_array, map_x_size, map_y_size, number_of_rooms);
	}
	else
	{
		//k=make_dungeon_map(dungion_map, dungion_hardness_map, room_array, map_x_size, map_y_size, number_of_rooms);
	}
	if(prompt_save==1)
	{
		//k=save_dungion(dungion_map, dungion_hardness_map, room_array, map_x_size, map_y_size, number_of_rooms);

		//save file
	}
	else
	{
		//dont save
	}
	
	struct creature creature_array[6];
	
  creature_array[0].is_pc=0;
  creature_array[0].type="human";//is here if the pc can ever shapeshift
  creature_array[0].name="JefferySchons";//yes the pc name is the name of the programmer
  creature_array[0].specs=3;//unimportant for pc; but we are smart and can see everything
  creature_array[0].icon='@';
  creature_array[0].move_time=0;
  creature_array[0].next_turn=-1;
  creature_array[0].x_pos=-1;
  creature_array[0].y_pos=-1;
  creature_array[0].speed=10; //player speed is defult 10
  creature_array[0].alive=1;//dead untill placed    
  
	for (x = 1; x < num_monsters; x++)
	{
    creature_array[x].is_pc=1;
    creature_array[x].type="generic";
    creature_array[x].name="moster";
    creature_array[x].specs=0;//erratic, tunneling, telepathy, intelligence ; initalize brain dead
    creature_array[x].icon='?';
    creature_array[x].move_time=0;
    creature_array[x].next_turn=-1;
    creature_array[x].x_pos=-1;
    creature_array[x].y_pos=-1;
    creature_array[x].speed=0; //no speed untill placed
    creature_array[x].alive=1;//dead untill placed    
	}
	
	k=make_dungeon_map(dungion_map, dungion_hardness_map, room_array, map_x_size, map_y_size, number_of_rooms);
	//k=pathfinding(dungion_map, dungion_hardness_map, pathfinding_map, map_x_size, map_y_size, room_array[0].x_pos+1 ,room_array[0].y_pos+1);
	
	//get monster from factory list
	//get amount of monsters loaded, replace that number of monsters with monsters in factory
	//	"randomly" make rest of monsters eather loaded or defult
	//monster_disc_factory_list
	//random make few objects
	
	
	k=read_in_monsters();
	
	//place_objects
	
	//place objects inside rungame
	if(k!=1)
	{
		//k=0;
		//k=place_objects();
		//num_monsters
		//both makes monsters, adds to que, and add to map
		k=place_monsters(monster_map, dungion_map, room_array, map_x_size, map_y_size, number_of_rooms, num_monsters, creature_array);
		
		if(k!=1)
		{
			k=run_game(monster_map, dungion_map, dungion_hardness_map, pathfinding_map, room_array, map_x_size, map_y_size, number_of_rooms, num_monsters, creature_array, line_of_sight_map);
			
			//the game runs and terminates in here
			endwin();
			
			
			if(k==0)
			{
			}
			else if(k==1)
			{
				printf("%s \n", "quit game");
			}
			else if(k==2)
			{
				printf("%s \n", "you dead");
			}
			else if(k==3)
			{
				printf("%s \n", "max turns");
			}
			else
			{
				printf("%s \n", "run game returned and unexpected value 0");
			}
		}
		else
		{
		}
	}
	else
	{	
	}
	
	//k=print_map(dungion_map, map_x_size, map_y_size, number_of_rooms, monster_map);
	
	if(k!=1)
	{
		//k=print_hardness_map(dungion_hardness_map, map_x_size, map_y_size, number_of_rooms, pathfinding_map);
	}  
	else
	{
		//printf("%c", 'a');
	}
	
	//for pathing through wall
	//k=pathfinding_wall(dungion_map, dungion_hardness_map, pathfinding_map, map_x_size, map_y_size, room_array[0].x_pos+1 ,room_array[0].y_pos+1);
	if(k!=1)
	{
		//k=print_path_through_wall_map(dungion_hardness_map, map_x_size, map_y_size, number_of_rooms, pathfinding_map);
	}  
	else
	{
		//printf("%c", 'a');
	}
	//pc_x, pc_y
	
	

	return 0;
}

int make_dungeon_map(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], room room_array_input[6], int map_x_size, int map_y_size, int number_of_rooms)
{
	/*
	min reqs
	80 units horizontal = x
	21 units vertical = y
	romms per dungion =6
	rooms 4(x) * 3(y)
	1 cell of non room between any 2 rooms
	rooms cells drawn with .
	corridor cells #
	rock is spaces
	no corridors inside rooms
	edge is rock
	*/ 
	//int map_x_size = 80; //horizontal
	//int map_y_size = 21; // veritical
	//int number_of_rooms = 6;
	int rand_x_size; //for x size of room
	int rand_y_size; //for y size of room
	int rand_x_place; //for x placement of room
	int rand_y_place; //for y placement of room
	int i;
	int r; //counter
	int j; //for printing to and from map (x)
	int k; //for printing to and from map (y)	
	
	char dungion_map[map_x_size][map_y_size];
	int dungion_hardness_map[map_x_size][map_y_size];
	int room_placement_array[number_of_rooms][2]; //[row][collum]
	struct room room_array[number_of_rooms];

	//preset dungion map, will ignore all + when printing
	for(j=0; j<map_x_size; j++)
	{
		for(k=0; k<map_y_size; k++)
		{
			dungion_map[j][k]='+';
			dungion_hardness_map[j][k]=1;
	  }
	}
	
	//set edge to 255
	for(j=0; j<map_y_size; j++)
	{
		dungion_hardness_map[0][j]=255;
		dungion_hardness_map[map_x_size-1][j]=255;
	}
	for(k=0; k<map_x_size; k++)
	{
		dungion_hardness_map[k][0]=255;
		dungion_hardness_map[k][map_y_size-1]=255;
	}
	

	int number_of_rooms_placed;
  //foirst make of room_array
  number_of_rooms_placed = 0;
  int number_of_failures = 0; //number of times it has cycled through while
  while(number_of_rooms_placed < number_of_rooms || number_of_failures < number_of_rooms)
  {
  	//populate array
  	for(i=0; i<number_of_rooms; i++)
  	{
  		rand_x_size = (rand() %5) + 4;
  		rand_y_size = (rand() %5) + 3;
  		room_placement_array[i][0]=rand_x_size;
  		room_placement_array[i][1]=rand_y_size;
  		
  		number_of_failures++;
  	}
  	
  	//cycle through array trying to place rooms
  	for(i=0; i<number_of_rooms-number_of_rooms_placed; i++)
  	{
  		rand_x_place = (rand() %80)+1; //+1 is so it wont have anything on row/columb 0
  		rand_y_place = (rand() %21)+1;
  		if(room_placement_array[i][0]+rand_x_place<80 && room_placement_array[i][1]+rand_y_place<21)
  		{
  			//valid room check if place ocupued
  			int occupied =0;
  			
				for(j=rand_x_place-1; j<(rand_x_place+room_placement_array[i][0]+1); j++)
				{
					for(k=rand_y_place-1; k<(rand_y_place+room_placement_array[i][1]+1); k++)
					{
						if(dungion_map[j][k]=='.')
						{
							occupied=1;
						}
				  }
				  //printf("Can't get here\n");
				}
  			
  			if(occupied==0)
  			{
	  			
	  			//add room
	  			
	  			struct room add_room;
	  			add_room.x_pos = rand_x_place;
	  			add_room.y_pos = rand_y_place;
	  			add_room.x_size = room_placement_array[i][0];
	  			add_room.y_size = room_placement_array[i][1];
	  			
	  			//add room to struct array
	  			room_array[number_of_rooms_placed]=add_room;
					
					// edit map
					for(j=rand_x_place; j<rand_x_place+room_placement_array[i][0]; j++)
					{
						for(k=rand_y_place; k<rand_y_place+room_placement_array[i][1]; k++)
						{
							dungion_map[j][k]='.';
							dungion_hardness_map[j][k]=0;
					  }
					}
					
	  			number_of_rooms_placed++;
  			}
  		}
  		//if not added go to next room on list
  	}
  }
  
  //make hallways
  /*
    connect room in order
    room #: 1->2->3-> ... ->last->1
    if goes through another room do not 
    eddit map
    will not be saving the hallways
    (in a structure)
	
	//also adds hallways
  */
  int next_room_numb;
  next_room_numb=0;
  for(r=0; r<number_of_rooms; r++)
  {
  	//make coordor from room_array[r] to room_array[r+1]
  	int left_right;  //x axes (also j is used above for it), right pos
  	int up_down; //y axes (also k is used above for it), up is pos
  	
  	next_room_numb=r+1;
  	if(next_room_numb==number_of_rooms)
  	{
  		next_room_numb=0;
  	}
  	
    left_right = room_array[next_room_numb].x_pos - room_array[r].x_pos;
  	up_down = room_array[next_room_numb].y_pos - room_array[r].y_pos;

  	//"it" is refuring to room[r+1]
  	//do note the up down here is asuming that increasing y goes up
  	//i know inreality it is printed down
  	if(left_right>0)
		{
			//its right 
			for(i=room_array[r].x_pos; i<=room_array[next_room_numb].x_pos; i++)
			{
				if(dungion_map[i][room_array[r].y_pos] != '.')
				{
					dungion_map[i][room_array[r].y_pos] = '#';
					dungion_hardness_map[i][room_array[r].y_pos]=0;
					
				}
			}
			//now to make the up/down corrador
			if(up_down>0)//up
			{
				for(i=room_array[r].y_pos; i<= room_array[next_room_numb].y_pos; i++)
				{
					if(dungion_map[room_array[next_room_numb].x_pos][i] != '.')
					{
						dungion_map[room_array[next_room_numb].x_pos][i] = '#';
						dungion_hardness_map[room_array[next_room_numb].x_pos][i]=0;
					}
				}
			}
			else if (up_down<0) //down
			{
				for(i=room_array[next_room_numb].y_pos; i<= room_array[r].y_pos; i++)
				{
					if(dungion_map[room_array[next_room_numb].x_pos][i] != '.')
					{
						dungion_map[room_array[next_room_numb].x_pos][i] = '#';
						dungion_hardness_map[room_array[next_room_numb].x_pos][i]=0;
					}
				}
			}
		}
  	else if (left_right<0)
		{
			//its left
			for(i=room_array[next_room_numb].x_pos; i<=room_array[r].x_pos; i++)
			{
				if(dungion_map[i][room_array[r].y_pos] != '.')
				{
					dungion_map[i][room_array[r].y_pos] = '#';
					dungion_hardness_map[i][room_array[r].y_pos]=0;
				}
			}
			//now to make the up/down corrador 
			if(up_down<0)//down
			{
				for(i=room_array[next_room_numb].y_pos; i<= room_array[r].y_pos; i++)
				{
					if(dungion_map[room_array[next_room_numb].x_pos][i] != '.')
					{
						dungion_map[room_array[next_room_numb].x_pos][i] = '#';
						dungion_hardness_map[room_array[next_room_numb].x_pos][i]=0;
					}
				}
			}
			else if (up_down>0) //up
			{
				for(i=room_array[r].y_pos; i<= room_array[next_room_numb].y_pos; i++)
				{
					if(dungion_map[room_array[next_room_numb].x_pos][i] != '.')
					{
						dungion_map[room_array[next_room_numb].x_pos][i] = '#';
						dungion_hardness_map[room_array[next_room_numb].x_pos][i]=0;
					}
				}
			}
		}
  		//if streight up/down
  	else if(up_down>0)
		{
			//its up or stright left/right
			for(i=room_array[r].y_pos; i<=room_array[next_room_numb].y_pos; i++)
			{
				if(dungion_map[room_array[r].x_pos][i] != '.')
				{
					dungion_map[room_array[r].x_pos][i] = '#';
					dungion_hardness_map[room_array[r].x_pos][i]=0;
				}
			}
		}
  	else if (up_down<0)
	{
		//its down
		for(i=room_array[next_room_numb].y_pos; i<=room_array[r].y_pos; i++)
		{
			if(dungion_map[room_array[r].x_pos][i] != '.')
			{
				dungion_map[room_array[r].x_pos][i] = '#';
				dungion_hardness_map[room_array[r].x_pos][i]=0;
			}
		}
	}
	
	//add stairway to every even room
	if(r==0)
	{
		//add stairway at room 0
	}
	else
	{
		if((r & 1) == 0)
		{
			//is even
			//dungion_map[j][k]='.';
			if(dungion_map[(room_array[r].x_pos)+2][(room_array[r].y_pos)+2]=='.')
			{
				dungion_map[(room_array[r].x_pos)+2][(room_array[r].y_pos)+2]='>';
			}
		}
	}
  }
  /*
	//print map
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			if(dungion_map[k][j]=='+')
			{
				printf("%c", ' ');
			}
			else
			{
				printf("%c", dungion_map[k][j]);
			}
			
	  }
	  printf("\n");
	}
	*/
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			dungion_map_input[k][j]=dungion_map[k][j];
			dungion_hardness_map_input[k][j]=dungion_hardness_map[k][j];
	  }
	}
	
	for(j=0; j<number_of_rooms; j++)
	{
		room_array_input[j]=room_array[j];
	}
	//dungion_map_input=dungion_map;
	//dungion_hardness_map_input=dungion_hardness_map;
	//room_array_input=room_array;
	
	return 0;
	
		/*
	make array for room sizes
		try to randomly place going down the array
		if unable to place repopulate array 
		untill eather x rooms made or repopulated
		so many times (== number of rooms)
		make room struct after placing the room
		hide corridors inside rooms by drawing over 
		(or not drawing if its a room)
	*/
}

int print_map(char dungion_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char monster_map_input[80][21])
{
	//changed for ncurses
	int j;
	int k;	
	//int map_x_size = 80; //horizontal
	//int map_y_size = 21; // veritical
	//char *print_char=dungion_map;
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			if(monster_map_input[k][j]!='#')
			{
				mvaddch(j, k, monster_map_input[k][j]);
				//printf("%c", monster_map_input[k][j]);
			}
			else if(dungion_map[k][j]=='+')
			{
				mvaddch(j, k, ' ');
				//printf("%c", ' ');
			}
			else
			{
				if(dungion_map[k][j]=='#')
				{
					mvaddch(j, k, '.');
					//printf("%c", '.');
				}
				else
				{
					mvaddch(j, k, dungion_map[k][j]);
					//printf("%c", dungion_map[k][j]);
				}
			}
	  }
	  //printf("\n");
	}
	
	//printf("%c", 'a');
	//printf("\n");
	usleep(125000);
	refresh();
	return 0;
}

int print_hardness_map(int dungion_hardness_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char pathfinding_map_input[80][21])
{
	int j;
	int k;	
	//int map_x_size = 80; //horizontal
	//int map_y_size = 21; // veritical
	//int *print_char=dungion_hardness_map;
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			if(dungion_hardness_map[k][j]==255)
				{
					printf("%c", '.');
				}
			else
				{
					printf("%d", dungion_hardness_map[k][j]);
				}
	  }
	  printf("\n");
	}
	
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			if(pathfinding_map_input[k][j]=='~')
			{
				//printf("%2d", '7');
				//printf("%c",pathfinding_map_input[j][k]);
				printf("%1c",' ');
			}
			else
			{
				if((int)pathfinding_map_input[k][j]>61)
				{
					printf("%1c", '.');  
				}
				else//26 letters in alphabet
				{
					if((int)pathfinding_map_input[k][j]>9)
					{
						if((int)pathfinding_map_input[k][j]<36)//36
						{
							printf("%1c",(int)pathfinding_map_input[k][j]+87);//lower case
						}
						else
						{//is at 36
							printf("%1c",(int)pathfinding_map_input[k][j]+29); //upper case
							//printf("%d",pathfinding_map_input[k][j]);
						} 
					}
					else
					{
						printf("%1d",pathfinding_map_input[k][j]);
					}
				}
			}
		}
		printf("\n");
	}
	return 0;
}

int print_path_through_wall_map(int dungion_hardness_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char pathfinding_wall_map_input[80][21])
{
	int j;
	int k;	
	//int map_x_size = 80; //horizontal
	//int map_y_size = 21; // veritical
	//int *print_char=dungion_hardness_map;
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			if(pathfinding_wall_map_input[k][j]=='~')
			{
				//printf("%2d", '7');
				//printf("%c",pathfinding_map_input[j][k]);
				printf("%1c",' ');
			}
			else
			{
				if((int)pathfinding_wall_map_input[k][j]>61 || (int)pathfinding_wall_map_input[k][j]<0)
				{
					if(dungion_hardness_map[k][j]==0)
					{
						printf("%1c", '.');  
					}
					else
					{
						printf("%1c", ' '); 
					}
				}
				else//26 letters in alphabet
				{
					if((int)pathfinding_wall_map_input[k][j]>9)
					{
						if((int)pathfinding_wall_map_input[k][j]<36)//36
						{
							printf("%1c",(int)pathfinding_wall_map_input[k][j]+87);//lower case
						}
						else
						{//is at 36
							printf("%1c",(int)pathfinding_wall_map_input[k][j]+29); //upper case
							//printf("%d",pathfinding_map_input[k][j]);
						} 
					}
					else
					{
						printf("%1d",pathfinding_wall_map_input[k][j]);
					}
				}
			}
		}
		printf("\n");
	}
	return 0;
}

int load_dungion(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], room room_array_input[6],  int map_x_size, int map_y_size, int number_of_rooms)
{
	/*
	//bytes : info
	//0-5				:	file type marker
	//6-9				:	unsigned 32-bit int; file version marker; value =0
	//10-13			:	unsigned 32-bit int; size of file
	//14-1693		:	 cel by cell dungion hardness
	//						-one byte is cell harness
	//1694-end	:	position of rooms
	//						- 4 unsigned 8-bit integers each
	//						--upperleft corner x position
	//						--width (x demention)
	//						--upperleft corner y position
	//						--height (y demention)
	
	
	//int i,j,k,numOfRooms;
	//char markerString[5]={0x52,0x4c,0x47,0x32,0x32}; //to checck if file has correct name is hex for RLG229
	//unsigned char FileVersionbuffer[4];
	//unsigned char FileSizebuffer[4];
	//unsigned char Cellbuffer[4];
	//
	//map_x_size*map_y_size 1 byte cells
	
	//4 bytes per room tell end
	//
	//unsigned char NumberOfRoomsbuffer[2]; 
	//unsigned char PosOfRoomsbuffer[4];
	
	//5			: file type
	//4 		: version number (0)
	//4 		: size of file
	//1680	: cell by cell hardness
	//size - 1693	: rooms (each has 4bytes)
	
	
	//int i;
	//int j;
	FILE *fp;
  //char data_to_write[] = "this is tutorialspoint";
  //int size= 5+4+4+1680+(number_of_rooms*4);
 // char data_to_write[size];
 
  char file_name[]="0";
  //char file_name[]="$HOME/.rlg229/dungeon";
  char file_ending[]=".rlg327";
  strcat(file_name, file_ending);
   
   

   

   // Open file for both reading and writing 
  // fp = fopen(file_name, "w+");
   fp = fopen("test.txt", "w+");

   // Write data to the file 
  // fwrite(data_to_write, strlen(data_to_write) + 1, 1, fp);

   // Seek to the beginning of the file 
   fseek(fp, SEEK_SET, 0);

   //Read and display data /
   
   char *buffer;
   buffer= malloc(3000);
   
   fread(buffer, 3000, 1, fp);
   printf("%s\n", buffer);
   fclose(fp);

   //
  //if(buffer[0]='r' && buffer[1]='l' && buffer[2]='g' && buffer[3]='3' && buffer[4]='2' && buffer[5]='7')
   //{
   //	printf("%c\n", 'y');
   //}
  // else
   //{
   	//printf("%c\n", 'n');
  // }
   
   		
		//-- 4 unsigned 8-bit integers each
		//--upperleft corner x position
		//--width (x demention)
		//--upperleft corner y position
		//--height (y demention)
		
		
		struct room
		{
			int x_pos; //uper left corrner
			int y_pos;
			int x_size;
			int y_size;
		};
	
   //after load into buffer
   
   	data_to_write[0]='r';
	data_to_write[1]='l';
	data_to_write[2]='g';
	data_to_write[3]='3';
	data_to_write[4]='2';
	data_to_write[5]='7';
	data_to_write[6]='0';
	data_to_write[7]='0';
	data_to_write[8]='0';
	data_to_write[9]='0';
	
	
  k=10;
  for(i=0;i<map_x_size;i++)
	{
		for(j=0;j<map_y_size;j++)
		{
			//dungion_hardness_map_input[map_x_size][map_y_size]
			//  
			data_to_write[k]=dungion_hardness_map_input[i][j]+'0';
			k++;
		}
	}
	for(i=0;i<number_of_rooms;i++)
	{

		data_to_write[k]=room_array_input[i].x_pos+'0';
		k++;
		data_to_write[k]=room_array_input[i].y_pos+'0';
		k++;
		data_to_write[k]=room_array_input[i].x_size+'0';
		k++;
		data_to_write[k]=room_array_input[i].y_size+'0';
		k++;
	}
	
   
	//deallocate buffer
	*/
	return 0;
	
}

int save_dungion(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], room room_array_input[6],  int map_x_size, int map_y_size, int number_of_rooms)
{
	/*
		//5			: file type
	//4 		: version number (0)
	//4 		: size of file
	//1680	: cell by cell hardness
	//file size - 1693	: rooms (each has 4bytes)
	
	
	int i;
	int j;
	int k;
	FILE *fp;
	int size= 5+4+4+1680+(number_of_rooms*4);
  char data_to_write[size];
  //char buffer[100];
  //char file_name[]="0";
  
  //char file_ending[]=".rlg327";
  //strcat(file_name, file_ending);
   
	//file is name.rlg327
	data_to_write[0]='r';
	data_to_write[1]='l';
	data_to_write[2]='g';
	data_to_write[3]='3';
	data_to_write[4]='2';
	data_to_write[5]='7';
	data_to_write[6]='0';
	data_to_write[7]='0';
	data_to_write[8]='0';
	data_to_write[9]='0';
	
	
  k=10;
  for(i=0;i<map_x_size;i++)
	{
		for(j=0;j<map_y_size;j++)
		{
			//dungion_hardness_map_input[map_x_size][map_y_size]
			//  
			data_to_write[k]=dungion_hardness_map_input[i][j]+'0';
			k++;
		}
	}
	for(i=0;i<number_of_rooms;i++)
	{
		
		-- 4 unsigned 8-bit integers each
		--upperleft corner x position
		--width (x demention)
		--upperleft corner y position
		--height (y demention)
		
		struct room
		{
			int x_pos; //uper left corrner
			int y_pos;
			int x_size;
			int y_size;
		};
		
		data_to_write[k]=room_array_input[i].x_pos+'0';
		k++;
		data_to_write[k]=room_array_input[i].y_pos+'0';
		k++;
		data_to_write[k]=room_array_input[i].x_size+'0';
		k++;
		data_to_write[k]=room_array_input[i].y_size+'0';
		k++;
	}

   // Open file for both reading and writing 
   fp = fopen("test.txt", "w+");

   //Write data to the file 
   fwrite(data_to_write, strlen(data_to_write) + 1, 1, fp);

  // Seek to the beginning of the file 
   //fseek(fp, SEEK_SET, 0);

  // Read and display data 
 	//fread(buffer, strlen(data_to_write)+1, 1, fp);
  //printf("%s\n", buffer);
  
  fclose(fp);

	*/
	return 0;
}

int pathfinding(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], char pathfinding_map_input[80][21], int map_x_size, int map_y_size, int pc_x, int pc_y)
{
	char pathfinding_map[map_x_size][map_y_size];
	if(dungion_hardness_map_input[pc_x][pc_y] !=0)
	{
		//location for this placment is invallid please use the path through walls
		//to player (not writtent yet)
		return 1;
	}
	
	int j;
	int k;
	
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			pathfinding_map[k][j]='~';
	  }
	}
	int nochange;
	nochange=1;
	int loops;
	loops=0;
	pathfinding_map[pc_x][pc_y]=0;
	
	while(nochange!=0 && loops<(map_y_size*map_x_size))
	{
		//no change is to mark if we hcave changed every thing (0 is no change,
		//andything above is how many changes that loop
		nochange=0;
		for(j=0; j<map_y_size; j++)
		{
			for(k=0; k<map_x_size; k++)
			{
				if(pathfinding_map[k][j]!='~')
				{
					if(dungion_hardness_map_input[k][j]==0)
					{
						//change right
						if(dungion_hardness_map_input[k+1][j]==0 && pathfinding_map[k+1][j]=='~')
						{
							pathfinding_map[k+1][j]=((int)pathfinding_map[k][j])+1;
							nochange=1;
						}
						//change left
						if(dungion_hardness_map_input[k-1][j]==0 && pathfinding_map[k-1][j]=='~')
						{
							pathfinding_map[k-1][j]=((int)pathfinding_map[k][j])+1;
							nochange=1;
						}
						//change upper
						if(dungion_hardness_map_input[k][j+1]==0 && pathfinding_map[k][j+1]=='~')
						{
							pathfinding_map[k][j+1]=((int)pathfinding_map[k][j])+1;
							nochange=1;
						}
						//change lower
						if(dungion_hardness_map_input[k][j-1]==0 && pathfinding_map[k][j-1]=='~')
						{
							pathfinding_map[k][j-1]=((int)pathfinding_map[k][j])+1;
							nochange=1;
						}
					}
				}
			}
		}
		loops++;
	}
	
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			pathfinding_map_input[k][j]=pathfinding_map[k][j];
	  }
	}
	return 0;
}

int pathfinding_wall(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], char pathfinding_wall_map_input[80][21], int map_x_size, int map_y_size, int pc_x, int pc_y)
{
	char pathfinding_map_wall[map_x_size][map_y_size];
	if(dungion_hardness_map_input[pc_x][pc_y] !=0)
	{
		//location for this placment is invallid please use the path through walls
		//to player (not writtent yet)
		return 1;
	}
	
	int j;
	int k;
	
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			pathfinding_map_wall[k][j]='~';
	  }
	}
	int nochange;
	nochange=1;
	int loops;
	loops=0;
	pathfinding_map_wall[pc_x][pc_y]=0;
	
	while(nochange!=0 && loops<(map_y_size*map_x_size))
	{
		//no change is to mark if we hcave changed every thing (0 is no change,
		//andything above is how many changes that loop
		nochange=0;
		for(j=0; j<map_y_size; j++)
		{
			for(k=0; k<map_x_size; k++)
			{
				if(pathfinding_map_wall[k][j]!='~')
				{
					if(dungion_hardness_map_input[k][j]!=255)
					{
						//change right
						if(dungion_hardness_map_input[k+1][j]!=255 && pathfinding_map_wall[k+1][j]=='~')
						{
							pathfinding_map_wall[k+1][j]=((int)pathfinding_map_wall[k][j])+1+dungion_hardness_map_input[k+1][j];
							nochange=1;
						}
						//change left
						if(dungion_hardness_map_input[k-1][j]!=255 && pathfinding_map_wall[k-1][j]=='~')
						{
							pathfinding_map_wall[k-1][j]=((int)pathfinding_map_wall[k][j])+1+dungion_hardness_map_input[k-1][j];
							nochange=1;
						}
						//change upper
						if(dungion_hardness_map_input[k][j+1]!=255 && pathfinding_map_wall[k][j+1]=='~')
						{
							pathfinding_map_wall[k][j+1]=((int)pathfinding_map_wall[k][j])+1+dungion_hardness_map_input[k][j+1];
							nochange=1;
						}
						//change lower
						if(dungion_hardness_map_input[k][j-1]!=255 && pathfinding_map_wall[k][j-1]=='~')
						{
							pathfinding_map_wall[k][j-1]=((int)pathfinding_map_wall[k][j])+1+dungion_hardness_map_input[k][j-1];
							nochange=1;
						}
					}
				}
			}
		}
		loops++;
	}
	
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			pathfinding_wall_map_input[k][j]=pathfinding_map_wall[k][j];
	  }
	}
	return 0;
}

int place_monsters(char monster_map_input[80][21], char dungion_map_input[80][21], room room_array_input[6], int map_x_size, int map_y_size, int number_of_rooms, int num_monsters, struct creature creature_array[6])
{
	
	//printf("number of monsters");
	//printf("%d \n", num_monsters-1);
	//idea: i dont need a que, i could loop through monster array to get monsters next turn
	
	//place pc
	creature_array[0].is_pc=0;
  creature_array[0].type="human";//is here if the pc can ever shapeshift
  creature_array[0].name="JefferySchons";//yes the pc name is the name of the programmer
  creature_array[0].specs=3;//unimportant for pc; but we are smart and can see everything, can not tunnal and are not erratic

  creature_array[0].speed=10; //player speed is defult 10
  creature_array[0].icon='@';
  
  creature_array[0].move_time=100/creature_array[0].speed;
  creature_array[0].next_turn=0; //pc goes first
  //set  //defult is the upperleft corner of room 0
  creature_array[0].x_pos=room_array_input[0].x_pos+1;
  creature_array[0].y_pos=room_array_input[0].y_pos+1;
  
  creature_array[0].alive=0;//now that are placed come alive   
	
	int i;
	
	//monster_map_input[creature_array[0].x_pos][creature_array[0].y_pos]=creature_array[0].icon;
	//make monsters (hard)
	//start make that many that do not nove 
	//random_direction = (rand() %16); //random number between 0 and 15 
	//one needs to be tunnaling, one telepathic
	
	int tries;
	tries=0;
	int rand_x;
	int rand_y;
	int rand_speed;
	int rand_specs;
	int rand_char_type;//for now is just the symble
	//reset all monsters to blank
	for(i=1; i<num_monsters; i++)
	{
		creature_array[i].x_pos=0;
		creature_array[i].y_pos=0;
		creature_array[i].alive=1;//defult dead
		
    creature_array[i].is_pc=1;
    creature_array[i].type="generic";
    creature_array[i].name="moster";
    
    creature_array[i].speed=0;
    creature_array[i].move_time=0;
    creature_array[i].next_turn=0;
    
   	creature_array[i].specs=0;//erratic, tunneling, telepathy, intelligence ; initalize brain dead
    creature_array[i].icon='?';
	}
	
	
	for(i=1; i<num_monsters; i++)
	{
		if(creature_array[i].alive!=0) //if monster is alive it is placed
		{
			
			//to see if there is opening
			tries=0;
			while(tries<32)//I try to place it 32 times before giving up on this monster and it is DOD (dead on Delivery)
			{
				
				rand_x = (rand() %80); //+1 is so it wont have anything on row/columb 0
  			rand_y = (rand() %21);
  			
  			
  			if(dungion_map_input[rand_x][rand_y]!='+' && dungion_map_input[rand_x][rand_y]!='<')
  			{
  				if(monster_map_input[rand_x][rand_y]=='#')
  				{
	  				creature_array[i].x_pos=rand_x;
						creature_array[i].y_pos=rand_y;
						creature_array[i].alive=0;
						
						//set rest of monster
						rand_speed=(rand() %15)+5;
						rand_specs=(rand() %15);
						rand_char_type=(rand() %4);
						
				    creature_array[i].is_pc=1;
				    //creature_array[i].type="generic";
				    creature_array[i].name="moster";
				    
				    creature_array[i].speed=rand_speed;
				    creature_array[i].move_time=100/(creature_array[i].speed);
				    creature_array[i].next_turn=0;
				    
				   	creature_array[i].specs=rand_specs;//erratic, tunneling, telepathy, intelligence ; initalize brain dead
				   	
				   	//creature_array[i].icon='?';
				   	//this is to define all randomly made monsters
				   	switch (rand_char_type) {
					    case 1: 
					    	creature_array[i].icon='R';
					    	creature_array[i].type="rat";
					   		break;
					    case 2:
					    	creature_array[i].icon='P';
					    	creature_array[i].type="person";
					    case 3:
					    	creature_array[i].icon='K';
					    	creature_array[i].type="kobald";
					    	break;
					    case 4:
					    	creature_array[i].icon='B';
					    	creature_array[i].type="bandit";
					    	break;
				   		default:
				   			creature_array[i].icon='?';
				   			creature_array[i].type="generic";
					      break;
				   	}
				    
						tries=64;
						//printf("%c", 'd');
  				}
  			}
  			tries++;
			}
			
		}
	}
	//make the monster map  //unimportant exept that the run game sends to print at first then it is overwiten
	
	for(i=0; i<num_monsters; i++)
	{
		if(creature_array[i].alive==0) //if monster is allive
		{
			monster_map_input[creature_array[i].x_pos][creature_array[i].y_pos]=creature_array[i].icon;
		}
	}
	
	return 0;
	
}

int run_game(char monster_map_input[80][21], char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], char pathfinding_map_input[80][21], room room_array_input[6], int map_x_size, int map_y_size, int number_of_rooms, int num_monsters, creature creature_array[6], int line_of_sight_map_input[80][21])
{
	int max_turns;
	max_turns=1000; 
	//monster charactoristics
	//erratic, tunneling, telepathy, intelligence
	//erratic (50% chance to have one of the other traits (if tunneling it still cant move through walls without that trait),
	//		 50% chance move in random direction)
	//tunneling : can move through walls, replaces walls with hallway
	//telepathy : pathfinds with knowing pc position, others move to where pc was last
	//	i will do if not telepathic the PC needs to be in same room
	//intelegence : strait path (no) or pathfinding (yes)

	//if do not know where pc is randon move
	
	//idea: i dont need a que, i could loop through monster array to get monsters next turn
	
	//pc is for now rendom move

	
	int i;
	int k;
	int j;
	int m;
	int var_new_dung;
	int32_t key;//for getting keybord input
	int turn;
	turn=0;
	int random_direction; //for random
	//char* monst_list_print_string;
	
	char object_map[80][21];
	//make object array
	struct object object_list[15];
	char fow_map[80][21];//for the fog of war map done in c++
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			fow_map[k][j]='+';
			object_map[k][j]='*';
	  }
	}
	
	//place ojecrs
	
	k=place_objects(dungion_map_input, map_x_size, map_y_size, object_map, object_list);
	
	initscr();//enter ncurses here
	
	//are to display a list of monsters in playyer input (will be 4)
	int disp_monst_top; 
	disp_monst_top=1;
	
	//start
	//k=print_map(dungion_map_input, map_x_size, map_y_size, number_of_rooms, monster_map_input);
	k=print_fow_map(dungion_map_input, map_x_size, map_y_size, number_of_rooms, monster_map_input, creature_array[0].x_pos, creature_array[0].y_pos, fow_map, object_map);
	

	while(creature_array[0].alive==0 && turn<=max_turns) //the turn limmit is temperary for quick testing
	{
		//move pc
		//print map
		
		// kill/reset monster map
		for(j=0; j<map_y_size; j++)
		{
			for(k=0; k<map_x_size; k++)
			{
				monster_map_input[k][j]='#';
		  }
		}
		
		for(i=0; i<num_monsters; i++)
		{
			if(creature_array[i].alive==0) //if monster is allive
			{
				if(creature_array[i].next_turn==turn)
				{
					random_direction = (rand() %5); //random number between 0 and 3 for directions and no move
		
					//  1
					// 342
					//  0   pos y
					
					//PC
					
					if (creature_array[i].is_pc==0)
					{
						//old code to randomly move pc
						/*
						if(random_direction==0) //pos x, down
						{
							if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
							{
								creature_array[i].x_pos=creature_array[i].x_pos+1;
								creature_array[i].y_pos=creature_array[i].y_pos;
							}
						}
						else if(random_direction==1) 
						{
							if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
							{
								creature_array[i].x_pos=creature_array[i].x_pos-1;
								creature_array[i].y_pos=creature_array[i].y_pos;
							}
						}
						else if(random_direction==2) 
						{
							if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
							{
								creature_array[i].x_pos=creature_array[i].x_pos;
								creature_array[i].y_pos=creature_array[i].y_pos+1;
							}
						}
						else if(random_direction==3) 
						{
							if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
							{
								creature_array[i].x_pos=creature_array[i].x_pos;
								creature_array[i].y_pos=creature_array[i].y_pos-1;
							}
						}
						else //if(random_direction==4) 
						{
							//do nothing
						}
						*/
						
						//get keyboard input
						key = getch();
				    switch (key) {
				    	//every thing that teachure uses is lowercase
				    	//exept for Q (quit)
				    	//		y-1
				    	//x-1			x+1
				    	//		y+1
					  	//KEY_B2 is 5 on keypad
					    //quit game
					    case 'Q': case 'q':
					    	//quit game
					    	endwin();
					    	return 1;
					      break;
					    
					    //direction
					    case '8': case 'k':
					    	//up
					    	if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
								{
									creature_array[i].x_pos=creature_array[i].x_pos;
									creature_array[i].y_pos=creature_array[i].y_pos-1;
									if(object_map[creature_array[i].x_pos][creature_array[i].y_pos-1]!='*')
									{
										object_map[creature_array[i].x_pos][creature_array[i].y_pos-1]='*';
									}
								}
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					    	break;
					   	case '9': case 'u': case KEY_A3:
					   		//upper right
					   		if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos-1]!='+')
								{
									creature_array[i].x_pos=creature_array[i].x_pos+1;
									creature_array[i].y_pos=creature_array[i].y_pos-1;
									if(object_map[creature_array[i].x_pos+1][creature_array[i].y_pos-1]!='*')
									{
										object_map[creature_array[i].x_pos+1][creature_array[i].y_pos-1]='*';
									}
								}
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					   		break;
					   	case '6': case 'l':
					   		//right
						    if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
								{
									creature_array[i].x_pos=creature_array[i].x_pos+1;
									creature_array[i].y_pos=creature_array[i].y_pos;
									if(object_map[creature_array[i].x_pos+1][creature_array[i].y_pos]!='*')
									{
										object_map[creature_array[i].x_pos+1][creature_array[i].y_pos]='*';
									}
								}
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					   		break;
					    case '3': case 'n': case KEY_C3:
					    	//down right
					    	if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos+1]!='+')
								{
									creature_array[i].x_pos=creature_array[i].x_pos+1;
									creature_array[i].y_pos=creature_array[i].y_pos+1;
									if(object_map[creature_array[i].x_pos+1][creature_array[i].y_pos+1]!='*')
									{
										object_map[creature_array[i].x_pos+1][creature_array[i].y_pos+1]='*';
									}
								}
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
								break;
					    case '2': case 'j':
					    	//down
					    	if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
								{
									creature_array[i].x_pos=creature_array[i].x_pos;
									creature_array[i].y_pos=creature_array[i].y_pos+1;
									if(object_map[creature_array[i].x_pos][creature_array[i].y_pos+1]!='*')
									{
										object_map[creature_array[i].x_pos][creature_array[i].y_pos+1]='*';
									}
								}
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					    	break;
					    case '1': case 'b': case KEY_C1:
					    	//down left
						    if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos+1]!='+')
								{
									creature_array[i].x_pos=creature_array[i].x_pos-1;
									creature_array[i].y_pos=creature_array[i].y_pos+1;
									if(object_map[creature_array[i].x_pos-1][creature_array[i].y_pos+1]!='*')
									{
										object_map[creature_array[i].x_pos-1][creature_array[i].y_pos+1]='*';
									}
								}
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					    	break;
					    case '4': case 'h':
					    	//left
						    if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
								{
									creature_array[i].x_pos=creature_array[i].x_pos-1;
									creature_array[i].y_pos=creature_array[i].y_pos;
									if(object_map[creature_array[i].x_pos-1][creature_array[i].y_pos]!='*')
									{
										object_map[creature_array[i].x_pos-1][creature_array[i].y_pos]='*';
									}
								}
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					    	break;
					    case '7': case 'y': case KEY_A1:
					    	//up left
					    	if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos-1]!='+')
								{
									creature_array[i].x_pos=creature_array[i].x_pos-1;
									creature_array[i].y_pos=creature_array[i].y_pos-1;
									if(object_map[creature_array[i].x_pos-1][creature_array[i].y_pos-1]!='*')
									{
										object_map[creature_array[i].x_pos-1][creature_array[i].y_pos-1]='*';
									}
								}
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					    	break;
					    case '5': case ' ':
					    	//do not move
					    	creature_array[i].x_pos=creature_array[i].x_pos;
								creature_array[i].y_pos=creature_array[i].y_pos;
								creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					    	break;
					   
					    //up down stares
					    case '<': case '>':
					    	if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]=='<' || dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]=='>')
								{
						    	
						    	//make preset
						    	for(j=0; j<map_y_size; j++)
									{
										for(k=0; k<map_x_size; k++)
										{
											dungion_map_input[k][j]='+';
											//note 255 is max hardness
											dungion_hardness_map_input[k][j]=1;
											monster_map_input[k][j]='#';
									  }
									}
						    	
						    	var_new_dung=make_dungeon_map(dungion_map_input, dungion_hardness_map_input, room_array_input, map_x_size, map_y_size, number_of_rooms);
									if(var_new_dung!=1)
									{
										var_new_dung=0;
										//both makes monsters, adds to que, and add to map
										//this will overwrite the current monster array
										var_new_dung=place_monsters(monster_map_input, dungion_map_input, room_array_input, map_x_size, map_y_size, number_of_rooms, num_monsters, creature_array);
										
										//overwrite the old dungion
									
										//reset turn to 0 so i dont have to reset all the monsters
										turn=0;
										i=0;
										creature_array[i].next_turn=1; //so the PC imediatley goes
									}
								}
								else
								{
									i=0;
					    		turn--;
								}
								refresh();
								break;
					    
					    //monster list things
					    case 'm':
					    	//will list out as ex:? generic 2 north 4 east
					    	//redisplay
					    	if((disp_monst_top+4)<(num_monsters+1))
					    	{
					    		for(m=disp_monst_top; m<=num_monsters; m++)
					    		{
					    			//use strcat()
					    			//strcat(monst_list_print_string, creature_array[i].icon);
					    			//strcat(monst_list_print_string, creature_array[i].type);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].x_pos);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].y_pos);
					    			//mvprintw(5, m, monst_list_print_string);
					    			
					    			mvaddch(5, m, creature_array[i].icon);
					    			//mvaddch(6, m, creature_array[i].type);
					    			mvaddch(7, m, 'e');
					    			mvaddch(8, m, creature_array[i].x_pos);
					    			mvaddch(9, m, 'n');
					    			mvaddch(10, m, creature_array[i].y_pos);
					    			
					    		}
					    	}
					    	else
					    	{
					    		for(m=disp_monst_top; m<=(disp_monst_top+4); m++)
					    		{
					    			//strcat(monst_list_print_string, creature_array[i].icon);
					    			//strcat(monst_list_print_string, creature_array[i].type);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].x_pos);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].y_pos);
					    			//mvprintw(5, m, monst_list_print_string);
					    			
					    			mvaddch(5, m, creature_array[i].icon);
					    			//mvaddch(6, m, creature_array[i].type);
					    			mvaddch(7, m, 'e');
					    			mvaddch(8, m, creature_array[i].x_pos);
					    			mvaddch(9, m, 'n');
					    			mvaddch(10, m, creature_array[i].y_pos);
					    		}
					    	}
					    	i=0;
					    	turn--;
					      break;
					    case KEY_DOWN:
					    	disp_monst_top++;
					    	if(disp_monst_top==(num_monsters+1))
					    	{
					    		disp_monst_top=num_monsters;
					    	}
					    	//redisplay
					    	if((disp_monst_top+4)<(num_monsters+1))
					    	{
					    		for(m=disp_monst_top; m<=num_monsters; m++)
					    		{
					    			//strcat(monst_list_print_string, creature_array[i].icon);
					    			//strcat(monst_list_print_string, creature_array[i].type);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].x_pos);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].y_pos);
					    			//mvprintw(5, m, monst_list_print_string);
					    			
					    			mvaddch(5, m, creature_array[i].icon);
					    			//mvaddch(6, m, creature_array[i].type);
					    			mvaddch(7, m, 'e');
					    			mvaddch(8, m, creature_array[i].x_pos);
					    			mvaddch(9, m, 'n');
					    			mvaddch(10, m, creature_array[i].y_pos);
					    		}
					    	}
					    	else
					    	{
					    		for(m=disp_monst_top; m<=(disp_monst_top+4); m++)
					    		{
					    			//strcat(monst_list_print_string, creature_array[i].icon);
					    			//strcat(monst_list_print_string, creature_array[i].type);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].x_pos);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].y_pos);
					    			//mvprintw(m, 5, monst_list_print_string);
					    			
					    			mvaddch(m, 5, creature_array[i].icon);
					    			//mvaddch(m, 6, creature_array[i].type);
					    			mvaddch(m, 7, 'e');
					    			mvaddch(m, 8, creature_array[i].x_pos);
					    			mvaddch(m, 9, 'n');
					    			mvaddch(m, 10, creature_array[i].y_pos);
					    		}
					    	}
					    	i=0;
					    	turn--;
					      break;
					    case KEY_UP:
					    	disp_monst_top--;
					    	if(disp_monst_top==0)
					    	{
					    		disp_monst_top=1;
					    	}
					    	//redisplay
					    		if((disp_monst_top+4)<(num_monsters+1))
					    	{
					    		for(m=disp_monst_top; m<=num_monsters; m++)
					    		{
					    			//strcat(monst_list_print_string, creature_array[i].icon);
					    			//strcat(monst_list_print_string, creature_array[i].type);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].x_pos);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].y_pos);
					    			//mvprintw(5, m, monst_list_print_string);
					    			
					    			mvaddch(5, m, creature_array[i].icon);
					    			//mvaddch(6, m, creature_array[i].type);
					    			mvaddch(7, m, 'e');
					    			mvaddch(8, m, creature_array[i].x_pos);
					    			mvaddch(9, m, 'n');
					    			mvaddch(10, m, creature_array[i].y_pos);
					    		}
					    	}
					    	else
					    	{
					    		for(m=disp_monst_top; m<=(disp_monst_top+4); m++)
					    		{
					    			//strcat(monst_list_print_string, creature_array[i].icon);
					    			//strcat(monst_list_print_string, creature_array[i].type);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].x_pos);
					    			//sprintf(monst_list_print_string,"%d",creature_array[i].y_pos);
					    			//mvprintw(5, m, monst_list_print_string);
					    			
					    			mvaddch(5, m, creature_array[i].icon);
					    			//mvaddch(6, m, creature_array[i].type);
					    			mvaddch(7, m, 'e');
					    			mvaddch(8, m, creature_array[i].x_pos);
					    			mvaddch(9, m, 'n');
					    			mvaddch(10, m, creature_array[i].y_pos);
					    		}
					    	}
					    	i=0;
					    	turn--;
					      break;
					      
					    case 0x1b: case 'c':
					    	i=0;
					    	turn--;
					    	refresh();
					    	//leave monster map
					    	//should be escape
					      break;
					    
					    //defult (random letter hit)
					    default:
					    	i=0;
					    	turn--;
					    	//no-op (no operation)
					    	//print value of key
					      break;
						}
						//ckeck if killed a monster
						for(k=0; k<=num_monsters; k++)
						{
							if(creature_array[i].x_pos == creature_array[k].x_pos)
							{
								if(creature_array[i].y_pos == creature_array[k].y_pos)
								{
									if(i!=k)
									{
										creature_array[k].alive=1;
									}
								}
							}
						}
						//creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
					}
					else
					{
						//THE MONSTER MOVEMENT
						//creature_array[i].x_pos=creature_array[i].x_pos;
						//creature_array[i].y_pos=creature_array[i].y_pos;
						
						//to note both pathfinding use the same map (would have been smarter to have them use seperate)
						int pc_spoted;
						//for specs
						//erratic, tunneler, telapathic, intellignet
						int path_line;
						int erratic=0;
						int tunneler=0;
						int telapathic=0;
						int intellignet=0;
						
						//creature_array[i].specs;
						if(creature_array[i].specs % 2)
						{//intellignet is all odd
							intellignet=1;
						}
						if(creature_array[i].specs==2 || creature_array[i].specs==3 || creature_array[i].specs==6 || creature_array[i].specs==7 || creature_array[i].specs==10 || creature_array[i].specs==11 || creature_array[i].specs==14 || creature_array[i].specs==15)
						{
							telapathic=1;
						}
						if(creature_array[i].specs==4 || creature_array[i].specs==5 || creature_array[i].specs==6 || creature_array[i].specs==7 || creature_array[i].specs==12 || creature_array[i].specs==13 || creature_array[i].specs==14 || creature_array[i].specs==15)
						{
							tunneler=1; 
						}
						if(8 < creature_array[i].specs  && creature_array[i].specs < 16)
						{
							erratic=1;
						}
						
						int do_erratic = (rand() %2);
						//get direction from monster to player
						if(abs(creature_array[0].y_pos-creature_array[i].y_pos)>abs(creature_array[0].x_pos-creature_array[i].x_pos))
						{
							if((creature_array[0].y_pos-creature_array[i].y_pos)>0)
							{
								//pc is to right
								path_line=3;
							}
							else
							{
								//pc is to left
								path_line=4;
							}
						}
						else //its up/down
						{
							if((creature_array[0].x_pos-creature_array[i].x_pos)>0)
							{
								//pc is up
								path_line=1;
							}
							else
							{
								//pc is down
								path_line=2;
							}
						}
						
						if(erratic==1)
						{
							if(do_erratic==0)
							{
								//random movement
								if(random_direction==0) //pos x, down
								{
									if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+' && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
									{
										creature_array[i].x_pos=creature_array[i].x_pos+1;
										creature_array[i].y_pos=creature_array[i].y_pos;
									}
								}
								else if(random_direction==1) 
								{
									if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+' && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
									{
										creature_array[i].x_pos=creature_array[i].x_pos-1;
										creature_array[i].y_pos=creature_array[i].y_pos;
									}
								}
								else if(random_direction==2) 
								{
									if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
									{
										creature_array[i].x_pos=creature_array[i].x_pos;
										creature_array[i].y_pos=creature_array[i].y_pos+1;
									}
								}
								else if(random_direction==3) 
								{
									if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
									{
										creature_array[i].x_pos=creature_array[i].x_pos;
										creature_array[i].y_pos=creature_array[i].y_pos-1;
									}
								}
								else
								{
									//is motionless
								}
							}
							else
							{
								if(telapathic==1) //p
								{
									if(intellignet==1) //p i
									{
										if(tunneler==1) //p i t
										{
											//will tunnel using pathfinding to PC
											k=pathfinding_wall(dungion_map_input, dungion_hardness_map_input, pathfinding_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
										
											//get lowist of surrounding
											
											//damage wall
											//if wall destroyed then move
											//can distroy and move same turn
											
											int low=100;//way above posible
											
											if(pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos])
											{
												//to far away
											}
											else//x+1
											{
												low=1;
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos])
											{
												//to far away
											}
											else//x-1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<low)
												{
													low=2;
												}
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1])
											{
												//to far away
											}
											else//y+1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<low)
												{
													low=3;
												}
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1])
											{
												//to far away
											}
											else//y-1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<low)
												{
													low=4;
												}
											}
											
											//direction found now move
											if(low!=100)
											{
												if(low==1) //x+1
												{
													if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
													}
												}
												else if(low==2)//x-1
												{
													if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
													}
												}
												else if(low==3)//y+1
												{
													if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
													}
												}
												else if(low==4)//y-1
												{
													if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
													}
												}
												else
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else
											{
												//random movement
												if(random_direction==0) //pos x, down
												{
													if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+' && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
													}
												}
												else if(random_direction==1) 
												{
													if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+' && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
													}
												}
												else if(random_direction==2) 
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
													}
												}
												else if(random_direction==3) 
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
													}
												}
												else //if(random_direction==4) 
												{
													//do nothing
												}
											}
										}
										else //p i n
										{
											//will use pathfinding to get to PC
											k=pathfinding(dungion_map_input, dungion_hardness_map_input, pathfinding_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
										
											//get lowist of surrounding
											
											int low=100;//way above posible
											
											if(pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos])
											{
												//to far away
											}
											else//x+1
											{
												low=1;
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos])
											{
												//to far away
											}
											else//x-1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<low)
												{
													low=2;
												}
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1])
											{
												//to far away
											}
											else//y+1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<low)
												{
													low=3;
												}
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1])
											{
												//to far away
											}
											else//y-1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<low)
												{
													low=4;
												}
											}
											
											//direction found now move
											if(low!=100)
											{
												if(low==1  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] == '+'  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //x+1
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
												else if(low==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] == '+' && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')//x-1
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
												else if(low==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] == '+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')//y+1
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
												}
												else if(low==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] == '+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')//y-1
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
												}
												else
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else
											{
												//random movement
												if(random_direction==0) //pos x, down
												{
													if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+' && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
												else if(random_direction==1) 
												{
													if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+' && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
												else if(random_direction==2) 
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
													}
												}
												else if(random_direction==3) 
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
													}
												}
												else //if(random_direction==4) 
												{
													//do nothing
												}
											}
										
										}
									}
									else
									{
										if(tunneler==1) //p n t
										{
											//streight line through walls to PC
											//add in stuff for wall
											if(path_line==1  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
											{
												
												if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
												else if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
												}
											}
											else if(path_line==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
												else if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
												}
											}
											else if(path_line==3  && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
												}
												else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
												}
											}
											else if(path_line==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
												}
												else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
												}
											}
											else
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
										}
										else //p n n
										{
											//stright line tword pc
											//will run into walls and get "stuck"
											if(path_line==1 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else if(path_line==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else if(path_line==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
												}
											}
											else if(path_line==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
												}
											}
											else
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
										}
									}
								}								
								else //not telopathic
								{
									
									pc_spoted=1;
									//los
									k=line_of_sight(dungion_map_input, dungion_hardness_map_input, line_of_sight_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
									//if(line_of_sight_map_input
									//if x/y location of monster is a int in los map do that, else random move
									if(line_of_sight_map_input[creature_array[i].x_pos][creature_array[i].y_pos] ==0)
									{
										pc_spoted=0;
									}
									pc_spoted=1;
									if(pc_spoted==1)//random wondering pc not spoted
									{
										if(tunneler==1)
										{
											if(random_direction==0  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
											{
												if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
												}
											}
											else if(random_direction==1  && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
											{
												if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
												}
											}
											else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
											{
												if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
												}
											}
											else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
											{
												if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
												}
											}
											else //if(random_direction==4) 
											{
												//do nothing
											}
										}
										else
										{
											if(random_direction==0 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
											{
												if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else if(random_direction==1 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
												}
											}
											else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
												}
											}
											else //if(random_direction==4) 
											{
												//do nothing
											}
										}
									}
									else//not telapathic (line of sight)
									{
										if(intellignet==1) //n i
										{
											if(tunneler==1) //n i t
											{
												//will tunnel using pathfinding to PC
												k=pathfinding_wall(dungion_map_input, dungion_hardness_map_input, pathfinding_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
											
												//get lowist of surrounding
												
												//damage wall
												//if wall destroyed then move
												//can distroy and move same turn
												
												int low=100;//way above posible
												
												if(pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos])
												{
													//to far away
												}
												else//x+1
												{
													low=1;
												}
												
												if(pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos])
												{
													//to far away
												}
												else//x-1
												{
													if((int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<low)
													{
														low=2;
													}
												}
												
												if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1])
												{
													//to far away
												}
												else//y+1
												{
													if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<low)
													{
														low=3;
													}
												}
												
												if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1])
												{
													//to far away
												}
												else//y-1
												{
													if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<low)
													{
														low=4;
													}
												}
												
												//direction found now move
												if(low!=100)
												{
													if(low==1  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //x+1
													{
														if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
														{
															creature_array[i].x_pos=creature_array[i].x_pos+1;
															creature_array[i].y_pos=creature_array[i].y_pos;
															
															dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
															dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
														}
													}
													else if(low==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')//x-1
													{
														if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
														{
															creature_array[i].x_pos=creature_array[i].x_pos-1;
															creature_array[i].y_pos=creature_array[i].y_pos;
															
															dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
															dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
														}
													}
													else if(low==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')//y+1
													{
														if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
														{
															creature_array[i].x_pos=creature_array[i].x_pos;
															creature_array[i].y_pos=creature_array[i].y_pos+1;
															
															dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
															dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
														}
													}
													else if(low==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')//y-1
													{
														if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
														{
															creature_array[i].x_pos=creature_array[i].x_pos;
															creature_array[i].y_pos=creature_array[i].y_pos-1;
															
															dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
															dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
														}
													}
													else
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
												else
												{
													//random movement
													if(random_direction==0 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
													{
														if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
														{
															creature_array[i].x_pos=creature_array[i].x_pos+1;
															creature_array[i].y_pos=creature_array[i].y_pos;
															
															dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
															dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
														}
													}
													else if(random_direction==1 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
													{
														if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
														{
															creature_array[i].x_pos=creature_array[i].x_pos-1;
															creature_array[i].y_pos=creature_array[i].y_pos;
															
															dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
															dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
														}
													}
													else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
													{
														if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
														{
															creature_array[i].x_pos=creature_array[i].x_pos;
															creature_array[i].y_pos=creature_array[i].y_pos+1;
															
															dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
															dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
														}
													}
													else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
													{
														if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
														{
															creature_array[i].x_pos=creature_array[i].x_pos;
															creature_array[i].y_pos=creature_array[i].y_pos-1;
															
															dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
															dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
														}
													}
													else //if(random_direction==4) 
													{
														//do nothing
													}
												}
											}
											else //n i n
											{
												//will use pathfinding to get to PC
												k=pathfinding(dungion_map_input, dungion_hardness_map_input, pathfinding_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
											
												//get lowist of surrounding
												
												int low=100;//way above posible
												
												if(pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos])
												{
													//to far away
												}
												else//x+1
												{
													low=1;
												}
												
												if(pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos])
												{
													//to far away
												}
												else//x-1
												{
													if((int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<low)
													{
														low=2;
													}
												}
												
												if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1])
												{
													//to far away
												}
												else//y+1
												{
													if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<low)
													{
														low=3;
													}
												}
												
												if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1])
												{
													//to far away
												}
												else//y-1
												{
													if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<low)
													{
														low=4;
													}
												}
												
												//direction found now move
												if(low!=100)
												{
													if(low==1  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] == '+'  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //x+1
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
													else if(low==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] == '+' && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')//x-1
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
													else if(low==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] == '+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')//y+1
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
													}
													else if(low==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] == '+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')//y-1
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
													}
													else
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
												else
												{
													//random movement
													if(random_direction==0) //pos x, down
													{
														if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+' && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
														{
															creature_array[i].x_pos=creature_array[i].x_pos+1;
															creature_array[i].y_pos=creature_array[i].y_pos;
														}
													}
													else if(random_direction==1) 
													{
														if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+' && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
														{
															creature_array[i].x_pos=creature_array[i].x_pos-1;
															creature_array[i].y_pos=creature_array[i].y_pos;
														}
													}
													else if(random_direction==2) 
													{
														if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
														{
															creature_array[i].x_pos=creature_array[i].x_pos;
															creature_array[i].y_pos=creature_array[i].y_pos+1;
														}
													}
													else if(random_direction==3) 
													{
														if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
														{
															creature_array[i].x_pos=creature_array[i].x_pos;
															creature_array[i].y_pos=creature_array[i].y_pos-1;
														}
													}
													else //if(random_direction==4) 
													{
														//do nothing
													}
												}
											
											}
										}
										else
										{
											if(tunneler==1) //n n t
											{
												//streight line through walls to PC
												//add in stuff for wall
												if(path_line==1 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
												{
													
													if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
													else if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
													}
												}
												else if(path_line==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
												{
													if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
													else if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
													}
												}
												else if(path_line==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
													}
													else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
													}
												}
												else if(path_line==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
													}
													else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
													}
												}
												else
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
											}
											else //n n n
											{
												//stright line tword pc
												//will run into walls and get "stuck"
												if(path_line==1 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
												{
													if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
												else if(path_line==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
												{
													if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
												else if(path_line==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
													}
												}
												else if(path_line==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
													}
												}
												else
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
											}
										}
									}
								}
							}
						}
						else//is not erratic
						{
							if(telapathic==1) //p
							{
								if(intellignet==1) //p i
								{
									if(tunneler==1) //p i t
									{
										//will tunnel using pathfinding to PC
										k=pathfinding_wall(dungion_map_input, dungion_hardness_map_input, pathfinding_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
									
										//get lowist of surrounding
										
										//damage wall
										//if wall destroyed then move
										//can distroy and move same turn
										
										int low=100;//way above posible
										
										if(pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos])
										{
											//to far away
										}
										else//x+1
										{
											low=1;
										}
										
										if(pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos])
										{
											//to far away
										}
										else//x-1
										{
											if((int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<low)
											{
												low=2;
											}
										}
										
										if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1])
										{
											//to far away
										}
										else//y+1
										{
											if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<low)
											{
												low=3;
											}
										}
										
										if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1])
										{
											//to far away
										}
										else//y-1
										{
											if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<low)
											{
												low=4;
											}
										}
										
										//direction found now move
										if(low!=100)
										{
											if(low==1  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //x+1
											{
												if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
												}
											}
											else if(low==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')//x-1
											{
												if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
												}
											}
											else if(low==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')//y+1
											{
												if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
												}
											}
											else if(low==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')//y-1
											{
												if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
												}
											}
											else
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
										}
										else
										{
											//random movement
											if(random_direction==0 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
											{
												if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
												}
											}
											else if(random_direction==1 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
												}
											}
											else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
												}
											}
											else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
												}
											}
											else //if(random_direction==4) 
											{
												//do nothing
											}
										}
									}
									else //p i n
									{
										//will use pathfinding to get to PC
										k=pathfinding(dungion_map_input, dungion_hardness_map_input, pathfinding_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
									
										//get lowist of surrounding
										
										int low=100;//way above posible
										
										if(pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos])
										{
											//to far away
										}
										else//x+1
										{
											low=1;
										}
										
										if(pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos])
										{
											//to far away
										}
										else//x-1
										{
											if((int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<low)
											{
												low=2;
											}
										}
										
										if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1])
										{
											//to far away
										}
										else//y+1
										{
											if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<low)
											{
												low=3;
											}
										}
										
										if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1])
										{
											//to far away
										}
										else//y-1
										{
											if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<low)
											{
												low=4;
											}
										}
										
										//direction found now move
										if(low!=100)
										{
											if(low==1  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] == '+'  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //x+1
											{
												creature_array[i].x_pos=creature_array[i].x_pos+1;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
											else if(low==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] == '+' && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')//x-1
											{
												creature_array[i].x_pos=creature_array[i].x_pos-1;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
											else if(low==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] == '+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')//y+1
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos+1;
											}
											else if(low==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] == '+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')//y-1
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos-1;
											}
											else
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
										}
										else
										{
											//random movement
											if(random_direction==0 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
											{
												if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else if(random_direction==1 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
												}
											}
											else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
												}
											}
											else //if(random_direction==4) 
											{
												//do nothing
											}
										}
									
									}
								}
								else
								{
									if(tunneler==1) //p n t
									{
										//streight line through walls to PC
										//add in stuff for wall
										if(path_line==1 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
										{
											
											if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos+1;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
											else if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
											{
												creature_array[i].x_pos=creature_array[i].x_pos+1;
												creature_array[i].y_pos=creature_array[i].y_pos;
												
												dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
												dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
											}
										}
										else if(path_line==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
										{
											if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos-1;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
											else if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
											{
												creature_array[i].x_pos=creature_array[i].x_pos-1;
												creature_array[i].y_pos=creature_array[i].y_pos;
												
												dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
												dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
											}
										}
										else if(path_line==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
										{
											if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos+1;
											}
											else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos+1;
												
												dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
												dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
											}
										}
										else if(path_line==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
										{
											if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos-1;
											}
											else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos-1;
												
												dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
												dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
											}
										}
										else
										{
											if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
										}
									}
									else //p n n
									{
										//stright line tword pc
										//will run into walls and get "stuck"
										if(path_line==1 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
										{
											if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos+1;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
										}
										else if(path_line==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
										{
											if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos-1;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
										}
										else if(path_line==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
										{
											if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos+1;
											}
										}
										else if(path_line==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
										{
											if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos-1;
											}
										}
										else
										{
											if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
										}
									}
								}
							}
							
							else //not telopathic
							{
								
								pc_spoted=1;
								//los
								k=line_of_sight(dungion_map_input, dungion_hardness_map_input, line_of_sight_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
								//if(line_of_sight_map_input
								//if x/y location of monster is a int in los map do that, else random move
								if(line_of_sight_map_input[creature_array[i].x_pos][creature_array[i].y_pos] ==0)
								{
									pc_spoted=0;
								}
								pc_spoted=1;
								if(pc_spoted==1)//random wondering pc not spoted
								{
									if(tunneler==1)
									{
										if(random_direction==0 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
										{
											if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
											{
												creature_array[i].x_pos=creature_array[i].x_pos+1;
												creature_array[i].y_pos=creature_array[i].y_pos;
												
												dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
												dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
											}
										}
										else if(random_direction==1 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
										{
											if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
											{
												creature_array[i].x_pos=creature_array[i].x_pos-1;
												creature_array[i].y_pos=creature_array[i].y_pos;
												
												dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
												dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
											}
										}
										else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
										{
											if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos+1;
												
												dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
												dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
											}
										}
										else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
										{
											if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos-1;
												
												dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
												dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
											}
										}
										else //if(random_direction==4) 
										{
											//do nothing
										}
									}
									else
									{
										if(random_direction==0 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
										{
											if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos+1;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
										}
										else if(random_direction==1 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
										{
											if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos-1;
												creature_array[i].y_pos=creature_array[i].y_pos;
											}
										}
										else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
										{
											if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos+1;
											}
										}
										else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
										{
											if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
											{
												creature_array[i].x_pos=creature_array[i].x_pos;
												creature_array[i].y_pos=creature_array[i].y_pos-1;
											}
										}
										else //if(random_direction==4) 
										{
											//do nothing
										}
									}
								}
								else//not telapathic (line of sight)
								{
									if(intellignet==1) //n i
									{
										if(tunneler==1) //n i t
										{
											//will tunnel using pathfinding to PC
											k=pathfinding_wall(dungion_map_input, dungion_hardness_map_input, pathfinding_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
										
											//get lowist of surrounding
											
											//damage wall
											//if wall destroyed then move
											//can distroy and move same turn
											
											int low=100;//way above posible
											
											if(pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos])
											{
												//to far away
											}
											else//x+1
											{
												low=1;
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos])
											{
												//to far away
											}
											else//x-1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<low)
												{
													low=2;
												}
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1])
											{
												//to far away
											}
											else//y+1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<low)
												{
													low=3;
												}
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1])
											{
												//to far away
											}
											else//y-1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<low)
												{
													low=4;
												}
											}
											
											//direction found now move
											if(low!=100)
											{
												if(low==1 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //x+1
												{
													if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
													}
												}
												else if(low==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')//x-1
												{
													if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
													}
												}
												else if(low==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')//y+1
												{
													if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
													}
												}
												else if(low==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')//y-1
												{
													if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
													}
												}
												else
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else
											{
												//random movement
												if(random_direction==0 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
												{
													if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
													}
												}
												else if(random_direction==1 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
												{
													if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
														
														dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
													}
												}
												else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
													}
												}
												else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
														
														dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
														dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
													}
												}
												else //if(random_direction==4) 
												{
													//do nothing
												}
											}
										}
										else //n i n
										{
											//will use pathfinding to get to PC
											k=pathfinding(dungion_map_input, dungion_hardness_map_input, pathfinding_map_input, map_x_size, map_y_size, creature_array[0].x_pos, creature_array[0].y_pos);
										
											//get lowist of surrounding
											
											int low=100;//way above posible
											
											if(pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos])
											{
												//to far away
											}
											else//x+1
											{
												low=1;
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos])
											{
												//to far away
											}
											else//x-1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]<low)
												{
													low=2;
												}
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1])
											{
												//to far away
											}
											else//y+1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]<low)
												{
													low=3;
												}
											}
											
											if(pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=='~' && (int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<=0 && 61<=(int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1])
											{
												//to far away
											}
											else//y-1
											{
												if((int)pathfinding_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]<low)
												{
													low=4;
												}
											}
											
											//direction found now move
											if(low!=100)
											{
												if(low==1  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] == '+'  && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //x+1
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
												else if(low==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] == '+' && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')//x-1
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
												else if(low==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] == '+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')//y+1
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
												}
												else if(low==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] == '+' && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')//y-1
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
												}
												else
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else
											{
												//random movement
												if(random_direction==0 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<') //pos x, down
												{
													if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos+1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
												else if(random_direction==1 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<') 
												{
													if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos-1;
														creature_array[i].y_pos=creature_array[i].y_pos;
													}
												}
												else if(random_direction==2 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<') 
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos+1;
													}
												}
												else if(random_direction==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<') 
												{
													if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
													{
														creature_array[i].x_pos=creature_array[i].x_pos;
														creature_array[i].y_pos=creature_array[i].y_pos-1;
													}
												}
												else //if(random_direction==4) 
												{
													//do nothing
												}
											}
										
										}
									}
									else
									{
										if(tunneler==1) //n n t
										{
											//streight line through walls to PC
											//add in stuff for wall
											if(path_line==1 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
											{
												
												if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
												else if(dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]=0;
												}
											}
											else if(path_line==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
												else if(dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
													
													dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]=0;
												}
											}
											else if(path_line==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
												}
												else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]=0;
												}
											}
											else if(path_line==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
												}
												else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!=255)
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
													
													dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1] = '#';
													dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]=0;
												}
											}
											else
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
										}
										else //n n n
										{
											//stright line tword pc
											//will run into walls and get "stuck"
											if(path_line==1 && dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos+1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos+1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else if(path_line==2 && dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos-1][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos-1;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
											else if(path_line==3 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos+1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos+1;
												}
											}
											else if(path_line==4 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='<')
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos-1]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos-1;
												}
											}
											else
											{
												if(dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='+')
												{
													creature_array[i].x_pos=creature_array[i].x_pos;
													creature_array[i].y_pos=creature_array[i].y_pos;
												}
											}
										}
									}
								}
							}
						}
						
						//if monster is erratic or not
						//	then is it telepathic
						//		then if intellignet
						//			then can tunnel
						for(k=0; k<=num_monsters; k++)
						{
							if(creature_array[i].x_pos == creature_array[k].x_pos)
							{
								if(creature_array[i].y_pos == creature_array[k].y_pos)
								{
									if(i!=k)
									{
										creature_array[k].alive=1;
									}
								}
							}
						}
						
						creature_array[i].next_turn=creature_array[i].next_turn + creature_array[i].move_time;
						
					  
						//increace turn 
					}
				}
			}
		}
		
		usleep(1000);
		for(i=0; i<num_monsters; i++)
		{
			if(creature_array[i].alive==0) //if monster is allive
			{
				monster_map_input[creature_array[i].x_pos][creature_array[i].y_pos]=creature_array[i].icon;
			}
		}
		//loop through 
		if(turn == creature_array[0].next_turn - creature_array[0].move_time)
		{
			//k=print_map(dungion_map_input, map_x_size, map_y_size, number_of_rooms, monster_map_input);creature_array[0].next_turn - creature_array[0]
			k=print_fow_map(dungion_map_input, map_x_size, map_y_size, number_of_rooms, monster_map_input, creature_array[0].x_pos, creature_array[0].y_pos, fow_map, object_map);
	
			if(k!=1)
			{
			}  
			else
			{
				printf("%c", 'a');
			}
		}
		
		turn++;
	}
	
	
	endwin();//get out of ncurses here
	
	if(creature_array[0].alive!=0)
	{
		return 2;
		//printf("%s \n", "you dead");
	}
	else
	{
		return 3;
		//printf("%s \n", "max turns");
	}

	return 0;
}

int line_of_sight(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], int line_of_sight_map_input[80][21], int map_x_size, int map_y_size, int pc_x, int pc_y)
{
	//this only has to say what is in sight of player not how far
	int line_of_sight[map_x_size][map_y_size];
	if(dungion_hardness_map_input[pc_x][pc_y] !=0)
	{
		//location for this placment is invallid please use the path through walls
		//to player (not writtent yet)
		return 1;
	}
	
	int j;
	int k;
	
	//variables for the part that does not work
	/*
	int t;
	//is it up/down; left/right; these are "boobleans"
	int left_right;//left 0, right 1
	int up_down; //up 1, down 0
	
	int delta_x;
	int delta_y;
	
	int abs_x;
	int abs_y; 
	
	int temp_j;
	int temp_k;
	*/
	
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			line_of_sight[k][j]=100;
	  }
	}
	//100's are not done
	
	line_of_sight[pc_x][pc_y]=0;
	
	//loop through each cell (only cells that arnt wall)
	//draw line to light
	//if any go through wall ('+' in dungion_map_input) set to 90
	//		90 is shadow
	int success;
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			if(dungion_map_input[k][j]!='+')
			{
				success=0;
				if(k==pc_x && j==pc_y)
				{
					success=0;
				}
				//dosent work couses it to hang
				///the code to check the los
				/*
				else
				{
					//line drawing here, each time it meses up success++
				//from[k][j] to [pc_x][pc_y]
					delta_x=pc_x-k;
					delta_y=pc_y-j;
					
					abs_x=abs(delta_x);
					abs_y=abs(delta_y);
					
					//pc to monster
					if(delta_y<0)
					{
						left_right=1;
					}
					else
					{
						left_right=0;
					}
					
					if(delta_y<0)
					{
						up_down=1;
					}
					else
					{
						up_down=0;
					}
					
					temp_j=j;//the y
					temp_k=k;//the x
					if(abs_x>abs_y)//x domonate
					{
						t=(abs_y *2) - abs_x;
						do
						{
							if(t>=0)
							{
								temp_j=temp_j+left_right;
								t=t-(abs_x*2);
							}
							temp_k=temp_k+up_down;
							t=t-(abs_y*2);
							if(temp_k==pc_x && temp_j==pc_y)
							{
								//true;
							}
						}
						while(dungion_map_input[k][j]!='+');
						success++;
					}
					else//y domonate
					{
						t=(abs_x *2) - abs_y;
						do
						{
							if(t>=0)
							{
								temp_k=temp_k+up_down;
								t=t-(abs_y*2);
							}
							temp_j=temp_j+left_right;
							t=t-(abs_x*2);
							if(temp_k==pc_x && temp_j==pc_y)
							{
								//true;
							}
						}
						while(dungion_map_input[k][j]!='+');
						success++;
					}
					//done with this cell
				}
				*/
				if(success==0)
				{
					line_of_sight[k][j]=0;
				}
				else
				{
					line_of_sight[k][j]=90;
				}
			}
			else
			{
				line_of_sight[k][j]=90;
			}
		}
	}
	
	for(j=0; j<map_y_size; j++)
	{
		for(k=0; k<map_x_size; k++)
		{
			line_of_sight_map_input[k][j]=line_of_sight[k][j];
	  }
	}
	
	return 0;
}
