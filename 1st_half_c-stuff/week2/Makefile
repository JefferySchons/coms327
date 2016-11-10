fc: f2c.o c2f.o main.o
	gcc f2c.o c2f.o main.o -o fc

f2c.o: f2c.c cf.h
	gcc -ggdb -Wall f2c.c -c

c2f.o: c2f.c cf.h
	gcc -ggdb -Wall c2f.c -c

main.o: main.c cf.h
	gcc -ggdb -Wall main.c -c

clean:
	rm -f *.o fc *~
