#pragma once

#include <vector>
#include <map>
#include <cassert>
#include <sstream>

#include "text_data_view.hpp"


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
	bool incomplete_from_begin;
	bool incomplete_from_end;

	void Merge(const Layer& other, size_t layering)
	{
		for(auto it = other.appearences.cbegin(); it != other.appearences.cend(); ++it)
		{
			auto distance = it->first + length - (length > 0 ? layering : 0);
			const auto& word = it->second;
			if(appearences.empty() || (distance - appearences.crbegin()->first >= layering))
			{
				appearences.insert(std::pair<size_t, std::string>(distance, word));
			}
		}
		length += (other.length - layering);
	}

	void Reset()
	{
		appearences.clear();
		length = 0;
		incomplete_from_begin = false;
		incomplete_from_end = false;
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
			Layer current_line{{}, 0, false, false};
			for(SeqIteratorType it = mResultSequence.cbegin(); it != mResultSequence.cend(); ++it)
			{
				const auto idx = it->first;
				const auto& layers = it->second;
				for(auto l_it = layers.cbegin(); l_it != layers.cend(); ++l_it)
				{
					current_line.Merge(*l_it, mLayeringSize);
					if(!l_it->incomplete_from_end || (++SeqIteratorType(it) == mResultSequence.cend() && l_it + 1 == layers.cend()))
					{
						merged_layers.emplace_back(std::move(current_line));
						current_line.Reset();
					}
				}
			}
		}
	}

	// std::string GetValue()
	// {
	// 	std::ostringstream result;
	// 	if(!mLayers.empty())
	// 	{
	// 		using LayerIterator = std::vector<Layer>::const_iterator;
	// 		LayerIterator current, prev;
	// 		auto current_layer = mLayers.cbegin();
	// 		current = prev = current_layer->second.cbegin();
	// 		std::string layer_result;
	// 		Layer real_line;
	// 		size_t real_lines_count;
	// 		size_t current_line_idx{1};
	// 		while(true)
	// 		{
	// 			if(current != prev)
	// 			{
	// 				bool prev_incomplete = (*prev).incomplete_from_end;
	// 				bool curr_incomplete = (*current).incomplete_from_begin;
	// 				bool part_of_prev = prev_incomplete && curr_incomplete;
	// 				assert((prev_incomplete && curr_incomplete) || (!prev_incomplete && !curr_incomplete));

	// 				if(part_of_prev)
	// 				{
	// 					 for(const auto& appearence : current->appearences)
	// 					 {
	// 					 	real_line.appearences[real_line.length + appearence.first] = appearence.second;
	// 					 }
	// 					 real_line.length += (current->length - mLayeringSize);
	// 				}
	// 				else
	// 				{
	// 					if(!real_line.appearences.empty())
	// 					{
	// 						for(const auto& word : real_line.appearences)
	// 						{
	// 							result << current_line_idx << " " << word.first << " " << word.second << "\n";
	// 						}
	// 					}
	// 					real_line = *current;
	// 				}
	// 			}
	// 			else
	// 			{
	// 				real_line = *prev;
	// 			}

	// 			prev = current;
	// 			if(++current == current_layer->second.cend())
	// 			{
	// 				if(++current_layer == mLayers.cend())
	// 				{
	// 					if(!real_line.appearences.empty())
	// 					{
	// 						for(const auto& word : real_line.appearences)
	// 						{
	// 							result << current_line_idx << " " << word.first << " " << word.second << "\n";
	// 						}
	// 					}
	// 					break;
	// 				}
	// 				else
	// 				{
	// 					current = current_layer->second.cbegin();
	// 				}
	// 			}
	// 			++current_line_idx;
	// 		}

	// 	}
	// 	return result.str();
	// }

	/*
	Raw data, need to merge layers before read,
	first - order of data chunk, second - layers(lines) of text
	*/
	std::map<size_t, std::vector<Layer>> mResultSequence;
	size_t mLayeringSize;
};

class TextParser
{
public:
	using IteratorType = TextDataView::IteratorType;

	TextParser(const TextDataView& data, const std::string& mask);

	void Parse(std::vector<Layer>& result);
private:
	bool Compare(char mask, char symbol) const;
	void ProcessNewLayer(Layer& result, IteratorType& position, const IteratorType& bound) const;

	const TextDataView& mDataView;
	const std::string& mMask;
	const char mLayerSeparator{'\n'};
};

} // namespace text_processing