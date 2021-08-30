#include <string>


namespace test_utils
{



class MockDataProvider
{
public:
	struct Options
	{
		// Size of data per Fetch() call
		size_t max_fetch_size{0};
	};

	using IteratorType = std::string::const_iterator;

	MockDataProvider(const Options& options, std::string&& buffer);
	MockDataProvider(MockDataProvider&& provider)
		: mOptions(std::move(provider.mOptions))
		, mBuffer(std::move(provider.mBuffer))
		, mCurrentPosition(mBuffer.cbegin())
	{}
	~MockDataProvider() = default;

	std::string Fetch();
private:
	Options mOptions;
	std::string mBuffer;
	IteratorType mCurrentPosition;
};

} // namespace test_utils