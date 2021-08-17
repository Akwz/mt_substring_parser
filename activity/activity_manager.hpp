#pragma once

// #include "data_unit.hpp"
#include "activity_dispatcher.hpp"

namespace activity
{

class IActivityManager
{
public:
	virtual void Process(/* DataUnit& input, DataUnit& output */) = 0;

private:

};

} // namespace activity