all:
	g++ -c -Wall -std=c++11 orders.cpp -o orders.o

clean:
	rm orders.o