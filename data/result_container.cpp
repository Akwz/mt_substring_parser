#include <iostream>

#include "result_container.hpp"

namespace data
{

template<>
void ResultContainer<size_t>::Merge(const ResultContainer<size_t>& other)
{
	mValue += other.Value();
}

} // namespace data