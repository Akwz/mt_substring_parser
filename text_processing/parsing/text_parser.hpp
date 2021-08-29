#pragma once

#include <vector>
#include <map>
#include <cassert>
#include <sstream>

#include "text_data_view.hpp"
#include "single_prefix_mask.hpp"


namespace text_processing
{

struct Layer
{
	/*
	first - distance from layer begin
	second - found word
	*/
	std::map<size_t, std::string> appearences;
	size_t length;
	bool incomplete_from_end;
	bool stars_in_layering_range;
	bool ends_in_layering_range;

	void Merge(const Layer& other, size_t layering, size_t appearence_size)
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

	void Reset()
	{
		appearences.clear();
		length = 0;
		incomplete_from_end = false;
		stars_in_layering_range = false;
		ends_in_layering_range = false;
	}
};

struct ParsingResult
{
	void Merge(const ParsingResult& other)
	{
		assert(mLayeringSize == other.mLayeringSize);
		for(const auto& entry : other.mResultSequence)
		{
			mResultSequence[entry.first] = entry.second;
		}
	}

	void SetLayering(size_t layering)
	{
		mLayeringSize = layering;
	}

	std::string GetValue()
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

	void GetMergedLayerSequence(std::vector<Layer>& merged_layers) const
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

	/*
	Raw data, need to merge layers before read,
	first - order of data chunk, second - layers(lines) of text
	*/
	std::map<size_t, std::vector<Layer>> mResultSequence;
	/*
	Layering size == mask size
	*/
	size_t mLayeringSize;
};

class TextParser
{
public:
	using IteratorType = TextDataView::IteratorType;

	TextParser(const TextDataView& data, const MaskView& mask);

	void Parse(std::vector<Layer>& result);
private:
	bool Compare(char mask, char symbol) const;
	void ProcessNewLayer(Layer& result, IteratorType& position, const IteratorType& bound);

	const TextDataView& mDataView;
	MaskView mMask;
	const char mLayerSeparator{'\n'};
};

} // namespace text_processing