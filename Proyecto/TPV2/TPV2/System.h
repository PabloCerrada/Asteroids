#pragma once
#include "Message.h"
#include "src/utils/Singleton.h"
class Manager;
class System : public Singleton<System>
{
	public:
		virtual ~System() { }
		void setContext(Manager* mngr) {
			mngr_ = mngr;
		}
		virtual void initSystem() { }	
		virtual void update() { }
		virtual void receive(const Message& m) { }
	protected:
		Manager* mngr_;
};

