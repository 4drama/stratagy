#include "order_subordinate.hpp"

#include <list>
#include <memory>

namespace order{
	
//=======================SUBORDINATE=================================
	std::list<Subordinate::Order_ptr> Subordinate::CreateDefault(){
		typedef Subordinate::Order_ptr Order_ptr;
		std::list<Order_ptr> new_;
		Attributes setting;
		setting.firstPosition = this->CoordinateGet();
		Order_ptr order = std::make_unique<norder::Default>(std::move(setting));
		new_.push_front(std::move(order));
		return new_;
	}
	
	Subordinate::Subordinate(const ObjectAttributes *attr)
			:	Object(attr){
		this->orders = CreateDefault();
	}
	
	void Subordinate::AddOrder(Order_ptr &&next){
		this->orders.push_front(std::move(next));
	}
	
	void Subordinate::NewOrders(Order_ptr &&new_){
		this->orders = CreateDefault();
		this->AddOrder(std::move(new_));
	}
	
	void Subordinate::ResetOrders(){
		this->orders = CreateDefault();
	}
	
	void Subordinate::Update(float time){
		order::INFO status = this->orders.front()->Do(this, time);
		
		if(status == order::INFO::EXERCISE)
			this->orders.pop_front();
		
	/*	while(status != order::INFO::EXERCISE){
			this->orders.pop_front();
			status = this->orders.front()->Do(this, time);
		}*/
	}
//-------------------------------------------------------------------

}