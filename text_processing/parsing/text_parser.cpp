#include <cassert>
#include <sstream>

#include "text_parser.hpp"


namespace text_processing
{

constexpr char LAYER_SEPARATOR{'\n'};

void Layer::Merge(const Layer& other, size_t layering, size_t appearence_size)
{
	size_t layering_offset = length > 0 ? layering : 0;
	for(auto it = other.appearences.cbegin(); it != other.appearences.cend(); ++it)
	{ 
		size_t distance = it->first + length - layering_offset;
		const auto& word = it->second;
		if(appearences.empty() || (distance >= appearences.crbegin()->first + appearence_size))
		{
			appearences.insert(std::pair<size_t, std::string>(distance, word));
		}
	}
	length += (other.length - layering_offset);
}

void Layer::Reset()
{
	appearences.clear();
	length = 0;
	incomplete_from_end = false;
	stars_in_layering_range = false;
	ends_in_layering_range = false;
}

void ParsingResult::Merge(const ParsingResult& other)
{
	assert(mLayeringSize == other.mLayeringSize);
	for(const auto& entry : other.mResultSequence)
	{
		mResultSequence[entry.first] = entry.second;
	}
}

void ParsingResult::SetLayering(size_t layering)
{
	mLayeringSize = layering;
}

std::string ParsingResult::GetValue() const
{
	std::ostringstream result;
	std::vector<Layer> layers;
	GetMergedLayerSequence(layers);
	for(size_t i = 0; i < layers.size(); ++i)
	{
		if(!layers[i].appearences.empty())
		{
			for(const auto& entry : layers[i].appearences)
			{
				result << i + 1 << " " << entry.first << " " << entry.second << "\n";
			}
		}
	}
	return result.str();
}

void ParsingResult::GetMergedLayerSequence(std::vector<Layer>& merged_layers) const
{
	using SeqIteratorType = std::map<size_t, std::vector<Layer>>::const_iterator;
	if(!mResultSequence.empty())
	{
		Layer current_line{{}, 0, false, false, false};
		size_t ghost_layering{0};
		for(SeqIteratorType it = mResultSequence.cbegin(); it != mResultSequence.cend(); ++it)
		{
			const auto& layers = it->second;
			for(auto l_it = layers.cbegin(); l_it != layers.cend(); ++l_it)
			{
				bool is_ghost_layer = l_it->stars_in_layering_range
					&& l_it->ends_in_layering_range
					&& (!merged_layers.empty() || !current_line.appearences.empty());
				if(is_ghost_layer)
				{
					ghost_layering += (l_it->length + 1);
				}
				else
				{
					current_line.Merge(*l_it, mLayeringSize - ghost_layering, mLayeringSize);
					if(!l_it->incomplete_from_end || (++SeqIteratorType(it) == mResultSequence.cend() && l_it + 1 == layers.cend()))
					{
						merged_layers.emplace_back(std::move(current_line));
						current_line.Reset();
					}
					ghost_layering = 0;
				}
			}
		}
	}
}

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
	while(position != bound && *position != LAYER_SEPARATOR)
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

	if(position != bound && *position == LAYER_SEPARATOR)
	{
		result.incomplete_from_end = false;
		++position;
	}
	mMask.Reset();
}

} // namespace text_processing