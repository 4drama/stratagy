all:
	make ord
	make base
	make geom
	g++ -c -Wall -std=c++14 ordertest.cpp -o ordertest.o
	g++ -o ordertest.exe ordertest.o base_objects.o new_orders.o geometry.o order_subordinate.o

clean:
	rm ordertest.o
	rm ordertest.exe