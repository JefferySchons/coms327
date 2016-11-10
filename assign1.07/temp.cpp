#include <iostream>
#include <fstream>


#include <time.h>
#include <cstdlib>

using namespace std;

struct creature
{
//for both monster and PC
	int is_pc;//not needed becouse pc will always be the first [0] in the array of monsters
	int alive;
	
	char* type;//like skelliten or zombie or dragon
	char* name; //for specific names like this one is named bob or allice
	char* description;  //new as 1.07
	char* color;  //new as 1.07
	int speed;
	int specs; //technically 4 bit used, called charactersitics in assignment
	//assign intelligence to the least significant bit (on the right), telepathy next, tunneling third, 
	//and erratic behavior last (on the left),
	//erratic, tunneling, telepathy, intelligence
	char icon; //defult for monster is ?; will never use it for monster
	int health; //new as 1.07
	int damage; //new as 1.07
	
	int move_time; //=100/creature_array[x].speed;
	int next_turn;//for what this creatures next turn is (prev turn +move_time)
	int x_pos;
	int y_pos;
};

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

int print_monsters(int num_monsters, struct creature creature_array[num_monsters])
{
	int i;
	for(i=1;i<num_monsters;i++)
	{
		//starts at 1 to skip player
		//print all
	}
}

int read_in_monsters(int num_monsters, struct creature creature_array[num_monsters])
{
	
  ifstream file("monster_desc.txt");
  string str; 
  string str2 ("needle");
  
  
  int linenumber;
  linenumber=0;
  while (getline(file, str))
	{
   cout << str << endl;
   
   
   str.find(BEGIN)
   if (str.find(str2) != string::npos) {
		//.. found.
		} 
   //parse str
   //posobilities
   /*
   //if first line
   //		needs RLG327 MONSTER DESCRIPTION
   // order after begin not specified

   BEGIN MONSTER
NAME Amazon Lich Queen
DESC
She was a powerful Amazon warrior in life. Death at the hands of the undead
hordes was followed by her resurrection through dark, necromantic arts. Her
power in life didn't approach her undead glory. Clad in night-black robes
that don't move in the wind, her incorporeal form commands the power of death
over life. You may just be her next victim. You fear for your soul as you
quake before her malevolent majesty.
.
SYMB p
COLOR BLACK
ABIL SMART PASS
DAM 30+5d9
HP 2999+1d1001
SPEED 10+10d2
END
*/
   
	}
       
}

int main(int argc, char *argv[])
{
  int num_monsters=6;
  int k;
  
  struct creature creature_array[num_monsters];
  
  //premake player
  creature_array[0].is_pc=0;
  creature_array[0].type="human";//is here if the pc can ever shapeshift
  creature_array[0].name="JefferySchons";//yes the pc name is the name of the programmer
  creature_array[0].specs=3;//unimportant for pc; but we are smart and can see everything
  creature_array[0].icon='@';
  creature_array[0].move_time=0;
  creature_array[0].next_turn=-1;
  creature_array[0].x_pos=-1;
  creature_array[0].y_pos=-1;
  creature_array[0].speed=10; //player speed is defult 10
  creature_array[0].alive=1;//dead untill placed  
  
  creature_array[0].description="this is the player charactor";  //new as 1.07
	creature_array[0].color;  //new as 1.07
  creature_array[0].health=50; //new as 1.07
	creature_array[0].damage=10; //new as 1.07
  
  k=read_in_monsters(num_monsters, creature_array);
  
  return 0;
}
