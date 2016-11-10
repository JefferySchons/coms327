
typedef struct room room_t;


struct room
{
	int x_pos; //upper left corrner
	int y_pos;
	int x_size;
	int y_size;
};

int make_dungion_map(char dungion_map_input[80][21], int dungion_hardness_map_input[80][21], room_t room_array_input[6]);
