//loop through files to get monster one (monster_desc.txt

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
	
	//add 
	color
	name (already above)
	discription
	health
	attack damage
	abilities	(AKA spects)
	
	make function for rolling
	//<base> + <dice> d <sides>
	
	/*if failure reading monster in (field missing, or duplicate field, or bad
		feald) and continue to next monster
		//i think for the TA i should tell how many mosters failed to render
		then display monster information
		
		eddit main so it does not go into game*/
		
/*todo
	make dungion_make_dungion.cpp work
	
	//change main to input monsters right before starting curses and run game
	//disable run game and statrting of ncurses for now
	//where those  where call print function that prints monsters
	
	//at least 3 new functions
	// read and decode (into struct) txt
	// print monster inputs
	// dice roll
*/
#include <iostream>
#include <fstream>


#include <time.h>
#include <cstdlib>

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

int read_in_monsters()
{
	
  std::ifstream file("monster_desc.txt");
    std::string str; 
    while (std::getline(file, str))
    {
        // Process str
    }
}

  string mon_name;
  string mon_symb;
	string mon_COLOUR;
	string mon_desc;
	string mon_speed;
	string mon_dam;
	string mon_hp;
	
	//erratic, tunneler, telapathic, intellignet
	int mon_abil;
	mon_abil=0; //initalize
	int mon_is_smart=1;
	int mon_is_tele=1;
	int mon_is_tunnel=1;
	int mon_is_erratic=1;
	int mon_is_pass=1;