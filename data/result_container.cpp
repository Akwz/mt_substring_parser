#include "result_container.hpp"

namespace data
{

template<>
void ResultContainer<int>::Merge(const ResultContainer<int>& other)
{
	mValue += other.Value();
}

} // namespace data