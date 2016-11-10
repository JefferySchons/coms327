all: dungion clean

dungion: dungion_make_dungion.cpp dungion_cpp.cpp
	g++ -Wall -Werror -Wno-write-strings -ggdb dungion_make_dungion.cpp dungion_cpp.cpp -o dungion -lncurses


clean:
	rm *.o