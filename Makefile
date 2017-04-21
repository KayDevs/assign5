CC = g++

dynpipe TwoPipesTwoChildren TwoPipesThreeChildren onepipe: dynpipe.o TwoPipesTwoChildren.o TwoPipesThreeChildren.o onepipe.o
	$(CC) -o dynpipe dynpipe.o
	$(CC) -o TwoPipesTwoChildren TwoPipesTwoChildren.o
	$(CC) -o TwoPipesThreeChildren TwoPipesThreeChildren.o
	$(CC) -o onepipe onepipe.o

dynpipe.o TwoPipesTwoChildren.o TwoPipesThreeChildren.o onepipe.o: DynPipe.cpp TwoPipesTwoChildren.cpp TwoPipesThreeChildren.cpp onepipe.cpp
	$(CC) -o dynpipe.o -c DynPipe.cpp
	$(CC) -o TwoPipesTwoChildren.o -c TwoPipesTwoChildren.cpp
	$(CC) -o TwoPipesThreeChildren.o -c TwoPipesThreeChildren.cpp
	$(CC) -o onepipe.o -c onepipe.cpp

clean:
	rm *.o
	rm onepipe dynpipe TwoPipesTwoChildren TwoPipesThreeChildren
