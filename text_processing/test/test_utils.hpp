#include <string>


namespace test_utils
{

class MockDataProvider
{
public:
	MockDataProvider() = default;
	MockDataProvider(MockDataProvider&& provider) = default;
	~MockDataProvider() = default;

	std::string Fetch(){return std::string();}
private:
};

} // namespace test_utils