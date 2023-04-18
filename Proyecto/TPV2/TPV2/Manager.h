#pragma once
#include <vector>
#include "Entity.h"
#include "src/utils/Singleton.h"
#include "System.h"
#include "GameStateMachine.h"
using namespace std;
class Manager :public Singleton <Manager> {
protected:
	string id;
	vector<Entity*> ents_;
	array<Entity*, maxHandlerId> hdlrs_;
	array<std::vector<Entity*>, maxGroupId> entsByGroup_;
	array<System*, maxSystemId> sys_;

	vector<Message> msgs_;
	vector<Message>aux_msgs_;
public:
	Manager();
	virtual ~Manager();
	inline Entity* addEntity(grpId_type gId) 
	{
		Entity* e = new Entity(gId);
		setAlive(e, true);
		e->setContext(this);
		entsByGroup_[gId].push_back(e);
		return e;
	}		
	inline const auto& getEntitiesByGroup(grpId_type gId)
	{
		return entsByGroup_[gId];
	}				
	inline void setHandler(hdlrId_type hId, Entity* e) {
		hdlrs_[hId] = e;
	}
	inline Entity* getHandler(hdlrId_type hId) 
	{
		return hdlrs_[hId];
	}
	virtual void update();
	virtual void refresh();
	inline const auto& getEntities() { return ents_; }
	
	template<typename T>
	inline void removeComponent(Entity* e) {
		constexpr cmpId_type cId = T::id;
		if (e->cmps_[cId] != nullptr) {
			auto iter = find(e->currCmps_.begin(),
				e->currCmps_.end(),
				e->cmps_[cId]);
			e->currCmps_.erase(iter);
			delete e->cmps_[cId];
			e->cmps_[cId] = nullptr;
		}
	}
	template<typename T, typename ...Ts>
	inline T* addComponent(Entity* e, Ts&&...args) {
		T* c = new T(forward<Ts>(args)...);
		constexpr cmpId_type cId = T::id;
		removeComponent<T>(e);
		e->currCmps_.push_back(c);
		e->cmps_[cId] = c;
		e->setContext(this);
		e->cmps_[cId]->initComponent();
		//e->initComponent();
		return c;
	}
	inline void send(const Message& m) {
		for (System* s : sys_) {
			if (s != nullptr)
			{
				s->receive(m);
			}
				
		}
	}
	template<typename T>
	inline bool hasComponent(Entity* e) {
		constexpr cmpId_type cId = T::id;
		return e->cmps_[cId] != nullptr;
	}
	template<typename T>
	inline T* getComponent(Entity* e) {
		constexpr cmpId_type cId = T::id;
		return static_cast<T*>(e->cmps_[cId]);
	}
	inline void setAlive(Entity* e, bool alive) {
		e->alive_ = alive;
	}
	inline bool isAlive(Entity* e) {
		return e->alive_;
	}
	inline grpId_type groupId(Entity* e) {
		return e->gId_;
	}

	template<typename T, typename ...Ts>
	inline T* addSystem(Ts&&...args) {
		constexpr sysId_type sId = T::id;
		//removeSystem<T>();
		System* s = new T(forward<Ts>(args)...);
		s->setContext(this);
		s->initSystem();
		sys_[sId] = s;
		return static_cast<T*>(s);
	}
	template<typename T>
	inline T* getSystem() {
		constexpr sysId_type sId = T::id;
		return static_cast<T*>(sys_[sId]);
	}
	template<typename T>
	inline void removeSystem() {
		constexpr sysId_type sId = T::id;
		if (sys_[sId] != nullptr) {
			delete sys_[sId];
			sys_[sId] = nullptr;
		}
	}
	inline void flushMessages() {
		swap(msgs_, aux_msgs_);
		for (auto& m : aux_msgs_) {
			for (System* s : sys_) {
				if (s != nullptr)
					s->receive(m);
			}
		}
		aux_msgs_.clear();
	}
	inline string getStateId() { return id; } // Return the id state
};

