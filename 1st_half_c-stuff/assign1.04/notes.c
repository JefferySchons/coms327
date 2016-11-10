//ckeck if kill
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



// for stight line
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

//tunneling 
else if(dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos]!=255)
{
	creature_array[i].x_pos=creature_array[i].x_pos;
	creature_array[i].y_pos=creature_array[i].y_pos;
	
	dungion_map_input[creature_array[i].x_pos][creature_array[i].y_pos] = '#';
	dungion_hardness_map_input[creature_array[i].x_pos][creature_array[i].y_pos]=0;
}

//no move 
creature_array[i].x_pos=creature_array[i].x_pos;
creature_array[i].y_pos=creature_array[i].y_pos;