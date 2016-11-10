all: dungion clean

dungion: dungion_make_dungion.cpp dungion_cpp.cpp
	g++ -Wall -Werror -ggdb dungion_make_dungion.cpp dungion_cpp.cpp -o dungion -lncurses


clean:
	rm *.o