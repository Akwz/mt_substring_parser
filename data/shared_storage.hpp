#pragma once

#include <memory>


namespace data
{

template<typename DataType>
class SharedStorage
{
public:

private:
	SharedStorage() = delete;
	SharedStorage(const SharedStorage&) = delete;
	SharedStorage& operator=(const SharedStorage&) = delete;

	std::unique_ptr<DataType> mData;
};

} // data