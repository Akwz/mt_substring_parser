#pragma once

#include <string>

namespace text_processing
{

class TextDataView
{
public:
	using IteratorType = std::string::const_iterator;
	
	TextDataView();
	explicit TextDataView(const IteratorType& begin, const IteratorType& end);
	TextDataView(const TextDataView&) = default;
	TextDataView& operator=(const TextDataView&) = default;
	bool Empty() const;
	size_t Size() const;
	IteratorType Begin() const;
	IteratorType End() const;
private:
	IteratorType mBegin;
	IteratorType mEnd;
};

} // namespace text_processing