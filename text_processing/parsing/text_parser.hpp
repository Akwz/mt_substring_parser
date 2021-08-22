#pragma once

#include <vector>
#include <map>

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
	size_t distance; // from data frame begin
	bool incomplete_from_begin;
	bool incomplete_from_end;
};

struct ParsingResult
{
	void Merge(const ParsingResult& other)
	{
		for(const auto& entry : other.mLayers)
		{
			mLayers[entry.first] = entry.second;
		}
	}

	// first - order of data chunk, second - layers(lines) of text
	std::map<size_t, std::vector<Layer>> mLayers;
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