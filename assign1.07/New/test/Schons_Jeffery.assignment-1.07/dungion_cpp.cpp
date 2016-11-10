#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <cstdlib>
//#include <ncurses.h>
#include <string>
using namespace std;


//int map_x_size; =80
//int map_y_size; =21
//int number_of_rooms; =6
//int print_fow_map(char dungion_map[map_x_size][map_y_size], int map_x_size, int map_y_size, int number_of_rooms, char monster_map_input[map_x_size][map_y_size], int player_x, int player_y, char fow_map[map_x_size][map_y_size])

int dice_roll(int base, int num_dice, int sides)
{
	//this function is to simmulate dice rolls
	//as of yet it can accept any number of dice greater then zero
	//if sides is zero or lestt it assumes no dice where rolled and 
	//uses the base
	
	//in the original place i used this function this was already seeded
	//earlyer but this is if i use this ageian
	srand(time(NULL));
	
	int amount;
	amount=0; 
	int dice_roll;
	int i;
	
	if(sides>0)
	{
		for(i=0; i<sides; i++)
		{
			dice_roll = rand() % sides + 1;;
			amount=amount+dice_roll;
		}
	}
	
	amount=amount+base;
	return amount;
}

//int read_in_monsters()
int main()
{
	ifstream file("monster_desc.txt");
	string str; 

	string str_init("RLG327");
	string str_begin("BEGIN MONSTER");
	string str_name("NAME");
	string str_symb("SYMB");
	string str_COLOUR("COLOR");
	string str_desc("DESC");
	string str_end_desc("."+'\n');
	string str_speed("SPEED");
	string str_dam("DAM");
	string str_hp("HP");
	string str_abil("ABIL");
	string str_smart("SMART");
	string str_tele("TELE");
	string str_tunnel("TUNNEL");
	string str_erratic("ERRATIC");
	string str_pass("PASS");
	string str_end("END");

  int in_monster=1;
  int in_discription=1;
  int linenumber;
  linenumber=0;
  
  string mon_name;
  string mon_symb;
	string mon_COLOUR;
	string mon_desc;
	mon_desc="";
	
	//int mon_color;
	
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
	mon_abil=0; //initalize
	int mon_is_smart=1;
	int mon_is_tele=1;
	int mon_is_tunnel=1;
	int mon_is_erratic=1;
	int mon_is_pass=1;
	
	while (getline(file, str))
	{
		
		linenumber++;
		if(linenumber==1)
		{
			if (str.find(str_init) != string::npos) 
			{
	  		//cout << "start" << endl;
				//cout << str << endl;
			} 
			else
			{
				return 1;
			}
		}
		else if (str.find(str_begin) != string::npos) 
		{
			in_monster=0;
			//cout << "begin monster" << endl;
			//if entermonster without leaving it asume previouse is bad
			
			//clear vars
			
			mon_desc="";
			mon_abil=0;
			mon_is_smart=1;
			mon_is_tele=1;
			mon_is_tunnel=1;
			mon_is_erratic=1;
			mon_is_pass=1;
		}
		else if (str.find(str_end) != string::npos) 
		{
			in_monster=1;
			//cout << "end monster" << endl;
			
			//put whole monster in
			cout << mon_name << endl;
			//cout << "disctritption" << endl;
			cout << mon_desc << endl;
			cout << mon_symb << endl;
			
			// int is stored in int mon_color;
			cout << mon_COLOUR << endl;
			
			//speed
			cout << speed_base;
			cout << '+';
			cout << speed_dice_num;
			cout << 'd';
			cout << speed_dice_side;
			cout << endl;
			
			//cout << mon_abil << endl;
			if(mon_is_smart==0)
			{
				cout << "SMART";
			}
			else if(mon_is_tele==0)
			{
				cout << "TELE";
			}
			else if(mon_is_tunnel==0)
			{
				cout << "TUNNEL";
			}
			else if(mon_is_erratic==0)
			{
				cout << "ERRATIC";
			}
			else if(mon_is_pass==0)
			{
				cout << "PASS";
			}
			cout << endl;
			
			//hp
			cout << hp_base;
			cout << '+';
			cout << hp_dice_num;
			cout << 'd';
			cout << hp_dice_side;
			cout << endl;
			
			//damage
			cout << dam_base;
			cout << '+';
			cout << dam_dice_num;
			cout << 'd';
			cout << dam_dice_side;
			cout << endl;
			
			cout << "" << endl;//space between monsters
		}
		else if (in_monster==0)
		{
			if (str.find(str_symb) != string::npos) 
			{
				//cout << "end disctritption" << endl;
				in_discription=1;
				mon_symb=str.substr(str.find(str_symb) + 5);
				
			}
			else if(str.find(str_name) != string::npos)
			{
				//cout << "name" << endl;
				mon_name=str.substr(str.find(str_name) + 5);
				//cout << mon_name << endl;
				in_discription=1;
			}
			else if (str.find(str_symb) != string::npos)
			{
				//cout << "symb" << endl;
				mon_symb=str.substr(str.find(str_symb) + 5);
				//cout << mon_symb << endl;
				in_discription=1;
			}
			else if (str.find(str_COLOUR) != string::npos) 
			{
				//change to int
				//cout << "color" << endl;
				mon_COLOUR=str.substr(str.find(str_COLOUR) + 6);
				//cout << mon_COLOUR << endl;
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
        /*
				switch(mon_COLOUR)
				{
					case COLOR_BLACK:
					    	mon_color=0;
					      break;
					case COLOR_RED:
					    	mon_color=0;
					      break;
					case COLOR_GREEN:
					    	mon_color=0;
					      break;
					case COLOR_YELLOW:
					    	mon_color=0;
					      break;
					case COLOR_BLUE:
					    	mon_color=0;
					      break;
					case COLOR_MAGENTA:
					    	mon_color=0;
					      break;
					case COLOR_WHITE:
					    	mon_color=0;
					      break;
					case COLOR_BLUE:
					    	mon_color=0;
					      break;
					default:
						mon_color=7;
						break;
				}
				*/
				in_discription=1;
			}
			else if (str.find(str_dam) != string::npos) 
			{
				//cout << "damage" << endl;
				
				string dam_substing;
				string dice_substing;
				string base_substing;
				string sides_substing;
				string number_dice_substing;
				
				dam_substing=str.substr(str.find(" ")+1, str.find('+')); //xx+yyds
				dice_substing=str.substr(str.find("+")+1, str.find('d')); //yyds
				base_substing=str.substr(str.find(" ")+1, str.find('+')-4); //xx
				sides_substing=dice_substing.substr(dice_substing.find('d')+1); //s
				number_dice_substing=dice_substing.substr(0, dice_substing.find('d'));//y

				dam_base=atoi(base_substing.c_str());
				dam_dice_num=atoi(number_dice_substing.c_str());
				dam_dice_side=atoi(sides_substing.c_str());
				
				/*
				cout << "damage sub" << endl;
				cout << dam_substing << endl;
				cout << "dice sub" << endl;
				cout << dice_substing << endl;
				cout << "base sub" << endl;
				cout << base_substing << endl;
				cout << dam_base << endl;
				cout << "number dice sub" << endl;
				cout << number_dice_substing << endl;
				cout << dam_dice_num << endl;
				cout << "sides sub" << endl;
				cout << sides_substing << endl;
				cout << dam_dice_side << endl;
				*/
				
				in_discription=1;
			}
			else if (str.find(str_hp) != string::npos) 
			{
				//cout << "hp" << endl;
				
				string hp_substing;
				string dice_substing;
				string base_substing;
				string sides_substing;
				string number_dice_substing;
				
				hp_substing=str.substr(str.find(" ")+1, str.find('+')); //xx+yyds
				dice_substing=str.substr(str.find("+")+1, str.find('d')); //yyds
				base_substing=str.substr(str.find(" ")+1, str.find('+')-4); //xx
				sides_substing=dice_substing.substr(dice_substing.find('d')+1); //s
				number_dice_substing=dice_substing.substr(0, dice_substing.find('d'));//y

				hp_base=atoi(base_substing.c_str());
				hp_dice_num=atoi(number_dice_substing.c_str());
				hp_dice_side=atoi(sides_substing.c_str());
				
				in_discription=1;
			}
			else if (str.find(str_speed) != string::npos) 
			{
				//cout << "speed" << endl;
				
				string speed_substing;
				string dice_substing;
				string base_substing;
				string sides_substing;
				string number_dice_substing;
				
				speed_substing=str.substr(str.find(" ")+1, str.find('+')); //xx+yyds
				dice_substing=str.substr(str.find("+")+1, str.find('d')); //yyds
				base_substing=str.substr(str.find(" ")+1, str.find('+')-4); //xx
				sides_substing=dice_substing.substr(dice_substing.find('d')+1); //s
				number_dice_substing=dice_substing.substr(0, dice_substing.find('d'));//y

				speed_base=atoi(base_substing.c_str());
				speed_dice_num=atoi(number_dice_substing.c_str());
				speed_dice_side=atoi(sides_substing.c_str());
				
				in_discription=1;
			}
			else if (str.find(str_abil) != string::npos) 
			{
				//cout << "abilities" << endl;
				//erratic, tunneler, telapathic, intellignet
				if (str.find(str_pass) != string::npos) 
				{
					//cout << "passive" << endl;
					mon_is_pass=0;
					//mon_abil=0;
				}
				else if (str.find(str_tele) != string::npos) 
				{
					//cout << "telepathic" << endl;
					mon_is_tele=0;
					mon_abil=mon_abil+2;
				}
				else if (str.find(str_tunnel) != string::npos) 
				{
					//cout << "tunnel" << endl;
					mon_is_tunnel=0;
					mon_abil=mon_abil+4;
				}
				else if (str.find(str_erratic) != string::npos)
				{
					//cout << "erratic" << endl;
					mon_is_erratic=0;
					mon_abil=mon_abil+8;
				}
				else if (str.find(str_smart) != string::npos) 
				{
					//cout << "smart" << endl;
					mon_is_smart=0;
					mon_abil=mon_abil+1;
				}
				in_discription=1;
			}
			else if (str.find(str_desc) != string::npos) 
			{
				//cout << "start disctritption" << endl;
				//cout << str << endl;
				//mon_desc=str;
				
				in_discription=0;
			}
			else if(in_discription==0)
			{
				//str.erase(remove(str.begin(), str.end(), '\n'), str.end());
				//mon_desc.erase(remove(mon_desc.begin(), mon_desc.end(), '\n'), mon_desc.end());
				//mon_desc.append(str);
				//cout << "strat disctritption" << endl;
				//cout << mon_desc << endl;
				//mon_desc+=str;
				
				//mon_desc=str+mon_desc;
				/*
				cout << "1 disctritption" << endl;
				cout << mon_desc << endl;
				
				cout << "2 disctritption" << endl;
				cout << str << endl;
				*/
				mon_desc=str+mon_desc;
				//mon_desc=mon_desc+str;
				/*
				cout << "3 disctritption" << endl;
				cout << mon_desc << endl;
				*/
				//mon_desc=str+mon_desc;
				//mon_desc=mon_desc+str;
				
				//cout << "in disctritption" << endl;
				//cout << str << endl;
				//cout << "to disctritption" << endl;
			}
		}
	}
	       
	return 0;	
}

