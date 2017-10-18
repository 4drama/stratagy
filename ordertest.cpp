#include "base_objects.hpp"
#include "orders.hpp"
#include "geometry.hpp"

class Unit : 	virtual public Object, 
		virtual public Able_to_see,
		virtual public Able_to_attack,
		virtual public Able_to_move,
		virtual public Able_to_destroy,
		virtual public order::Subordinate{
private:
	
public:
	Unit(		ObjectsRoster *zone_, double speed_, double attackRange_, 
			double visibilityRange_, geometry::Point coordinate_)
			:	Object(coordinate_),
				Able_to_attack(attackRange_, zone_, visibilityRange_, coordinate_),
				Able_to_move(speed_, zone_, visibilityRange_, coordinate_),
				Able_to_destroy(coordinate_),
				Able_to_see(zone_, visibilityRange_, coordinate_),
				order::Subordinate(coordinate_){	
	}
};

int main(){
	ObjectsRoster roster;
	
	roster.Add(Unit(&roster, 1.5, 9, 12, geometry::Point{250, 250}));
	roster.Add(Unit(&roster, 1.5, 9, 12, geometry::Point{260, 250}));
	roster.Add(Unit(&roster, 1.5, 9, 12, geometry::Point{400, 250}));
	roster.Add(Unit(&roster, 1.5, 9, 12, geometry::Point{200, 150}));
	
	return 0;
}