The map is made and printed in the dungion_map function and is all in the
dungion_make_dunfion.c file. All the main does is call the dungion_map function.

1.01
The dungion_map function
	first: pre populates the array (dungion_map[][]) with '+', these are rocks
and when printed will be shown as spaces instead.
	then: it tries to place correct number of rooms by first making an array
of posible rooms then trying to place them, when it failes it will refresh the 
array and try agein (keeping rooms already placed). as the rooms are placed room 
structs are made with the information of place and size and stored in a array 
room_array[]. The dungion_map[][] is edited so '.' are where the rooms are
	next: placing cooridors. the rooms are connected in order (room 1->2->3
...->last->1). Fist it is found what direction the next room is from the current
room (left/right or stright up/down). the rooms are connected by uper left corrner
first horizontaly then vertically. The dungion_map[][] is edited so '#' are where 
the corrdors are unless there is a room there.
	last: the dungion_map[][] is printed line by line, and the replacemnet of
'+' with spaces is done here.
	further note: when looking at the map on the terminal the bottom of the map
is up (its upside down), this is not fixed becouse doing it diferently would 
confuse me. By up I mean going positive in the y direction.

1.02
	not included, teacher said we did not have to have save/load in latter assig.
	this was off of teachers code for 1.01, could not get it working for my 1.01 

1.03
	the place it choses for right now is upper left corrner of room # 1.
	name of the .exe is dungion.exe, the binheap was not used

1.04
	The pc randomly moves around. the tunnaling monsters turn all tiles they run
	over into hallways, I did not get around to making that fix.
	There is a max turns variable in the run game function set to 5000; 
	this is just in case non of the monsters are telpathic and none of them kill
	the pc in a reasonable time.
	implemented the --nummon for temanal input which if followed by a number will
	make the function have that many monsters (defult 5)