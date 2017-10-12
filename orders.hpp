#ifndef ORDERS__
#define ORDERS__

#include "geometry.hpp"
#include "objects.hpp"

class Object;
class Unit;

namespace order{

	enum class INFO{
		IS_DONE,
		EXERCISE,
		IMPOSSIBLY
	};
	
	struct Attributes{
		using Point = geometry::Point;
		Point firstPosition;
		Point secondPosition;
		Object *target;
	};

	class Order{
	protected:
		Order(){
			
		};
	//	Order(const Attributes& entry);
		
		virtual INFO Do(Unit *current) = 0;
		
	//	virtual ~Order() = 0;
	};

	class Attack : public Order{
	private:
		Object *target;
	public:
		Attack(const Attributes&& entry);
	
		INFO Do(Unit *current) override;
	};
	
	class Hold : public Order{
		using Point = geometry::Point;
	private:
		Point position;
		
	public:
		Hold(const Attributes&& entry);
		
		INFO Do(Unit *current) override;
	};

	class Move : public Order{
		using Point = geometry::Point;
	private:
		Point position;
		
	public:
		Move(const Attributes&& entry);
		
		INFO Do(Unit *current) override;
	};

	class Follow : public Order{
	private:
		Object *target;
	public:
		Follow(const Attributes&& entry);
		
		INFO Do(Unit *current) override;
	};
	
	class Patrol : public Order{
		using Point = geometry::Point;
	private:
		Point firstPosition;
		Point secondPosition;
		
		Point *currentPosition;
		
		void swapPosition();
		
	public:
		Patrol(const Attributes&& entry);
		
		INFO Do(Unit *current) override;
	};
	
}
#endif