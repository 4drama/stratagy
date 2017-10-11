#ifndef ORDERS__
#define ORDERS__

#include "geometry.hpp"
#include "objects.hpp"

namespace order{

	enum class INFO{
		IS_DONE,
		EXERCISE,
		IMPOSSIBLY
	};
	
	class Object;
	class Unit;
	
	struct Attributes{
		using geometry::Point Point;
		Point firstPosition;
		Point secondPosition;
		Object *target;
	}

	class Order{
	private:
		Order() = delete;
		Order(const Attributes& entry);
		
		virtual INFO Do(Unit *current);
		
		~Order() = 0;
	};

	class Attack : public Order{
	private:
		Object *target;
	public:
		Attack(const Attributes& entry)
			: target(entry.target){
		}
	
		INFO Do(Unit *current) override{
			double range = geometry::Range(	current->CoordinateGet(), 
											target->CoordinateGet())
			
			INFO status;
			
			if(	range < current->gatRange()){
				status = current->Attack(target);
				return status == INFO::IS_DONE ? INFO::IS_DONE : INFO::EXERCISE
			} else {
				status = current->Move(target->CoordinateGet());
				return status == INFO::IMPOSSIBLY ? 
						INFO::IMPOSSIBLY : INFO::EXERCISE
			}
		}
	}

	class Hold : public Order{
		using geometry::Point Point;
	private:
		Point position;
	}

	class Move : public Order{
		using geometry::Point Point;
	private:
		Point position;
	}

	class Follow : public Order{
	private:
	
	}
	
	class Patrol : public Order{
		using geometry::Point Point;
	private:
		Point firstPosition;
		Point secondPosition;
	}
	
}
#endif