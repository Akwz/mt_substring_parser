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
		for(auto it = mDataView.Begin(); it != mDataView.End();)
		{
			Layer new_layer{
				{},
				0,
				it == mDataView.Begin(),
				true
			};

			ProcessNewLayer(new_layer, it, mDataView.End());
			result.emplace_back(std::move(new_layer));
		}
	}
}

void TextParser::ProcessNewLayer(Layer& result, IteratorType& position, const IteratorType& bound) const
{
	auto mask_iter = mMask.cbegin();
	size_t curr_position_offset{0};
	while(position != bound && *position != mLayerSeparator)
	{
		if(Compare(*mask_iter, *position))
		{
			if(mask_iter + 1 == mMask.cend())
			{
				result.appearences.insert(std::pair<size_t, std::string>(curr_position_offset - mMask.size() + 1, std::string(position - mMask.size() + 1, position + 1)));
				mask_iter = mMask.cbegin();
			}
			else
			{
				++mask_iter;
			}
		}
		else
		{
			mask_iter = mMask.cbegin();
		}

		++position;
		++curr_position_offset;
		++result.length;
	}

	if(position != bound && *position == mLayerSeparator)
	{
		result.incomplete_from_end = false;
		++position;
	}
}

// void TextParser::Parse(std::vector<Layer>& result)
// {
// 	if(!mDataView.Empty())
// 	{
// 		auto mask_iter = mMask.cbegin();
// 		size_t new_layer_distance{0};
// 		TextDataView::IteratorType it = mDataView.Begin();
// 		for(size_t i = 0; i < mDataView.Size(); ++i)
// 		{
// 			it = mDataView.Begin() + i;
// 			if(result.empty())
// 			{
// 				result.emplace_back(
// 					Layer{
// 						std::map<size_t, std::string>(),
// 						0,
// 						true,
// 						true
// 					}
// 				);
// 			}

// 			if(*it == '\n')
// 			{
// 				result.back().length = it - mDataView.Begin() - new_layer_distance;
// 				result.back().incomplete_from_end = false;
// 				result.emplace_back(
// 					Layer{
// 						std::map<size_t, std::string>(),
// 						0,
// 						false,
// 						true
// 					}
// 				);
// 				new_layer_distance = i + 1;
// 			}
// 			// std::cout << ((*it) + " ");
// 			if(Compare(*mask_iter, *it))
// 			{
// 				// std::cout << (std::to_string(i) + " " + *it + "\n");
// 				if(mask_iter + 1 == mMask.cend())
// 				{
// 					std::string found_word{it - mMask.size() + 1, it + 1};
// 					// std::cout << ("Found: " + found_word + "\n");
// 					size_t distance = i - new_layer_distance;
// 					result.back().appearences.insert(std::make_pair<size_t, std::string>(std::move(distance), std::move(found_word)));
// 					mask_iter = mMask.cbegin();
// 				}
// 				else
// 				{
// 					++mask_iter;
// 				}

// 			}
// 			else if(mask_iter != mMask.cbegin())
// 			{
// 				mask_iter = mMask.cbegin();
// 			}
// 		}
// 		if(!result.empty()) result.back().length = (*it != '\n') ? it - mDataView.Begin() - new_layer_distance + 1 : it - mDataView.Begin() - new_layer_distance;
// 	}
// }

bool TextParser::Compare(char mask, char symbol) const
{
	return symbol != '\n' && (mask == '?' || mask == symbol);
}

} // namespace text_processing