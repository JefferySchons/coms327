#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

#include "heap.h"

#define SHOW_DISTANCES 1 //Should we display the dijkstra distances?

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21
#define NUM_ROOMS 6

#define MAX_WIDTH 8
#define MAX_HEIGHT 8
#define OFFSET 4//Used so we don't have 0 width rooms

#define WALL ' '
#define CORRIDOR '#'
#define ROOM '.' 

#define ROOM_UNOCCUPIED '/' //Could define 0 for more succinct comparisons, but this is readable.
#define ROOM_OCCUPIED_PLAYER '@'

#define HARDNESS_IMPENETRABLE 255
#define HARDNESS_HARD 254
#define HARDNESS_NORMAL 170
#define HARDNESS_SOFT 84
#define HARDNESS_WALKABLE 0

#define ByteSwap32(n) \
	(((((unsigned long) n) << 24) & 0xFF000000)  | \
	 ((((unsigned long) n) <<  8) & 0x00FF0000) | \
	 ((((unsigned long) n) >>  8) & 0x0000FF00) | \
	 ((((unsigned long) n) >> 24) & 0x000000FF) )

#define SEMANTIC_MARKER "RLG327"
#define VERSION_ZERO_BYTES 1693 //The number of bytes contained in a version 0 file with no rooms

#define INT_MAX 2147483647
#define CHAR_MAX 255

typedef struct
{
	int cost;
	heap_node_t * n;
	int posx;
	int posy;
	int fromx;
	int fromy;
}node;

struct option longopts[] = {
	{"load", optional_argument, 0, 'l'},
	{"save", optional_argument, 0, 's'},
	{0, 0, 0, 0}
	};

typedef struct
{
	int posx, posy;
	int width, height;
} room;

//It would probably be better to have a struct of chars, 
//And then have an array of those structs. But with this
//Approach, I'm unsure how to load in just one of those
//Characters (for hardness)
struct
{
	char floor[DUNGEON_HEIGHT][DUNGEON_WIDTH];//Type of floor (room, wall, etc)
	char hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];//One byte, more of uint8_t than a char.
	char occupant[DUNGEON_HEIGHT][DUNGEON_WIDTH];//Who's in the room?
}dungeon;

room create_room()
{
	room ret;
	//-1 / +1 is in place to preserve outer walls.
	ret.posx = rand() % (DUNGEON_WIDTH - MAX_WIDTH - OFFSET - 1) + 1 ;
	ret.posy = rand() % (DUNGEON_HEIGHT - MAX_WIDTH - OFFSET - 1) + 1;

	ret.width = rand() % MAX_WIDTH + OFFSET;
	ret.height = rand() % MAX_HEIGHT + OFFSET;

	return ret;
}

//Apparently overloading isn't a thing in C?
room create_room_2(int x,int y,int w,int h)
{
	room ret;
	ret.posx = x;
	ret.posy = y;
	ret.width = w;
	ret.height = h;
	return ret;
}

//Returns 1 if there is a collision,
//Where a collision is defined as two
//Overlapping or immediately adjacent 
//Rooms.
int check_collision(room a, room b)
{
	//2D collision detection
	if (a.posx <= b.posx + b.width &&
	    a.posx + a.width >= b.posx &&
	    a.posy <= b.posy + b.height &&
        a.posy + a.height >= b.posy)
	   {
		   return 1;
	   }
	
	return 0;
}

//Used for heap compare. Pass in the costs and it *should* work
//Here be errors, potentially.
static int32_t heap_compare(const void *key, const void *with) {
  return ((node *) key)->cost - ((node *) with)->cost;
}

//Returns the dijkstra cost 
//For a given tile
int getCost(int hardness)
{
	if(hardness <= HARDNESS_SOFT)
	{
		return 1;
	}
	if(hardness <= HARDNESS_NORMAL)
	{
		return 2;
	}
	if(hardness <= HARDNESS_HARD)
	{
		return 3;
	}
	return HARDNESS_IMPENETRABLE; //Should never be reached.
}

/*
 * Returns a random hardness [1,254]
 * Assumes rand has already been assigned
 */
int getRandomHardness()
{
	return rand() % HARDNESS_IMPENETRABLE + 1;
}

//nodes could probably be part of dungeon
node nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], * p; //Used for Dijkstra

int main(int argc, char * argv[])
{
	int i, j, k;
	int isValid = 0; //Is the configuration of rooms valid?
	int indexptr;
	char ch;
	int shouldSave = 0;
	int shouldLoad = 0;
	int numRooms = NUM_ROOMS;
	char * path = strcat(getenv("HOME"),"/.rlg327/dungeon");
	FILE * f;

	room rooms[numRooms];	
	srand(time(NULL));
	
	//Process flags
	while ((ch = getopt_long(argc, argv, "", longopts, &indexptr)) != -1) {
		switch (ch) {
			case 'l':
				shouldLoad = 1;
				break;
			case 's':
				shouldSave = 1;
				break;
		}
	}	
	
	if(shouldLoad)
	{
		char * str = malloc(255);
		int v, sz;
		if(!(f = fopen(path, "r")))
		{
			printf("Oh dear, something went wrong with open()! %s\n", strerror(errno));
			exit(1);
		}	
		fread(str, 1, 6, f); //Semantic marker
		if(strcmp(str, SEMANTIC_MARKER))
		{
			printf("Improper file given\n");
			exit(2);
		}
		fread(&v, 1, 4, f); //Version
		v = ByteSwap32(v);
		fread(&sz, 1, 4, f);//Size
		sz = ByteSwap32(sz);
		//printf("size: %d\n", sz);
		switch(v)
		{
			case 0:
				fread(&dungeon.hardness, 1, DUNGEON_HEIGHT * DUNGEON_WIDTH, f);
				//Load walls and corridors
				for(i = 0; i < DUNGEON_HEIGHT; i++)
				{
					for(j = 0; j < DUNGEON_WIDTH; j++)
					{
						if(dungeon.hardness[i][j])
						{
							dungeon.floor[i][j] = WALL;
						}
						else
						{
							dungeon.floor[i][j] = CORRIDOR;
						}
					}
				}
				//Then load the rooms.
				numRooms = (sz - VERSION_ZERO_BYTES) / 4;
				for(i = 0; i < numRooms; i++)
				{
					int k, m;
					char x, y, w, h;//x, y, width height
					fread(&x, 1, 1, f);
					fread(&w, 1, 1, f);
					fread(&y, 1, 1, f);
					fread(&h, 1, 1, f);
					rooms[i] = create_room_2(x, y, w, h);
					
					//This shuold be a paintRooms() function,
					//but still uncomfortable with passing arrays.
					for(k = y; k < y + h; k++)
					{
						for(m = x; m < x + w; m++)
						{
							dungeon.floor[k][m] = ROOM;
						}
					}
				}
				
			break;
			default:
			printf("Version not supported\n");
			exit(3);
		}	
		if (NULL == f)
		{
			perror ("File is somehow null\n");
		}
		else
		{
			fclose (f);
		}
	}
	else //Not loading, generate unique Dungeon
	{
		//Initialize dungeon to be all wall
		for(i = 0; i < DUNGEON_HEIGHT; i++)
		{
			for(j = 0; j < DUNGEON_WIDTH; j++)
			{
				dungeon.floor[i][j] = WALL;
				dungeon.occupant[i][j] = ROOM_UNOCCUPIED;
				
				//Set the outer wall to be impenetrable
				if(0 == i || 0 == j || DUNGEON_HEIGHT - 1 == i || DUNGEON_WIDTH - 1 == j)
				{
					dungeon.hardness[i][j] = HARDNESS_IMPENETRABLE;
				}
				else
				{
					dungeon.hardness[i][j] = getRandomHardness();
				}
			}
		}
		
		//Create rooms
		do
		{
			int flag = 0;
			
			//Create rooms
			for(i = 0; i < numRooms; i++)
			{		
				rooms[i] = create_room();
			}
			//Check for collision, repeat until none
			for(i = 0; i < numRooms; i++)
			{
				for(j = i + 1; j < numRooms; j++)
				{
					if(check_collision(rooms[i], rooms[j]))
					{
						flag = 1;
						break;
					}
				}
				if(flag)
				{
					break;
				}
			}
			//Reached an incorrect room arrangement
			if(flag)
			{
				flag = 0;
				continue;
			}
			
			isValid = 1;
			//Once no collisions, add to dungeon
			for(i = 0; i < numRooms; i++)
			{
				for(j = 0; j < rooms[i].height; j++)
				{
					for(k = 0; k < rooms[i].width; k++)
					{
						//Change tile for all the area to room character.					
						dungeon.floor[rooms[i].posy + j][rooms[i].posx + k] = ROOM;	
						dungeon.hardness[rooms[i].posy + j][rooms[i].posx + k] = HARDNESS_WALKABLE;	
					}		
				}
			}
			//Then connect with corridors
			//Connect 1 with 2, 2 with 3, etc.
			for(i = 0; i < numRooms - 1; i ++)
			{
				//I would prefer to have this as a function, but I couldn't
				//Figure out how to pass around my 2D array.
				room a = rooms[i];
				room b = rooms[i+1];
				int midx, midy;	
				int modHeight = a. posy;
				int modWidth = b.posx;
				
				midx = a.posx + a.width / 2;
				midy = b.posy + b.height / 2;
				
				if(modHeight <= b.posy)
				{
					while(modHeight <= midy)
					{
						if(ROOM != dungeon.floor[modHeight][midx])
						{
							dungeon.floor[modHeight][midx] = CORRIDOR;
							dungeon.hardness[modHeight][midx] = HARDNESS_WALKABLE;
						}
						modHeight++;
					}
				}
				else
				{
					while(modHeight >= midy)
					{
						if(ROOM != dungeon.floor[modHeight][midx])
						{
							dungeon.floor[modHeight][midx] = CORRIDOR;
							dungeon.hardness[modHeight][midx] = HARDNESS_WALKABLE;
						}
						modHeight--;
					}
				}				
				if(modWidth <= midx)
				{
					while(modWidth <= midx)
					{		
						if(ROOM != dungeon.floor[midy][modWidth])
						{
							dungeon.floor[midy][modWidth] = CORRIDOR;
							dungeon.hardness[midy][modWidth] = HARDNESS_WALKABLE;
						}
						modWidth++;
					}
				}
				else
				{
					while(modWidth >= midx)
					{	
						if(ROOM != dungeon.floor[midy][modWidth])
						{
							dungeon.floor[midy][modWidth] = CORRIDOR;
							dungeon.hardness[midy][modWidth] = HARDNESS_WALKABLE;
						}
						modWidth--;
					}
				}					
			}
		}
		while(0 == isValid);
		//Now we have a valid dungeon. Insert the player randomly.
		int player_placed = 0, playerx, playery;
		do
		{
			playery = rand () % DUNGEON_HEIGHT;
			playerx = rand() % DUNGEON_WIDTH;
			if(ROOM == dungeon.floor[playery][playerx])
			{
				dungeon.occupant[playery][playerx] = ROOM_OCCUPIED_PLAYER;
				player_placed = 1;
			}
		}while(!player_placed);		
		//Calculate Dijkstra 
		heap_t h;
		int x, y;
				
		//Initialize distance to infinity
		for (y = 0; y < DUNGEON_HEIGHT; y++)
		{
			for (x = 0; x < DUNGEON_WIDTH; x++)
			{
				nodes[y][x].posy = y;
				nodes[y][x].posx = x;
				nodes[y][x].cost = CHAR_MAX -1;
			}
		}
		
		//Except the source is 0
		nodes[playery][playerx].cost = 0;
		
		heap_init(&h, heap_compare, NULL);

		//Add each of the mutable nodes to the set.
		for (y = 0; y < DUNGEON_HEIGHT; y++)
		{
			for (x = 0; x < DUNGEON_WIDTH; x++)
			{
				if (HARDNESS_IMPENETRABLE != dungeon.floor[y][x])
				{
					nodes[y][x].n = heap_insert(&h, &nodes[y][x]);
				}
				else
				{
					nodes[y][x].n = NULL;
				}
			}
		}	
		
		while ((p = heap_remove_min(&h)))
		{
			p->n = NULL;
			
			//TODO change +1 to + roomCost
			//Where roomcost is 1 for a floor, or the function 
			//We can probably initialize all those, and store it 
			//In the node?
			
			//If it's in bounds and not null and cheaper
			if(p->posy > 0 && nodes[p->posy - 1][p->posx].n && 
			   nodes[p->posy - 1][p->posx].cost > p->cost + getCost(dungeon.hardness[p->posy - 1][p->posx]))
			   {
				   //Replace the cost with the cheaper cost
				   nodes[p->posy - 1][p->posx].cost = p->cost + getCost(dungeon.hardness[p->posy - 1][p->posx]);
				   //Set ptrevious node
				   nodes[p->posy - 1][p->posx].fromx = p->posx; 
				   nodes[p->posy - 1][p->posx].fromy = p->posy;
				   heap_decrease_key_no_replace(&h, nodes[p->posy - 1][p->posx].n);
			   }

			//Repeat for Y+
			if(p->posy < DUNGEON_HEIGHT - 1 && nodes[p->posy + 1][p->posx].n && 
			   nodes[p->posy + 1][p->posx].cost > p->cost + getCost(dungeon.hardness[p->posy + 1][p->posx]))
			   {
				   //Replace the cost with the cheaper cost
				   nodes[p->posy + 1][p->posx].cost = p->cost + getCost(dungeon.hardness[p->posy + 1][p->posx]);
				   //Set ptrevious node
				   nodes[p->posy + 1][p->posx].fromx = p->posx; 
				   nodes[p->posy + 1][p->posx].fromy = p->posy;
				   heap_decrease_key_no_replace(&h, nodes[p->posy + 1][p->posx].n);
			   }
			   
			//Repeat for X-
			if(p->posx > 0 && nodes[p->posy][p->posx - 1].n && 
			   nodes[p->posy][p->posx - 1].cost > p->cost + getCost(dungeon.hardness[p->posy][p->posx - 1]))
			   {
				   //Replace the cost with the cheaper cost
				   nodes[p->posy][p->posx - 1].cost = p->cost + getCost(dungeon.hardness[p->posy][p->posx - 1]);
				   //Set ptrevious node
				   nodes[p->posy][p->posx - 1].fromx = p->posx; 
				   nodes[p->posy][p->posx - 1].fromy = p->posy;
				   heap_decrease_key_no_replace(&h, nodes[p->posy][p->posx - 1].n);
			   }
			
			//Repeat for X+
			if(p->posx < DUNGEON_WIDTH - 1 && nodes[p->posy][p->posx + 1].n && 
			   nodes[p->posy][p->posx + 1].cost > p->cost + getCost(dungeon.hardness[p->posy][p->posx + 1]))
			   {
				   //Replace the cost with the cheaper cost
				   nodes[p->posy][p->posx + 1].cost = p->cost + getCost(dungeon.hardness[p->posy][p->posx + 1]);
				   //Set ptrevious node
				   nodes[p->posy][p->posx + 1].fromx = p->posx; 
				   nodes[p->posy][p->posx + 1].fromy = p->posy;
				   heap_decrease_key_no_replace(&h, nodes[p->posy][p->posx + 1].n);
			   } 
			   
			//Now the diagonals
			//Southeast
			if(p->posx < DUNGEON_WIDTH - 1 && p->posy < DUNGEON_HEIGHT - 1 &&
				nodes[p->posy + 1][p->posx + 1].n && 
			    nodes[p->posy + 1][p->posx + 1].cost > p->cost + getCost(dungeon.hardness[p->posy + 1][p->posx + 1]))
			   {
				   //Replace the cost with the cheaper cost
				   nodes[p->posy + 1][p->posx + 1].cost = p->cost + getCost(dungeon.hardness[p->posy + 1][p->posx + 1]);
				   //Set ptrevious node
				   nodes[p->posy + 1][p->posx + 1].fromx = p->posx; 
				   nodes[p->posy + 1][p->posx + 1].fromy = p->posy;
				   heap_decrease_key_no_replace(&h, nodes[p->posy + 1][p->posx + 1].n);
			   } 
			   
			   //Southwest
			   if(p->posx > 0 && p->posy < DUNGEON_HEIGHT - 1 &&
				nodes[p->posy + 1][p->posx - 1].n && 
			    nodes[p->posy + 1][p->posx - 1].cost > p->cost + getCost(dungeon.hardness[p->posy + 1][p->posx - 1]))
			   {
				   //Replace the cost with the cheaper cost
				   nodes[p->posy + 1][p->posx - 1].cost = p->cost + getCost(dungeon.hardness[p->posy + 1][p->posx - 1]);
				   //Set ptrevious node
				   nodes[p->posy + 1][p->posx - 1].fromx = p->posx; 
				   nodes[p->posy + 1][p->posx - 1].fromy = p->posy;
				   heap_decrease_key_no_replace(&h, nodes[p->posy + 1][p->posx - 1].n);
			   } 
			   
			   //Northwest
			   if(p->posx > 0 && p->posy > 0 &&
				nodes[p->posy - 1][p->posx - 1].n && 
			    nodes[p->posy - 1][p->posx - 1].cost > p->cost + getCost(dungeon.hardness[p->posy - 1][p->posx - 1]))
			   {
				   //Replace the cost with the cheaper cost
				   nodes[p->posy - 1][p->posx - 1].cost = p->cost + getCost(dungeon.hardness[p->posy - 1][p->posx - 1]);
				   //Set ptrevious node
				   nodes[p->posy - 1][p->posx - 1].fromx = p->posx; 
				   nodes[p->posy - 1][p->posx - 1].fromy = p->posy;
				   heap_decrease_key_no_replace(&h, nodes[p->posy - 1][p->posx - 1].n);
			   }
			   
			   //Northeast
			   if(p->posx < DUNGEON_WIDTH - 1 && p->posy > 0 &&
				nodes[p->posy - 1][p->posx + 1].n && 
			    nodes[p->posy - 1][p->posx + 1].cost > p->cost + getCost(dungeon.hardness[p->posy - 1][p->posx + 1]))
			   {
				   //Replace the cost with the cheaper cost
				   nodes[p->posy - 1][p->posx + 1].cost = p->cost + getCost(dungeon.hardness[p->posy - 1][p->posx + 1]);
				   //Set ptrevious node
				   nodes[p->posy - 1][p->posx + 1].fromx = p->posx; 
				   nodes[p->posy - 1][p->posx + 1].fromy = p->posy;
				   heap_decrease_key_no_replace(&h, nodes[p->posy - 1][p->posx + 1].n);
			   } 
			   
		}		
		//End Dijkstra
	
	}//End generation / loading
	if(shouldSave)
	{
		int r;
		if(!(f = fopen(path, "w")))
		{
			printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));		
		}			
		fprintf(f, "%s",SEMANTIC_MARKER);
		unsigned int version = 0;
		fwrite(&version, sizeof(version), 1, f);
		unsigned int size = 1693 + 4 * numRooms; //bytes before room position + room position data
		size = ByteSwap32(size);//Big Endian
		fwrite(&size, sizeof(size), 1, f);
		fwrite(dungeon.hardness, 1, DUNGEON_WIDTH * DUNGEON_HEIGHT, f);
		for(r = 0; r < numRooms; r++)
		{
			fwrite((&rooms[r].posx), 1, 1, f);
			fwrite((&rooms[r].width), 1, 1, f);
			fwrite((&rooms[r].posy), 1, 1, f);
			fwrite((&rooms[r].height), 1, 1, f);		
		}
		fclose(f);
	}
	
	//Print dungeon (I would also like this to be a function)
	//First print normally
	for(i = 0; i < DUNGEON_HEIGHT; i++)
	{
		for(j = 0; j < DUNGEON_WIDTH; j++)
		{
			if(ROOM_UNOCCUPIED != dungeon.occupant[i][j])
			{
				printf("%c", dungeon.occupant[i][j]);
			}
			else
			{
				printf("%c", dungeon.floor[i][j]);
			}	
		}
		printf("\n");
	}
	printf("\n");
	
	//Then print dijkstra no tunnels
	int normal_print = 1;
	for(i = 0; i < DUNGEON_HEIGHT; i++)
	{
		for(j = 0; j < DUNGEON_WIDTH; j++)
		{
			if(SHOW_DISTANCES)//Dijkstra mode
			{
				int dist = nodes[i][j].cost;
				normal_print = 0;
			
				if(dungeon.hardness[i][j])
				{
					printf(" ");
				}
				else
				{
					//We subtract 10 because we shifted up 10
					//By the time we get to the second set of
					//Contiguous ascii characters, we've shifted
					//26 (a-z) plus the original 10 (0-9)
					if(dist < 10)//Garbage hardcoded values
					{
						printf("%c", dist + '0');
					}
					else if(dist < 36)
					{
						printf("%c", dist + 'a' - 10);
					}
					else if(dist < 62)
					{
						printf("%c", dist + 'A' - 36);
					}
					else
					{
						normal_print = 1;
					}
				}
			}
			if(normal_print)
			{
				if(ROOM_UNOCCUPIED != dungeon.occupant[i][j])
				{
					printf("%c", dungeon.occupant[i][j]);
				}
				else
				{
					printf("%c", dungeon.floor[i][j]);
				}	
			}
		}
		printf("\n");
	}
		
	//Then print dijkstra with tunnels
	//Copy paste code
	normal_print = 1;
	for(i = 0; i < DUNGEON_HEIGHT; i++)
	{
		for(j = 0; j < DUNGEON_WIDTH; j++)
		{
			if(SHOW_DISTANCES)//Dijkstra mode
			{
				int dist = nodes[i][j].cost;
				normal_print = 0;
				//We subtract 10 because we shifted up 10
				//By the time we get to the second set of
				//Contiguous ascii characters, we've shifted
				//26 (a-z) plus the original 10 (0-9)
				if(dist < 10)//Garbage hardcoded values
				{
					printf("%c", dist + '0');
				}
				else if(dist < 36)
				{
					printf("%c", dist + 'a' - 10);
				}
				else if(dist < 62)
				{
					printf("%c", dist + 'A' - 36);
				}
				else
				{
					normal_print = 1;
				}
			}
			if(normal_print)
			{
				if(ROOM_UNOCCUPIED != dungeon.occupant[i][j])
				{
					printf("%c", dungeon.occupant[i][j]);
				}
				else
				{
					printf("%c", dungeon.floor[i][j]);
				}	
			}
		}
		printf("\n");
	}
	return 0;
}