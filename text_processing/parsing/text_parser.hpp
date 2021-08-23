#pragma once

#include <vector>
#include <map>
#include <cassert>
#include <sstream>

#include "text_data_view.hpp"


namespace text_processing
{

struct Appearence
{
	std::string word;
	size_t distance; // from layer begin
};

struct Layer
{
	std::vector<Appearence> appearences;
	size_t distance; // from data frame begin [deprecated]
	size_t length;
	bool incomplete_from_begin;
	bool incomplete_from_end;
};

struct ParsingResult
{
	void Merge(const ParsingResult& other)
	{
		assert(mLayeringSize == other.mLayeringSize);
		for(const auto& entry : other.mLayers)
		{
			mLayers[entry.first] = entry.second;
		}
	}

	void SetLayering(size_t layering)
	{
		mLayeringSize = layering;
	}

	std::string GetValue()
	{
		std::ostringstream result;
		if(!mLayers.empty())
		{
			using LayerIterator = std::vector<Layer>::const_iterator;
			LayerIterator current, prev;
			auto current_layer = mLayers.cbegin();
			current = prev = current_layer->second.cbegin();
			std::string layer_result;
			Layer real_line;
			size_t real_lines_count;
			while(true)
			{
				if(current != prev)
				{
					bool prev_incomplete = (*prev).incomplete_from_end;
					bool curr_incomplete = (*current).incomplete_from_begin;
					bool part_of_prev = prev_incomplete && curr_incomplete;
					assert((prev_incomplete && curr_incomplete) || (!prev_incomplete && !curr_incomplete));

					if(part_of_prev)
					{
						 auto appears = current->appearences;
					}
					else
					{
						
					}
				}

				prev = current;
				if(++current == current_layer->second.cend())
				{
					if(++current_layer == mLayers.cend())
					{
						break;
					}
					else
					{
						current = current_layer->second.cbegin();
					}
				}
			}

		}
		return result.str();
	}

	/*
	Raw data, need to merge layers before read,
	first - order of data chunk, second - layers(lines) of text
	*/
	std::map<size_t, std::vector<Layer>> mLayers;
	size_t mLayeringSize;
};

class TextParser
{
public:
	TextParser(const TextDataView& data, const std::string& mask);

	void Parse(std::vector<Layer>& result);
private:
	bool Compare(char mask, char symbol);

	const TextDataView& mDataView;
	const std::string& mMask;
};

} // namespace text_processing