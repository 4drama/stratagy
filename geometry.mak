all:
	g++ -c -Wall -std=c++11 geometry.cpp -o geometry.o

clean:
	rm geometry.o