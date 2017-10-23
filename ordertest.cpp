#include "base_objects.hpp"
#include "order_subordinate.hpp"
#include "geometry.hpp"

#include <iostream>

class Unit : 	virtual public Object, 
				virtual public Able_to_see,
				virtual public Able_to_attack,
				virtual public Able_to_move,
				virtual public Able_to_destroy,
				virtual public order::Subordinate{
private:
	
public:
	Unit(const ObjectAttributes *attr)
			:	Object(attr),
				Able_to_attack(attr),
				Able_to_move(attr),
				Able_to_destroy(attr),
				Able_to_see(attr),
				order::Subordinate(attr){	
	}
	
	void Tick(float time) override{
		order::Subordinate::Update(time);
	}
};

int main(){
	ObjectsRoster roster;
	float time = 0.1;
	
	ObjectAttributes base;
	base.zone		=	&roster;
	base.visibilityRange	=	12;
	base.speed		=	1.5;
	base.attackRange	=	9;
	
	roster.Add(std::make_unique<Unit>(base.setCoordinate(geometry::Point{215, 230})));
	roster.Add(std::make_unique<Unit>(base.setCoordinate(geometry::Point{280, 250})));
	roster.Add(std::make_unique<Unit>(base.setCoordinate(geometry::Point{260, 255})));
	roster.Add(std::make_unique<Unit>(base.setCoordinate(geometry::Point{265, 254})));
	
	roster.Tick(time);
	
	return 0;
}