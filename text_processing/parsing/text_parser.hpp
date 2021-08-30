#pragma once

#include <vector>
#include <map>

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

	void Merge(const Layer& other, size_t layering, size_t appearence_size);
	void Reset();
};

struct ParsingResult
{
	void Merge(const ParsingResult& other);

	void SetLayering(size_t layering);

	std::string GetValue() const;

	void GetMergedLayerSequence(std::vector<Layer>& merged_layers) const;

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
	void ProcessNewLayer(Layer& result, IteratorType& position, const IteratorType& bound);

	const TextDataView& mDataView;
	MaskView mMask;
};

} // namespace text_processing