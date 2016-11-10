#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char *argv[]){
	
	
	enum {
		arryn , baratheon , greyjoy , lannister ,
		martell , stark , targaryen , tully
	};
	char * words [] = {
		"As  High  as  Honor ", " Ours  is  the   Fury ",
		"We Do Not Sow", " Hear  me  Roar !",
		" Unbowed , Unbent ,  Unbroken ", " Winter  is  Coming ",
		" Fire  and  Blood ", " Family , Duty ,  Honor "
	};
	char lords [][9] = {
		"Jon", " Robert ", " Balon ", " Tywin ",
		" Doran ", " Eddard ", " Viserys ", " Hoster "
	};


	char *s;
	int i;
	
	//a	
	printf ("%s: %s\n", s = lords[ stark ], words [ stark ]);
	putchar ('\n');
	
	//b
	s = "Rob";
	printf (s);
	putchar ('\n');
	putchar ('\n');
	
	//c
	printf ("%s\n", lords [ stark ]);
	putchar ('\n');
	
	//d
	strcpy ( lords [ stark ], s);
	printf ("%s\n", s = lords [ stark ]);
	putchar ('\n');

	//e
	s = " Cersei ";
	for (i = 0; i < 7; i ++) {
		lords [ lannister ][i] = *s ++;
	}
	printf ("%s\n", lords [ lannister ]);
	
	putchar ('\n');
	putchar ('f');
	putchar ('\n');
	
	//f
	printf ("%s\n", lords [ targaryen ]);
	putchar ('\n');
	
	//g
	strcpy ( lords [ tully ], " Brynden ");
	printf ("%s\n", lords [ tully ]);
	putchar ('\n');
	
	//h
	printf ("%s\n", (( char *) lords ) + 14);
	
	//i
	s = words [4];
	printf (s + 19);
	putchar ('\n');
	putchar ('\n');
	
	//j
	
	//strcpy ( words [ baratheon ], words [ lannister ]);
	//printf (" words [ lannister ]\n");
	
	
	/*
	int i;
	char *s = words [6];
	i = -7;
	putchar (s[ -2] & 0xdf );
	lords [0][18] = ' ';
	printf ( lords [2] + 3);
	putchar (s[i ++]);
	putchar ( lords [-i][ -(i + 1)]);
	putchar (s[i - 1] | 0x20 );
	putchar (s[(i *= -2) - 2]);
	putchar (s[--i]);
	s = lords [i / 2];
	s [19]++;
	s [17] -= (++i / 2);
	printf ("%s", s + 17);
	putchar ( words [i / 4 + 1][ -2]);
	putchar ('\n');
		
	printf ("%s\n", "here is end of part one");
	*/
	
	return 0;
}