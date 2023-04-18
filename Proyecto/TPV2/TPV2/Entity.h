#pragma once
#include "Component.h"
#include "ecs.h"
#include <array>
#include <vector>
#include <bitset>
#include <iostream>

using namespace std;

class Entity
{
private:
	friend Manager;
	grpId_type gId_;
	Manager* mngr_;
	vector<Component*> currCmps_;
	array<Component*, maxComponentId> cmps_;
	std::bitset<maxGroupId> groups_;
public:
	bool alive_;
	Entity(grpId_type gId) :mngr_(nullptr), cmps_(), currCmps_(), alive_() {
		gId_ = gId;
		currCmps_.reserve(maxComponentId);
	}
	inline void setContext(Manager* mngr) { mngr_ = mngr; }

	virtual ~Entity() {
		for (auto c : currCmps_) {
			delete c;
		}
	}

	inline bool isAlive() {
		return alive_;
	}
};

