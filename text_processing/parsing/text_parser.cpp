#include <stack>
#include <iostream>

#include "text_parser.hpp"


namespace text_processing
{

TextParser::TextParser(const TextDataView& data_view, const std::string& mask)
	: mDataView(data_view)
	, mMask(mask)
{
}

void TextParser::Parse(std::vector<Layer>& result)
{
	if(!mDataView.Empty())
	{
		auto mask_iter = mMask.cbegin();
		size_t new_layer_distance{0};
		TextDataView::IteratorType it = mDataView.Begin();
		for(size_t i = 0; i < mDataView.Size(); ++i)
		{
			it = mDataView.Begin() + i;
			if(result.empty())
			{
				result.emplace_back(
					Layer{
						std::vector<Appearence>(),
						0,
						0,
						true,
						true
					}
				);
			}
			
			if(*it == '\n')
			{
				result.back().length = it - mDataView.Begin() - new_layer_distance;
				result.back().incomplete_from_end = false;
				result.emplace_back(
					Layer{
						std::vector<Appearence>(),
						i + 1,
						0,
						false,
						true
					}
				);
				new_layer_distance = i + 1;
			}

			if(Compare(*mask_iter, *it))
			{
				if(mask_iter + 1 == mMask.end())
				{
					std::string found_word{it - mMask.size() + 1, it + 1};
					size_t distance = i - result.back().distance - mMask.size() + 1;
					result.back().appearences.emplace_back(Appearence{found_word, distance});
					mask_iter = mMask.cbegin();
				}
				else
				{
					++mask_iter;
				}

			}
			else if(mask_iter != mMask.cbegin())
			{
				mask_iter = mMask.cbegin();
			}
		}
		result.back().length = it - mDataView.Begin() - new_layer_distance;
	}
}

bool TextParser::Compare(char mask, char symbol)
{
	return symbol != '\n' && (mask == '?' || mask == symbol);
}

} // namespace text_processing