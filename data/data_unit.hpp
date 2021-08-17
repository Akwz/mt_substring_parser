#pragma once

#include <memory>

#include "shared_storage.hpp"

namespace data
{

template<typename DataType>
class DataUnit
{
public:

private:
	std::shared_ptr<SharedStorage<DataType>> mDataStorage;

};

} // namespace data
