all:
	g++ -c -Wall -std=c++14 new_orders.cpp -o new_orders.o

clean:
	rm new_orders.o