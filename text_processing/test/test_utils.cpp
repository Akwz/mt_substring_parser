#include <iostream>

#include "test_utils.hpp"


namespace test_utils
{

MockDataProvider::MockDataProvider(const Options& options, std::string&& buffer)
	: mOptions(options)
	, mBuffer(buffer)
	, mCurrentPosition(mBuffer.cbegin())
{
}

std::string MockDataProvider::Fetch()
{
	std::string result;
	if(mCurrentPosition < mBuffer.cend())
	{
		size_t fetch_size = std::min<size_t>(
			mBuffer.cend() - mCurrentPosition,
			mOptions.max_fetch_size
		);
		result = std::string(mCurrentPosition, mCurrentPosition + fetch_size);
		mCurrentPosition += fetch_size;
	}
	return result;
}

} // namespace test_utils