#include "gtest/gtest.h"
#include "text_processor.hpp"
#include "text_provider.hpp"
#include "test_utils.hpp"

TEST(DummySuite, SimpleDummyTest)
{
	const std::string fname("test_data/light_test_file");
	auto provider = test_utils::MockDataProvider();
	text_processing::TextProcessor<test_utils::MockDataProvider> processor{std::move(provider), "ha?"};
	auto result = processor.Process().GetValue();
	EXPECT_EQ(result.size(), 0);
}
