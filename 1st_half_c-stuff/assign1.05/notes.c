 && dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!='<')
 
 exit ncurses terminal :line: ~3395

 line ~1235 enter ncurses
 line ~1245 initaial print map (calls print_map)
  line ~3380 is where write to teminal is (calls print_map)
  
  int top of the displayed list
  int bottom bottom of displayed list
  if(bottom>=amount of monsters)
  {
	top to amount of monsteers
  }
  else
  {
	top to bottom
  }
 
 int run_game(char monster_map_input[map_x_size][map_y_size], char dungion_map_input[map_x_size][map_y_size], int dungion_hardness_map_input[map_x_size][map_y_size], char pathfinding_map_input[map_x_size][map_y_size], struct room room_array_input[number_of_rooms], int map_x_size, int map_y_size, int number_of_rooms, int num_monsters, struct creature creature_array[num_monsters], int line_of_sight_map_input[map_x_size][map_y_size])

ex:? generic 2 north 4 east
	
	creature_array[i].icon='P';
	creature_array[i].type="person";
	creature_array[i].x_pos=rand_x;
	creature_array[i].y_pos=rand_y;
	
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