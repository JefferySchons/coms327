int dungion_map();

struct room
{
	int x_pos; //upper left corrner
	int y_pos;
	int x_size;
	int y_size;
};

int print_map(char dungion_map[][]);