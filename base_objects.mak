all:
	g++ -c -Wall -std=c++11 base_objects.cpp -o base_objects.o

clean:
	rm base_objects.o