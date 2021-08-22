#pragma once

#include <functional>
#include <vector>

namespace activity
{

template<typename ResultType>
struct Activity
{
	std::vector<std::function<ResultType()>> tasks;
	ResultType result;
};

} // namespace activity