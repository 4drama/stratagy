#ifndef ORDERS__
#define ORDERS__

#include "geometry.hpp"
#include "base_objects.hpp"
#include "order_info.hpp"

class Object;

namespace order{
	
	class Order;
	
	class Subordinate : virtual public Object{
		using Point = geometry::Point;
	private:
		Order *porder;
		
	public:
		
		virtual ~Subordinate() = default;
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
		
		virtual INFO Check(Subordinate *current) = 0;
		
	//	virtual ~Order() = 0;
	};

	class Attack : public Order{
	private:
		Object *target;
	public:
		Attack(const Attributes&& entry);
	
		order::INFO Check(Subordinate *current) override;
	};
	
	class Hold : public Order{
		using Point = geometry::Point;
	private:
		Point position;
		
	public:
		Hold(const Attributes&& entry);
		
		INFO Check(Subordinate *current) override;
	};

	class Move : public Order{
		using Point = geometry::Point;
	private:
		Point position;
		
	public:
		Move(const Attributes&& entry);
		
		INFO Check(Subordinate *current) override;
	};

	class Follow : public Order{
	private:
		Object *target;
	public:
		Follow(const Attributes&& entry);
		
		INFO Check(Subordinate *current) override;
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
		
		INFO Check(Subordinate *current) override;
	};
	
}
#endif