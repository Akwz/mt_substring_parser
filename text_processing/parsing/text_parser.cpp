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
						std::map<size_t, std::string>(),
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
						std::map<size_t, std::string>(),
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
					size_t distance = i - new_layer_distance;
					result.back().appearences.insert(std::make_pair<size_t, std::string>(std::move(distance), std::move(found_word)));
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
		if(!result.empty()) result.back().length = (*it != '\n') ? it - mDataView.Begin() - new_layer_distance + 1 : it - mDataView.Begin() - new_layer_distance;
	}
}

bool TextParser::Compare(char mask, char symbol)
{
	return symbol != '\n' && (mask == '?' || mask == symbol);
}

} // namespace text_processing