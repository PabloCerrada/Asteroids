#include "Manager.h"

Manager::Manager(): entsByGroup_() 
{
	for(auto& groupEntities : entsByGroup_)
	{
		groupEntities.reserve(100);
	}
}


Manager::~Manager() {
	for (auto& ents : entsByGroup_)
	{
		for(auto e : ents)
		{
			delete e;
		}
	}
}


void Manager::refresh()
{
	for (grpId_type gId = 0; gId < maxGroupId; gId++) {
		auto& grpEnts = entsByGroup_[gId];
		grpEnts.erase(
			std::remove_if(grpEnts.begin(), grpEnts.end(),
				[](Entity* e) {
					if (e->isAlive()) {
						return false;
					}
					else {
						delete e;
						return true;
					}
				}),
			grpEnts.end());
	}
}

void Manager::update() 
{
	for(auto& sys : sys_)
	{
		if (sys != nullptr)
		{
			sys->update();
		}
	}
}



