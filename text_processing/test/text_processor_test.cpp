#include "gtest/gtest.h"
#include "text_processor.hpp"

TEST(DummySuite, SimpleDummyTest)
{
	const std::string fname("test_data/light_test_file");
	text_processing::TextProcessor processor{fname, "ha?"};
	auto result = processor.Process().GetValue();
	EXPECT_EQ(result.size(), 0);
}
