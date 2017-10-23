#ifndef ORDER_SUBORDINATE__
#define ORDER_SUBORDINATE__

#include "base_objects.hpp"
#include "orders.hpp"

#include <list>
#include <memory>

struct ObjectAttributes;

namespace order{
	
	class Order;
	
	struct Attributes{
		using Point = geometry::Point;
		Point firstPosition;
		Point secondPosition;
		std::shared_ptr<Destructible_object> target;
	};
	
	class Subordinate : virtual public Object{
		using Order_ptr = std::unique_ptr<Order>;
	private:
		std::list<Order_ptr> orders;
		
		std::list<Order_ptr> CreateDefault();
		void ReplaceOrders(std::list<Order_ptr> &&new_);
	public:
		Subordinate() = delete;
		Subordinate(const ObjectAttributes *attr);
		Subordinate(const Subordinate&) = delete;
		Subordinate(Subordinate&&) = default;
		
		void AddOrder(Order_ptr &&next);
		void NewOrders(Order_ptr &&new_);
		void ResetOrders();
		
		void Update(float time);
		
		virtual ~Subordinate() = default;
	};
	
}

#endif