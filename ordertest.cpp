#include "base_objects.hpp"
#include "order_subordinate.hpp"
#include "geometry.hpp"

#include <iostream>
#include <chrono>
#include <thread>

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
				Interacts_with_other(attr),
				Able_to_attack(attr),
				Able_to_move(attr),
				Able_to_destroy(attr),
				Able_to_see(attr),
				order::Subordinate(attr){	
	}
	
	void Tick(float time) override{
		std::cerr	<< "____________________________________" << std::endl;
		std::cerr 	<< "UNIT\tPosition:\t[" << this->CoordinateGet().x << ',' 
					<< this->CoordinateGet().y << ']' << std::endl 
					<< "\tHealth:\t\t" << this->GetCurrHealth() << '/' << this-> GetMaxHealth()
					<< std::endl << std::endl;
		std::cerr	<< "Order log:";
		order::Subordinate::Update(time);
		std::cerr << std::endl;
	}
};

int main(){
	ObjectsRoster roster;
	
	ObjectAttributes base;
	base.health				=	250;
	base.zone				=	&roster;
	base.visibilityRange	=	20;
	base.speed				=	1.5;
	base.attackSpeed		=	3000;
	base.attackRange		=	9;
	base.attackDamage		=	75;
	
	roster.Add(std::make_unique<Unit>(base.setCoordinate(geometry::Point{215, 230})));
	roster.Add(std::make_unique<Unit>(base.setCoordinate(geometry::Point{280, 250})));
	roster.Add(std::make_unique<Unit>(base.setCoordinate(geometry::Point{260, 255})));
	roster.Add(std::make_unique<Unit>(base.setCoordinate(geometry::Point{265, 254})));
	
	
	std::chrono::time_point<std::chrono::high_resolution_clock> t_old, t_new, t_del;
	std::chrono::seconds time_delay(1);
	
	t_new = std::chrono::high_resolution_clock::now();
	float time;
	t_old = t_del = t_new;
	while(true){
		if(t_del < (t_new = std::chrono::high_resolution_clock::now())){
			time = std::chrono::duration<float, std::milli>(t_new-t_old).count();
			t_old = t_new;
			
			roster.Tick(time);
			std::cout << "Turn end. Delay: " << time << '.' << std::endl << std::endl;
			
			t_del = t_old + time_delay;
		} else {
			std::this_thread::yield();
		}
	}
	
	
	return 0;
}