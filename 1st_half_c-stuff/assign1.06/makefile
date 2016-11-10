all: dungion cplus comp clean

dungion: dungion_make_dungion.c
	gcc -c -Wall -Werror -ggdb dungion_make_dungion.c -o dungion_make.o -lncurses

cplus: dungion_cpp.cpp
	g++ -c -Wall -Werror -ggdb dungion_cpp.cpp -o dungion_cpp.o -lncurses

comp: dungion_cpp.o dungion_make.o
	g++ dungion_cpp.o dungion_make.o -o dungion -lncurses

clean:
	rm *.o