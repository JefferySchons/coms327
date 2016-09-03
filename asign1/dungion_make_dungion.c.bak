#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int dungion_map();

int main(){
	//this main will get removed once more of the code is writen
	int k;
	srand(time(NULL));
	k = dungion_map();
	return k;
}

struct room
{
	int x_pos; //uper left corrner
	int y_pos;
	int x_size;
	int y_size;
};

int dungion_map()
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
	int map_x_size = 80; //horizontal
	int map_y_size = 21; // veritical
	int number_of_rooms = 6;
	int rand_x; //for x size of room
	int rand_y; //for y size of room
	int rand_x_place; //for x placement of room
	int rand_y_place; //for y placement of room
	int i;
	int r; //counter
	int j; //for printing to and from map (x)
	int k; //for printing to and from map (y)	
	
	char dungion_map[map_x_size][map_y_size];
	int room_placement_array[number_of_rooms][2]; //[row][collum]
	struct room room_array[number_of_rooms];

	//preset dungion map, will ignore all + when printing
		for(j=0; j<map_x_size; j++)
	{
		for(k=0; k<map_y_size; k++)
		{
			dungion_map[j][k]='+';
	  }
	}

	int number_of_rooms_placed;
  //foirst make of room_array
  number_of_rooms_placed = 0;
  int number_of_failures = 0; //number of times it has cycled through while
  while(number_of_rooms_placed <= number_of_rooms || number_of_failures <number_of_rooms)
  {
  	//populate array
  	for(i=0; i<number_of_rooms; i++)
  	{
  		rand_x = (rand() %5) + 4;
  		rand_y = (rand() %5) + 3;
  		room_placement_array[i][0]=rand_x;
  		room_placement_array[i][1]=rand_y;
  		
  		number_of_failures++;
  	}
  	
  	//cycle through array trying to place rooms
  	for(i=0; i<number_of_rooms; i++)
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
  */
  int next_room_numb;
  next_room_numb=0;
  for(r=0; r<=number_of_rooms; r++)
  {
  	//make coordor from room_array[r] to room_array[r+1]
  	int left_right;  //x axes (also j is used above for it), right pos
  	int up_down; //y axes (also k is used above for it), up is pos
  	if(next_room_numb==number_of_rooms)
  	{
  		next_room_numb=0;
  	}
  	else
  	{
  		next_room_numb=r+1;
    }
    left_right = room_array[next_room_numb].x_pos - room_array[r].x_pos;
  	up_down = room_array[next_room_numb].y_pos - room_array[r].y_pos;

  	//"it" is refuring to room[r+1]
  	//do note the up down here is asuming that increasing y goes up
  	//i know inreality it is printed down
  	if(left_right>0)
		{
			//its right or stright down/up
			for(i=room_array[r].x_pos; i<=room_array[next_room_numb].x_pos; i++)
			{
				if(dungion_map[i][room_array[r].y_pos] != '.')
				{
					dungion_map[i][room_array[r].y_pos] = '#';
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
				}
			}
		}
  		
  	if(up_down>0)
		{
			//its up or stright left/right
			for(i=room_array[r].y_pos; i<=room_array[next_room_numb].y_pos; i++)
			{
				if(dungion_map[room_array[r].x_pos][i] != '.')
				{
					dungion_map[room_array[r].x_pos][i] = '#';
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
				}
			}
		}
  }
  
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