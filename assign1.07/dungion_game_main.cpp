#include <string.h>
#include <iostream>

using namespace std;

class room{
	int x_pos; //uper left corrner
	int y_pos;
	int x_size;
	int y_size;
	public:
			int set_position_x_y(int x, int y);
			int set_size_x_y(int x, int y);
			int get_x_pos(){return x_size;}
			int get_y_pos(){return y_pos;}
			int get_y_size(){return y_size;}
			int get_x_size(){return x_size;}
};

room::set_position_x_y(int x, int y);
{
	x_pos=x;
	y_pos=y;
	return 0;
}

room::set_size_x_y(int x, int y);
{
	x_size=x;
	y_size=y;
	return 0;
}

int main(int argc, char *argv[])
{
  

  return 0;
}


