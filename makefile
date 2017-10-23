all: ord base

geom:
	make -f geometry.mak
	
ord:
	make -f order_subordinate.mak
	make -f new_orders.mak

testord:
	make -f ordertest.mak
	
base:
	make -f base_objects.mak
	
clean:
	make -f geometry.mak clean
	make -f order_subordinate.mak clean
	make -f orders.mak clean
	make -f base_objects.mak clean
	make -f ordertest.mak clean