#include <tuple>
#include <string>

#include "gtest/gtest.h"
#include "text_processor.hpp"
#include "text_provider.hpp"
#include "test_utils.hpp"

using namespace test_utils;
using namespace text_processing;

/*
0 - case description
1 - options
2 - text buffer
3 - mask
4 - expected
*/
using ParamsType = std::tuple<std::string, MockDataProvider::Options, std::string, std::string, std::string>;

class ParsingInputFixture : public testing::TestWithParam<ParamsType> {};

TEST_P(ParsingInputFixture, ParsingTest)
{
	auto test_param = GetParam();
	auto provider = MockDataProvider(std::get<1>(test_param), std::move(std::get<2>(test_param)));
	TextProcessor<MockDataProvider> processor{std::move(provider), std::get<3>(test_param)};
	EXPECT_EQ(processor.Process().GetValue(), std::get<4>(test_param)) << "Case: " << std::get<0>(test_param);
}

INSTANTIATE_TEST_SUITE_P(
	Parsing,
	ParsingInputFixture,
	testing::Values(
		ParamsType(
			"TextEqualsMask_NoNewLine_FetchSizeGreaterThanTextSize",
			MockDataProvider::Options{4},
			"abc",
			"abc",
			"1 0 abc\n"
		)
		// ParamsType(
		// 	"TextEqualsMultipleMask_NoNewLine_FetchSizeEqualsTextSize",
		// 	MockDataProvider::Options{6},
		// 	"abcabc",
		// 	"abc",
		// 	"1 0 abc\n" \
		// 	"1 3 abc\n"
		// ),
		// ParamsType(
		// 	"TextEqualsMultipleMask_NoNewLine_FetchSizeLowerThanTextSize",
		// 	MockDataProvider::Options{4},
		// 	"abcabc",
		// 	"abc",
		// 	"1 0 abc\n" \
		// 	"1 3 abc\n"
		// ),
		// ParamsType(
		// 	"TextEqualsMultipleMask_WithOneNewLine_FetchSizeEqualsTextSize",
		// 	MockDataProvider::Options{7},
		// 	"abc\nabc",
		// 	"abc",
		// 	"1 0 abc\n" \
		// 	"2 0 abc\n"
		// ),
		// ParamsType(
		// 	"TextEqualsMultipleMask_WithOneNewLine_FetchSizeLowerThanTextSize_FetchSizeGreaterThanLineSize",
		// 	MockDataProvider::Options{5},
		// 	"abc\nabc",
		// 	"abc",
		// 	"1 0 abc\n" \
		// 	"2 0 abc\n"
		// ),
		// ParamsType(
		// 	"TextEqualsMultipleMask_WithMultipleNewLine_FetchSizeLowerThanTextSize_FetchSizeEqualsLineSize",
		// 	MockDataProvider::Options{4},
		// 	"abc\nabc\nabc\nabc\nabc\nabc",
		// 	"abc",
		// 	"1 0 abc\n" \
		// 	"2 0 abc\n" \
		// 	"3 0 abc\n" \
		// 	"4 0 abc\n" \
		// 	"5 0 abc\n" \
		// 	"6 0 abc\n"
		// ),
		// ParamsType(
		// 	"TextEqualsMultipleMaskWithinEveryLine_WithMultipleNewLine_FetchSizeLowerThanTextSize_FetchSizeEqualsLineSize",
		// 	MockDataProvider::Options{7},
		// 	"abcabc\nabcabc\nabcabc\nabcabc",
		// 	"abc",
		// 	"1 0 abc\n" \
		// 	"1 3 abc\n" \
		// 	"2 0 abc\n" \
		// 	"2 3 abc\n" \
		// 	"3 0 abc\n" \
		// 	"3 3 abc\n" \
		// 	"4 0 abc\n" \
		// 	"4 3 abc\n"
		// ),
		// ParamsType(
		// 	"EmptyText",
		// 	MockDataProvider::Options{4},
		// 	"",
		// 	"abc",
		// 	""
		// ),
		// ParamsType(
		// 	"TextWithoutMask_NoNewLine_FetchSizeGreaterThanTextSize",
		// 	MockDataProvider::Options{4},
		// 	"cba",
		// 	"abc",
		// 	""
		// ),
		// ParamsType(
		// 	"TextWithMaskAtBegin_TextWithNonEqualMaskText_NoNewLine_FetchSizeLowerThanTextSize",
		// 	MockDataProvider::Options{4},
		// 	"abcdef",
		// 	"abc",
		// 	"1 0 abc\n"
		// ),
		// ParamsType(
		// 	"TextWithMaskAtBegin_TextWithNonEqualMaskText_NoNewLine_FetchSizeGreaterThanTextSize",
		// 	MockDataProvider::Options{7},
		// 	"abcdef",
		// 	"abc",
		// 	"1 0 abc\n"
		// ),
		// ParamsType(
		// 	"TextWithMaskAtTheEnd_TextWithNonEqualMaskText_NoNewLine_FetchSizeLowerThanTextSize",
		// 	MockDataProvider::Options{4},
		// 	"defabc",
		// 	"abc",
		// 	"1 3 abc\n"
		// ),
		// ParamsType(
		// 	"TextWithMaskAtTheEnd_TextWithNonEqualMaskText_NoNewLine_FetchSizeEqualsTextSize",
		// 	MockDataProvider::Options{4},
		// 	"defabc",
		// 	"abc",
		// 	"1 3 abc\n"
		// ),
		// ParamsType(
		// 	"TextWithMaskAtBegin_TextWithNonEqualMaskText_WithNewLine_FetchSizeLowerThanTextSize_FetchSizeEqualsLineSize",
		// 	MockDataProvider::Options{4},
		// 	"abc\ndef",
		// 	"abc",
		// 	"1 0 abc\n"
		// ),
		// ParamsType(
		// 	"TextWithMaskAtTheEnd_TextWithNonEqualMaskText_WithNewLine_FetchSizeLowerThanTextSize_FetchSizeEqualsLineSize",
		// 	MockDataProvider::Options{4},
		// 	"def\nabc",
		// 	"abc",
		// 	"2 0 abc\n"
		// ),
		// ParamsType(
		// 	"TextEqualsMultipleMask_TextWithNonEqualMaskText_WithNewLine_FetchSizeLowerThanTextSize_FetchSizeLowerThanLineSize",
		// 	MockDataProvider::Options{10},
		// 	"abcdefghijklmnopqrstuvwxyz\ndefghijklmnopabcqrstuvwxyz\ndefghijklmnopqrstuvwxyzabc\n",
		// 	"abc",
		// 	"1 0 abc\n" \
		// 	"2 13 abc\n" \
		// 	"3 23 abc\n"
		// ),
		// ParamsType(
		// 	"TextEqualMaskWithRegexpCharacters_WithNewLine",
		// 	MockDataProvider::Options{10},
		// 	"laaqwertylauzxcvblat\nasladfghlairetylamtuy",
		// 	"la?",
		// 	"1 0 laa\n" \
		// 	"1 9 lau\n" \
		// 	"1 17 lat\n" \
		// 	"2 2 lad\n" \
		// 	"2 8 lai\n" \
		// 	"2 15 lam\n"
		// ),
		// ParamsType(
		// 	"OnlyRegexMask_MaskSizeGreterThanLineSize_ExpectedCropNonMaskSuitableText",
		// 	MockDataProvider::Options{10},
		// 	"aeproigaf\nompahgore",
		// 	"?????",
		// 	"1 0 aepro\n" \
		// 	"2 0 ompah\n"
		// ),
		// ParamsType(
		// 	"PrefixMaskLayeredText",
		// 	MockDataProvider::Options{10},
		// 	"evevery",
		// 	"every",
		// 	"1 2 every\n"
		// )
	)
);