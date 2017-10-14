#include "base_objects.hpp"
#include "orders.hpp"

class Unit : 	virtual public Object, 
				virtual public Able_to_attack
				virtual public Able_to_move
				virtual public order::Subordinate{
private:
	int health;
	
public:
	
};

int main(){
	
	return 0;
}