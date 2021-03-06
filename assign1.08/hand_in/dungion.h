#ifndef __DUNGION_H
#define __DUNGION_H

struct object
{
	char* name;
	char icon;
};

//int print_fow_map(char dungion_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char monster_map_input[80][21], int player_x, int player_y, char &fow_map[80][21]);
int print_fow_map(char dungion_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char monster_map_input[80][21], int player_x, int player_y, char fow_map[80][21], char object_map[80][21]);

int dice_roll(int base, int num_dice, int sides);

int read_in_monsters();

int place_objects(char dungion_map[80][21], int map_x_size, int map_y_size, char object_map[80][21], object object_list[15]);

#endif