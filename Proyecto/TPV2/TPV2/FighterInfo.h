#pragma once
#include <string>
#include "Component.h"

using namespace std;
class FighterInfo : public Component
{
private:
	char id;
public:

	FighterInfo();

	inline void setName(char name) { id = name; }

	inline char getName() { return id; }

};

