#pragma once

#include <functional>
#include <vector>

namespace activity
{

// class ActivityResult{};

template<typename ResultType>
struct Activity
{
	std::vector<std::function<ResultType()>> tasks;
	ResultType result;
};

} // namespace activity