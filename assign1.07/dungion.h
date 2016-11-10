#ifndef __DUNGION_H
#define __DUNGION_H



#ifdef __cplusplus
extern "C" {
#endif

//int print_fow_map(char dungion_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char monster_map_input[80][21], int player_x, int player_y, char &fow_map[80][21]);
int print_fow_map(char dungion_map[80][21], int map_x_size, int map_y_size, int number_of_rooms, char monster_map_input[80][21], int player_x, int player_y, char fow_map[80][21]);

int dice_roll(int base, int num_dice, int sides);

int read_in_monsters();
	
#ifdef __cplusplus
}
#endif
#endif