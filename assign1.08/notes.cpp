
//class that takes in information
	//tthis class will be used to populate the sturct
	//add invintory to 
	
//add color things

//change made dungion to add 10 objects
//add object class
//objects go bellow monster and player level

//ignore optional




discription (struct) -> factory (class) -> individual (struct) (becouse history)
//object is right now random name and symble


class monster_disc_factory{
	public
	//sets
	
	monster_disc_factory(string name, string symb, string color, string desc, int speed_base, int speed_num, int speed_side, int dam_base, int dam_num, int dam_side, int hp_base, int hp_num, int hp_side);
	
	//get 
	string get_desc(){return mon_desc;}
	int get_abil(){return mon_abil;}
	int get_dam_base(){return dam_base;}
	int get_dam_dice_num(){return dam_dice_num;}
	int get_dam_dice_side(){return dam_dice_side;}
	
	char* get_name()//transilate to char* type; 
	int get_color();
	char get_symb(); //string to char
	
	//roll and return;
	int get_speed();
	int get_health();
	int get_in_use();
	private
	int in_use=0;
	string mon_name;
  string mon_symb;
	string mon_COLOUR;
	string mon_desc;
	
	int speed_base;
	int speed_dice_num;
	int speed_dice_side;
	
	int dam_base;
	int dam_dice_num;
	int dam_dice_side;
	
	int hp_base;
	int hp_dice_num;
	int hp_dice_side;
	//erratic, tunneler, telapathic, intellignet
	int mon_abil;
		//int mon_color;
};

monster_disc_factory::	monster_disc_factory(string name, string symb, string color, string desc, int speed_base_in, int speed_num_in, int speed_side_in, int dam_base_in, int dam_num_in, int dam_side_in, int hp_base_in, int hp_num_in, int hp_side_in)
{
	in_use=1;
	mon_name=name;
	mon_symb=symb;
	mon_COLOUR=color;
	mon_desc=description;
	
	speed_base=speed_base_in;
	speed_dice_num=speed_num_in;
	speed_dice_sid=speed_side_in;
	
	dam_base=dam_base_in;
	dam_dice_num=dam_num_in;
	dam_dice_sid=dam_side_in;
	
	hp_base=hp_base_in;
	hp_dice_num=hp_num_in;
	hp_dice_sid=hp_side_in;
}

//gets
char monster_disc_factory::get_symb(){
	//char get_symb();
	const char *name = mon_symb.c_str();
	char a = *name;
	return a;
}

char* monster_disc_factory::get_name(){
	//string mon_symb;
	const char *name = mon_name.c_str();
	return name;
}

int monster_disc_factory::get_health(){
	int health;
	health=dungion_cpp::dice_roll(hp_base, hp_dice_num, hp_dice_side);
	return health;
}

int monster_disc_factory::get_speed(){
	int speed;
	speed=dungion_cpp::dice_roll(speed_base, speed_dice_num, speed_dice_side);
	return speed;
}

int monster_disc_factory::get_in_use(){
	return in_use;
}

int monster_disc_factory::get_color(){
	//string mon_COLOUR;
	//convert color to int;
	/*
				COLOR_BLACK   0
        COLOR_RED     1
        COLOR_GREEN   2
        COLOR_YELLOW  3
        COLOR_BLUE    4
        COLOR_MAGENTA 5
        COLOR_CYAN    6
        COLOR_WHITE   7
        */
  int color_number;
  if (!mon_COLOUR.compare("COLOR_BLACK")) {
    color_number=0;
	}
	else if(!mon_COLOUR.compare("COLOR_RED")){
		color_number=1;
	}
	else if(!mon_COLOUR.compare("COLOR_GREEN")){
		color_number=2;
	}
	else if(!mon_COLOUR.compare("COLOR_YELLOW")){
		color_number=3;
	}
	else if(!mon_COLOUR.compare("COLOR_BLUE")){
		color_number=4;
	}
	else if(!mon_COLOUR.compare("COLOR_MAGENTA")){
		color_number=5;
	}
	else if(!mon_COLOUR.compare("COLOR_CYAN")){
		color_number=6;
	}
	else{//(!mon_COLOUR.compare("COLOR_WHITE")){
		color_number=7;
	}
  return color_number;
}