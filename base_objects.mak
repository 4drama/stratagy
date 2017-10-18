all:
	g++ -c -Wall -std=c++14 base_objects.cpp -o base_objects.o

clean:
	rm base_objects.o