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
			MockDataProvider::Options{4},
			"abc",
			"abc",
			"1 0 abc\n"
		),
		ParamsType(
			MockDataProvider::Options{6},
			"abcabc",
			"abc",
			"1 0 abc\n" \
			"1 3 abc\n"
		),
		ParamsType(
			MockDataProvider::Options{4},
			"abcabc",
			"abc",
			"1 0 abc\n" \
			"1 3 abc\n"
		),
		ParamsType(
			MockDataProvider::Options{7},
			"abc\nabc",
			"abc",
			"1 0 abc\n" \
			"2 0 abc\n"
		),
		ParamsType(
			MockDataProvider::Options{4},
			"abc\nabc\nabc\nabc\nabc\nabc",
			"abc",
			"1 0 abc\n" \
			"2 0 abc\n" \
			"3 0 abc\n" \
			"4 0 abc\n" \
			"5 0 abc\n" \
			"6 0 abc\n"
		),
		ParamsType(
			MockDataProvider::Options{7},
			"abcabc\nabcabc\nabcabc\nabcabc",
			"abc",
			"1 0 abc\n" \
			"1 3 abc\n" \
			"2 0 abc\n" \
			"2 3 abc\n" \
			"3 0 abc\n" \
			"3 3 abc\n" \
			"4 0 abc\n" \
			"4 3 abc\n"
		)
	)
);