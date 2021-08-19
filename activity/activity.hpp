#pragma once

#include <functional>
#include <vector>

namespace activity
{

class ActivityResult{};

struct Activity
{
	std::vector<std::function<void()>> tasks;
	ActivityResult result;
};

} // namespace activity