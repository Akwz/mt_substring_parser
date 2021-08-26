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
				true,
				it < mDataView.Begin() + mMask.size(),
				false
			};

			ProcessNewLayer(new_layer, it, mDataView.End());
			new_layer.ends_in_layering_range = it - mDataView.Begin() <= mMask.size();
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

bool TextParser::Compare(char mask, char symbol) const
{
	return symbol != '\n' && (mask == '?' || mask == symbol);
}

} // namespace text_processing