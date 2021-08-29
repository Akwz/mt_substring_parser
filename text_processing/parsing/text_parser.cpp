#include <stack>
#include <iostream>

#include "text_parser.hpp"


namespace text_processing
{

TextParser::TextParser(const TextDataView& data_view, const MaskView& mask)
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
				it < mDataView.Begin() + mMask.Size(),
				false
			};

			ProcessNewLayer(new_layer, it, mDataView.End());
			new_layer.ends_in_layering_range = it - mDataView.Begin() <= mMask.Size();
			result.emplace_back(std::move(new_layer));
		}
	}
}

void TextParser::ProcessNewLayer(Layer& result, IteratorType& position, const IteratorType& bound)
{
	size_t curr_position_offset{0};
	while(position != bound && *position != mLayerSeparator)
	{
		if(mMask.Compare(*position))
		{
			if(mMask.IsReachedEnd())
			{
				result.appearences.insert(std::pair<size_t, std::string>(curr_position_offset - mMask.Size() + 1, std::string(position - mMask.Size() + 1, position + 1)));
				mMask.Reset();
			}
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