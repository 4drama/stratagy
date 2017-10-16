all: ord base

ord:
	make -f orders.mak
	
base:
	make -f base_objects.mak
	
clean:
	make -f orders.mak clean
	make -f base_objects.mak clean