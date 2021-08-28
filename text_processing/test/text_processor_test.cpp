#include <tuple>
#include <string>

#include "gtest/gtest.h"
#include "text_processor.hpp"
#include "text_provider.hpp"
#include "test_utils.hpp"

using namespace test_utils;
using namespace text_processing;

/*
0 - options
1 - text buffer
2 - mask
3 - expected
*/
using ParamsType = std::tuple<MockDataProvider::Options, std::string, std::string, std::string>;

class ParsingInputFixture : public testing::TestWithParam<ParamsType> {};

TEST_P(ParsingInputFixture, ParsingTest)
{
	auto test_param = GetParam();
	auto provider = MockDataProvider(std::get<0>(test_param), std::move(std::get<1>(test_param)));
	TextProcessor<MockDataProvider> processor{std::move(provider), std::get<2>(test_param)};
	EXPECT_EQ(processor.Process().GetValue(), std::get<3>(test_param));
}

INSTANTIATE_TEST_SUITE_P(
	Parsing,
	ParsingInputFixture,
	testing::Values(
		ParamsType(
			MockDataProvider::Options{100},

			"hello this is test text, please - test it! =) 289054kladjgnp ;povmvqdcmjsp" \
			"second line ads'gjag[ ia[aijg[",

			"la?",

			"1 53 lad\n"
		)
	)
);