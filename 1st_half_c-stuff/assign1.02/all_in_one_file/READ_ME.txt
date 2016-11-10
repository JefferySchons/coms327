The map is made and printed in the dungion_map function and is all in the
dungion_make_dunfion.c file. All the main does is call the dungion_map function.
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